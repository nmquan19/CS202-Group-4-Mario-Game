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
        float deltaTime = GetFrameTime();
        context->handleInput();
        context->update(deltaTime);
        context->draw();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}