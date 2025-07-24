#include "../../include/UI/Button.h"

Button::Button() {
    position = { 0, 0 };
    recButton = { 0, 0, 0, 0 };
    textureButton = LoadTexture(nullptr);
    scale = 0.2f;
}

void Button::load(const char* textureFile) {
    textureButton = LoadTexture(textureFile);
}

Button::~Button() {
    UnloadTexture(textureButton);
}

void Button::setRectangle() {
    recButton = { position.x - 5, position.y - 15, static_cast<float>(textureButton.width) * scale / 1.5f,
        static_cast<float>(textureButton.height) * scale / 1.5f };
}

void Button::draw(Vector2 pos) {
    position = pos;
    setRectangle();
    DrawTextureEx(textureButton, position, 0, scale / 1.5f, WHITE);
}

bool Button::checkCollision(Vector2 pos) {
    if (CheckCollisionPointRec(pos, recButton)) return true;
    return false;
}