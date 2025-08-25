#include "../../include/UI/Menu.h"

MenuManager::MenuManager() {
    isActive = true;

    logo = LoadTexture("./assets/Super Mario Bros.png");
    select = 0; character01Select = 0;
    settingDialog = false;
    exitDialog = false;

    exit = false;
    board = LoadTexture("./assets/button/board1.png");
    menuBackground = LoadTexture("./assets/menu background.jpg");
    redirectBackground = LoadTexture("./assets/redirect background.jpg");

    check.load("./assets/button/check.png", "./assets/button/check_selected.png");

    cross.load("./assets/button/cross.png", "./assets/button/cross_selected.png");

    setting.load("./assets/button/option.png", "./assets/button/option_selected.png");

    returnButton.load("./assets/button/back.png", "./assets/button/back_selected.png");
    save.load("./assets/button/save.png", "./assets/button/save_selected.png");
    loadButton.load("./assets/button/load.png", "./assets/button/load_selected.png");

    saveLevel.load("./assets/button/save.png", "./assets/button/save_selected.png");
    loadLevel.load("./assets/button/load.png", "./assets/button/load_selected.png");

    playBoard.load("./assets/button/thin_board.png", "./assets/button/thin_board_selected.png");
    settingBoard.load("./assets/button/thin_board.png", "./assets/button/thin_board_selected.png");
    exitBoard.load("./assets/button/thin_board.png", "./assets/button/thin_board_selected.png");
    editingBoard.load("./assets/button/thin_board.png", "./assets/button/thin_board_selected.png");


    characterBoard.load("./assets/button/small_board.png", "./assets/button/small_board.png");
    continueBoard.load("./assets/button/small_board.png", "./assets/button/small_board.png");
    restartBoard.load("./assets/button/small_board.png", "./assets/button/small_board.png");
    levelBoard.load("./assets/button/small_board.png", "./assets/button/small_board.png");
    menuBoard.load("./assets/button/small_board.png", "./assets/button/small_board.png");


    day_groundBoard.load("./assets/button/small_board.png", "./assets/button/small_board_selected.png");
    day_undergroundBoard.load("./assets/button/small_board.png", "./assets/button/small_board_selected.png");
    night_airshipBoard.load("./assets/button/small_board.png", "./assets/button/small_board_selected.png");
    night_snowBoard.load("./assets/button/small_board.png", "./assets/button/small_board_selected.png");

    available.load("./assets/button/small_board.png", "./assets/button/small_board_selected.png");
    custom.load("./assets/button/small_board.png", "./assets/button/small_board_selected.png");

    OnePlayer.load("./assets/button/small_board.png", "./assets/button/small_board_selected.png");
    TwoPlayers.load("./assets/button/small_board.png", "./assets/button/small_board_selected.png");

    mario = LoadTexture("./assets/Mario.png");
    luigi = LoadTexture("./assets/Luigi.png");
    toad = LoadTexture("./assets/Toad.png");
    toadette = LoadTexture("./assets/Toadette.png");

    level_map1 = LoadTexture("./assets/level_map1.png");
    level_map4 = LoadTexture("./assets/level_map4.png");

    const int screenWidth = UIManager::getInstance().screenWidth;
    const int screenHeight = UIManager::getInstance().screenHeight;

    boardPosition.x = (static_cast<float>(screenWidth) - board.width * scale) / 2;
    boardPosition.y = (static_cast<float>(screenHeight) - board.height * scale) / 2;

    crossPosition.x = boardPosition.x + board.width * scale / 2 - 100;
    crossPosition.y = boardPosition.y + board.height * scale / 2;

    checkPosition.x = boardPosition.x + board.width * scale / 2 + 100;
    checkPosition.y = boardPosition.y + board.height * scale / 2;

    settingPosition.x = screenWidth * 9 / 10.0f;
    settingPosition.y = screenHeight / 10.0f;

    returnButtonPosition.x = boardPosition.x + board.width * scale / 3.0f;
    returnButtonPosition.y = boardPosition.y + board.height * scale * 4 / 5.0f;

    savePosition.x = boardPosition.x + board.width * scale * 2.0f / 3.0f;
    savePosition.y = boardPosition.y + board.height * scale * 4 / 5.0f;

    saveLevelPosition.x = UIManager::getInstance().screenWidth / 20.0f * 1.0f;
    saveLevelPosition.y = UIManager::getInstance().screenHeight / 20.0f * 1.0f;

    loadLevelPosition.x = UIManager::getInstance().screenWidth / 20.0f * 2.5f;
    loadLevelPosition.y = UIManager::getInstance().screenHeight / 20.0f * 1.0f;

    slideBarMasterPosition.x = boardPosition.x + board.width * scale / 5.0f;
    slideBarMasterPosition.y = boardPosition.y + board.height * scale * 3 / 10.0f;

    slideBarMusicPosition.x = boardPosition.x + board.width * scale / 5.0f;
    slideBarMusicPosition.y = boardPosition.y + board.height * scale * 4.5 / 10.0f;

    slideBarSoundPosition.x = boardPosition.x + board.width * scale / 5.0f;
    slideBarSoundPosition.y = boardPosition.y + board.height * scale * 6 / 10.0f;

    manager.Init(4, UIManager::getInstance().screenHeight / 2.0f, UIManager::getInstance().screenWidth / 6.0f, UIManager::getInstance().screenHeight, SKYBLUE, MARIO_COLOR, LUIGI_COLOR, TOAD_COLOR, TOADETTE_COLOR);
    


    scale1 = (float)UIManager::getInstance().screenHeight / (float)level_map1.height;
    newWidth1 = level_map1.width * scale1;
    scale2 = (float)UIManager::getInstance().screenHeight / (float)level_map4.height;
    newWidth2 = level_map4.width * scale2;
    src1 = { 0, 0, (float)level_map1.width, (float)level_map1.height };
    dst1 = { backgroundOffsetX, 0, newWidth1, (float)UIManager::getInstance().screenHeight };
    origin1 = { 0, 0 };

    src2 = { 0, 0, (float)level_map4.width, (float)level_map4.height };
    dst2 = { backgroundOffsetX + newWidth1, 0, newWidth2, (float)UIManager::getInstance().screenHeight };
    origin2 = { 0, 0 };
    positionList = {
        {backgroundOffsetX + newWidth1 / 21.0f * 2.0f + newWidth1 / 42.0f, newWidth1 / 21.0f * 2.0f + newWidth1 / 42.0f},
        {backgroundOffsetX + newWidth1 / 21.0f * 2.0f + newWidth1 / 42.0f, newWidth1 / 21.0f * 4.0f},
        {backgroundOffsetX + newWidth1 / 21.0f * 3.0f, newWidth1 / 21.0f * 4.5f},
        {backgroundOffsetX + newWidth1 / 21.0f * 7.0f, newWidth1 / 21.0f * 4.5f},
        {backgroundOffsetX + newWidth1 / 21.0f * 8.0f, newWidth1 / 21.0f * 3.5f},
        {backgroundOffsetX + newWidth1 / 21.0f * 8.5f, newWidth1 / 21.0f * 3.5f}, //level 1
        {backgroundOffsetX + newWidth1 / 21.0f * 9.0f, newWidth1 / 21.0f * 3.5f},
        {backgroundOffsetX + newWidth1 / 21.0f * 11.0f, newWidth1 / 21.0f * 5.5f},
        {backgroundOffsetX + newWidth1 / 21.0f * 13.0f, newWidth1 / 21.0f * 5.5f},
        {backgroundOffsetX + newWidth1 / 21.0f * 13.5f, newWidth1 / 21.0f * 5.0f},
        {backgroundOffsetX + newWidth1 / 21.0f * 13.5f, newWidth1 / 21.0f * 1.5f}, //level 2
        {backgroundOffsetX + newWidth1 / 21.0f * 15.0f, newWidth1 / 21.0f * 1.5f}
    };

    mt.tex = mario;
    mt.pos = positionList[0];
    mt.currentIndex = 0;
}

