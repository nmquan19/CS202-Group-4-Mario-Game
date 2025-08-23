#include "../../include/Game/GameContext.h"
#include "../../include/Game/GameStates.h"
#include "../../include/System/LevelEditor.h"
#include "../../include/System/Box2DWorldManager.h"
#include "../../include/Characters/Character.h"
#include "../../include/System/Interface.h"
#include <raylib.h>
#include <memory>
#include <cstdlib>
#include <iostream>
#include "../../include/System/CameraSystem.h"
#include "../../include/System/Constant.h"
#include "../../include/Enemy/EnemyAI/EnemyNavigator.h"

#include "../../include/System/LightingSystem.h"
#include <raymath.h>
void handleCamera();

void MenuState::handleInput(GameContext& context) {
    context.menuManager.HandleInput();
    if (context.menuManager.HandleExit()) {
        CloseWindow();
        exit(0);
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
        && context.menuManager.playBoard.checkCollision(GetMousePosition())) {
        context.setState(context.redirectState);
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
        && context.menuManager.editingBoard.checkCollision(GetMousePosition())) {
        context.setState(context.editorState);
    }
}

void MenuState::update(GameContext& context, float deltaTime) {
    context.menuManager.UpdateMenu(deltaTime);
    context.menuManager.UpdateExit(deltaTime);
}

void MenuState::draw(GameContext& context) {
    BeginDrawing();
    context.menuManager.DrawMenu();
    EndDrawing();
}

void RedirectState::handleInput(GameContext& context) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (context.menuManager.characterBoard.checkCollision(mousePos)) context.setState(context.playerSelectingState);
        if (context.menuManager.continueBoard.checkCollision(mousePos)) context.setState(context.informationState);
        if (context.menuManager.restartBoard.checkCollision(mousePos)) context.setState(context.informationState);
        if (context.menuManager.levelBoard.checkCollision(mousePos)) context.setState(context.informationState);
        if (context.menuManager.menuBoard.checkCollision(mousePos)) context.setState(context.menuState);
    }
}

void RedirectState::draw(GameContext& context) {
    BeginDrawing();
    ClearBackground(SKYBLUE);
    context.menuManager.DrawRedirect();
    EndDrawing();
}

void RedirectState::update(GameContext& context, float deltaTime) {
    context.menuManager.characterBoard.update(deltaTime);
    context.menuManager.continueBoard.update(deltaTime);
    context.menuManager.restartBoard.update(deltaTime);
    context.menuManager.levelBoard.update(deltaTime);
    context.menuManager.menuBoard.update(deltaTime);
}

void PlayerSelectingState::handleInput(GameContext& context) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (context.menuManager.OnePlayer.checkCollision(mousePos)) {
            context.setState(context.characterSelectingState);
        }
        if (context.menuManager.TwoPlayers.checkCollision(mousePos)) {
            context.setState(context.characterSelectingState);
        }
    }
}

void PlayerSelectingState::update(GameContext& context, float deltaTime) {
    context.menuManager.UpdatePlayer(deltaTime);
}

void PlayerSelectingState::draw(GameContext& context) {
	BeginDrawing();
	ClearBackground(SKYBLUE);
    context.menuManager.DrawPlayer();
	EndDrawing();
}

void CharacterSelectingState::handleInput(GameContext& context) {
    context.menuManager.HandleSelecting();
    if (IsKeyPressed(KEY_ENTER)) context.setState(context.redirectState);
}

void CharacterSelectingState::update(GameContext& context, float deltaTime) {
    context.menuManager.UpdateSelecting(deltaTime);
}

void CharacterSelectingState::draw(GameContext& context) {
    BeginDrawing();
    ClearBackground(SKYBLUE);
    //int screenWidth = UIManager::getInstance().screenWidth;
    //int screenHeight = UIManager::getInstance().screenHeight;
    //DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 1.0f - 0.5f));
    context.menuManager.DrawSelecting();
    EndDrawing();
}

void InformationState::handleInput(GameContext& context) {
    if (animationTime > 2.0f) {
        animationTime = 0.0f;
        context.setState(context.gamePlayState);
    }
}

void InformationState::update(GameContext& context, float deltaTime) {
    animationTime += deltaTime;
}

void InformationState::draw(GameContext& context) {
    BeginDrawing();
    ClearBackground(BLACK);
    UIManager::getInstance().drawInformationBoard(WHITE);
    EndDrawing();
}

