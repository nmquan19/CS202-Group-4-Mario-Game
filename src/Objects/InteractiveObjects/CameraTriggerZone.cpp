#include "../../../include/Objects/InteractiveObjects/CameraTriggerZone.h"
#include "../../../include/System/Box2DWorldManager.h"
#include "../../../include/System/TextureManager.h"
#include "../../../include/Game/GameContext.h"
#include "../../../include/Objects/ObjectFactory.h"
#include "../../../include/System/CameraSystem.h"
#include "../../../include/System/Grid.h"
SwitchCameraTriggerZone::SwitchCameraTriggerZone(Vector2 position, Vector2 size,SwitchCameraTriggerZoneData data) : InteractiveObject(position, size), data(data) {
    physicsBody = Box2DWorldManager::getInstance().createRectangleBody(position, Vector2{ size.x * Constants::TILE_SIZE, size.y * Constants::TILE_SIZE });
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
	interactiveType = InteractiveType::SWITCH_CAMERA_TRIGGER_ZONE; 
	gridPosition = GridSystem::getGridCoord(position);
}
SwitchCameraTriggerZone::SwitchCameraTriggerZone(Vector2 position, Vector2 siz):InteractiveObject(position,size),data() {
    gridPosition = GridSystem::getGridCoord(position);
    physicsBody = Box2DWorldManager::getInstance().createRectangleBody(position, Vector2{ 1 * Constants::TILE_SIZE,1* Constants::TILE_SIZE });
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
    interactiveType = InteractiveType::SWITCH_CAMERA_TRIGGER_ZONE;

}
SwitchCameraTriggerZone::~SwitchCameraTriggerZone() {
    if (physicsBody) {
        Box2DWorldManager::getInstance().destroyBody(physicsBody);
        physicsBody = nullptr;
    }

}

void SwitchCameraTriggerZone::update(float deltaTime) {

}

void SwitchCameraTriggerZone::draw() {
	DrawRectangleRec(hitBox, { 255, 0, 0, 100 });
}

void SwitchCameraTriggerZone::onCollision(std::shared_ptr<Object> other, Direction direction) {
    activeContacts[other.get()]++;

    if (activeContacts[other.get()] == 1) {
        if (auto character = std::dynamic_pointer_cast<Character>(other)) {
            GameCameraSystem::getInstance().switchCamera(data.toIndex);
            GameCameraSystem::getInstance().setCameraBounds(data.outWorldBounds);
            if (data.toIndex)
            {
                GameCameraSystem::getInstance().setCamera(data.cameraOut);
            }
        }
    }
}

void SwitchCameraTriggerZone::onCollisionExit(std::shared_ptr<Object> other, Direction direction) {
    auto it = activeContacts.find(other.get());
    if (it != activeContacts.end()) {
        it->second--;

        if (it->second <= 0) {
            activeContacts.erase(it);

            if (auto character = std::dynamic_pointer_cast<Character>(other)) {
                // Switch camera based on exit direction
                if(!data.directSwitch) {
                    switch (direction) {
                    case Direction::RIGHT:
                    case Direction::DOWN:

                        GameCameraSystem::getInstance().switchCamera(data.toIndex);
                        GameCameraSystem::getInstance().setCameraBounds(data.outWorldBounds);
                        if (data.toIndex)
                        {
                            GameCameraSystem::getInstance().setCamera(data.cameraOut);
                        }
                        break;
                    case Direction::LEFT:
                    case Direction::UP:
                        GameCameraSystem::getInstance().switchCamera(data.fromIndex);
                        GameCameraSystem::getInstance().setCameraBounds(data.inWorldBounds);
                        if (data.toIndex)
                        {
                            if (data.fromIndex == 0)
                            {
								Camera2D ncam = data.cameraIn;
                                ncam.target = character->getCenterPos();
                                GameCameraSystem::getInstance().setCamera(ncam);

                            }
                            else
                            {
                                GameCameraSystem::getInstance().setCamera(data.cameraIn);
                            }
                        }
                   
                        break;

                    default:
                        GameCameraSystem::getInstance().switchCamera(data.toIndex);
                        GameCameraSystem::getInstance().setCameraBounds(data.outWorldBounds);
                        //GameCameraSystem::getInstance().setCamera(data.cameraOut);
                        break;
                    }
                    return;
				}
                switch (direction) {
                case Direction::LEFT:
                case Direction::UP:

                    GameCameraSystem::getInstance().switchCamera(data.toIndex);
                    GameCameraSystem::getInstance().setCameraBounds(data.outWorldBounds);
                    if (data.toIndex)
                    {
                        GameCameraSystem::getInstance().setCamera(data.cameraOut);
                    }
                    //GameCameraSystem::getInstance().setCamera(data.cameraOut);
                    break;

                case Direction::RIGHT:
                case Direction::DOWN:
                    GameCameraSystem::getInstance().switchCamera(data.fromIndex);
                    GameCameraSystem::getInstance().setCameraBounds(data.inWorldBounds);
                    if (data.toIndex)
                    {
                        if (data.fromIndex == 0)
                        {
                            Camera2D ncam = data.cameraIn;
                            ncam.target = character->getCenterPos();
                            GameCameraSystem::getInstance().setCamera(ncam);

                        }
                        else
                        {
                            GameCameraSystem::getInstance().setCamera(data.cameraIn);
                        }
                    }
                 
                    //GameCameraSystem::getInstance().setCamera(data.cameraIn);
                    break;

                default:
                    GameCameraSystem::getInstance().switchCamera(data.toIndex);
                    GameCameraSystem::getInstance().setCameraBounds(data.outWorldBounds);
                    //GameCameraSystem::getInstance().setCamera(data.cameraOut);
                    break;
                }
            }
        }
    }
}



