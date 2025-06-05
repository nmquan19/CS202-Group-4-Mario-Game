#include <raylib.h>
#include "../include/UI/SoundEffect.h"
#include "../include/UI/Menu.h"
#include "../include/UI/UI.h"

int main() {
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Mario Game Demo");
    InitAudioDevice();
    SetTargetFPS(120);

    AudioManager audioManager;
    UIManager uiManager;
    MenuManager menuManager;
    

    //audioManager.LoadBackgroundMusic("some music path");
    //audioManager.PlayBackgroundMusic();

    enum class GameState { MENU, GAME, GAME_OVER };
    GameState state = GameState::MENU;

    while (!WindowShouldClose()) {
        //audioManager.UpdateBackgroundMusic();
        switch (state) {
        case GameState::MENU:
            menuManager.HandleInput();
            if (IsKeyPressed(KEY_ENTER)) {
                state = GameState::GAME;
            }
            break;
        case GameState::GAME:
            //change sample
            if (IsKeyPressed(KEY_ENTER)) {
                state = GameState::GAME_OVER;
            }
            break;

        case GameState::GAME_OVER:
            //change sample
            if (IsKeyPressed(KEY_ENTER)) {
                state = GameState::MENU;
            }
            break;
        }

        BeginDrawing();

        switch (state) {
        case GameState::MENU:
            menuManager.DrawMenu();
            // KEY_U, KEY_UP: 1 player, KEY_D, KEY_DOWN: 2 players, Enter for next state
            break;
        case GameState::GAME:
            //change sample
            ClearBackground(WHITE);
            DrawText("Press Enter", 500, 100, 20, BLACK);
            break;
        case GameState::GAME_OVER:
            //change sample
            ClearBackground(WHITE);
            uiManager.DrawGameOver();
            break;
        }

        EndDrawing();
    }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
