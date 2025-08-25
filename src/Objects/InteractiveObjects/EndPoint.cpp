#include "../../../include/Objects/InteractiveObjects/EndPoint.h"
#include "../../../include/System/Box2DWorldManager.h"
#include "../../../include/System/TextureManager.h"
#include "../../../include/Game/GameContext.h"
#include "../../../include/System/Grid.h"
#include "../../../include/System/CameraSystem.h"
#include "../../../include/UI/UI.h"
#include "../../../include/Game/GameStates.h"
Endpoint::Endpoint(Vector2 position, Vector2 size, EndpointData data)
    : InteractiveObject(position, size), data(data) {

    physicsBody = Box2DWorldManager::getInstance().createRectangleBody(
        position,
        Vector2{ size.x * Constants::TILE_SIZE, size.y * Constants::TILE_SIZE }
    );

    if (physicsBody) {
        physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

        for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            b2Filter filter = fixture->GetFilterData();
            filter.maskBits = static_cast<uint16>(ObjectCategory::TRIGGER);
            filter.categoryBits = static_cast<uint16>(ObjectCategory::CHARACTER);
            fixture->SetFilterData(filter);
            fixture->SetSensor(true);
        }
    }

    interactiveType = InteractiveType::ENDPOINT;
    gridPosition = GridSystem::getGridCoord(position);
}

Endpoint::Endpoint(Vector2 position, Vector2 size)
    : InteractiveObject(position, size) {

    data.position = position;
    data.size = size;

    physicsBody = Box2DWorldManager::getInstance().createRectangleBody(
        position,
        Vector2{ 1 * Constants::TILE_SIZE, 1 * Constants::TILE_SIZE }
    );

    if (physicsBody) {
        physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

        for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            b2Filter filter = fixture->GetFilterData();
            filter.maskBits = static_cast<uint16>(ObjectCategory::TRIGGER);
            filter.categoryBits = static_cast<uint16>(ObjectCategory::CHARACTER);
            fixture->SetFilterData(filter);
            fixture->SetSensor(true);
        }
    }

    interactiveType = InteractiveType::ENDPOINT;
    gridPosition = GridSystem::getGridCoord(position);
}

Endpoint::~Endpoint() {
    if (physicsBody) {
        Box2DWorldManager::getInstance().destroyBody(physicsBody);
        physicsBody = nullptr;
    }
}

void Endpoint::update(float deltaTime) {
    // Handle activation timer
    if (isActivated && activationTimer < activationTime) {
        activationTimer += deltaTime;

        // Fully activated
        if (activationTimer >= activationTime) {
            // Trigger level transition
            GameContext& context = GameContext::getInstance();

            if (data.targetLevel == -1) {
                // Go to level selector
                context.setState(context.levelSelectingState);
            }
            else {
                // Go to specific level
                context.level = data.targetLevel;
                context.setState(context.gamePlayState);
                context.gamePlayState->setLevel(context);
            }

            isActivated = false;
            activationTimer = 0.0f;
        }
    }
}

void Endpoint::draw() {
    Color endpointColor = isActivated ?
        Color{ 0, 255, 0, 150 } : // Green when activated
        Color{ 0, 0, 255, 150 };  // Blue when inactive

    // Draw with progress indicator if activating
    if (isActivated && activationTimer > 0) {
        float progress = activationTimer / activationTime;
        DrawRectangleRec(hitBox, endpointColor);

        // Progress bar at the top
        Rectangle progressBar = {
            hitBox.x,
            hitBox.y - 5,
            hitBox.width * progress,
            3
        };
        DrawRectangleRec(progressBar, GREEN);
    }
    else {
        DrawRectangleRec(hitBox, endpointColor);
    }

    // Draw label
    std::string label = data.targetLevel == -1 ?
        "Level Select" :
        "Level " + std::to_string(data.targetLevel);

    Vector2 textPos = {
        hitBox.x + hitBox.width / 2 - MeasureText(label.c_str(), 10) / 2,
        hitBox.y + hitBox.height / 2 - 5
    };

    DrawText(label.c_str(), textPos.x, textPos.y, 10, WHITE);
}

void Endpoint::onCollision(std::shared_ptr<Object> other, Direction direction) {
    activeContacts[other.get()]++;

    if (activeContacts[other.get()] == 1) {
        if (auto character = std::dynamic_pointer_cast<Character>(other)) {
            // Start activation process
            isActivated = true;
        }
    }
}

void Endpoint::onCollisionExit(std::shared_ptr<Object> other, Direction direction) {
    auto it = activeContacts.find(other.get());
    if (it != activeContacts.end()) {
        it->second--;
        if (it->second <= 0) {
            activeContacts.erase(it);
            if (auto character = std::dynamic_pointer_cast<Character>(other)) {
                // Cancel activation if player leaves
                isActivated = false;
                activationTimer = 0.0f;
            }
        }
    }
}

ObjectType Endpoint::getObjectType() const {
    return InteractiveType::ENDPOINT;
}

json Endpoint::toJson() const {
    json j = InteractiveObject::toJson();
    j["type"] = "Endpoint";
    j["endpointData"] = data.toJson();
    return j;
}

void Endpoint::fromJson(const json& j) {
    InteractiveObject::fromJson(j);

    // Load endpoint data
    if (j.contains("endpointData")) {
        data.fromJson(j["endpointData"]);
    }

    // Update physics body size and position based on loaded data
    if (physicsBody) {
        while (b2Fixture* f = physicsBody->GetFixtureList()) {
            physicsBody->DestroyFixture(f);
        }

        b2PolygonShape boxShape;
        boxShape.SetAsBox(
            Box2DWorldManager::raylibToB2(size.x * GridSystem::GRID_SIZE * 0.5f),
            Box2DWorldManager::raylibToB2(size.y * GridSystem::GRID_SIZE * 0.5f)
        );

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.isSensor = true;

        b2Fixture* fixture = physicsBody->CreateFixture(&fixtureDef);
        b2Filter filter;
        filter.categoryBits = static_cast<uint16>(ObjectCategory::CHARACTER);
        filter.maskBits = static_cast<uint16>(ObjectCategory::TRIGGER);
        fixture->SetFilterData(filter);

        b2Vec2 b2Pos = Box2DWorldManager::raylibToB2({
            position.x + size.x * GridSystem::GRID_SIZE * 0.5f,
            position.y + size.y * GridSystem::GRID_SIZE * 0.5f
            });

        physicsBody->SetTransform(b2Pos, 0.0f);
        physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
    }
}

void Endpoint::activate() {
    isActivated = true;
}

void Endpoint::deactivate() {
    isActivated = false;
    activationTimer = 0.0f;
}