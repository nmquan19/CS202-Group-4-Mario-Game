#include <raylib.h>
#include "../include/Game/GameContext.h"
#include "../include/Game/GameStates.h"
int main() {
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Mario Game Demo");
    InitAudioDevice();
    SetTargetFPS(60);

    GameContext* context = &GameContext::getInstance(); 
    MenuState menuState;
    GamePlayState gamePlayState;
    GameOverState gameOverState;

    context->setGameStates(&menuState, &gamePlayState, &gameOverState);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        context->handleInput();
        context->update(deltaTime);
        context->draw();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}