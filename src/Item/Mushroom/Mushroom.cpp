#pragma once
#include "../../../include/Item/Mushroom/Mushroom.h"
#include "../../../include/Game/GameContext.h"
#include "../../../include/System/Constant.h"
#include "../../../include/System/Grid.h"
#include "../../../include/System/Box2DWorldManager.h"

Mushroom::Mushroom(Vector2 position) : Item(position) {
	animation->SetType(ItemType::MUSHROOM);
}

Mushroom::~Mushroom() {
	// Destructor logic if needed
}

void Mushroom::update(float deltaTime) {
	animation->Update();
    // 1. Lấy vị trí item và mario
    Vector2 itemPos = this->getPosition();
    Vector2 marioPos1 = GameContext::getInstance().character01->getPosition(); // bạn cần truyền mario vào hoặc lấy từ GameContext

    int itemGridX = (int)(itemPos.x / GridSystem::GRID_SIZE);
    int itemGridY = (int)(itemPos.y / GridSystem::GRID_SIZE);
    int marioGridX = (int)(marioPos1.x / GridSystem::GRID_SIZE);
    int marioGridY = (int)(marioPos1.y / GridSystem::GRID_SIZE);

    int dx = itemGridX - marioGridX;
    int dy = itemGridY - marioGridY;
    float distance = sqrt(dx * dx + dy * dy);

    // 2. Lấy velocity hiện tại
    b2Vec2 vel = physicsBody->GetLinearVelocity();

    // 3. Quyết định hành vi
    if (distance <= 5) {
        Vector2 dir = { (float)dx, (float)dy };
        float len = sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len != 0) {
            dir.x /= len;
            dir.y /= len;
        }
        float speed = 3.0f;
        vel.Set(dir.x * speed, dir.y * speed);
        physicsBody->SetLinearVelocity(vel);
    }
    else {
		float normalSpeed = 1.0f; // Tốc độ mặc định khi không gần Mario
        // ====== Hành vi mặc định ======
        vel.Set(-normalSpeed, vel.y); // ví dụ: di chuyển ngang với tốc độ normalSpeed
        physicsBody->SetLinearVelocity(vel);
    }

    //giúp tôi sửa đoạn dưới này để cái texture đi theo cái body đi
    //hitbox = physicsBody;
    //position = physicsBody;

    Vector2 renderPos = Box2DWorldManager::b2ToRaylib(physicsBody->GetPosition());
    position.x = renderPos.x - Constants::TILE_SIZE/2;
    position.y = renderPos.y - Constants::TILE_SIZE / 2;
    hitbox.x = position.x;
    hitbox.y = position.y;
}



