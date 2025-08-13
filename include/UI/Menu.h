#pragma once

#include <raylib.h>
#include <rlgl.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include "UI.h"
#include "SoundEffect.h"
#include "Button.h"
#include "SlideBar.h"
#define MARIO_COLOR CLITERAL(Color){239, 83, 80, 255}
#define TOADETTE_COLOR CLITERAL(Color){255, 128, 171, 255}
#define LUIGI_COLOR CLITERAL(Color){178, 255, 89, 255}
#define TOAD_COLOR CLITERAL(Color){77, 208, 225, 255}


class Parallelogram {
private:
    Vector2 topLeft, topRight, bottomRight, bottomLeft;
    Color color1, color2;
public:
    Parallelogram() : topLeft({ 0,0 }), topRight({ 0,0 }), bottomRight({ 0,0 }), bottomLeft({ 0,0 }), color1(BLANK), color2(BLANK) {}
    Parallelogram(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, Color c1, Color c2) :
        topLeft(p1), topRight(p2), bottomRight(p3), bottomLeft(p4), color1(c1), color2(c2) {}
    void DrawParallelogram(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, Color c1) {
        DrawTriangle(p1, p2, p3, c1);
        DrawTriangle(p1, p2, p4, c1);
        DrawTriangle(p1, p3, p4, c1);
        DrawTriangle(p2, p3, p4, c1);
    }
    void DrawParallelogramGradient(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, Color c1, Color c2) {
        topLeft = p1; topRight = p2; bottomRight = p3; bottomLeft = p4;
        color1 = c1; color2 = c2;
        rlBegin(RL_QUADS);
        /*
        rlColor4ub(color1.r, color1.g, color1.b, color1.a);
        rlVertex2f(topRight.x, topRight.y);
        rlVertex2f(topLeft.x, topLeft.y);


        rlColor4ub(color2.r, color2.g, color2.b, color2.a);
        rlVertex2f(bottomLeft.x, bottomLeft.y);
        rlVertex2f(bottomRight.x, bottomRight.y);
        */
        rlColor4ub(color1.r, color1.g, color1.b, color1.a);
        rlVertex2f(topRight.x, topRight.y);
        rlVertex2f(topLeft.x, topLeft.y);


        rlColor4ub(color2.r, color2.g, color2.b, color2.a);
        rlVertex2f(bottomLeft.x / 2.0f + topLeft.x / 2.0f, bottomLeft.y / 2.0f + topLeft.y / 2.0f);
        rlVertex2f(bottomRight.x / 2.0f + topRight.x / 2.0f, bottomRight.y / 2.0f + topRight.y / 2.0f);

        rlEnd();

        rlBegin(RL_QUADS);

        rlColor4ub(color2.r, color2.g, color2.b, color2.a);
        rlVertex2f(bottomRight.x / 2.0f + topRight.x / 2.0f, bottomRight.y / 2.0f + topRight.y / 2.0f);
        rlVertex2f(bottomLeft.x / 2.0f + topLeft.x / 2.0f, bottomLeft.y / 2.0f + topLeft.y / 2.0f);

        rlColor4ub(color1.r, color1.g, color1.b, color1.a);
        rlVertex2f(bottomLeft.x, bottomLeft.y);
        rlVertex2f(bottomRight.x, bottomRight.y);

        rlEnd();
    }
    bool PointInTriangle(Vector2 pt, Vector2 v1, Vector2 v2, Vector2 v3) {
        float dX = pt.x - v3.x;
        float dY = pt.y - v3.y;
        float dX21 = v3.x - v2.x;
        float dY12 = v2.y - v3.y;
        float D = dY12 * (v1.x - v3.x) + dX21 * (v1.y - v3.y);
        float s = dY12 * dX + dX21 * dY;
        float t = (v3.y - v1.y) * dX + (v1.x - v3.x) * dY;
        if (D < 0) return (s <= 0 && t <= 0 && s + t >= D);
        return (s >= 0 && t >= 0 && s + t <= D);
    }

    bool CheckCollisionPointParallelogram(Vector2 point) {
        return PointInTriangle(point, topLeft, topRight, bottomRight) || PointInTriangle(point, bottomRight, bottomLeft, topLeft);
    }
};




class ParallelogramHoverManager {
private:
    struct HoverableParallelogram {
        Parallelogram shape;
        Vector2 center;
        float width, height;
        float scale = 1.0f;
        float targetScale = 1.0f;
        float hoverScale = 1.2f;
        float scaleSpeed = 6.0f;
        Color color1 = WHITE, color2 = BLUE;
    };