MenuManager::~MenuManager() {
    UnloadTexture(logo);
    UnloadTexture(board);
    UnloadTexture(menuBackground);
    UnloadTexture(redirectBackground);
    UnloadTexture(mario);
    UnloadTexture(luigi);
    UnloadTexture(toad);
    UnloadTexture(toadette);

    UnloadTexture(level_map1);
    UnloadTexture(level_map4);
}

void MenuManager::DrawMenu() {
    ClearBackground(SKYBLUE);
    
    const float width = static_cast<float>(UIManager::getInstance().screenWidth);
    const float height = static_cast<float>(UIManager::getInstance().screenHeight);
    DrawTexturePro(menuBackground, { 0, 0, (float)menuBackground.width, (float)menuBackground.height }, {0, 0, width, height}, {0,0}, 0, WHITE);
    Rectangle recLogo; recLogo.x = 0; recLogo.y = 0; recLogo.width = 886; recLogo.height = 352;
    float scale = 0.75;
    DrawTextureEx(logo, { (width - recLogo.width * scale) / 2, height / 5 }, 0, scale, WHITE); // center

    Font currentMenuFont = UIManager::getInstance().menuFont;
    if (!settingDialog && !exitDialog) {
        playBoard.draw({ width / 2.0f, height * 6.0f / 10.0f }, "Play");
        //settingBoard.draw({ width / 2.0f, height * 7.0f / 10.0f }, "Setting");
        exitBoard.draw({ width / 2.0f, height * 7.0f / 10.0f }, "Exit");
        editingBoard.draw({ width / 2.0f, height * 8.0f / 10.0f }, "Editing");
    }

    if (settingDialog) {
        DrawSetting();
    }

    if (exitDialog) {
        DrawExit();
    }
}

