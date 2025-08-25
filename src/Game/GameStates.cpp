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
#include <algorithm>
#include "../../include/System/CameraSystem.h"
#include "../../include/System/Constant.h"
#include "../../include/Enemy/EnemyAI/EnemyNavigator.h"
#include "../../include/Enemy/Boss/DryBowser/DryBowser.h"
#include "../../include/System/LightingSystem.h"
#include <raymath.h>
#include <string>
void handleCamera();

void MenuState::handleInput(GameContext& context) {
    context.menuManager.HandleInput();
    if (context.menuManager.HandleExit()) {
        CloseWindow();
        exit(0);
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && context.menuManager.playBoard.checkCollision(GetMousePosition())) {
        context.setState(context.redirectState);
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && context.menuManager.editingBoard.checkCollision(GetMousePosition())) {
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
        if (context.menuManager.characterBoard.checkCollision(mousePos)) {
            AudioManager::getInstance().PlaySoundEffect("click");
            context.setState(context.playerSelectingState);
        }
        else if (context.menuManager.continueBoard.checkCollision(mousePos)) {
            AudioManager::getInstance().PlaySoundEffect("click");
            context.setState(context.informationState);
        }
        else if (context.menuManager.menuBoard.checkCollision(mousePos)) {
            AudioManager::getInstance().PlaySoundEffect("click");
            context.setState(context.menuState);
        }
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
    context.menuManager.menuBoard.update(deltaTime);
}

void PlayerSelectingState::handleInput(GameContext& context) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (context.menuManager.OnePlayer.checkCollision(mousePos)) {
            AudioManager::getInstance().PlaySoundEffect("click");
            context.menuManager.character02Select = -1;
            context.setState(context.characterSelectingState);
        }
        else if (context.menuManager.TwoPlayers.checkCollision(mousePos)) {
            AudioManager::getInstance().PlaySoundEffect("click");
            context.menuManager.character02Select = 0;
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
    if (IsKeyPressed(KEY_ENTER)) context.setState(context.levelRedirectState);
}

void CharacterSelectingState::update(GameContext& context, float deltaTime) {
    context.menuManager.UpdateSelecting(deltaTime);
}

void CharacterSelectingState::draw(GameContext& context) {
    BeginDrawing();
    ClearBackground(SKYBLUE);
    context.menuManager.DrawSelecting();
    EndDrawing();
}


void LevelRedirectState::handleInput(GameContext& context) {
    if (IsKeyPressed(KEY_ENTER)) {
        context.setState(context.levelSelectingState);
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (context.menuManager.available.checkCollision(GetMousePosition())) {
            context.setState(context.levelSelectingState);
        }
        if (context.menuManager.custom.checkCollision(GetMousePosition())) {
            context.level = 4;
            context.informationState->setLevel(context);
            context.gamePlayState->setLevel(context);
            context.setState(context.informationState);
        }
    }
    
}

void LevelRedirectState::update(GameContext& context, float deltaTime) {
    context.menuManager.UpdateLevelRedirect(deltaTime);
}

void LevelRedirectState::draw(GameContext& context) {
    BeginDrawing();
    ClearBackground(SKYBLUE);
    context.menuManager.DrawLevelRedirect();
    EndDrawing();
}

void LevelSelectingState::handleInput(GameContext& context) {
    context.menuManager.HandleLevel();
    
    context.level = context.menuManager.mt.level;
    context.informationState->setLevel(context);
    context.gamePlayState->setLevel(context);
    if (IsKeyPressed(KEY_ENTER)) context.setState(context.informationState);
    
}

void LevelSelectingState::update(GameContext& context, float deltaTime) {
    context.menuManager.UpdateLevel(deltaTime);
}

void LevelSelectingState::draw(GameContext& context) {
    BeginDrawing();
    ClearBackground(WHITE);
    context.menuManager.DrawLevel();
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
    if (level == 1) UIManager::getInstance().setWorld("1");
    if (level == 2) UIManager::getInstance().setWorld("2");
    if (level == 3) UIManager::getInstance().setWorld("3");
    if (level == 4) UIManager::getInstance().setWorld("");
    UIManager::getInstance().drawInformationBoard(WHITE);
    EndDrawing();
}

void GamePlayState::handleInput(GameContext& context) {
    context.menuManager.HandleSetting();
    if (IsKeyPressed(KEY_ENTER)) {
        context.setState(context.gameOverState);
    }
    
    context.gameOverState->setLevel(context);

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
       /* Camera2D ncam = {};
        ncam.target = { 11750.0f, 1450.0f };
        ncam.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
        ncam.zoom = 1.1f;
        ncam.rotation = 0.0f;
        GameCameraSystem::getInstance().addCamera(std::make_unique<StaticGameCamera>(ncam));
        GameCameraSystem::getInstance().switchCamera(2);*/
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
    LightingManager::getInstance().update(deltaTime);
    //GameCameraSystem::getInstance().shakeCurrentCamera(100.0f, 0.1xf);
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), cam.getCamera());
        GameContext::getInstance().addObject(BackGroundObjectType::TORCH, mousePos, { 1,1});

    }

    if (!context.menuManager.settingDialog) {
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
            if (updatableObj && isInCameraBound(GameCameraSystem::getInstance().getCamera(), obj->getPosition(), 300.f))
            {
                if (auto dry = dynamic_cast<DryBowser*>(obj.get())) {
                    if (isInCameraBound(GameCameraSystem::getInstance().getCamera(), dry->getPosition(), 100.f)) {
                        dry->update(deltaTime);
                    }
                }
                else
                {
                    updatableObj->update(deltaTime);

                }
            }

        }
        context.menuManager.UpdateSetting(deltaTime);
        context.spawnObject();  
        context.deleteObjects();
        UIManager::getInstance().updateInformationBoard(deltaTime);
        Box2DWorldManager::getInstance().step(deltaTime);
        ParticleSystem::getInstance().update(deltaTime);
    }
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

    //DrawParallaxBackground(bg, cam, 0.5f);
    if (level == 1 || level == 4) {
        Background::getInstance().draw("Forest_1", { 0,0 });
        Background::getInstance().draw("Forest_1", { 0, 512 });
        Background::getInstance().draw("Ghost_house_1", { 0, 1024 });
    }
    if (level == 2) {
        Background::getInstance().draw("Airship_night_3", { 0,0 });
        Background::getInstance().draw("Airship_night_3", { 0, 512 });
    }
    if (level == 3) {
        Background::getInstance().draw("Snow_night_1", { 0,0 });
        Background::getInstance().draw("Snow_night_1", { 0,512 });
    }
    
    BeginMode2D(GameCameraSystem::getInstance().getCamera());
    for (auto& obj : context.Objects) {
        if(isInCameraBound(GameCameraSystem::getInstance().getCamera(),obj->getPosition(),100.f)) {
            obj->draw();
		}
    }

    if (context.character01 && context.character02) {
        context.character01->draw();
        std::shared_ptr<Character> character01 = std::dynamic_pointer_cast<Character>(context.character01);
        character01->drawIndicator();
        context.character02->draw();
        std::shared_ptr<Character> character02 = std::dynamic_pointer_cast<Character>(context.character02);
        character02->drawIndicator();
    }
    else if (context.character01) {
        context.character01->draw();
    }
    else if (context.character02) {
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
    for (auto& obj : context.Objects) {
        if (auto dry = dynamic_cast<DryBowser*>(obj.get())) {
            if (isInCameraBound(GameCameraSystem::getInstance().getCamera(), dry->getPosition(), 100.f)) {
                dry->drawHealthBar();
            }
        }
    }
    context.menuManager.DrawSetting();
    UIManager::getInstance().drawInformationBoard(WHITE);
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
        if ((IsKeyPressed(KEY_F7) || (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && context.menuManager.saveLevel.checkCollision(GetMousePosition()))) && LevelEditor::getInstance().isInEditMode()) {
            LevelEditor::getInstance().saveLevel("testlevel.json");
        }
        if ((IsKeyPressed(KEY_F8) || (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && context.menuManager.loadLevel.checkCollision(GetMousePosition()))) && LevelEditor::getInstance().isInEditMode()) {
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
        context.menuManager.loadLevel.update(deltaTime);
        context.menuManager.saveLevel.update(deltaTime);
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
        
         ClearBackground(BLANK); // Transparent clear for light accumulation
         
         LevelEditor::getInstance().mapSelect = mapSelect;
         LevelEditor::getInstance().draw();
         context.menuManager.loadLevel.draw(context.menuManager.loadLevelPosition);
         context.menuManager.saveLevel.draw(context.menuManager.saveLevelPosition);
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
    
}

void GameOverState::update(GameContext& context, float deltaTime) {
    if (IsKeyPressed(KEY_ENTER)) {
        context.setState(context.menuState);
    }
    blinkTimer += deltaTime;
}

void GameOverState::draw(GameContext& context) {
    BeginDrawing();
    ClearBackground(BLACK);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Get UIManager instance
    UIManager& uiManager = UIManager::getInstance();

    // Level complete banner
    const char* levelCompleteText = "OOPS! NICE TRY!";
    Vector2 titleSize = MeasureTextEx(uiManager.menuFont, levelCompleteText, 60, 3);
    Vector2 titlePos = { (screenWidth - titleSize.x) / 2, screenHeight * 0.1f };

    // Draw title with glow effect
    DrawTextEx(uiManager.menuFont, levelCompleteText, { titlePos.x + 2, titlePos.y + 2 }, 60, 3, DARKBLUE);
    DrawTextEx(uiManager.menuFont, levelCompleteText, titlePos, 60, 3, GOLD);

    // Score section
    float sectionY = screenHeight * 0.3f;
    float lineHeight = 80;

    // Draw decorative border
    Rectangle borderRect = {
        screenWidth * 0.1f,
        sectionY - 40,
        screenWidth * 0.8f,
        lineHeight * 4 + 80
    };
    DrawRectangleRounded(borderRect, 0.1f, 16, Fade(BLUE, 0.3f));
    // Draw border outline manually
    DrawRectangleLinesEx(borderRect, 4, WHITE);

    // Score breakdown
    float textX = screenWidth * 0.25f;
    float valueX = screenWidth * 0.7f;

    // Get current values
    int currentScore = uiManager.getScore();
    int coinCount = uiManager.getCoinCount();
    float timeLeft = uiManager.getTimeLeft();

    // Calculate bonuses
    int coinBonus = coinCount * 200; // 200 points per coin
    int timeBonus = (int)(timeLeft * 50); // 50 points per remaining second
    int totalScore = currentScore + coinBonus + timeBonus;

    // Current Score
    const char* currentScoreText = "CURRENT SCORE:";
    DrawTextEx(uiManager.menuFont, currentScoreText, { textX, sectionY }, 40, 2, WHITE);
    const char* scoreValue = TextFormat("%06d", currentScore);
    DrawTextEx(uiManager.menuFont, scoreValue, { valueX, sectionY }, 40, 2, YELLOW);

    sectionY += lineHeight;

    // Coins collected
    const char* coinsText = "COINS COLLECTED:";
    DrawTextEx(uiManager.menuFont, coinsText, { textX, sectionY }, 40, 2, WHITE);
    const char* coinValue = TextFormat("x %02d (+%d pts)", coinCount, coinBonus);
    DrawTextEx(uiManager.menuFont, coinValue, { valueX, sectionY }, 40, 2, YELLOW);

    // Draw coin icons to visualize collection
    for (int i = 0; i < std::min(coinCount, 8); i++) {
        float coinX = textX + 280 + i * 25;
        DrawCircle(coinX, sectionY + 20, 10, GOLD);
        DrawCircle(coinX, sectionY + 20, 8, YELLOW);
        DrawText("$", coinX - 3, sectionY + 15, 12, ORANGE);
    }
    if (coinCount > 8) {
        DrawTextEx(uiManager.menuFont, "...", { textX + 280 + 8 * 25, sectionY + 10 }, 20, 1, YELLOW);
    }

    sectionY += lineHeight;

    // Line separator
    DrawLine(textX, sectionY - 20, valueX + 200, sectionY - 20, WHITE);

    // Total score
    const char* totalText = "TOTAL SCORE:";
    DrawTextEx(uiManager.menuFont, totalText, { textX, sectionY }, 45, 2, GOLD);
    const char* totalValue = TextFormat("%06d", totalScore);
    DrawTextEx(uiManager.menuFont, totalValue, { valueX, sectionY }, 45, 2, GOLD);

    // Continue prompt
    const char* continueText = "PRESS ENTER TO RETURN TO MENU";
    Vector2 continueSize = MeasureTextEx(uiManager.menuFont, continueText, 25, 2);
    Vector2 continuePos = { (screenWidth - continueSize.x) / 2, screenHeight * 0.9f };

    // Blinking effect
    if (fmod(blinkTimer, 1.0f) < 0.5f) {
        DrawTextEx(uiManager.menuFont, continueText, continuePos, 25, 2, WHITE);
    }

    EndDrawing();
}

ScoreState::ScoreState() {
    for (int i = 1; i <= 16; ++i) {
        std::string filePath = "assets/gif/frame_00" + (i < 10 ? std::to_string(0) : "") + std::to_string(i) + ".png";
        gif1.push_back(LoadTexture(filePath.c_str()));
    }
    for (int i = 1; i <= 21; ++i) {
        std::string filePath = "assets/gif/frame2_00" + (i < 10 ? std::to_string(0) : "") + std::to_string(i) + ".png";
        gif2.push_back(LoadTexture(filePath.c_str()));
    }
    background = LoadTexture("assets/cc.jpg");
}

ScoreState::~ScoreState() {
    for (auto& t : gif1) {
        UnloadTexture(t);
    }
    for (auto& t : gif2) {
        UnloadTexture(t);
    }
    UnloadTexture(background);
}

void ScoreState::handleInput(GameContext& context) {

}

void ScoreState::update(GameContext& context, float deltaTime) {
    if (IsKeyPressed(KEY_ENTER)) {
        context.setState(context.levelSelectingState);
    }

    blinkTimer += deltaTime;

    gif1Timer += deltaTime;
    if (gif1Timer > 1.0f/60.0f) {
        gif1Frame = (gif1Frame + 1) % gif1.size();
        gif1Timer = 0.0f;
    }

    gif2Timer += deltaTime;
    if (gif2Timer > 1.0f / 30.0f) {
        gif2Frame = (gif2Frame + 1) % gif2.size();
        gif2Timer = 0.0f;
    }
}

void ScoreState::draw(GameContext& context) {
    BeginDrawing();
    ClearBackground(BLACK);
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    DrawTexturePro(background, { 0, 0, (float)background.width, (float)background.height }, { 0, 0, (float)screenWidth, (float)screenHeight }, { 0, 0 }, 0.0f, WHITE);
    
    // Get UIManager instance
    UIManager& uiManager = UIManager::getInstance();
    
    // Level complete banner
    const char* levelCompleteText = "LEVEL COMPLETE!";
    Vector2 titleSize = MeasureTextEx(uiManager.menuFont, levelCompleteText, 60, 3);
    Vector2 titlePos = { (screenWidth - titleSize.x) / 2, screenHeight * 0.1f };
    
    // Draw title with glow effect
    DrawTextEx(uiManager.menuFont, levelCompleteText, { titlePos.x + 2, titlePos.y + 2 }, 60, 3, DARKBLUE);
    DrawTextEx(uiManager.menuFont, levelCompleteText, titlePos, 60, 3, GOLD);
    
    // Score section
    float sectionY = screenHeight * 0.3f;
    float lineHeight = 80;
    
    // Draw decorative border
    Rectangle borderRect = { 
        screenWidth * 0.1f, 
        sectionY - 40, 
        screenWidth * 0.8f, 
        lineHeight * 4 + 80 
    };
    DrawRectangleRounded(borderRect, 0.1f, 16, Fade(BLUE, 0.3f));
    // Draw border outline manually
    DrawRectangleLinesEx(borderRect, 4, WHITE);
    
    // Score breakdown
    float textX = screenWidth * 0.25f;
    float valueX = screenWidth * 0.7f;
    
    // Get current values
    int currentScore = uiManager.getScore();
    int coinCount = uiManager.getCoinCount();
    float timeLeft = uiManager.getTimeLeft();
    
    // Calculate bonuses
    int coinBonus = coinCount * 200; // 200 points per coin
    int timeBonus = (int)(timeLeft * 50); // 50 points per remaining second
    int totalScore = currentScore + coinBonus + timeBonus;
    
    // Current Score
    const char* currentScoreText = "CURRENT SCORE:";
    DrawTextEx(uiManager.menuFont, currentScoreText, { textX, sectionY }, 40, 2, WHITE);
    const char* scoreValue = TextFormat("%06d", currentScore);
    DrawTextEx(uiManager.menuFont, scoreValue, { valueX, sectionY }, 40, 2, YELLOW);
    
    sectionY += lineHeight;
    
    // Coins collected
    const char* coinsText = "COINS COLLECTED:";
    DrawTextEx(uiManager.menuFont, coinsText, { textX, sectionY }, 40, 2, WHITE);
    const char* coinValue = TextFormat("x %02d (+%d pts)", coinCount, coinBonus);
    DrawTextEx(uiManager.menuFont, coinValue, { valueX, sectionY }, 40, 2, YELLOW);
    
    // Draw coin icons to visualize collection
    for (int i = 0; i < std::min(coinCount, 8); i++) {
        float coinX = textX + 280 + i * 25;
        DrawCircle(coinX, sectionY + 20, 10, GOLD);
        DrawCircle(coinX, sectionY + 20, 8, YELLOW);
        DrawText("$", coinX - 3, sectionY + 15, 12, ORANGE);
    }
    if (coinCount > 8) {
        DrawTextEx(uiManager.menuFont, "...", { textX + 280 + 8 * 25, sectionY + 10 }, 20, 1, YELLOW);
    }
    
    sectionY += lineHeight;
    
    // Time bonus
    const char* timeBonusText = "TIME BONUS:";
    DrawTextEx(uiManager.menuFont, timeBonusText, { textX, sectionY }, 40, 2, WHITE);
    const char* timeBonusValue = TextFormat("%06d", timeBonus);
    
    // Color code the time bonus based on performance
    Color timeColor = YELLOW; // Default
    if (timeLeft >= 300) timeColor = LIME;      // Excellent time
    else if (timeLeft >= 200) timeColor = GREEN;    // Good time  
    else if (timeLeft >= 100) timeColor = YELLOW;   // Average time
    else if (timeLeft >= 50) timeColor = ORANGE;    // Poor time
    else timeColor = RED;                            // Very poor time
    
    DrawTextEx(uiManager.menuFont, timeBonusValue, { valueX, sectionY }, 40, 2, timeColor);
    
    // Add time performance text
    const char* timePerformance = "";
    if (timeLeft >= 300) timePerformance = "SPEED RUN!";
    else if (timeLeft >= 200) timePerformance = "FAST!";
    else if (timeLeft >= 100) timePerformance = "GOOD";
    else if (timeLeft >= 50) timePerformance = "SLOW";
    else timePerformance = "TOO SLOW";
    
    if (strlen(timePerformance) > 0) {
        DrawTextEx(uiManager.menuFont, timePerformance, { valueX + 180, sectionY + 15 }, 30, 1, timeColor);
    }
    
    sectionY += lineHeight;
    
    // Line separator
    DrawLine(textX, sectionY - 20, valueX + 200, sectionY - 20, WHITE);
    
    // Total score
    const char* totalText = "TOTAL SCORE:";
    DrawTextEx(uiManager.menuFont, totalText, { textX, sectionY }, 45, 2, GOLD);
    const char* totalValue = TextFormat("%06d", totalScore);
    DrawTextEx(uiManager.menuFont, totalValue, { valueX, sectionY }, 45, 2, GOLD);
    
    // Star rating based on performance
    float starY = screenHeight * 0.75f;
    const char* ratingText = "PERFORMANCE RATING:";
    Vector2 ratingSize = MeasureTextEx(uiManager.menuFont, ratingText, 35, 2);
    Vector2 ratingPos = { (screenWidth - ratingSize.x) / 2, starY };
    DrawTextEx(uiManager.menuFont, ratingText, ratingPos, 35, 2, WHITE);
    
    // Draw stars (example: 3 out of 5 stars)
    float starSize = 40;
    float starSpacing = 60;
    float starsStartX = (screenWidth - (5 * starSpacing - (starSpacing - starSize))) / 2;
    // Calculate star rating based on performance
    int earnedStars = 1; // Base star for completing the level
    
    // Additional stars based on performance criteria
    if (totalScore >= 10000) earnedStars++; // High score star
    if (coinCount >= 5) earnedStars++; // Coin collector star  
    if (timeLeft >= 200) earnedStars++; // Speed completion star
    if (totalScore >= 20000) earnedStars++; // Perfect score star
    
    earnedStars = std::min(earnedStars, 5); // Cap at 5 stars
    
    for (int i = 0; i < 5; i++) {
        float starX = starsStartX + i * starSpacing;
        float starYPos = starY + 60;
        Color starColor = (i < earnedStars) ? GOLD : GRAY;
        
        // Draw simple star shape
        Vector2 center = { starX + starSize/2, starYPos + starSize/2 };
        DrawPoly(center, 5, starSize/2, 0, starColor);
        DrawPolyLines(center, 5, starSize/2, 0, DARKGRAY);
    }
    
    // Continue prompt
    const char* continueText = "PRESS ENTER TO CONTINUE";
    Vector2 continueSize = MeasureTextEx(uiManager.menuFont, continueText, 25, 2);
    Vector2 continuePos = { (screenWidth - continueSize.x) / 2, screenHeight * 0.9f };
    
    // Blinking effect

    if (fmod(blinkTimer, 1.0f) < 0.5f) {
        DrawTextEx(uiManager.menuFont, continueText, continuePos, 25, 2, WHITE);
    }

    Rectangle src1 = { 0, 0, gif1[gif1Frame].width, gif1[gif1Frame].height };
    Rectangle dest1 = { screenWidth * 0.1f, screenHeight * 0.75f, gif1[gif1Frame].width * 0.5f, gif1[gif1Frame].height * 0.5f };
    DrawTexturePro(gif1[gif1Frame], src1, dest1, { 0, 0 }, 0.0f, WHITE);

    Rectangle src2 = { 0, 0, gif2[gif2Frame].width, gif2[gif2Frame].height };
    Rectangle dest2 = { screenWidth * 0.8f, screenHeight * 0.75f, gif2[gif2Frame].width * 0.5f, gif2[gif2Frame].height * 0.5f };
    DrawTexturePro(gif2[gif2Frame], src2, dest2, { 0, 0 }, 0.0f, WHITE);

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
