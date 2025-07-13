#include <raylib.h>
#include <memory>
#include "../include/UI/SoundEffect.h"
#include "../include/UI/Menu.h"
#include "../include/UI/UI.h"
#include "../include/System/PhysicsManager.h"
#include "../include/System/LevelEditor.h"
#include "../include/System/TextureManager.h"
#include "../include/Characters/Character.h"
#include "../include/Enemy/Goomba/Goomba.h"
#include "../include/System/Interface.h"
#include "../include/Objects/ObjectFactory.h"

class GameState {
public:
    virtual ~GameState() = default;
    virtual void handleInput(class GameContext& context) = 0;
    virtual void update(class GameContext& context, float deltaTime) = 0;
    virtual void draw(class GameContext& context) = 0;
};


class GameContext {
public:
    AudioManager audioManager;
    UIManager uiManager;
    MenuManager menuManager;
    std::unique_ptr<Character> character;
    std::unique_ptr<Goomba> goomba;
    GameState* previousState = nullptr;

    GameContext() {
        TextureManager::getInstance().loadTextures();
    }

    ~GameContext() {
        if (previousState) {
            LevelEditor::getInstance().cleanup();
            PhysicsManager::getInstance().cleanup();
        }
        TextureManager::getInstance().unloadTextures();
    }

    void setState(GameState* newState) {
        if (currentState != newState) {
            if (currentState == gamePlayState && newState != gamePlayState) {
                LevelEditor::getInstance().cleanup();
                PhysicsManager::getInstance().cleanup();
                character.reset();
                goomba.reset();
            }
            if (newState == gamePlayState) {
                PhysicsManager::getInstance().setWorldBounds({ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() });
                character = ObjectFactory::createCharacter(CharacterType::MARIO, Vector2{ 500, 500 });
                goomba = std::make_unique<Goomba>(Vector2{ 700, 0 }, Vector2{ 70, 0 }, Vector2{ 0, 0 });
            }
            previousState = currentState;
            currentState = newState;
        }
    }

    void handleInput() {
        if (currentState) {
            currentState->handleInput(*this);
        }
    }

    void update(float deltaTime) {
        if (currentState) {
            currentState->update(*this, deltaTime);
        }
    }

    void draw() {
        if (currentState) {
            currentState->draw(*this);
        }
    }

    void setGameStates(GameState* menu, GameState* game, GameState* gameOver) {
        menuState = menu;
        gamePlayState = game;
        gameOverState = gameOver;
        currentState = menuState;
    }


    GameState* currentState = nullptr;
    GameState* menuState = nullptr;
    GameState* gamePlayState = nullptr;
    GameState* gameOverState = nullptr;
};


class MenuState : public GameState {
public:
    void handleInput(GameContext& context) override {
        context.menuManager.HandleInput();
        if (context.menuManager.HandleExit()) {
            CloseWindow();
            exit(0);
        }
        if (IsKeyPressed(KEY_ENTER) && !context.menuManager.dialog) {
            context.setState(context.gamePlayState);
        }
    }

    void update(GameContext& context, float deltaTime) override {

    }

    void draw(GameContext& context) override {

        BeginDrawing();
        context.menuManager.DrawMenu();
        EndDrawing();
    }
};


class GamePlayState : public GameState {
public:
    void handleInput(GameContext& context) override {
        context.menuManager.HandleSetting();

        if (IsKeyPressed(KEY_ENTER)) {
            context.setState(context.gameOverState);
        }

        if (IsKeyPressed(KEY_TAB)) {
            LevelEditor::getInstance().toggleEditMode();
        }
    }

    void update(GameContext& context, float deltaTime) override {
        PhysicsManager::getInstance().update();
        LevelEditor::getInstance().update();

        if (context.character) {
            context.character->update(deltaTime);
        }

        if (context.goomba) {
            context.goomba->update(deltaTime);
        }
    }

    void draw(GameContext& context) override {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawText("Press Enter", 500, 100, 20, BLACK);

        if (context.character) {
            context.character->draw();
        }

        if (context.goomba) {
            context.goomba->draw();
        }

        LevelEditor::getInstance().draw();
        PhysicsManager::getInstance().drawDebug();
        DrawFPS(20, 50);
        context.menuManager.DrawSetting();
        EndDrawing();
    }
};


class GameOverState : public GameState {
public:
    void handleInput(GameContext& context) override {
        if (IsKeyPressed(KEY_ENTER)) {
            context.setState(context.menuState);
        }
    }

    void update(GameContext& context, float deltaTime) override {
        // add later
    }

    void draw(GameContext& context) override {
        BeginDrawing();

        ClearBackground(WHITE);
        context.uiManager.DrawGameOver();

        EndDrawing();
    }
};

int main() {
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Mario Game Demo");
    InitAudioDevice();
    SetTargetFPS(60);

    GameContext context;
    MenuState menuState;
    GamePlayState gamePlayState;
    GameOverState gameOverState;

    context.setGameStates(&menuState, &gamePlayState, &gameOverState);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        context.handleInput();
        context.update(deltaTime);
        context.draw();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}