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
//#include "../include/Game/GameContext.h"
//#include "../include/Game/GameStates.h"

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

#include "raylib.h"
#include <memory>

#include "../include/Objects/ObjectFactory.h"
#include "../include/System/PhysicsManager.h"
#include <iostream>

//int main() {
//    const int screenWidth = 800;
//    const int screenHeight = 600;
//
//    InitWindow(screenWidth, screenHeight, "Test Shared Item with PhysicsManager");
//    SetTargetFPS(60);
//
//    // Đặt world bounds cho PhysicsManager (nếu cần)
//    PhysicsManager::getInstance().setWorldBounds({ 0, 0, (float)screenWidth, (float)screenHeight });
//
//    // Tạo item và thêm vào PhysicsManager
//    auto coin = ObjectFactory::createItem(ItemType::COIN, { 100, 200 }, { 16, 16 });
//    auto mushroom = ObjectFactory::createItem(ItemType::MUSHROOM, { 300, 200 }, { 16, 16 });
//
//    // Convert std::unique_ptr to std::shared_ptr before adding to PhysicsManager
//    PhysicsManager::getInstance().addObject(std::shared_ptr<Object>(std::move(coin)));
//    //PhysicsManager::getInstance().addObject(std::shared_ptr<Object>(std::move(mushroom)));
//
//    while (!WindowShouldClose()) {
//        BeginDrawing();
//        ClearBackground(RAYWHITE);
//
//        // Update logic (nếu có animation sẽ hoạt động)
//        PhysicsManager::getInstance().update();
//
//        // Gọi vẽ tất cả object đã quản lý
//        for (auto obj : PhysicsManager::getInstance().getObjectsInLayer(ObjectCategory::ITEM)) {
//            if (obj->isActive()) obj->draw();
//        }
//
//        // Optional: debug quadtree
//        // PhysicsManager::getInstance().drawDebug();
//
//        DrawText("Item test - ESC to exit", 10, 10, 20, DARKGRAY);
//
//        EndDrawing();
//    }
//
//    CloseWindow();
//    return 0;
//}


int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Palette Test");
    SetTargetFPS(60);

    TextureManager::getInstance().loadTextures();
    std::cout <<"Item Size: " << TextureManager::getInstance().getSizeofItemSprite();

    ObjectPalette palette;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        palette.handleSelection();
        palette.drawPalette();      
    
        EndDrawing();
    }

    TextureManager::getInstance().unloadTextures();
    CloseWindow();
    return 0;
}


