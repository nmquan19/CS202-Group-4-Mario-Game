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

    Vector2 marioPos1 = Box2DWorldManager::b2ToRaylib(
		GameContext::getInstance().character01->getPhysicsBody()->GetPosition()
    );
    
    Vector2 marioPos2 = Box2DWorldManager::b2ToRaylib(
        GameContext::getInstance().character02->getPhysicsBody()->GetPosition()
    );

    int itemGridX = (int)(itemPos.x / GridSystem::GRID_SIZE);
    int itemGridY = (int)(itemPos.y / GridSystem::GRID_SIZE);

	int mario1GridX = (int)(marioPos1.x / GridSystem::GRID_SIZE);
	int mario1GridY = (int)(marioPos1.y / GridSystem::GRID_SIZE);
	int mario2GridX = (int)(marioPos2.x / GridSystem::GRID_SIZE);
	int mario2GridY = (int)(marioPos2.y / GridSystem::GRID_SIZE);

	int dx1 = mario1GridX - itemGridX;
	int dy1 = mario1GridY - itemGridY;
	int dx2 = mario2GridX - itemGridX;
	int dy2 = mario2GridY - itemGridY;

	int marioGridX = (abs(dx1) < abs(dx2)) ? mario1GridX : mario2GridX;
	int marioGridY = (abs(dy1) < abs(dy2)) ? mario1GridY : mario2GridY;

    int dx = marioGridX - itemGridX;
    int dy = marioGridY - itemGridY;
    float distance = sqrt(dx * dx + dy * dy);

    b2Vec2 vel = physicsBody->GetLinearVelocity();


    std::cout << "distance: " << distance << std::endl;
    if (distance <= 5) {
        Vector2 dir = { (float)(itemGridX - marioGridX), (float)(itemGridY - marioGridY) };
        float len = sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len != 0) {
            dir.x /= len;
        }

        moveDirX = dir.x > 0 ? 1.0f : -1.0f; // lưu hướng mới
        vel.Set(moveDirX * 5.0f, vel.y); // tốc độ chạy tránh Mario
        physicsBody->SetLinearVelocity(vel);

        isEscaping = true;
    }
    else {
        if (isEscaping) {
            // Vừa thoát khỏi chạy trốn → đặt lại centerPosition tại vị trí hiện tại
            centerPosition = Box2DWorldManager::b2ToRaylib(physicsBody->GetPosition());
            isEscaping = false;
            totalTime = 0; // reset pha dao động để tránh nhảy
        }

        //float vx = omega * Amplitude * cos(omega * totalTime);
		//float vy = omega * Amplitude * sin(omega * totalTime);
        //vel.Set(vx, vel.y);
        //physicsBody->SetLinearVelocity(vel);
		HarmonicOscillationMove(Amplitude, omega, deltaTime);
    }

    Vector2 renderPos = Box2DWorldManager::b2ToRaylib(physicsBody->GetPosition());
    position.x = renderPos.x - Constants::TILE_SIZE / 2;
    position.y = renderPos.y - Constants::TILE_SIZE / 2;
    hitbox.x = position.x;
    hitbox.y = position.y;
}


