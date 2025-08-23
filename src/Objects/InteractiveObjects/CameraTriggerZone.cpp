#include "../../../include/Objects/InteractiveObjects/CameraTriggerZone.h"
#include "../../../include/System/Box2DWorldManager.h"
#include "../../../include/System/TextureManager.h"
#include "../../../include/Game/GameContext.h"
#include "../../../include/Objects/ObjectFactory.h"
#include "../../../include/System/CameraSystem.h"
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
   
}

ObjectType SwitchCameraTriggerZone::getObjectType() const {
    return InteractiveType::SWITCH_CAMERA_TRIGGER_ZONE;
}

void SwitchCameraTriggerZone::onCollisionExit(std::shared_ptr<Object> other, Direction direction) {
    if (direction == Direction::LEFT) {
        GameCameraSystem::getInstance().switchCamera(data.toIndex);
        GameCameraSystem::getInstance().setCameraBounds(data.inWorldBounds);
		GameCameraSystem::getInstance().setCamera(data.cameraIn);
    }
    if(direction == Direction::RIGHT) {
        GameCameraSystem::getInstance().switchCamera(data.fromIndex);
        GameCameraSystem::getInstance().setCameraBounds(data.outWorldBounds);
        GameCameraSystem::getInstance().setCamera(data.cameraOut);

	}
}   
