#pragma once
#include "../../../include/Item/Star/Star.h"
#include "../../../include/Game/GameContext.h"
#include "../../../include/System/Box2DWorldManager.h"

Star::Star(Vector2 position) : Item(position) {
	animation->SetType(ItemType::STAR);
    velocity = { 0.0f, -300.0f }; // bay lên ban đầu
    gravity = -200.0f;            // bay lên CHẬM DẦN (do gravity kéo lại)
    active = true;

    if (physicsBody) {
        Box2DWorldManager::getInstance().destroyBody(physicsBody);
    }
    physicsBody = Box2DWorldManager::getInstance().createItemBodyForCoin(position, { hitbox.width, hitbox.height });

	Amplitude = 50.0f; // Biên độ dao động
	frequency = 10.0f; // Tần số dao động
	radius = 3.0f; // Bán kính cho chuyển động tròn
}

Star::~Star() {
	// Destructor logic if needed
}

void Star::Move(double dt) {
    // Gravity làm giảm dần vận tốc đi lên
    velocity.y += gravity * dt * 0.0000000001;

    // Cập nhật vị trí: do velocity.y âm nên sẽ giảm y ⇒ đi lên
    position.y += velocity.y * dt;

    // Nếu ra khỏi màn hình phía trên, thì biến mất
    if (position.y + hitbox.height < 0) {
        active = false;
        GameContext::getInstance().mark_for_deletion_Object(
            GameContext::getInstance().getSharedPtrFromRaw(this)
        );
        return;
    }

    // Cập nhật hitbox
    hitbox.x = position.x;
    hitbox.y = position.y;

    //std::cout << "Star pos.y: " << position.y << ", vel.y: " << velocity.y << std::endl;
}

void Star::update(float deltaTime) {
    animation->Update();
	//Move(deltaTime);
    //HarmonicOscillationMove(Amplitude, frequency, deltaTime);
   
    Vector2 center;
	center.x = position.x + hitbox.width / 2;
	center.y = position.y + hitbox.height / 2;
	//CircleMove(center, radius, frequency, deltaTime);
    
	StarShapeMove(center,deltaTime, frequency);


    Vector2 renderPos = Box2DWorldManager::b2ToRaylib(physicsBody->GetPosition());
    position.x = renderPos.x - Constants::TILE_SIZE / 2;
    position.y = renderPos.y - Constants::TILE_SIZE / 2;
    hitbox.x = position.x;
    hitbox.y = position.y;
}
