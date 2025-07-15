#include "../../include/Game/GameContext.h"
#include "../../include/Game/GameStates.h"
#include <cstdlib>
#include "../../include/Game/GameStates.h"
#include "../../include/System/LevelEditor.h"
#include "../../include/System/PhysicsManager.h"
#include <raylib.h>
#include "../../include/System/Interface.h"

void MenuState::handleInput(GameContext& context) {
    context.menuManager.HandleInput();
    if (context.menuManager.HandleExit()) {
        CloseWindow();
        exit(0);
    }

    if (IsKeyPressed(KEY_ENTER) && !context.menuManager.dialog) {
        context.setState(context.gamePlayState);
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

    if (IsKeyPressed(KEY_TAB)) {
        LevelEditor::getInstance().toggleEditMode();
    }
}

void GamePlayState::update(GameContext& context, float deltaTime) {
    
    if (context.character) {
        context.character->update(deltaTime);
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
    DrawText("Press Enter", 500, 100, 20, BLACK);

    if (context.character) context.character->draw();
    for (auto obj : context.Objects)
    {
        obj->draw();
    }
    LevelEditor::getInstance().draw();
    PhysicsManager::getInstance().drawDebug();
    DrawFPS(20, 50);
    context.menuManager.DrawSetting();
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
