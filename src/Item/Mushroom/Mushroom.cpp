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

    totalTime += deltaTime;

    Vector2 itemPos = Box2DWorldManager::b2ToRaylib(physicsBody->GetPosition());
    Vector2 marioPos = Box2DWorldManager::b2ToRaylib(
		GameContext::getInstance().character01->getPhysicsBody()->GetPosition()
    );

    int itemGridX = (int)(itemPos.x / GridSystem::GRID_SIZE);
    int itemGridY = (int)(itemPos.y / GridSystem::GRID_SIZE);
    int marioGridX = (int)(marioPos.x / GridSystem::GRID_SIZE);
    int marioGridY = (int)(marioPos.y / GridSystem::GRID_SIZE);

    int dx = marioGridX - itemGridX;
    int dy = marioGridY - itemGridY;
    float distance = sqrt(dx * dx + dy * dy);

    b2Vec2 vel = physicsBody->GetLinearVelocity();

    if (distance <= 5) {
        Vector2 dir = { (float)(itemGridX - marioGridX), (float)(itemGridY - marioGridY) };
        float len = sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len != 0) {
            dir.x /= len;
        }

        moveDirX = dir.x > 0 ? 1.0f : -1.0f; // lưu hướng mới
        vel.Set(moveDirX * 5.0f, vel.y); // tốc độ chạy tránh Mario
        physicsBody->SetLinearVelocity(vel);
    }
    else {
        float vx = omega * Amplitude * cos(omega * totalTime);
        vel.Set(vx, vel.y);
        physicsBody->SetLinearVelocity(vel);
    }

    Vector2 renderPos = Box2DWorldManager::b2ToRaylib(physicsBody->GetPosition());
    position.x = renderPos.x - Constants::TILE_SIZE / 2;
    position.y = renderPos.y - Constants::TILE_SIZE / 2;
    hitbox.x = position.x;
    hitbox.y = position.y;
}


