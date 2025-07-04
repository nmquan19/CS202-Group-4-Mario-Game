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
#include "../include/Characters/CharacterFactory.h"
#include "../include/Enemy/Goomba/Goomba.h"
#include "../include/System/Resources.h"
#include <memory>
#include "../include/System/Interface.h"



/*
float deltaTime = GetFrameTime();
std::unique_ptr<Character> character;
class ScenePlayerState {
protected:
    AudioManager audioManager;
    UIManager uiManager;
    MenuManager menuManager;

public:
    std::string currentState;
    virtual ~ScenePlayerState() {}
    virtual void draw(ScenePlayer* scene) {};
    virtual void logic(ScenePlayer* scene) {};
};
class MenuState : public ScenePlayerState {
    virtual ~MenuState() {}
    void draw(ScenePlayer* scene) {
        menuManager.DrawMenu();
    }
    void logic(ScenePlayer* scene) {
        if (IsKeyPressed(KEY_ENTER) && !menuManager.dialog) {
            scene->setter(ScenePlayer::GAME);
            currentState = "GAME";
        }
    }
};
class GameState : public ScenePlayerState {
    virtual ~GameState() {}
    void draw(ScenePlayer* scene) {
        ClearBackground(WHITE);
        DrawText("Press Enter", 500, 100, 20, BLACK);
        PhysicsManager::getInstance().update();
        LevelEditor::getInstance().update();
        if (character) character->update(deltaTime);
        if (character) character->draw();
        if (IsKeyPressed(KEY_TAB)) {
            LevelEditor::getInstance().toggleEditMode();
        }
        PhysicsManager::getInstance().drawDebug();
        LevelEditor::getInstance().draw();
        DrawFPS(20, 50);

    }
    void logic(ScenePlayer* scene) {
        menuManager.HandleInput();
        if (IsKeyPressed(KEY_ENTER) && !menuManager.dialog) {
            scene->setter(ScenePlayer::GAME_OVER);
            currentState = "GAME_OVER";
        }
    }
};
class Game_overState : public ScenePlayerState {
    virtual ~Game_overState() {}
    void draw(ScenePlayer* scene) {
        ClearBackground(WHITE);
        uiManager.DrawGameOver();
    }
    void logic(ScenePlayer* scene) {
        if (IsKeyPressed(KEY_ENTER)) {
            scene->setter(ScenePlayer::MENU);
            currentState = "MENU";
        }
    }
};
class PauseState : public ScenePlayerState {
    virtual ~PauseState() {}
    void draw() {

    }
    void logic(ScenePlayer* scene) {
        if (IsKeyPressed(KEY_ENTER)) {
            scene->setter(ScenePlayer::GAME);
            currentState = "GAME";
        }
    }
};

class ScenePlayer {
private:
    ScenePlayerState* pPreviousState;
    ScenePlayerState* pState;
public:
    enum State {
        MENU,
        GAME,
        PAUSE,
        GAME_OVER
    };
    ScenePlayer() {
        pState = new MenuState();
        pPreviousState = pState;
    }
    virtual ~ScenePlayer() { delete pPreviousState; delete pState; }
    void draw() {
        pState->draw(this);
    }
    void logic() {
        pState->logic(this);
    }
    bool isGameScene() {
        if (pState->currentState == "GAME") return true;
        return false;
    }
    void setter(State state) {
        delete pPreviousState;
        if (state == MENU) { pPreviousState = pState; pState = new MenuState(); }
        else if (state == GAME) { pPreviousState = pState; pState = new GameState(); }
        else if (state == PAUSE) { pPreviousState = pState; pState = new PauseState(); }
        else if (state == GAME_OVER) { pPreviousState = pState; pState = new Game_overState(); }
    }
    void setMario() {
        if (pState->currentState != pPreviousState->currentState) {
            if (pPreviousState->currentState == "GAME") {
                LevelEditor::getInstance().cleanup();
                PhysicsManager::getInstance().cleanup();
            }

            if (pState->currentState == "GAME") {
                PhysicsManager::getInstance().setWorldBounds({ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() });
                character = std::make_unique<Character>(CharacterType::MARIO, Vector2{ 500, 500 });
            }
            pPreviousState = pState;
        }
    }
};
int main() {
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Mario Game Demo");
    InitAudioDevice();
    SetTargetFPS(60);
    ScenePlayer scenePlayer;
    TextureManager::getInstance().loadTextures();
    while (!WindowShouldClose()) {
        scenePlayer.setMario();
        scenePlayer.logic();
        BeginDrawing();
        scenePlayer.draw();
        EndDrawing();
    }
    if (scenePlayer.isGameScene()) {
        LevelEditor::getInstance().cleanup();
        PhysicsManager::getInstance().cleanup();
    }
    TextureManager::getInstance().unloadTextures();

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
*/















