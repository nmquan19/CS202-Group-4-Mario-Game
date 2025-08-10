#pragma once
#include "../../../include/Item/Star/Star.h"
#include "../../../include/Game/GameContext.h"

Star::Star(Vector2 position) : Item(position) {
	animation->SetType(ItemType::STAR);
    velocity = { 0.0f, -300.0f }; // bay lên ban đầu
    gravity = -200.0f;            // bay lên CHẬM DẦN (do gravity kéo lại)
    active = true;
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
	Move(deltaTime);
}
