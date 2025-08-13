#pragma once
#include "../../../include/Item/Star/Star.h"
#include "../../../include/Game/GameContext.h"
#include "../../../include/System/Box2DWorldManager.h"

Star::Star(Vector2 position) : Item(position) {
	animation->SetType(ItemType::STAR);
    active = true;

    type = ItemType::STAR;
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
        physicsBody->SetGravityScale(0);
    } 
}

Star::~Star() {
	// Destructor logic if needed
}

void Star::update(float deltaTime) {
    Item::update(deltaTime);

    Vector2 center;
	center.x = position.x + hitbox.width / 2;
	center.y = position.y + hitbox.height / 2;
    
	StarShapeMove(center,deltaTime, 10.0f);
}
