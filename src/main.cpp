#include <raylib.h>
#include "../include/UI/SoundEffect.h"
#include "../include/UI/Menu.h"
#include "../include/UI/UI.h"
#include "../include/System/PhysicsManager.h"
#include "../include/System/LevelEditor.h"
#include "../include/System/TextureManager.h"

int main() {
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Mario Game Demo");
    InitAudioDevice();
    SetTargetFPS(120);

    AudioManager audioManager;
    UIManager uiManager;
    MenuManager menuManager;

    //audioManager.LoadBackgroundMusic("some music path");
    //audioManager.PlayBackgroundMusic();

    TextureManager::getInstance().loadTextures();

    enum class GameState { MENU, GAME, GAME_OVER };
    GameState state = GameState::MENU;
    GameState previousState = GameState::MENU;

    while (!WindowShouldClose()) {

        if (state != previousState) {
            if (previousState == GameState::GAME) {
                LevelEditor::getInstance().cleanup();
                PhysicsManager::getInstance().cleanup();
            }
            
            if (state == GameState::GAME) {
                PhysicsManager::getInstance().setWorldBounds({0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()});
            }
            
            previousState = state;
        }

        switch (state) {
        case GameState::MENU:
            menuManager.HandleInput();
            if (IsKeyPressed(KEY_ENTER)) {
                state = GameState::GAME;
            }
            break;
        case GameState::GAME:
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
            ClearBackground(WHITE);
            DrawText("Press Enter", 500, 100, 20, BLACK);
            PhysicsManager::getInstance().update();
            LevelEditor::getInstance().update();
            if (IsKeyPressed(KEY_TAB)) {
                LevelEditor::getInstance().toggleEditMode();
            }
            PhysicsManager::getInstance().drawDebug();
            LevelEditor::getInstance().draw();
            DrawFPS(20, 50);

            break;
        case GameState::GAME_OVER:
            //change sample
            ClearBackground(WHITE);
            uiManager.DrawGameOver();
            break;
        }

        EndDrawing();
    }

    if (state == GameState::GAME) {
        LevelEditor::getInstance().cleanup();
        PhysicsManager::getInstance().cleanup();
    }
    TextureManager::getInstance().unloadTextures();

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
