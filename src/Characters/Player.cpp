#include "../../include/Characters/Player.h"

Player::Player(Vector2 pos, Vector2 size) {
    rect = { pos.x, pos.y, size.x, size.y };
    velocity = { 0, 0 };
    onGround = false;
}

void Player::Update(float deltaTime) {
    // Nhảy
    if (IsKeyPressed(KEY_SPACE) && onGround) {
        velocity.y = -350;
        onGround = false;
    }

    // Trọng lực
    velocity.y += 500 * deltaTime;
    rect.y += velocity.y * deltaTime;

    // Va chạm mặt đất
    if (rect.y + rect.height >= 600) {
        rect.y = 600 - rect.height;
        velocity.y = 0;
        onGround = true;
    }

    // Di chuyển trái/phải
    if (IsKeyDown(KEY_RIGHT)) rect.x += 200 * deltaTime;
    if (IsKeyDown(KEY_LEFT))  rect.x -= 200 * deltaTime;
}

void Player::Draw() const {
    DrawRectangleRec(rect, BLUE);
}

Rectangle Player::GetRect() const {
    return rect;
}
