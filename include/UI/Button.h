#pragma once

#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>
#include "UI.h"
#include "SoundEffect.h"

class Button {
private:
    Vector2 position;
    Rectangle recButton;
    Texture2D textureButton;
    float scale;
public:
    Button();
    void load(const char* textureFile);
    virtual ~Button();
    void setRectangle();
    void draw(Vector2 pos);
    bool checkCollision(Vector2 pos);
};