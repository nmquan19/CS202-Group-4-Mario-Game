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

    if (IsKeyPressed(KEY_TAB)) {
        LevelEditor::getInstance().toggleEditMode();
    }
}

void GamePlayState::update(GameContext& context, float deltaTime) {
    PhysicsManager::getInstance().update();
    LevelEditor::getInstance().update();

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

void EditorState::handleInput(GameContext& context) {
    context.menuManager.HandleSetting();

    if (IsKeyPressed(KEY_ENTER)) {
        context.setState(context.gameOverState);
    }
    if (IsKeyPressed(KEY_TAB)) {
        LevelEditor::getInstance().toggleEditMode();
    }
    if (IsKeyPressed(KEY_F9) && LevelEditor::getInstance().isInEditMode()) {
        LevelEditor::getInstance().clearLevel();
    }
    if (IsKeyPressed(KEY_F7) && LevelEditor::getInstance().isInEditMode()) {
        LevelEditor::getInstance().saveLevel("testlevel");
    }
    if (IsKeyPressed(KEY_F8) && LevelEditor::getInstance().isInEditMode()) {
        LevelEditor::getInstance().loadLevel("testlevel");
    }
}

void EditorState::update(GameContext& context, float deltaTime) {
    LevelEditor::getInstance().update();
    // PhysicsManager::getInstance().update();
}

void EditorState::draw(GameContext& context) {
    BeginDrawing();
    ClearBackground(WHITE);
    DrawText("Editor Mode", 500, 100, 20, BLACK);
    LevelEditor::getInstance().draw();
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
