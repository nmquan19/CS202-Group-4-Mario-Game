#pragma once
#include "../../../include/Item/One_Up/One_Up.h"
#include "../../../include/Game/GameContext.h"
#include "../../../include/System/Box2DWorldManager.h"
#include "../../../include/System/Grid.h"

One_Up::One_Up(Vector2 position) : Item(position) {
	animation->SetType(ItemType::ONE_UP);
    active = true;

    velocity = { 0, 0 };

    hitbox.width = GridSystem::GRID_SIZE;
    hitbox.height = GridSystem::GRID_SIZE;
    hitbox.x = position.x;
    hitbox.y = position.y;

    // ✅ Tạo Box2D body động (để rơi, va chạm với block)
    physicsBody = Box2DWorldManager::getInstance().createItemBody(
        position,
        { hitbox.width, hitbox.height }
    );
}

One_Up::~One_Up() {
	// Destructor logic if needed
}

void One_Up::update(float deltaTime) {
    animation->Update();
    Move(deltaTime);
}

void One_Up::Move(double dt) {
    if (!active || physicsBody == nullptr) return;

    // Lấy vị trí từ Box2D
    b2Vec2 bodyPos = physicsBody->GetPosition();

    // Scale về toạ độ pixel trong game (Box2D xài đơn vị mét)
    position.x = Box2DWorldManager::b2ToRaylib(bodyPos.x) - hitbox.width / 2.0f;
    position.y = Box2DWorldManager::b2ToRaylib(bodyPos.y) - hitbox.height / 2.0f;

    // Cập nhật hitbox theo position
    hitbox.x = position.x;
    hitbox.y = position.y;
}
