#include "../../include/Item/Item.h"

Item::Item(Vector2 pos, const char* imagePath, float s) {
    position = pos;
    texture = LoadTexture(imagePath);
    scale = s;
}

Item::~Item() {
    UnloadTexture(texture);
}

void Item::Draw() const {
    DrawTextureEx(texture, position, 0.0f, scale, WHITE);
}

Rectangle Item::GetRect() const {
    return {
        position.x,
        position.y,
        texture.width * scale,
        texture.height * scale
    };
}

// --- Coin ---

Coin::Coin(Vector2 pos) : Item(pos, "coin.png"), isCollected(false) {
    velocity = { 0, 0 };
    texture = LoadTexture("ItemTexture/coin.png");
}

void Coin::Update(float deltaTime) {
    if (isCollected) return;

    // Trọng lực
    velocity.y += 500 * deltaTime;
    position.y += velocity.y * deltaTime;

    // Va chạm mặt đất
    float groundY = GetScreenHeight() - texture.height * scale;
    if (position.y > groundY) {
        position.y = groundY;
        velocity.y = 0;
    }
}

void Coin::Draw() const {
    if (!isCollected)
        Item::Draw();
}

Rectangle Coin::GetRect() const {
    return Item::GetRect();
}

bool Coin::Collected() const {
    return isCollected;
}

void Coin::Collect() {
    isCollected = true;
}

// --- Mushroom ---

Mushroom::Mushroom(Vector2 pos) : Item(pos, "mushroom.png") {}

void Mushroom::Update(float deltaTime) {
    // Có thể thêm logic di chuyển sau
}

Rectangle Mushroom::GetRect() const {
    return Item::GetRect();
}
