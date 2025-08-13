#pragma once
#include "../../../include/Item/Fire_Flower/Fire_Flower.h"
#include "../../../include/Game/GameContext.h"
#include "../../../include/System/Box2DWorldManager.h"

Fire_Flower::Fire_Flower(Vector2 position) : Item(position) {
	animation->SetType(ItemType::FIRE_FLOWER);
	type = ItemType::FIRE_FLOWER;
	physicsBody = Box2DWorldManager::getInstance().createItemStaticBody(position, { hitbox.width, hitbox.height });
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

Fire_Flower::~Fire_Flower() {}

void Fire_Flower::update(float deltaTime) {
	Item::update(deltaTime);
}