ObjectType SwitchCameraTriggerZone::getObjectType() const {
    return InteractiveType::SWITCH_CAMERA_TRIGGER_ZONE;
}
json SwitchCameraTriggerZone::toJson() const {
    json j = InteractiveObject::toJson(); 
    j["type"] = "SwitchCameraTriggerZone";

    j["position"] = { data.position.x, data.position.y };
    j["size"] = { data.size.x, data.size.y };
    j["fromIndex"] = data.fromIndex;
    j["toIndex"] = data.toIndex;

    j["inWorldBounds"] = { data.inWorldBounds.x, data.inWorldBounds.y,
                            data.inWorldBounds.width, data.inWorldBounds.height };
    j["outWorldBounds"] = { data.outWorldBounds.x, data.outWorldBounds.y,
                            data.outWorldBounds.width, data.outWorldBounds.height };

    j["cameraIn"] = {
        { "target", { data.cameraIn.target.x, data.cameraIn.target.y } },
        { "offset", { data.cameraIn.offset.x, data.cameraIn.offset.y } },
        { "rotation", data.cameraIn.rotation },
        { "zoom", data.cameraIn.zoom }
    };
    j["cameraOut"] = {
        { "target", { data.cameraOut.target.x, data.cameraOut.target.y } },
        { "offset", { data.cameraOut.offset.x, data.cameraOut.offset.y } },
        { "rotation", data.cameraOut.rotation },
        { "zoom", data.cameraOut.zoom }
    };

    return j;
}

void SwitchCameraTriggerZone::fromJson(const json& j) {
    InteractiveObject::fromJson(j); // base fields
    
	position = { j["position"][0], j["position"][1] };
	size = { j["size"][0], j["size"][1] };
    data.position = { j["position"][0], j["position"][1] };
    data.size = { j["size"][0], j["size"][1] };
    data.fromIndex = j["fromIndex"];
    data.toIndex = j["toIndex"];  
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

    data.inWorldBounds = { j["inWorldBounds"][0], j["inWorldBounds"][1],
                           j["inWorldBounds"][2], j["inWorldBounds"][3] };
    data.outWorldBounds = { j["outWorldBounds"][0], j["outWorldBounds"][1],
                            j["outWorldBounds"][2], j["outWorldBounds"][3] };

    data.cameraIn.target = { j["cameraIn"]["target"][0], j["cameraIn"]["target"][1] };
    data.cameraIn.offset = { j["cameraIn"]["offset"][0], j["cameraIn"]["offset"][1] };
    data.cameraIn.rotation = j["cameraIn"]["rotation"];
    data.cameraIn.zoom = j["cameraIn"]["zoom"];

    data.cameraOut.target = { j["cameraOut"]["target"][0], j["cameraOut"]["target"][1] };
    data.cameraOut.offset = { j["cameraOut"]["offset"][0], j["cameraOut"]["offset"][1] };
    data.cameraOut.rotation = j["cameraOut"]["rotation"];
    data.cameraOut.zoom = j["cameraOut"]["zoom"];
}
