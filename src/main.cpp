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
#include "../include/System/LightingSystem.h"
int main() {
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Mario Game Demo");
    InitAudioDevice();
    SetTargetFPS(60);

    GameContext* context = &GameContext::getInstance();
    MenuState menuState;
    RedirectState redirectState;
    PlayerSelectingState playerSelectingState;
    CharacterSelectingState characterSelectingState;
    LevelRedirectState levelRedirectState;
    LevelSelectingState levelSelectingState;
    InformationState informationState;
    GamePlayState gamePlayState;
    EditorState editorState;
    EditorSelectingState editorSelectingState;
    GameOverState gameOverState;
    GameCameraSystem::getInstance().init();

    context->setGameStates(&menuState, &redirectState, &playerSelectingState, &characterSelectingState, &levelRedirectState, &levelSelectingState, &informationState, &gamePlayState, &editorState, &editorSelectingState, &gameOverState);
    LightingManager::getInstance().loadShader("assets/shaders/lightsource.fs");
    LightingManager::getInstance().setAmbientColor(WHITE); // Dark blue 
    while (!WindowShouldClose()) {
        //Box2DWorldManager::getInstance().setDebugDraw(true);

        float deltaTime = GetFrameTime();
        context->handleInput();
        context->update(deltaTime);
        context->draw();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

