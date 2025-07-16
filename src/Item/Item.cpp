#include "../../include/Item/Item.h"

Item::Item(Vector2 pos, const char* imagePath, float s) {
    position = pos;
    texture = LoadTexture(imagePath);
    scale = s;
}

Item::~Item() {
    UnloadTexture(texture);
}

Item::Item(){
	hitbox = { 0,0,0,0 };

    // Va chạm mặt đất
    float groundY = GetScreenHeight() - texture.height * scale;
    if (position.y > groundY) {
        position.y = groundY;
        velocity.y = 0;
    }
}

	PhysicsManager::getInstance().addObject(this);
}

Item::~Item() {
	PhysicsManager::getInstance().markForDeletion(this);
}

Rectangle Item::getHitBox() const{
	return hitbox;
}

void Coin::Collect() {
    isCollected = true;
}

// --- Mushroom ---

Mushroom::Mushroom(Vector2 pos) : Item(pos, "mushroom.png") {}

bool Item::isActive() {
	return IsActive;
}

Rectangle Mushroom::GetRect() const {
    return Item::GetRect();
}
