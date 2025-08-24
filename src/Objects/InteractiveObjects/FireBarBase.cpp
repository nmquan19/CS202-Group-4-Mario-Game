#include "../../../include/Objects/InteractiveObjects/FireBarBase.h"
#include "../../../include/System/Box2DWorldManager.h"
#include "../../../include/System/TextureManager.h"
#include "../../../include/Game/GameContext.h"
#include "../../../include/Objects/ObjectFactory.h"

FireBarBase::FireBarBase(Vector2 position, Vector2 size) : InteractiveObject(position, size) {
	physicsBody = Box2DWorldManager::getInstance().createBlockBody(position, Vector2{size.x * Constants::TILE_SIZE, size.y * Constants::TILE_SIZE});
	if (physicsBody) {
        physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
        for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            b2Filter filter = fixture->GetFilterData();
            filter.maskBits = static_cast<uint16>(ObjectCategory::BLOCK);
            filter.categoryBits = static_cast<uint16>(ObjectCategory::CHARACTER) | static_cast<uint16>(ObjectCategory::ENEMY) |
                static_cast<uint16>(ObjectCategory::PROJECTILE) | static_cast<uint16>(ObjectCategory::SHELL) |
                static_cast<uint16>(ObjectCategory::BLOCK) | static_cast<uint16>(ObjectCategory::ITEM);
            fixture->SetFilterData(filter);
        }
	}
    fireBar = ObjectFactory::createInteractiveObject(InteractiveType::FIRE_BAR, position);
    GameContext::getInstance().Objects.push_back(fireBar);
}

FireBarBase::~FireBarBase() {
    if (physicsBody) {
        Box2DWorldManager::getInstance().destroyBody(physicsBody);
        physicsBody = nullptr;
    }
    GameContext::getInstance().mark_for_deletion_Object(std::dynamic_pointer_cast<Object>(fireBar));
}

void FireBarBase::update(float deltaTime) {

}

void FireBarBase::draw() {
    Rectangle source = { 239, 86, 16, 16 };
    DrawTexturePro(TextureManager::interactiveTextures, source, hitBox, { 0, 0 }, 0.0f, WHITE);
    if (fireBar) {
        std::shared_ptr<FireBar> sharedFireBar = std::dynamic_pointer_cast<FireBar>(fireBar);
        sharedFireBar->draw(1);
    }
}

void FireBarBase::onCollision(std::shared_ptr<Object> other, Direction direction) {

}

std::vector<Rectangle> FireBarBase::getHitBox() const {
    return { hitBox };
}

ObjectType FireBarBase::getObjectType() const {
    return InteractiveType::FIRE_BAR_BASE;
}

Vector2 FireBarBase::getSize() const {
    return size;
}