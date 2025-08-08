#include <raylib.h>
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
#include "../include/System/CameraSystem.h"
#include "../include/Objects/Block.h"
#include "../include/System/Box2DWorldManager.h"

int main() {
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Mario Game Demo");
    InitAudioDevice();
    SetTargetFPS(60);

    GameContext* context = &GameContext::getInstance(); 
    MenuState menuState;
    CharacterSelectingState characterSelectingState;
    GamePlayState gamePlayState;
    EditorState editorState;
    GameOverState gameOverState;
    GameCameraSystem::getInstance().init();

    Box2DWorldManager::getInstance().setDebugDraw(true);

    context->setGameStates(&menuState, &characterSelectingState, &gamePlayState, &editorState, &gameOverState);

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

//int main() {
//    const int screenWidth = 800;
//    const int screenHeight = 600;
//    InitWindow(screenWidth, screenHeight, "Test Blocks Demo");
//
//    SetTargetFPS(60);
//
//    GameContext gameContext;
//    gameContext.createTestBlocks();  
//
//    while (!WindowShouldClose()) {
//        float deltaTime = GetFrameTime();
//
//        // Cập nhật toàn bộ thế giới (nếu có)
//        gameContext.update(deltaTime);
//
//        BeginDrawing();
//        ClearBackground(RAYWHITE);
//
//        // Vẽ toàn bộ thế giới
//        gameContext.draw();
//
//        EndDrawing();
//    }
//
//    CloseWindow();
//    return 0;
//}

//int main() {
//    InitWindow(600, 400, "Item Animation");
//
//    Texture2D texture = LoadTexture("assets/item/item_spritesheet.png");
//
//	
//    Coin coin({ 300, 100 });
//    Mushroom mushroom({ 200, 100 });
//    Fire_Flower fire_flower({ 100, 100 });
//    Star star({ 400, 100 });
//    One_Up one_up({ 500, 100 });
//
//    while (!WindowShouldClose()) {
//        float deltaTime = GetFrameTime();
//        coin.update(deltaTime);
//        mushroom.update(deltaTime);
//        fire_flower.update(deltaTime);
//        star.update(deltaTime);
//        one_up.update(deltaTime);
//
//        BeginDrawing();
//        ClearBackground(SKYBLUE);
//
//        coin.draw(texture);
//        mushroom.draw(texture);
//        fire_flower.draw(texture);
//        star.draw(texture);
//        one_up.draw(texture);
//
//        EndDrawing();
//    }
//
//    UnloadTexture(texture);
//    CloseWindow();
//
//    return 0;
//}

//int main() {
//    const int screenWidth = 800;
//    const int screenHeight = 600;
//    InitWindow(screenWidth, screenHeight, "Block Line Test");
//
//    SetTargetFPS(60);
//
//    // Tạo một vector lưu các block
//    std::vector<std::shared_ptr<Block>> blocks;
//
//    // Tạo 1 hàng block tại y = 384 pixels => gridY = 384 / GRID_SIZE = 12
//    int gridY = 12;
//    for (int gridX = 0; gridX <= 10; ++gridX) {
//        Vector2 gridPos = { (float)gridX, (float)gridY };
//        blocks.push_back(std::make_shared<GroundBlock>(gridPos));
//    }
//
//    // Load texture blocks (bắt buộc nếu bạn muốn thấy hình)
//    TextureManager::blocksTexture = LoadTexture("assets/blocks.png");  // Đặt đúng đường dẫn ảnh
//
//    while (!WindowShouldClose()) {
//        float deltaTime = GetFrameTime();
//
//        // Update
//        for (auto& block : blocks) {
//            block->update(deltaTime);
//        }
//
//        // Draw
//        BeginDrawing();
//        ClearBackground(RAYWHITE);
//
//        for (auto& block : blocks) {
//            block->draw();
//        }
//
//        EndDrawing();
//    }
//
//    UnloadTexture(TextureManager::blocksTexture);
//    CloseWindow();
//    return 0;
//}