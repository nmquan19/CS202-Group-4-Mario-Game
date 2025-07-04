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
    Button() {
        position = { 0, 0 };
        recButton = { 0, 0, 0, 0 };
        textureButton = LoadTexture(nullptr);
        scale = 0.2;
    }
    void load(const char* textureFile) {
        textureButton = LoadTexture(textureFile);
    }
    virtual ~Button() {
        UnloadTexture(textureButton);
    }
    void setRectangle() {
        recButton = { position.x, position.y, static_cast<float>(textureButton.width) * scale / float(1.75),
            static_cast<float>(textureButton.height) * scale / float(1.75) };
    }
    void draw(Vector2 pos) {
        position = pos;
        setRectangle();
        DrawTextureEx(textureButton, position, 0, scale / double(1.5), WHITE);
    }
    bool checkCollision(Vector2 pos) {
        if (CheckCollisionPointRec(pos, recButton)) return true;
        return false;
    }
};
class MenuManager : public UIManager {
private:
    Texture2D logo, board;
    Button check, cross, setting, returnButton;
    Vector2 boardPosition, crossPosition, checkPosition, settingPosition, returnButtonPosition;
    float scale = 0.2;
    //Texture2D startButton;
    bool isActive;
    AudioManager audioManager;
public:
    int select;
    bool dialog, exit;
    MenuManager();
    ~MenuManager();

    void HandleInput();
    void DrawMenu();
    bool HandleExit();
    void DrawExit();
    void DrawSetting();
    void HandleSetting();
};