void MenuManager::HandleInput() {
    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !settingDialog && !exitDialog) {
        if (playBoard.checkCollision(mousePos)) {
            AudioManager::getInstance().PlaySoundEffect("click"); select = 0;
        }
        if (settingBoard.checkCollision(mousePos)) {
            AudioManager::getInstance().PlaySoundEffect("click"); select = 1;
        }
        if (exitBoard.checkCollision(mousePos)) {
            AudioManager::getInstance().PlaySoundEffect("click"); select = 2;
            exitDialog = true;
        }
        if (editingBoard.checkCollision(mousePos)) {
            AudioManager::getInstance().PlaySoundEffect("click"); select = 3;
            std::cout << "Editor mode\n";
        }
    }

    if (settingDialog && !exitDialog) {
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && returnButton.checkCollision(mousePos)) {
            settingDialog = false;
        }

    }
    if (exitDialog && !settingDialog) {
        exit = HandleExit();
    }
}
void MenuManager::DrawExit() {
    const int screenWidth = UIManager::getInstance().screenWidth;
    const int screenHeight = UIManager::getInstance().screenHeight;
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 1.0f - 0.5f));
    DrawTextureEx(board, boardPosition, 0, scale, WHITE);


    Font currentMenuFont = UIManager::getInstance().menuFont;
    Vector2 textSize = MeasureTextEx(currentMenuFont, "Are you sure want to exit?", 40, 1);
    Vector2 textPosition = { boardPosition.x + (board.width * scale - textSize.x) / 2, boardPosition.y + 100 };
    DrawTextEx(currentMenuFont, "Are you sure want to exit?", textPosition, 40, 1, BROWN);

    Vector2 mousePos = GetMousePosition();
    cross.draw(crossPosition);

    check.draw(checkPosition);
}
bool MenuManager::HandleExit() {
    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && check.checkCollision(mousePos)) {
        return true;
    }
    else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && cross.checkCollision(mousePos)) {
        exitDialog = false;
    }
    return false;
}
void MenuManager::DrawSetting() {

    Vector2 mousePos = GetMousePosition();
    if (!settingDialog) setting.draw(settingPosition);

    if (settingDialog) {
        const int screenWidth = UIManager::getInstance().screenWidth;
        const int screenHeight = UIManager::getInstance().screenHeight;
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 1.0f - 0.5f));
        DrawTextureEx(board, boardPosition, 0, scale, WHITE);


        Font currentMenuFont = UIManager::getInstance().menuFont;
        Vector2 textSize = MeasureTextEx(currentMenuFont, "Setting", 40, 1);
        Vector2 textPosition = { boardPosition.x + (board.width * scale - textSize.x) / 2, boardPosition.y + 60 };
        DrawTextEx(currentMenuFont, "Setting", textPosition, 40, 1, BROWN);

        

        DrawTextEx(currentMenuFont, "Master: ", { slideBarMaster.getBorder().x - 100, slideBarMaster.getBorder().y + slideBarMaster.getBorder().height / 2 - 10 }, 20, 4, BORDER_BROWN);
        slideBarMaster.DrawSlideBar(slideBarMasterPosition);

        DrawTextEx(currentMenuFont, "Music: ", { slideBarMusic.getBorder().x - 100, slideBarMusic.getBorder().y + slideBarMusic.getBorder().height / 2 - 10 }, 20, 4, BORDER_BROWN);
        slideBarMusic.DrawSlideBar(slideBarMusicPosition);

        DrawTextEx(currentMenuFont, "Sound: ", { slideBarSound.getBorder().x - 100, slideBarSound.getBorder().y + slideBarSound.getBorder().height / 2 - 10 }, 20, 4, BORDER_BROWN);
        slideBarSound.DrawSlideBar(slideBarSoundPosition);

        returnButton.draw(returnButtonPosition);
        save.draw(savePosition);
    }
}
void MenuManager::HandleSetting() {
    Vector2 mousePos = GetMousePosition();
    if (!settingDialog && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && setting.checkCollision(mousePos)) {
        settingDialog = true;
    }

    if (settingDialog && IsKeyPressed(KEY_ENTER)) settingDialog = false;

    if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && returnButton.checkCollision(mousePos)) || IsKeyPressed(KEY_S)) {
        settingDialog = false;
    }
}