void GamePlayState::handleInput(GameContext& context) {
    context.menuManager.HandleSetting();
    if (IsKeyPressed(KEY_ENTER)) {
        context.setState(context.gameOverState);
    }
    
    if (IsKeyPressed(KEY_F5)) {
        context.saveGameState("saved_game.json");
    }
    
    if (IsKeyPressed(KEY_F6)) {
        context.loadGameState("saved_game.json");
    }
    
    if (IsKeyPressed(KEY_F10)) {
        Box2DWorldManager::getInstance().setDebugDraw(!Box2DWorldManager::getInstance().isDebugDrawEnabled());
    }
    if (IsKeyPressed(KEY_ONE)) {
        Camera2D cam = {};
        cam.target = { 2000.0f, 2000.0f };
        cam.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
        cam.zoom = 0.5f;
        cam.rotation = 0.0f;
        GameCameraSystem::getInstance().addCamera(std::make_unique<StaticGameCamera>(cam));
        GameCameraSystem::getInstance().switchCamera(1);
    }
    else if(IsKeyPressed(KEY_TWO))
    {
		GameCameraSystem::getInstance().switchCamera(0);
    }
}
bool isInCameraBound(const Camera2D& cam, Vector2 pos, float padding = 0.0f) {
    float worldX = cam.target.x - (cam.offset.x / cam.zoom);
    float worldY = cam.target.y - (cam.offset.y / cam.zoom);

    Rectangle camRect = {
        worldX - padding,
        worldY - padding,
        (GetScreenWidth() / cam.zoom) + 2 * padding,
        (GetScreenHeight() / cam.zoom) + 2 * padding
    };

    return CheckCollisionPointRec(pos, camRect);
}

void GamePlayState::update(GameContext& context, float deltaTime) {
    auto& cam = GameCameraSystem::getInstance(); 
    cam.update(deltaTime);
    //NavGraph::getInstance().buildNodes({ 0,0,2000,2000 });
    LightingManager::getInstance().update(deltaTime);
    //GameCameraSystem::getInstance().shakeCurrentCamera(100.0f, 0.1f);
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), cam.getCamera());
        GameContext::getInstance().addObject(BackGroundObjectType::TORCH, mousePos, { 1,1});

    }

    if (context.character01) {
        std::shared_ptr<Character> character01 = std::dynamic_pointer_cast<Character>(context.character01);
        character01->update(deltaTime);
    }
    if (context.character02) {
        std::shared_ptr<Character> character02 = std::dynamic_pointer_cast<Character>(context.character02);
        character02->update(deltaTime);
    }
    for (auto& obj :context.Objects)
    {
		IUpdatable* updatableObj = dynamic_cast<IUpdatable*>(obj.get());
        //if(updatableObj&& isInCameraBound(GameCameraSystem::getInstance().getCamera(), obj->getPosition(),100.f))
        //    updatableObj->update(deltaTime);
        updatableObj->update(deltaTime);

    }
    context.menuManager.UpdateSetting(deltaTime);
    context.spawnObject();  
    context.deleteObjects();
    UIManager::getInstance().updateInformationBoard(deltaTime);
    Box2DWorldManager::getInstance().step(deltaTime);
    ParticleSystem::getInstance().update(deltaTime);
}
void DrawParallaxBackground(Texture2D bg, Camera2D cam, float parallaxFactor) {
    Vector2 camPos = {
        cam.target.x - cam.offset.x,
        cam.target.y - cam.offset.y
    };

    int tilesX = GetScreenWidth() / bg.width + 2;
    int tilesY = GetScreenHeight() / bg.height + 2;

    // Offset based on parallax
    float offsetX = fmodf(camPos.x * parallaxFactor, (float)bg.width);
    float offsetY = fmodf(camPos.y * parallaxFactor, (float)bg.height);

    for (int y = -1; y < tilesY; y++) {
        for (int x = -1; x < tilesX; x++) {
            float drawX = x * bg.width - offsetX;
            float drawY = y * bg.height - offsetY;
            DrawTexture(bg, (int)drawX, (int)drawY, WHITE);
        }
    }
}

