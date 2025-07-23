#pragma once

#include <raylib.h>
#include <string>

class UIManager {
public:
    bool isGameOver;
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    Font menuFont;

    UIManager();
    ~UIManager();

    void ShowGameOver();
    bool IsGameOver() const;
    void DrawGameOver();
};