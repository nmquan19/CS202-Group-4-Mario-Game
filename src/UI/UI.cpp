#include "../../include/UI/UI.h"
#include <raylib.h>

UIManager& UIManager::getInstance() {
    static UIManager instance;
    return instance;
}

UIManager::UIManager() : score({ GetScreenWidth() / 10.0f, GetScreenHeight() / 10.0f }),
coin({ GetScreenWidth() * 3 / 10.0f, GetScreenHeight() / 10.0f }, "./assets/coin_sprite.png"),
world({ GetScreenWidth() * 5 / 10.0f, GetScreenHeight() / 10.0f }, "1 - 1"),
timer({ GetScreenWidth() * 7 / 10.0f, GetScreenHeight() / 10.0f }, 400.0f),
isGameOver(false) {
    menuFont = LoadFont("./assets/font_sprites.png");
}

UIManager::~UIManager() {
    UnloadFont(menuFont);
}


void UIManager::ShowGameOver() {
    isGameOver = true;
}

bool UIManager::IsGameOver() const {
    return isGameOver;
}

void UIManager::DrawGameOver() {
    if (!isGameOver) return;
    DrawTextEx(menuFont, "Game Over", { 300, 200 }, 40, 2, RED);
}