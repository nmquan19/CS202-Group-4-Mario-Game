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
    GameContext::getInstance().camera.zoom = expf(logf(GameContext::getInstance().camera.zoom) + ((float)GetMouseWheelMove()*0.1f));
    if (GameContext::getInstance().camera.zoom > 3.0f) GameContext::getInstance().camera.zoom = 3.0f;
    else if (GameContext::getInstance().camera.zoom < 1.0f) GameContext::getInstance().camera.zoom = 1.0f; 

    if (context.character) {
        std::shared_ptr<Character> character = std::dynamic_pointer_cast<Character>(context.character);
        character->update(deltaTime);
        // Get character position
        Vector2 characterPos = character->getPosition();
        GameContext::getInstance().camera.offset = characterPos;
        
        // Define world bounds (adjust these values based on your level size)
        float worldWidth = 3000.0f;  // Your level width
        float worldHeight = 2000.0f; // Your level height
        float leftBound = 0.0f;      // Left boundary
        float bottomBound = worldHeight; // Bottom boundary (assuming Y increases downward)
        
        // Calculate camera bounds based on screen size and zoom
        float screenWidth = GetScreenWidth();
        float screenHeight = GetScreenHeight();
        float halfScreenWidth = screenWidth / (2.0f * GameContext::getInstance().camera.zoom);
        float halfScreenHeight = screenHeight / (2.0f * GameContext::getInstance().camera.zoom);
        
        // Clamp camera target to world bounds
        Vector2 cameraTarget = characterPos;
        
        // Prevent camera from going past left boundary
        if (cameraTarget.x - halfScreenWidth < leftBound) {
            cameraTarget.x = 0;
        }
        
        // Prevent camera from going past bottom boundary
        if (cameraTarget.y + halfScreenHeight > bottomBound) {
            cameraTarget.y = bottomBound - halfScreenHeight;
        }
        
        // Prevent camera from going past top boundary (optional)
        if (cameraTarget.y - halfScreenHeight < 0) {
            cameraTarget.y = halfScreenHeight;
        }
        
        // Apply the clamped target to camera
        GameContext::getInstance().camera.target = cameraTarget;
    }
    for (auto obj :context.Objects)
    {
		IUpdatable* updatableObj = dynamic_cast<IUpdatable*>(obj.get());
        if(updatableObj)updatableObj->update(deltaTime); 
    }
    context.spawnObject();  
    context.deleteObjects();
    PhysicsManager::getInstance().update();
    LevelEditor::getInstance().update();
}

void GamePlayState::draw(GameContext& context) {
    BeginDrawing();
    ClearBackground(WHITE);
    BeginMode2D(GameContext::getInstance().camera);
    DrawText("Press Enter", 500, 100, 20, BLACK);

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
    if (context.character) {
        context.hud->draw();
    }
    DrawFPS(20, 50);
    context.menuManager.DrawSetting();
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
