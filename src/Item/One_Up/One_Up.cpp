#include "../../../include/Item/One_Up/One_Up.h"
#include "../../../include/Game/GameContext.h"
#include "../../../include/System/Box2DWorldManager.h"
#include "../../../include/System/Grid.h"
#include <raylib.h>

One_Up::One_Up(Vector2 startPos) : Item(startPos) {
    animation->SetType(ItemType::ONE_UP);
    type = ItemType::ONE_UP;
    physicsBody = Box2DWorldManager::getInstance().createItemDynamicBody(position, { hitbox.width, hitbox.height });
    if (physicsBody) {
        physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
        for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            //std::cout << "Item1\n";
            b2Filter filter = fixture->GetFilterData();
            filter.maskBits = static_cast<uint16>(ObjectCategory::ITEM);
            filter.categoryBits = static_cast<uint16>(ObjectCategory::CHARACTER) | static_cast<uint16>(ObjectCategory::BLOCK) | static_cast<uint16>(ObjectCategory::ENEMY) |
                static_cast<uint16>(ObjectCategory::INTERACTIVE) | static_cast<uint16>(ObjectCategory::SHELL) | static_cast<uint16>(ObjectCategory::PROJECTILE);
            fixture->SetFilterData(filter);
        }
    }
}

One_Up::~One_Up() {}

void One_Up::update(float deltaTime) {
    Item::update(deltaTime);

    b2Vec2 currentVel = physicsBody->GetLinearVelocity();
    if (currentVel.x >= 0) {
        physicsBody->SetLinearVelocity(b2Vec2(Box2DWorldManager::raylibToB2(2*Constants::Goomba::WANDERING_SPEED), currentVel.y));
    }
    else {
        physicsBody->SetLinearVelocity(b2Vec2(-Box2DWorldManager::raylibToB2(2*Constants::Goomba::WANDERING_SPEED), currentVel.y));
    }
}