void MenuManager::UpdateMenu(float deltaTime) {
    playBoard.update(deltaTime);
    settingBoard.update(deltaTime);
    exitBoard.update(deltaTime);
    editingBoard.update(deltaTime);
} 

void MenuManager::UpdateExit(float deltaTime) {
    if (exitDialog) {
        check.update(deltaTime);
        cross.update(deltaTime);
    }
}
void MenuManager::UpdateSetting(float deltaTime) {
    setting.update(deltaTime);
    
    if (settingDialog) {
        returnButton.update(deltaTime);
        save.update(deltaTime);
        slideBarMaster.HandleSlideBar();
        slideBarMusic.HandleSlideBar();
        slideBarSound.HandleSlideBar();
    }
    bg_timer += deltaTime;
}

void MenuManager::HandleSelecting() {
    if (IsKeyPressed(KEY_A) && character01Select != 0) {
        --character01Select;
        AudioManager::getInstance().PlaySoundEffect("click");
    }
    if (IsKeyPressed(KEY_D) && character01Select != 3) {
        ++character01Select;
        AudioManager::getInstance().PlaySoundEffect("click");
    }
    if (character02Select == -1) {
        return;
    }
    if (IsKeyPressed(KEY_LEFT) && character02Select != 0) {
        --character02Select;
        AudioManager::getInstance().PlaySoundEffect("click");
    }
    if (IsKeyPressed(KEY_RIGHT) && character02Select != 3) {
        ++character02Select;
        AudioManager::getInstance().PlaySoundEffect("click");
    }

}

