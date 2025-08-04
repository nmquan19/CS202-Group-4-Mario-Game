#pragma once

#include <raylib.h>
#include "UI.h"
class Button {
private:
    Vector2 position;
    Rectangle recButton;
    Texture2D normalButton;
    Texture2D selectedButton;

    float scale;
    float targetScale;
    float animationSpeed;
    bool isHovered;

public:
    Button();
    ~Button();
    Vector2 getSize() {
        return { recButton.width, recButton.height };
    }
    Vector2 getPosition() {
        return position;
    }
    Texture2D getNormalTexture() {
        return normalButton;
    }
    float getScale() {
        return scale;
    }
    void setScale(float newScale) {
        scale = newScale;
    }
    void load(const char* normalPath, const char* selectedPath);
    void setPosition(Vector2 pos);
    void update(float transitionTime);
    void draw(Vector2 pos);
    bool checkCollision(Vector2 point);
    void draw(Vector2 pos, const char* c) {
        draw(pos);
        Vector2 textSize = MeasureTextEx(UIManager::getInstance().menuFont, c, 50 * scale * 10, 10 * scale * 10);
        DrawTextEx(UIManager::getInstance().menuFont, c, { pos.x - textSize.x / 2.0f, pos.y - textSize.y / 2.0f }, 50 * scale * 10, 10 * scale * 10, WHITE);
    }

};
