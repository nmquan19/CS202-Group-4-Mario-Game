#pragma once
#include "../../../include/Item/Mushroom/Mushroom.h"
#include "../../../include/Game/GameContext.h"
#include "../../../include/System/Constant.h"
#include "../../../include/System/Grid.h"
#include "../../../include/System/Box2DWorldManager.h"

Mushroom::Mushroom(Vector2 position) : Item(position) {
	animation->SetType(ItemType::MUSHROOM);
    type = ItemType::MUSHROOM;
    physicsBody = Box2DWorldManager::getInstance().createItemDynamicBody(position, { hitbox.width, hitbox.height });
    if (physicsBody) {
        physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
        for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            b2Filter filter = fixture->GetFilterData();
            filter.maskBits = static_cast<uint16>(ObjectCategory::ITEM);
            filter.categoryBits = static_cast<uint16>(ObjectCategory::CHARACTER) | static_cast<uint16>(ObjectCategory::BLOCK) | static_cast<uint16>(ObjectCategory::ENEMY) |
                static_cast<uint16>(ObjectCategory::INTERACTIVE) | static_cast<uint16>(ObjectCategory::SHELL) | static_cast<uint16>(ObjectCategory::PROJECTILE);
            fixture->SetFilterData(filter);
        }
    }
}

Mushroom::~Mushroom() {
	// Destructor logic if needed
}

void Mushroom::update(float deltaTime) {
    Item::update(deltaTime);

    b2Vec2 currentVel = physicsBody->GetLinearVelocity();
    if (currentVel.x >= 0) {
        physicsBody->SetLinearVelocity(b2Vec2(Box2DWorldManager::raylibToB2(2*Constants::Goomba::WANDERING_SPEED), currentVel.y));
    }
    else {
        physicsBody->SetLinearVelocity(b2Vec2(-Box2DWorldManager::raylibToB2(2*Constants::Goomba::WANDERING_SPEED), currentVel.y));
    }
}