int main() {
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Mario Game Demo");
    InitAudioDevice();
    SetTargetFPS(60);

    AudioManager audioManager;
    UIManager uiManager;
    MenuManager menuManager;

    //audioManager.LoadBackgroundMusic("some music path");
    //audioManager.PlayBackgroundMusic();

    TextureManager::getInstance().loadTextures();

    enum class GameState { MENU, GAME, GAME_OVER };
    GameState state = GameState::MENU;
    GameState previousState = GameState::MENU;
    Resources::Load();

    std::unique_ptr<Character> character;
    std::unique_ptr<Goomba> goomba; 
    while (!WindowShouldClose()) {

        float deltaTime = GetFrameTime();

        if (state != previousState) {
            if (previousState == GameState::GAME) {
                LevelEditor::getInstance().cleanup();
                PhysicsManager::getInstance().cleanup();
            }

            if (state == GameState::GAME) {
                PhysicsManager::getInstance().setWorldBounds({ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() });
                character = CharacterFactory::createCharacter(CharacterType::MARIO, Vector2{ 500, 500 });
                goomba = std::make_unique<Goomba>(
                    Vector2{ 700, 0 }, Vector2{ 70, 0 }, Vector2{ 0, 0 }
                );
            
            }
            previousState = state;
        }
        switch (state) {
        case GameState::MENU:
            menuManager.HandleInput();
            if (menuManager.HandleExit()) {
                CloseWindow();
                return 0;
            }
            if (IsKeyPressed(KEY_ENTER) && !menuManager.dialog) {
                state = GameState::GAME;
            }
            break;
        case GameState::GAME:
            menuManager.HandleSetting();
            if (IsKeyPressed(KEY_ENTER)) {
                state = GameState::GAME_OVER;
            }
            break;

        case GameState::GAME_OVER:
            //change sample
            if (IsKeyPressed(KEY_ENTER)) {
                state = GameState::MENU;
            }
            break;
        }

        BeginDrawing();

        switch (state) {
        case GameState::MENU:
            menuManager.DrawMenu();
            break;
        case GameState::GAME:

            ClearBackground(WHITE);
            DrawText("Press Enter", 500, 100, 20, BLACK);
            PhysicsManager::getInstance().update();
            LevelEditor::getInstance().update();
            if (character) character->update(deltaTime);
            if (character) character->draw();
            if (goomba)
            {
                goomba->update(deltaTime);
                goomba->draw();
            }
            if (IsKeyPressed(KEY_TAB)) {    
                LevelEditor::getInstance().toggleEditMode();
            }
            PhysicsManager::getInstance().drawDebug();
            LevelEditor::getInstance().draw();
            DrawFPS(20, 50);
            menuManager.DrawSetting();
            break;
        case GameState::GAME_OVER:
            //change sample
            ClearBackground(WHITE);
            uiManager.DrawGameOver();
            break;
        }
        EndDrawing();
    }

    if (state == GameState::GAME) {
        LevelEditor::getInstance().cleanup();
        PhysicsManager::getInstance().cleanup();
    }
    TextureManager::getInstance().unloadTextures();
    Resources::UnLoad();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