void GamePlayState::draw(GameContext& context) {
    BeginDrawing();
    ClearBackground(WHITE);
    NavGraph::getInstance().clear();
    DrawText("Press Enter", 500, 100, 20, BLACK);
    DrawText("F5 - Save game", 1200, 500, 50, BLACK);
    DrawText("F6 - Load current game", 1200, 600, 50, BLACK);
    Vector2 camPos = GameCameraSystem::getInstance().getCamera().target;
    
    LightingManager& lm = LightingManager::getInstance();

    // 1. Render scene to lightmap (world-space)
    BeginTextureMode(lm.getLightMap());
    {
        ClearBackground(BLANK); // Transparent clear for light accumulation
    }
    Texture2D bg = TextureManager::getInstance().background_lv1;
    Camera2D cam = GameCameraSystem::getInstance().getCamera();

    DrawParallaxBackground(bg, cam, 0.5f);

    BeginMode2D(GameCameraSystem::getInstance().getCamera());
    for (auto& obj : context.Objects) {
       /* if(isInCameraBound(GameCameraSystem::getInstance().getCamera(),obj->getPosition(),100.f)) {
            obj->draw();
		}*/
        obj->draw();
    }

    if (context.character01) {
        context.character01->draw();
    }
    if (context.character02) {
        context.character02->draw();
    }
    ParticleSystem::getInstance().draw();

    Box2DWorldManager::getInstance().drawDebugBodies();
    EndMode2D();
    EndTextureMode();
    lm.prepareShader(GameCameraSystem::getInstance().getCamera());

    BeginShaderMode(lm.getShader());
    SetShaderValueTexture(lm.getShader(),
        GetShaderLocation(lm.getShader(), "sceneTexture"),
        lm.getLightMap().texture);

    // Draw fullscreen quad to apply shader
    DrawTextureRec(lm.getLightMap().texture,
        { 0, 0, (float)lm.getLightMap().texture.width, (float)-lm.getLightMap().texture.height },
        { 0, 0 },
        WHITE);
    EndShaderMode();
    //GameContext::getInstance().testParticle->draw();

    DrawFPS(20, 50); 

    if (Box2DWorldManager::getInstance().isDebugDrawEnabled()) {
        DrawText("Box2D Debug Mode: ON (Key F10 to toggle)", 520, 80, 20, GREEN);
    } else {
        DrawText("Box2D Debug Mode: OFF (Key F10 to toggle)", 520, 80, 20, RED);
    }
    
    context.menuManager.DrawSetting();
    UIManager::getInstance().drawInformationBoard(BLACK);
    EndDrawing();
}

void GamePlayState::DrawBackGround(Texture2D& texture) {
    if (texture.id != 0) {
        DrawTexturePro(texture, {0, 0, (float)texture.width, (float)texture.height}, {0, 0, Constants::WORLDBOUNDS_WIDTH, Constants::WORLDBOUNDS_HEIGHT}, {0,0}, 0.0f, WHITE);
    }
}












void EditorState::handleInput(GameContext& context) {
    if (stateSelect == 1) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();
            if (context.menuManager.day_groundBoard.checkCollision(mousePos)) {
                mapSelect = 1; stateSelect = 2;
            }
            if (context.menuManager.day_undergroundBoard.checkCollision(mousePos)) {
                mapSelect = 2; stateSelect = 2;
            }
            if (context.menuManager.night_airshipBoard.checkCollision(mousePos)) {
                mapSelect = 3; stateSelect = 2;
            }
            if (context.menuManager.night_snowBoard.checkCollision(mousePos)) {
                mapSelect = 4; stateSelect = 2;
            }
        }
    }
    else if (stateSelect == 2) {
        context.menuManager.HandleSetting();
        if (IsKeyPressed(KEY_ENTER)) {
            context.setState(context.menuState);
            stateSelect = 1;
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
    
}

void EditorState::update(GameContext& context, float deltaTime) {
    if (stateSelect == 1) {
        context.menuManager.UpdateEditorSelecting(deltaTime);
    }
    else if (stateSelect == 2) {
        handleCamera();
        LevelEditor::getInstance().update(deltaTime);
        Box2DWorldManager::getInstance().step(deltaTime);
        NavGraph::getInstance().buildNodes({ 0,0,2000,2000 });
    }
}

// GameStates.cpp
void EditorState::draw(GameContext& context) {
    if (stateSelect == 1) {
        BeginDrawing();
        ClearBackground(SKYBLUE);
        context.menuManager.DrawEditorSelecting();
        EndDrawing();
    }
    else if (stateSelect == 2) {
        BeginDrawing();
        ClearBackground(WHITE); // Clear main buffer

        LightingManager& lm = LightingManager::getInstance();

        // 1. Render scene to lightmap (world-space)
        BeginTextureMode(lm.getLightMap());
        {
            ClearBackground(BLANK); // Transparent clear for light accumulation
            LevelEditor::getInstance().mapSelect = mapSelect;
            LevelEditor::getInstance().draw();
        }
        EndTextureMode();

        // 2. Apply lighting shader (screen-space)
        lm.prepareShader(context.camera);
        BeginShaderMode(lm.getShader());

        // Important: Set texture slot 0 for sceneTexture
        SetShaderValueTexture(lm.getShader(),
            GetShaderLocation(lm.getShader(), "sceneTexture"),
            lm.getLightMap().texture);

        // Draw fullscreen quad to apply shader
        DrawTextureRec(lm.getLightMap().texture,
            { 0, 0, (float)lm.getLightMap().texture.width, (float)-lm.getLightMap().texture.height },
            { 0, 0 },
            WHITE);
        EndShaderMode();

        DrawFPS(20, 50);
        EndDrawing();
    }
    
}


void EditorSelectingState::handleInput(GameContext& context) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (context.menuManager.day_groundBoard.checkCollision(mousePos)) {
            context.setState(context.editorState);
        }
    }
}

