#include "../../include/Item/Item.h"
#include "../../include/System/Grid.h"
#include "../../include/System/TextureManager.h"
#include "../../include/Game/GameContext.h"
#include "../../include/System/Box2DWorldManager.h"
#include <raymath.h>
#include <memory> 
#include <numbers>

Item::Item() {}

Item::Item(Vector2 startPos)
    : position(startPos), active(true), scale(1.0f),
    velocity({ 0, 0 }),
    animation(new Anima(0, 3, 0.08f))
{
    this->spritebox = { 0, 0, 32, 32 };
    this->size = { 1, 1 };
    this->hitbox = { position.x, position.y, size.x * GridSystem::GRID_SIZE, size.y * GridSystem::GRID_SIZE };
	centerPosition = position; // Center position for circular movement
}

Item::~Item() {
    if (physicsBody) {
        Box2DWorldManager::getInstance().destroyBody(physicsBody);
        physicsBody = nullptr;
    }
}

void Item::update(float deltaTime) {
    animation->Update();
    
    if (physicsBody) {
        b2Vec2 b2Pos = physicsBody->GetPosition();
        Vector2 bodyPos = Box2DWorldManager::b2ToRaylib(b2Pos);
        position.x = bodyPos.x - hitbox.width * 0.5f;
        position.y = bodyPos.y - hitbox.height * 0.5f;
        hitbox.x = position.x;
        hitbox.y = position.y;
    }

}

void Item::draw() {
    // Draw the item using the animation frames
    DrawTexturePro(
        TextureManager::getInstance().getItemTexture(),
        animation->GetFrame(),
        hitbox,
        { 0, 0 },
        0,
        WHITE
    );
}

void Item::draw(Texture2D texture) {
    DrawTexturePro(
        texture,
        animation->GetFrame(),
        { position.x, position.y, 32, 32 },
        { 0, 0 },
        0,
        WHITE
    );
}

std::vector<Rectangle> Item::getHitBox() const {
    return { hitbox }; // Đưa Rectangle đơn vào trong vector
}

ObjectCategory Item::getObjectCategory() const {
    return ObjectCategory::ITEM;
}

std::vector<ObjectCategory> Item::getCollisionTargets() const {
    return { ObjectCategory::CHARACTER };
}

void Item::onCollision(std::shared_ptr<Object> other, Direction direction) {
    if (other->getObjectCategory() == ObjectCategory::CHARACTER) {
        for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            b2Filter filter = fixture->GetFilterData();
            filter.maskBits = 0;
            fixture->SetFilterData(filter);
        }
        GameContext::getInstance().mark_for_deletion_Object(GameContext::getInstance().getSharedPtrFromRaw(this));
    } 
    else if (type == ItemType::ONE_UP || type == ItemType::MUSHROOM) {
        b2Vec2 currentVel = this->physicsBody->GetLinearVelocity();
        if (direction == Direction::LEFT) {
            physicsBody->SetLinearVelocity(b2Vec2(abs(currentVel.x), currentVel.y));
            std::cout << "left\n";
        }
        else if (direction == Direction::RIGHT) {
            physicsBody->SetLinearVelocity(b2Vec2(-abs(currentVel.x), currentVel.y));
            std::cout << "right\n";
        }
    }
}

Vector2 Item::getPosition() const {
    return position;
}

void Item::setPosition(Vector2 newPosition) {
    position = newPosition;
    hitbox.x = position.x;
    hitbox.y = position.y;
}

bool Item::isActive() const {
    return active;
}

void Item::setActive(bool flag) {
    active = flag;
}

bool Item::isCollided() const {
    return collided;
}

void Item::setCollided(bool value) {
    collided = value;
}

Vector2 Item::getSize() const {
    return size;
}

ObjectType Item::getObjectType() const {
    return type;
}

void Item::CircleMove(Vector2 center, float radius, float speed, float deltaTime) {
    totalTime += deltaTime;
    position.x = center.x + radius * cos(speed * totalTime);
    position.y = center.y + radius * sin(speed * totalTime);
    if (physicsBody) {
        physicsBody->SetTransform(b2Vec2(position.x / GridSystem::GRID_SIZE, position.y / GridSystem::GRID_SIZE),
            physicsBody->GetAngle());
    }
}

void Item::HarmonicOscillationMove(float amplitude, float frequency, float deltaTime) {
    totalTime += deltaTime;

    // Tần số góc ω = 2πf
    float omega = 2.0f * 1; //std::numbers::pi_v<float> *frequency;

    // Tính vị trí offset (pixel)
    float offsetX = amplitude * cos(omega * totalTime);

    // Tính vận tốc theo pixel/s
    float vxPixelsPerSecond = -omega * amplitude * sin(omega * totalTime);

    // Đổi vận tốc sang mét/s cho Box2D
    float vxMetersPerSecond = Box2DWorldManager::raylibToB2(vxPixelsPerSecond);

    if (physicsBody) {
        b2Vec2 vel(vxMetersPerSecond, physicsBody->GetLinearVelocity().y);
        physicsBody->SetLinearVelocity(vel);
    }

    // Cập nhật vị trí hiển thị từ physics body
    Vector2 renderPos = Box2DWorldManager::b2ToRaylib(physicsBody->GetPosition());
    position.x = renderPos.x - Constants::TILE_SIZE / 2;
    position.y = renderPos.y - Constants::TILE_SIZE / 2;

}

void Item::StarShapeMove(Vector2 center, float deltaTime, float frequency) {
    totalTime += deltaTime;
    position.x = center.x - 0.25 * (3 * cos(0.4 * totalTime) + 2 * sin(0.6 * totalTime));
    position.y = center.y - 0.25 * (3 * sin(0.4 * totalTime) + 2 * cos(0.6 * totalTime));

    if (physicsBody) {
        physicsBody->SetTransform(b2Vec2(position.x / GridSystem::GRID_SIZE, position.y / GridSystem::GRID_SIZE),
            physicsBody->GetAngle());
    }
}

json Item::toJson() const {
    json data;
    data["saveType"] = getSaveType();
    data["itemType"] = static_cast<int>(type);
    data["position"] = { physicsBody->GetPosition().x, physicsBody->GetPosition().y };
    data["velocity"] = { physicsBody->GetLinearVelocity().x, physicsBody->GetLinearVelocity().y };
    return data;
}

void Item::fromJson(const json& data) {
    type = static_cast<ItemType>(data["itemType"]);
    physicsBody->SetTransform(b2Vec2(data["position"][0],data["position"][1]), 0.0f);
    physicsBody->SetLinearVelocity(b2Vec2(data["velocity"][0], data["velocity"][1]));
}

std::string Item::getSaveType() const {
    return "Item";
}