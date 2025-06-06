#include <raylib.h>
#include "../include/UI/SoundEffect.h"
#include "../include/UI/Menu.h"
#include "../include/UI/UI.h"
#include "../include/Item/Item.h"
#include "../include/Characters/Player.h"


int main() {
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Mario Game Demo");
    InitAudioDevice();
    SetTargetFPS(120);

    AudioManager audioManager;
    UIManager uiManager;
    MenuManager menuManager;

    Player player({ 100, 100 }, { 50, 50 });

    std::vector<Item*> items;
    items.push_back(new Coin({ 300, 50 }));
    items.push_back(new Mushroom({ 500, 100 }));

    

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
        case GameState::GAME:{
            float dt = GetFrameTime();

            player.Update(dt);
            for (Item* item : items) {
                item->Update(dt);
            }

            // Kiểm tra va chạm thu thập coin, va chạm với item khác
            for (Item* item : items) {
                Coin* coin = dynamic_cast<Coin*>(item);
                if (coin && !coin->Collected() && CheckCollisionRecs(player.GetRect(), coin->GetRect())) {
                    coin->Collect();
                }
                else if (!coin && CheckCollisionRecs(player.GetRect(), item->GetRect())) {
                    // Xử lý va chạm với item khác (nếu cần)
                }
            }

            if (IsKeyPressed(KEY_ENTER)) {
                state = GameState::GAME_OVER;
            }
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
        {
            ClearBackground(RAYWHITE);

            player.Draw();

            bool isColliding = false;
            for (Item* item : items) {
                Coin* coin = dynamic_cast<Coin*>(item);
                if (coin && !coin->Collected() && CheckCollisionRecs(player.GetRect(), coin->GetRect())) {
                    isColliding = true;
                }
                else if (!coin && CheckCollisionRecs(player.GetRect(), item->GetRect())) {
                    isColliding = true;
                }

                item->Draw();
            }

            if (isColliding) {
                DrawText("Collision!", 10, 10, 30, RED);
            }
        }



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