void EditorSelectingState::update(GameContext& context, float deltaTime) {
    context.menuManager.UpdateEditorSelecting(deltaTime);
}

void EditorSelectingState::draw(GameContext& context) {
    BeginDrawing();
    ClearBackground(SKYBLUE);
    context.menuManager.DrawEditorSelecting();
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
    ClearBackground(BLACK);
    UIManager::getInstance().drawInformationBoard(WHITE);
    EndDrawing();
}

void handleCamera() {
    Camera2D& cam = GameContext::getInstance().camera;

    // Handle zoom
    float wheel = GetMouseWheelMove();
    if (wheel != 0.0f && !CheckCollisionPointRec(GetMousePosition(), LevelEditor::getInstance().getObjectPalette().getPaletteRect())) {
        // Center zoom around mouse
        Vector2 mouseWorldBefore = GetScreenToWorld2D(GetMousePosition(), cam);
        cam.zoom *= expf(wheel * 0.1f);
        cam.zoom = fminf(fmaxf(cam.zoom, 0.5f), 4.0f);
        Vector2 mouseWorldAfter = GetScreenToWorld2D(GetMousePosition(), cam);
        cam.target = Vector2Add(cam.target, Vector2Subtract(mouseWorldBefore, mouseWorldAfter));
    }

    const int borderThreshold = 10; 
    const float scrollSpeed = 1000.0f * GetFrameTime(); 
    Vector2 mouse = GetMousePosition();

    if (mouse.x <= borderThreshold|| IsKeyDown(KEY_A))
        cam.target.x -= scrollSpeed;
    else if (mouse.x >= GetScreenWidth() - borderThreshold || IsKeyDown(KEY_D))
        cam.target.x += scrollSpeed;

    if (mouse.y <= borderThreshold || IsKeyDown(KEY_W))
        cam.target.y -= scrollSpeed;
    else if (mouse.y >= GetScreenHeight() - borderThreshold || IsKeyDown(KEY_S))
        cam.target.y += scrollSpeed;

    Vector2 topLeft = GetScreenToWorld2D({ 0, 0 }, cam);
    Vector2 bottomRight = GetScreenToWorld2D({ (float)GetScreenWidth(), (float)GetScreenHeight() }, cam);

    float cameraWidth = bottomRight.x - topLeft.x;
    float cameraHeight = bottomRight.y - topLeft.y;

    float minCameraX = cameraWidth * 0.5f;
    float maxCameraX = Constants::WORLDBOUNDS_WIDTH - cameraWidth * 0.5f;
    float minCameraY = cameraHeight * 0.5f;
    float maxCameraY = Constants::WORLDBOUNDS_HEIGHT - cameraHeight * 0.5f;

    cam.target.x = fminf(fmaxf(cam.target.x, minCameraX), maxCameraX);
    cam.target.y = fminf(fmaxf(cam.target.y, minCameraY), maxCameraY);
}
