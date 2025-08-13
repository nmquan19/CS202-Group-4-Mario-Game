#include <raylib.h>
#include <memory>
#include <string>
#include "../include/UI/SoundEffect.h"
#include "../include/UI/Menu.h"
#include "../include/UI/UI.h"
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
    RedirectState redirectState;
    CharacterSelectingState characterSelectingState;
    InformationState informationState;
    GamePlayState gamePlayState;
    EditorState editorState;
    EditorSelectingState editorSelectingState;
    GameOverState gameOverState;
    GameCameraSystem::getInstance().init();

    context->setGameStates(&menuState, &redirectState, &characterSelectingState, &informationState, &gamePlayState, &editorState, &editorSelectingState, &gameOverState);

    while (!WindowShouldClose()) {
        Box2DWorldManager::getInstance().setDebugDraw(true);

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
//	const int screenWidth = 800;
//	const int screenHeight = 600;
//	InitWindow(screenWidth, screenHeight, "Mario Game Demo");
//	
//
//	Vector2 startPos = { 20, 10 };
//	std::shared_ptr<One_Up> oneup = std::make_shared<One_Up>(startPos);
//
//
//	while (!WindowShouldClose()) {
//		float deltaTime = GetFrameTime();
//		oneup->update(deltaTime);
//		BeginDrawing();
//		ClearBackground(RAYWHITE);
//		oneup->draw();
//		EndDrawing();
//
//	}
//	CloseAudioDevice();
//	CloseWindow();
//	return 0;
//}

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



