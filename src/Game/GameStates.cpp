#include "../../include/Game/GameContext.h"
#include "../../include/Game/GameStates.h"
#include <cstdlib>
#include "../../include/Game/GameStates.h"
#include "../../include/System/LevelEditor.h"
#include "../../include/System/PhysicsManager.h"
#include <raylib.h>
#include "../../include/System/Interface.h"
#include <memory>
#include "../../include/Characters/Character.h"
#include <iostream>

void handleCamera();

void MenuState::handleInput(GameContext& context) {
    context.menuManager.HandleInput();
    if (context.menuManager.HandleExit()) {
        CloseWindow();
        exit(0);
    }

    if (IsKeyPressed(KEY_ENTER) && context.menuManager.select == 0) {
        context.setState(context.gamePlayState);
    }

    if (IsKeyPressed(KEY_ENTER) && context.menuManager.select == 3) {
        context.setState(context.editorState);
    }
}

void MenuState::update(GameContext& context, float deltaTime) {
    // Empty for now
}

void MenuState::draw(GameContext& context) {
    BeginDrawing();
    context.menuManager.DrawMenu();
    EndDrawing();
}

void GamePlayState::handleInput(GameContext& context) {
    context.menuManager.HandleSetting();
    if (IsKeyPressed(KEY_ENTER)) {
        context.setState(context.gameOverState);
    }
}

void GamePlayState::update(GameContext& context, float deltaTime) {
    handleCamera();
    if (context.character) {
        std::shared_ptr<Character> character = std::dynamic_pointer_cast<Character>(context.character);
        character->update(deltaTime);
    }
    for (auto obj :context.Objects)
    {
		IUpdatable* updatableObj = dynamic_cast<IUpdatable*>(obj.get());
        if(updatableObj)updatableObj->update(deltaTime); 
    }
    context.spawnObject();  
    context.deleteObjects();
    context.menuManager.updateInformationBoard(deltaTime);
    PhysicsManager::getInstance().update();
    LevelEditor::getInstance().update();
}

void GamePlayState::draw(GameContext& context) {
    BeginDrawing();
    ClearBackground(SKYBLUE);
    BeginMode2D(GameContext::getInstance().camera);
    DrawText("Press Enter", 500, 100, 20, BLACK);

    //DrawText("Press Enter", 500, 100, 20, BLACK);
    
    // Note: In GamePlayState, using draw of GameContext and Physics(for debug) instead of Level Editor!
    if (context.character) {
        context.character->draw();
    }
    for (auto obj : context.Objects)
    {
        obj->draw();
    }
    PhysicsManager::getInstance().drawDebug();
    EndMode2D();
    DrawFPS(20, 50); 
    context.menuManager.DrawSetting();
    context.menuManager.drawInformationBoard();
    EndDrawing();
}

void EditorState::handleInput(GameContext& context) {
    context.menuManager.HandleSetting();

    if (IsKeyPressed(KEY_ENTER)) {
        context.setState(context.gameOverState);
    }
    if (IsKeyPressed(KEY_F9) && LevelEditor::getInstance().isInEditMode()) {
        LevelEditor::getInstance().clearLevel();
    }
    if (IsKeyPressed(KEY_F7) && LevelEditor::getInstance().isInEditMode()) {
        LevelEditor::getInstance().saveLevel("testlevel.json");
    }
    if (IsKeyPressed(KEY_F8) && LevelEditor::getInstance().isInEditMode()) {
        LevelEditor::getInstance().loadLevel("testlevel.json");
    }
}

void EditorState::update(GameContext& context, float deltaTime) {
    handleCamera();
    LevelEditor::getInstance().update();
}

void EditorState::draw(GameContext& context) {
    BeginDrawing();
    ClearBackground(WHITE);
    DrawText("Editor Mode", 500, 100, 20, BLACK);
    LevelEditor::getInstance().draw();
    DrawFPS(20, 50);
    EndDrawing();
}

void GameOverState::handleInput(GameContext& context) {
    if (IsKeyPressed(KEY_ENTER)) {
        context.setState(context.menuState);
    }
}

void GameOverState::update(GameContext& context, float deltaTime) {
    // To be implemented
}

void GameOverState::draw(GameContext& context) {
    BeginDrawing();
    ClearBackground(WHITE);
    context.uiManager.DrawGameOver();
    EndDrawing();
}

void handleCamera() {
    GameContext::getInstance().camera.zoom = expf(logf(GameContext::getInstance().camera.zoom) + ((float)GetMouseWheelMove()*0.1f));
    if (GameContext::getInstance().camera.zoom > 2.0f) GameContext::getInstance().camera.zoom = 2.0f;
    else if (GameContext::getInstance().camera.zoom < 1.0f) GameContext::getInstance().camera.zoom = 1.0f; 
    if (GameContext::getInstance().character) {
        GameContext::getInstance().camera.target = GameContext::getInstance().character->getPosition();

        Vector2 target = GameContext::getInstance().camera.target;

        Vector2 topLeft = GetWorldToScreen2D({0,0}, GameContext::getInstance().camera);
        Vector2 bottomRight = GetWorldToScreen2D({(float)GetScreenWidth(), (float)GetScreenHeight()}, GameContext::getInstance().camera);

        float cameraWidth = bottomRight.x - topLeft.x;
        float cameraHeight = bottomRight.y - topLeft.y;

        // Calculate camera bounds
        float minCameraX = 0 + cameraWidth * 0.5f;
        float maxCameraX = 0 + 5000 - cameraWidth * 0.5f;
        float minCameraY = 0 + cameraHeight * 0.5f;
        float maxCameraY = 0 + GetScreenHeight() - cameraHeight * 0.5f;

        // Clamp the target
        target.x = fmaxf(minCameraX, fminf(target.x, maxCameraX));
        target.y = fmaxf(minCameraY, fminf(target.y, maxCameraY));

        GameContext::getInstance().camera.target = target;
    }
}