    std::vector<HoverableParallelogram> parallelograms;
    float spacing = 20.0f;
    float baseWidth = 100.0f;
    float height = 100.0f;

public:
    vector<Color> textColor;
    ParallelogramHoverManager() {}
    float getWidth(int i) { return baseWidth; }
    float getHeight(int i) { return height; }
    Vector2 getCenter(int i) { return parallelograms[i % parallelograms.size()].center; }
    float getScale(int i) { return parallelograms[i % parallelograms.size()].scale; }
    int getSize() { return parallelograms.size(); }
    bool checkCollision(Vector2 p, int i) {
        if (parallelograms[i % parallelograms.size()].shape.CheckCollisionPointParallelogram(p)) return true;
        return false;
    }

    void Init(int count, float y, float baseW, float h, Color main, Color c1, Color c2, Color c3, Color c4) {
        parallelograms.clear();
        baseWidth = baseW;
        height = h;
        for (int i = 0; i < count; i++) {
            textColor.push_back(WHITE);
        }
        for (int i = 0; i < count; i++) {
            HoverableParallelogram item;
            item.width = baseWidth;
            item.height = height;
            item.color1 = main;
            if (i == 0) item.color2 = c1;
            if (i == 1) item.color2 = c2;
            if (i == 2) item.color2 = c3;
            if (i >= 3) item.color2 = c4;
            item.scale = 1.0f;
            item.targetScale = 1.0f;
            item.center = { 0, y };
            parallelograms.push_back(item);
        }

        UpdateLayout();
    }

    void Update(float deltaTime) {
        Vector2 mouse = GetMousePosition();

        for (auto& p : parallelograms) {
            float halfW = (p.width * p.scale) / 2;
            float halfH = (p.height * p.scale) / 2;
            //Rectangle bounds = { p.center.x - halfW, p.center.y - halfH, p.width * p.scale, p.height * p.scale };
            //p.targetScale = CheckCollisionPointRec(mouse, bounds) ? p.hoverScale : 1.0f;
            p.targetScale = p.shape.CheckCollisionPointParallelogram(mouse) ? p.hoverScale : 1.0f;
        }

        bool needRelayout = false;
        for (auto& p : parallelograms) {
            float prev = p.scale;
            p.scale += (p.targetScale - p.scale) * deltaTime * p.scaleSpeed;
            if (std::abs(p.scale - prev) > 0.001f) needRelayout = true;
        }

        if (needRelayout) UpdateLayout();
    }

    void UpdateLayout() {
        float totalWidth = 0.0f;
        for (auto& p : parallelograms) {
            totalWidth += p.width * p.scale;
        }
        totalWidth += spacing * (parallelograms.size() - 1);

        float startX = (GetScreenWidth() - totalWidth) / 2.0f;
        float currentX = startX;

        for (auto& p : parallelograms) {
            p.center.x = currentX + (p.width * p.scale) / 2.0f;
            currentX += (p.width * p.scale) + spacing;
        }
    }

    void Draw() {
        for (auto& p : parallelograms) {
            float w = p.width * p.scale;
            float h = p.height * p.scale;
            float shear = 20.0f * p.scale;

            Vector2 p1 = { p.center.x - w / 2 + shear, p.center.y - h / 2 };
            Vector2 p2 = { p.center.x + w / 2 + shear, p.center.y - h / 2 };
            Vector2 p3 = { p.center.x + w / 2 - shear, p.center.y + h / 2 };
            Vector2 p4 = { p.center.x - w / 2 - shear, p.center.y + h / 2 };

            p.shape.DrawParallelogramGradient(p1, p2, p3, p4, p.color1, p.color2);
        }
    }
};


class MenuManager {
private:
    Texture2D logo, board;
    Button check, cross, setting, returnButton;
    Vector2 boardPosition, crossPosition, checkPosition, settingPosition, returnButtonPosition;

    Texture2D mario, luigi, toad, toadette;
    float scale = 0.2f;
    bool isActive;

public:
    Button playBoard, settingBoard, exitBoard, editingBoard;
    Button characterBoard, continueBoard, restartBoard, levelBoard, menuBoard;
    Button day_groundBoard, day_undergroundBoard, night_airshipBoard, night_snowBoard;
    SlideBar slideBarMusic, slideBarSound;
    Vector2 slideBarMusicPosition, slideBarSoundPosition;
    int select, characterSelect;
    bool settingDialog, exitDialog, exit;


    ParallelogramHoverManager manager;
    MenuManager();
    ~MenuManager();

    void HandleInput();
    void UpdateMenu(float deltaTime);
    void DrawMenu();

    void HandleRedirect();
    void UpdateRedirect(float deltaTime);
    void DrawRedirect();

    void HandleEditorSelecting();
    void UpdateEditorSelecting(float deltaTime);
    void DrawEditorSelecting();

    void HandleSelecting();
    void UpdateSelecting(float deltaTime);
    void DrawSelecting();

    bool HandleExit();
    void UpdateExit(float deltaTime);
    void DrawExit();

    void DrawSetting();
    void UpdateSetting(float deltaTime);
    void HandleSetting();

};

