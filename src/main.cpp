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


#include "raylib.h"
#include "raymath.h"
#include <vector>


class Coin {
public:
    Vector2 position;
    Texture2D texture;
    Rectangle frameRec;
    int frame;               // 0 -> 3
    float frameTime;         // thời gian mỗi frame
    float timer;             // bộ đếm

    Coin(Vector2 pos, Texture2D tex) {
        position = pos;
        texture = tex;
        frame = 0;
        frameTime = 0.2f;
        timer = 0.0f;
        frameRec = { 0.0f, 0.0f, 100.0f, 100.0f };
    }

    void Update(float deltaTime) {
        timer += deltaTime;
        if (timer >= frameTime) {
            timer = 0.0f;
            frame = (frame + 1) % 4; // quay vòng 0 -> 3
            frameRec.x = frame * 100.0f;
        }
    }

    void Draw() {
        DrawTextureRec(texture, frameRec, position, WHITE);
    }

    ~Coin() {
        UnloadTexture(texture); // chỉ nếu bạn sở hữu texture riêng
    }
};

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Coin Animation Test");
    SetTargetFPS(60);

    // Load texture với đường dẫn đúng
    Texture2D coinTexture = LoadTexture("../assets/item/item_spritesheet.png");

    // Frame animation setup
    const int frameWidth = 100;
    const int frameHeight = 100;
    const int frameCount = 4;
    int currentFrame = 0;

    Rectangle sourceRec = { 0, 0, (float)frameWidth, (float)frameHeight };
    Vector2 position = { screenWidth / 2.0f - frameWidth / 2.0f, screenHeight / 2.0f - frameHeight / 2.0f };

    float frameDuration = 0.2f;  // mỗi frame kéo dài 0.2 giây
    float frameTimer = 0.0f;

    while (!WindowShouldClose()) {
        // Cập nhật frame animation
        frameTimer += GetFrameTime();
        if (frameTimer >= frameDuration) {
            frameTimer = 0.0f;
            currentFrame = (currentFrame + 1) % frameCount;
            sourceRec.x = (float)(currentFrame * frameWidth);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Coin animation example", 10, 10, 20, DARKGRAY);
        DrawTextureRec(coinTexture, sourceRec, position, WHITE);

        EndDrawing();
    }

    UnloadTexture(coinTexture);
    CloseWindow();

    return 0;
}