void MenuManager::DrawSelecting() {
    Vector2 mousePos = GetMousePosition();
    Vector2 screen = { UIManager::getInstance().screenWidth, UIManager::getInstance().screenHeight };

    manager.Draw();
    DrawText("Press Enter", 50, 100, 25, BLACK);
    if (character02Select != -1) {
        Vector2 textSize = MeasureTextEx(UIManager::getInstance().menuFont, "Choose your characters", 50, 10);
        DrawTextEx(UIManager::getInstance().menuFont, "Choose your characters", { (screen.x - textSize.x) / 2.0f, screen.y / 5.0f }, 50, 10, WHITE);
    }
    else {
        Vector2 textSize = MeasureTextEx(UIManager::getInstance().menuFont, "Choose your character", 50, 10);
        DrawTextEx(UIManager::getInstance().menuFont, "Choose your character", { (screen.x - textSize.x) / 2.0f, screen.y / 5.0f }, 50, 10, WHITE);
    }

    Vector2 marioPosition = { manager.getCenter(0).x - mario.width / 2.0f, manager.getCenter(0).y - mario.height / 2.0f };
    Vector2 luigiPosition = { manager.getCenter(1).x - luigi.width / 2.0f, manager.getCenter(1).y - luigi.height / 2.0f };
    Vector2 toadPosition = { manager.getCenter(2).x - toad.width / 2.0f, manager.getCenter(2).y - toad.height / 2.0f };
    Vector2 toadettePosition = { manager.getCenter(3).x - toadette.width / 2.0f, manager.getCenter(3).y - toadette.height / 2.0f };
    DrawTextureEx(mario, marioPosition, 0, manager.getScale(0), WHITE);
    DrawTextureEx(luigi, luigiPosition, 0, manager.getScale(1), WHITE);
    DrawTextureEx(toad, toadPosition, 0, manager.getScale(2), WHITE);
    DrawTextureEx(toadette, toadettePosition, 0, manager.getScale(3), WHITE);

    Vector2 marioText = MeasureTextEx(UIManager::getInstance().menuFont, "MARIO", 50, 10);
    Vector2 luigiText = MeasureTextEx(UIManager::getInstance().menuFont, "LUIGI", 50, 10);
    Vector2 toadText = MeasureTextEx(UIManager::getInstance().menuFont, "TOAD", 50, 10);
    Vector2 toadetteText = MeasureTextEx(UIManager::getInstance().menuFont, "TOADETTE", 50, 10);

    Vector2 aa = { -1, -1 };
    Vector2 bb = { -1, -1 };

    for (int i = 0; i < manager.getSize(); i++) {
        if (character01Select == i) {
            manager.textColor[i] = YELLOW;
            switch (i) {
            case 0:
                aa = marioPosition;
                break;
            case 1:
                aa = luigiPosition;
                break;
            case 2:
                aa = toadPosition;
                break;
            case 3:
                aa = toadettePosition;
                break;
            }
        }
        if (character02Select == i) {
            if (character01Select == character02Select) {
                manager.textColor[i] = BLUE;
            }
            else {
                manager.textColor[i] = DARKPURPLE;
            }
            switch (i) {
            case 0:
                bb = marioPosition;
                break;
            case 1:
                bb = luigiPosition;
                break;
            case 2:
                bb = toadPosition;
                break;
            case 3:
                bb = toadettePosition;
                break;
            }
        }
        if (i != character01Select && i != character02Select) {
            manager.textColor[i] = WHITE;
        }
    }

    DrawTextEx(UIManager::getInstance().menuFont, "MARIO",
        { marioPosition.x + mario.width / 2.0f - marioText.x / 2.0f, marioPosition.y + mario.height + marioText.y }, 50, 10, manager.textColor[0]);
    DrawTextEx(UIManager::getInstance().menuFont, "LUIGI",
        { luigiPosition.x + luigi.width / 2.0f - luigiText.x / 2.0f, luigiPosition.y + luigi.height + luigiText.y }, 50, 10, manager.textColor[1]);
    DrawTextEx(UIManager::getInstance().menuFont, "TOAD",
        { toadPosition.x + toad.width / 2.0f - toadText.x / 2.0f, toadPosition.y + toad.height + toadText.y }, 50, 10, manager.textColor[2]);
    DrawTextEx(UIManager::getInstance().menuFont, "TOADETTE",
        { toadettePosition.x + toadette.width / 2.0f - toadetteText.x / 2.0f, toadettePosition.y + toadette.height + toadetteText.y }, 50, 10, manager.textColor[3]);

    if (character01Select != -1 && character02Select != -1) {
        DrawTextEx(UIManager::getInstance().menuFont, "P1",
            { aa.x + mario.width / 2.0f - marioText.x / 2.0f - 20.0f, marioPosition.y + mario.height + marioText.y + 200.0f }, 50, 10, YELLOW);
        DrawTextEx(UIManager::getInstance().menuFont, "P2",
            { bb.x + mario.width / 2.0f - marioText.x / 2.0f + 150.0f, marioPosition.y + mario.height + marioText.y + 200.0f }, 50, 10, DARKPURPLE);
    }
}

void MenuManager::UpdateSelecting(float deltaTime) {
    manager.Update(deltaTime, character01Select, character02Select);
}

void MenuManager::HandleRedirect() {

}

void MenuManager::UpdateRedirect(float deltaTime) {
    characterBoard.update(deltaTime);
    continueBoard.update(deltaTime);
    menuBoard.update(deltaTime);

}

void MenuManager::DrawRedirect() {
    float scale = UIManager::getInstance().screenHeight / (float)redirectBackground.height;
    float newWidth = redirectBackground.width * scale;
    float newHeight = UIManager::getInstance().screenHeight;

    // Vị trí để căn giữa theo trục dọc
    float posX = (UIManager::getInstance().screenWidth - newWidth) / 2.0f;
    float posY = 0;

    Rectangle src = { 0, 0, (float)redirectBackground.width, (float)redirectBackground.height };
    Rectangle dst = { posX, posY, newWidth, newHeight };
    Vector2 origin = { 0, 0 };

    DrawTexturePro(redirectBackground, src, dst, origin, 0.0f, WHITE);

    characterBoard.draw({ UIManager::getInstance().screenWidth / 2.0f, UIManager::getInstance().screenHeight * 7 / 20.0f }, "NewGame", 1.5, 1.0);
    continueBoard.draw({ UIManager::getInstance().screenWidth / 2.0f, UIManager::getInstance().screenHeight * 9 / 20.0f }, "Continue", 1.5, 1.0);
    menuBoard.draw({ UIManager::getInstance().screenWidth / 2.0f, UIManager::getInstance().screenHeight * 11 / 20.0f }, "Menu", 1.5, 1.0);
}

