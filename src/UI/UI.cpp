#include "../../include/UI/UI.h"
UIManager::UIManager() : isGameOver(false) {
    menuFont = LoadFont("./assets/romulus.png");
    
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

