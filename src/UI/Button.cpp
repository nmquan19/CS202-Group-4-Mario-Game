#include "../../include/UI/Button.h"

Button::Button() {
    position = { 0, 0 };
    scale = 0.1f;
    targetScale = 0.1f;
    animationSpeed = 5.0f;
    isHovered = false;
    recButton = { 0, 0, 0, 0 };

    normalButton = { 0 };
    selectedButton = { 0 };
}

Button::~Button() {
    UnloadTexture(normalButton);
    UnloadTexture(selectedButton);
}

void Button::load(const char* normalPath, const char* selectedPath) {
    normalButton = LoadTexture(normalPath);
    selectedButton = LoadTexture(selectedPath);
}

void Button::setPosition(Vector2 pos) {
    position = pos;
}

bool Button::checkCollision(Vector2 point) {
    Rectangle collision = { recButton.x, recButton.y - recButton.height / 2.5f, recButton.width, recButton.height };
    return CheckCollisionPointRec(point, collision);
}

void Button::update(float transitionTime) {
    Vector2 mousePos = GetMousePosition();
    isHovered = checkCollision(mousePos);
    targetScale = isHovered ? 0.11f : 0.1f;
    animationSpeed = 10.0f;

    scale += (targetScale - scale) * animationSpeed * transitionTime;

    float width = normalButton.width * scale;
    float height = normalButton.height * scale;
    recButton = {
        position.x - width / 2,
        position.y - height / 2,
        width,
        height
    };
}

void Button::draw(Vector2 pos) {
    position.x = pos.x; position.y = pos.y;
    Texture2D texture = isHovered ? selectedButton : normalButton;
    float width = texture.width * scale;
    float height = texture.height * scale;
    Vector2 drawPos = {
        position.x - width / 2,
        position.y - height / 2
    };

    DrawTextureEx(texture, drawPos, 0, scale, WHITE);
}
