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
        scale = 0.2f;
    }
    void load(const char* textureFile) {
        textureButton = LoadTexture(textureFile);
    }
    virtual ~Button() {
        UnloadTexture(textureButton);
    }
    void setRectangle() {
        recButton = { position.x - 5, position.y - 15, static_cast<float>(textureButton.width) * scale / 1.5f,
            static_cast<float>(textureButton.height) * scale / 1.5f };
    }
    void draw(Vector2 pos) {
        position = pos;
        setRectangle();
        DrawTextureEx(textureButton, position, 0, scale / 1.5f, WHITE);
    }
    bool checkCollision(Vector2 pos) {
        if (CheckCollisionPointRec(pos, recButton)) return true;
        return false;
    }
};

#define LIGHT_YELLOW CLITERAL(Color){254, 242, 0, 255}
#define CYBER_YELLOW CLITERAL(Color){255, 194, 14, 255}
#define BORDER_BROWN CLITERAL(Color){189, 163, 62, 255}

class SlideBar : public UIManager {
private:
    float percent;
    float thickness;
    float scale;
    Vector2 position;
    Rectangle border;
    bool dragging;

public:
    SlideBar() {
        scale = 0.1f;
        percent = 50.0f;
        thickness = 640.0f / 23.0f * scale;
        position = { 0,0 };
        border = { 0,0,0,0 };
        dragging = false;
    }

    ~SlideBar() {}

    float getValue() { return percent; }
    Rectangle getBorder() { return border; }
    void HandleSlideBar() {

        Vector2 mousePos = GetMousePosition();
        Rectangle inner = { border.x + thickness, border.y + thickness, border.width - 2 * thickness, border.height - 2 * thickness };

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, inner)) {
            dragging = true;
        }

        if (dragging) {
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                dragging = false;
            }
            float localX = mousePos.x - inner.x;
            if (localX < 0) localX = 0;
            if (localX > inner.width) localX = inner.width;

            percent = localX / inner.width * 100.0f;
        }
    }

    void DrawSlideBar(Vector2 pos) {
        position = pos;
        border = { pos.x, pos.y, 640.0f / 23.0f * 130.0f * scale, 640.0f / 23.0f * 18.0f * scale };
        DrawRectangleRec(border, BORDER_BROWN);

        Vector2 innerVec = { border.x + thickness, border.y + thickness };
        Rectangle inner = { innerVec.x, innerVec.y, border.width - 2 * thickness, border.height - 2 * thickness };
        DrawRectangleRec(inner, LIGHT_YELLOW);

        Rectangle value = { innerVec.x, innerVec.y, inner.width * percent / 100.0f, inner.height };
        DrawRectangleRec(value, CYBER_YELLOW);
        DrawTextEx(menuFont, TextFormat("%.0f%%", percent), { border.x + border.width + 10, border.y + border.height / 2 - 10 }, 20, 4, BORDER_BROWN);
        //DrawText(TextFormat("%.0f%%", percent), (int)(border.x + border.width + 10), (int)(border.y + border.height / 2 - 10), 20, BLACK);
        HandleSlideBar();
    }
};
class MenuManager : public UIManager {
private:
    Texture2D logo, board;
    Button check, cross, setting, returnButton, home, play;
    Button check_selected, cross_selected, setting_selected, returnButton_selected;
    Vector2 boardPosition, crossPosition, checkPosition, settingPosition, returnButtonPosition;

    float scale = 0.2f;
    bool isActive;
    AudioManager audioManager;
public:
    SlideBar slideBarMusic, slideBarSound; Vector2 slideBarMusicPosition, slideBarSoundPosition;
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