void MenuManager::HandleLevelRedirect() {
    
}

void MenuManager::DrawLevelRedirect() {
    available.draw({ UIManager::getInstance().screenWidth / 4.0f, UIManager::getInstance().screenHeight / 2.0f }, "Available", 1.5, 1.0);
    custom.draw({ UIManager::getInstance().screenWidth * 3.0f / 4.0f, UIManager::getInstance().screenHeight / 2.0f }, "Custom", 1.5, 1.0);
}

void MenuManager::UpdateLevelRedirect(float deltaTime) {
    available.update(deltaTime);
    custom.update(deltaTime);
}



void MenuManager::HandleEditorSelecting() {

}

void MenuManager::UpdateEditorSelecting(float deltaTime) {
    day_groundBoard.update(deltaTime);
    day_undergroundBoard.update(deltaTime);
    night_airshipBoard.update(deltaTime);
    night_snowBoard.update(deltaTime);
}
void MenuManager::DrawEditorSelecting() {
    day_groundBoard.draw({ UIManager::getInstance().screenWidth / 2.0f, UIManager::getInstance().screenHeight * 7 / 20.0f }, "Map 1", 1.5, 1.0);
    day_undergroundBoard.draw({ UIManager::getInstance().screenWidth / 2.0f, UIManager::getInstance().screenHeight * 9 / 20.0f }, "Map 2", 1.5, 1.0);
    night_airshipBoard.draw({ UIManager::getInstance().screenWidth / 2.0f, UIManager::getInstance().screenHeight * 11 / 20.0f }, "Map 3", 1.5, 1.0);
    night_snowBoard.draw({ UIManager::getInstance().screenWidth / 2.0f, UIManager::getInstance().screenHeight * 13 / 20.0f }, "Map 4", 1.5, 1.0);
}

void MenuManager::DrawPlayer() {
    Rectangle source1 = { 0, 0, (float)mario.width, (float)mario.height };
    Rectangle dest1 = { UIManager::getInstance().screenWidth / 3.0f, UIManager::getInstance().screenHeight / 2.0f,
                        (float)mario.width, (float)mario.height };
    Vector2 origin = { (float)mario.width / 2, (float)mario.height / 2 };

    DrawTexturePro(mario, source1, dest1, origin, 0.0f, WHITE);

    // Texture 2: tâm tại (screen.x * 2/3, screen.y / 2)
    Rectangle source2 = { 0,0,(float)mario.width * (-1), (float)mario.height };
    Rectangle dest2 = { UIManager::getInstance().screenWidth * 2.0f / 3.0f, UIManager::getInstance().screenHeight / 2.0f,
                        (float)mario.width, (float)mario.height };

    Rectangle source3 = { 0,0,(float)luigi.width * (-1), (float)luigi.height };
    Rectangle dest3 = { UIManager::getInstance().screenWidth * 2.0f / 3.0f + luigi.width / 2.0f, UIManager::getInstance().screenHeight / 2.0f,
                        (float)luigi.width, (float)luigi.height };

    DrawTexturePro(mario, source2, dest2, origin, 0.0f, WHITE);
    DrawTexturePro(luigi, source3, dest3, origin, 0.0f, WHITE);
    OnePlayer.draw({ dest1.x, dest1.y + mario.height }, "1 player", 1.5, 1.0);
    TwoPlayers.draw({ (dest2.x + dest3.x) / 2.0f, (dest2.y + dest3.y) / 2.0f + mario.height }, "2 players", 1.5, 1.0);

    /*
    Vector2 Center1 = { UIManager::getInstance().screenWidth / 3.0f, UIManager::getInstance().screenHeight / 2.0f };
    Vector2 Center2 = { UIManager::getInstance().screenWidth * 2 / 3.0f, UIManager::getInstance().screenHeight / 2.0f };
    DrawTexturePro(mario, { 0, 0, (float)mario.width, (float)mario.height}, {Center1.x, Center1.y, (float)mario.width, (float)mario.height}, {1.5f * Center1.x, 1.5f * Center1.y}, 0.0f, WHITE);
    DrawTexturePro(mario, { 0, 0, (float)mario.width * (-1), (float)mario.height }, { Center2.x, Center2.y, (float)mario.width, (float)mario.height }, { 1.5f * Center2.x, 1.5f * Center2.y }, 0.0f, WHITE);
    */
}

