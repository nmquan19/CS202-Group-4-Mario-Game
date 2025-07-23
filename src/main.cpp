//#include <raylib.h>
#include <memory>
#include <string>
#include "../include/UI/SoundEffect.h"
#include "../include/UI/Menu.h"
#include "../include/UI/UI.h"
#include "../include/System/PhysicsManager.h"
#include "../include/System/LevelEditor.h"
#include "../include/System/TextureManager.h"
#include "../include/Characters/Character.h"
#include "../include/Enemy/Goomba/Goomba.h"
#include "../include/System/Interface.h"
#include "../include/Game/GameContext.h"
#include "../include/Game/GameStates.h"
#include "../include/Item/Coin/Coin.h"
#include "../include/Item/Mushroom/Mushroom.h"
#include "../include/Item/Fire_Flower/Fire_Flower.h"
#include "../include/Item/Star/Star.h"
#include "../include/Item/One_Up/One_Up.h"

//int main() {
//    InitWindow(GetScreenWidth(), GetScreenHeight(), "Mario Game Demo");
//    InitAudioDevice();
//    SetTargetFPS(60);
//
//    GameContext* context = &GameContext::getInstance(); 
//    MenuState menuState;
//    GamePlayState gamePlayState;
//    EditorState editorState;
//    GameOverState gameOverState;
//
//    context->setGameStates(&menuState, &gamePlayState, &editorState, &gameOverState);
//
//    while (!WindowShouldClose()) {
//        float deltaTime = GetFrameTime();
//        context->handleInput();
//        context->update(deltaTime);
//        context->draw();
//    }
//
//    CloseAudioDevice();
//    CloseWindow();
//    return 0;
//}

int main() {
    InitWindow(600, 400, "Animation");

    Texture2D texture = LoadTexture("assets/item/item_spritesheet.png");

	
    Coin coin({ 300, 100 });
    Mushroom mushroom({ 200, 100 });
    Fire_Flower fire_flower({ 100, 100 });
    Star star({ 400, 100 });
    One_Up one_up({ 500, 100 });

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        coin.update(deltaTime);
        mushroom.update(deltaTime);
        fire_flower.update(deltaTime);
        star.update(deltaTime);
        one_up.update(deltaTime);

        BeginDrawing();
        ClearBackground(SKYBLUE);

        coin.draw(texture);
        mushroom.draw(texture);
        fire_flower.draw(texture);
        star.draw(texture);
        one_up.draw(texture);

        EndDrawing();
    }

    UnloadTexture(texture);
    CloseWindow();

    return 0;
}


