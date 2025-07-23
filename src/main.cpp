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
    //Mushroom mushroom({ 200, 100 });
    //Fire_Flower fire_flower({ 100, 100 });
    //Star star({ 400, 100 });
    //One_Up one_up({ 500, 100 });

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        coin.update(deltaTime);
        //mushroom.Update(deltaTime);
        //fire_flower.Update(deltaTime);
        //star.Update(deltaTime);
        //one_up.Update(deltaTime);

        BeginDrawing();
        ClearBackground(SKYBLUE);

        coin.draw(texture);
        //mushroom.Draw(texture);
        //fire_flower.Draw(texture);
        //star.Draw(texture);
        //one_up.Draw(texture);

        EndDrawing();
    }

    UnloadTexture(texture);
    CloseWindow();

    return 0;
}