void MenuManager::UpdatePlayer(float deltaTime) {
    OnePlayer.update(deltaTime);
    TwoPlayers.update(deltaTime);
}

void MenuManager::HandlePlayer() {

}






void MenuManager::DrawLevel() {
    DrawTexturePro(level_map1, src1, { backgroundOffsetX, 0, newWidth1, (float)UIManager::getInstance().screenHeight }, origin1, 0.0f, WHITE);
    DrawTexturePro(level_map4, src2, { backgroundOffsetX + newWidth1, 0, newWidth2, (float)UIManager::getInstance().screenHeight }, origin2, 0.0f, WHITE);

    //float screenCenterX = UIManager::getInstance().screenWidth / 2.0f;
    //float marioScreenX = (mt.pos.x < screenCenterX) ? mt.pos.x : screenCenterX;
    
    DrawTextureEx(mt.tex,
        { mt.pos.x + backgroundOffsetX - mt.tex.width / 2 * 0.5f,
        mt.pos.y - mt.tex.height / 2 * 0.5f }, 0.0f, 0.5f,
        WHITE);
    Vector2 instruction = MeasureTextEx(UIManager::getInstance().menuFont, "Select level and press Enter", 30, 1.0f);
    DrawTextEx(UIManager::getInstance().menuFont, "Select level and press Enter", {UIManager::getInstance().screenWidth / 2.0f - instruction.x / 2.0f, UIManager::getInstance().screenHeight * 4  /5.0f}, 30, 1.0f, BLACK);
}

void MenuManager::UpdateLevel(float deltaTime) {
    /*
    Vector2 mouse = GetMousePosition();
    float speed = 300.0f;

    if (mouse.x < UIManager::getInstance().screenWidth / 3.0f) {
        backgroundOffsetX += speed * deltaTime;
    }
    else if (mouse.x > UIManager::getInstance().screenWidth * 2.0f / 3.0f){
        backgroundOffsetX -= speed * deltaTime;
    }

    float backgroundWidth = newWidth1 + newWidth2;
    if (backgroundOffsetX > 0) backgroundOffsetX = 0;
    if (backgroundOffsetX < UIManager::getInstance().screenWidth - backgroundWidth)
        backgroundOffsetX = UIManager::getInstance().screenWidth - backgroundWidth;
    */
    mt.UpdateMovingTexture(positionList, deltaTime);

    positionList = {
        {backgroundOffsetX + newWidth1 / 21.0f * 2.0f + newWidth1 / 42.0f, newWidth1 / 21.0f * 2.0f + newWidth1 / 42.0f},
        {backgroundOffsetX + newWidth1 / 21.0f * 2.0f + newWidth1 / 42.0f, newWidth1 / 21.0f * 4.0f},
        {backgroundOffsetX + newWidth1 / 21.0f * 3.0f, newWidth1 / 21.0f * 4.5f},
        {backgroundOffsetX + newWidth1 / 21.0f * 7.0f, newWidth1 / 21.0f * 4.5f},
        {backgroundOffsetX + newWidth1 / 21.0f * 8.0f, newWidth1 / 21.0f * 3.5f},
        {backgroundOffsetX + newWidth1 / 21.0f * 8.5f, newWidth1 / 21.0f * 3.5f}, //level 1
        {backgroundOffsetX + newWidth1 / 21.0f * 9.0f, newWidth1 / 21.0f * 3.5f},
        {backgroundOffsetX + newWidth1 / 21.0f * 11.0f, newWidth1 / 21.0f * 5.5f},
        {backgroundOffsetX + newWidth1 / 21.0f * 13.0f, newWidth1 / 21.0f * 5.5f},
        {backgroundOffsetX + newWidth1 / 21.0f * 13.5f, newWidth1 / 21.0f * 5.0f},
        {backgroundOffsetX + newWidth1 / 21.0f * 13.5f, newWidth1 / 21.0f * 1.5f}, //level 2
        {backgroundOffsetX + newWidth1 / 21.0f + 15.0f, newWidth1 / 21.0f * 1.5f}
    };
}

/*
void MenuManager::DrawLevel() {
    scale1 = (float)UIManager::getInstance().screenHeight / (float)level_map1.height;
    newWidth1 = level_map1.width * scale1;
    scale2 = (float)UIManager::getInstance().screenHeight / (float)level_map4.height;
    newWidth2 = level_map4.width * scale2;
    src1 = { 0, 0, (float)level_map1.width, (float)level_map1.height };
    dst1 = { backgroundOffsetX, 0, newWidth1, (float)UIManager::getInstance().screenHeight };
    origin1 = { 0, 0 };
    DrawTexturePro(level_map1, src1, dst1, origin1, 0.0f, WHITE);

    src2 = { 0, 0, (float)level_map4.width, (float)level_map4.height };
    dst2 = { backgroundOffsetX + newWidth1, 0, newWidth2, (float)UIManager::getInstance().screenHeight };
    origin2 = { 0, 0 };

    positionList = {
        {backgroundOffsetX + newWidth1 / 21.0f * 2.0f + newWidth1 / 42.0f, newWidth1 / 21.0f * 2.0f + newWidth1 / 42.0f},
        {backgroundOffsetX + newWidth1 / 21.0f * 2.0f + newWidth1 / 42.0f, newWidth1 / 21.0f * 4.0f},
        {backgroundOffsetX + newWidth1 / 21.0f * 3.0f, newWidth1 / 21.0f * 4.5f},
        {backgroundOffsetX + newWidth1 / 21.0f * 7.0f, newWidth1 / 21.0f * 4.5f},
        {backgroundOffsetX + newWidth1 / 21.0f * 8.0f, newWidth1 / 21.0f * 3.5f},
        {backgroundOffsetX + newWidth1 / 21.0f * 8.5f, newWidth1 / 21.0f * 3.5f}, //level 1
        {backgroundOffsetX + newWidth1 / 21.0f * 9.0f, newWidth1 / 21.0f * 3.5f},
        {backgroundOffsetX + newWidth1 / 21.0f * 11.0f, newWidth1 / 21.0f * 5.5f},
        {backgroundOffsetX + newWidth1 / 21.0f * 13.0f, newWidth1 / 21.0f * 5.5f},
        {backgroundOffsetX + newWidth1 / 21.0f * 13.5f, newWidth1 / 21.0f * 5.0f},
        {backgroundOffsetX + newWidth1 / 21.0f * 13.5f, newWidth1 / 21.0f * 2.5f} //level 2
    };

    DrawTexturePro(level_map4, src2, dst2, origin2, 0.0f, WHITE);
    //DrawTexture(mt.tex, mt.pos.x - mt.tex.width / 2, mt.pos.y - mt.tex.height / 2, WHITE);
    DrawTextureEx(mt.tex, {mt.pos.x - mt.tex.width / 2.0f,mt.pos.y - mt.tex.height / 2.0f}, 0, 0.5f, WHITE);
    //DrawTextureEx(mario, {positionList[0].x - mario.width * 0.25f, positionList[0].y - mario.height * 0.25f}, 0, 0.5f, WHITE);
    
}

void MenuManager::UpdateLevel(float deltaTime) {
    //float delta = GetFrameTime();
    Vector2 mouse = mt.pos;
    float speed = 200.0f;
    if (mouse.x < UIManager::getInstance().screenWidth / 2.0f - 20) {
        backgroundOffsetX += speed * deltaTime;
    }
    else if (mouse.x > UIManager::getInstance().screenWidth / 2.0f + 20) {
        backgroundOffsetX -= speed * deltaTime;
    }

    if (backgroundOffsetX > 0) backgroundOffsetX = 0;
    float backgroundWidth = level_map1.width * (float)UIManager::getInstance().screenHeight / (float)level_map1.height + 
							  level_map4.width * (float)UIManager::getInstance().screenHeight / (float)level_map4.height;
    if (backgroundOffsetX < UIManager::getInstance().screenWidth - backgroundWidth)
        backgroundOffsetX = UIManager::getInstance().screenWidth - backgroundWidth;

    mt.UpdateMovingTexture(positionList, deltaTime);
}
*/
void MenuManager::HandleLevel() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        mt.HandleClick(positionList, GetMousePosition());
    }
}