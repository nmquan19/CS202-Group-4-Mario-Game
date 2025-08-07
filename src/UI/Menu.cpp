#include "../../include/UI/Menu.h"

MenuManager::MenuManager() {
    isActive = true;

    logo = LoadTexture("./assets/Super Mario Bros.png");
    select = 0; characterSelect = 0;
    settingDialog = false;
    exitDialog = false;

    exit = false;
    board = LoadTexture("./assets/button/board1.png");

    check.load("./assets/button/check.png", "./assets/button/check_selected.png");

    cross.load("./assets/button/cross.png", "./assets/button/cross_selected.png");

    setting.load("./assets/button/option.png", "./assets/button/option_selected.png");

    returnButton.load("./assets/button/back.png", "./assets/button/back_selected.png");

    playBoard.load("./assets/button/thin_board.png", "./assets/button/thin_board_selected.png");
    settingBoard.load("./assets/button/thin_board.png", "./assets/button/thin_board_selected.png");
    exitBoard.load("./assets/button/thin_board.png", "./assets/button/thin_board_selected.png");
    editingBoard.load("./assets/button/thin_board.png", "./assets/button/thin_board_selected.png");



    //character = LoadTexture("./assets/character_sample.png");
    mario = LoadTexture("./assets/Mario.png");
    luigi = LoadTexture("./assets/Luigi.png");
    toad = LoadTexture("./assets/Toad.png");
    toadette = LoadTexture("./assets/Toadette.png");

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

    returnButtonPosition.x = boardPosition.x + board.width * scale / 2.0f;
    returnButtonPosition.y = boardPosition.y + board.height * scale * 3 / 4.0f;

    slideBarMusicPosition.x = boardPosition.x + board.width * scale / 5.0f;
    slideBarMusicPosition.y = boardPosition.y + board.height * scale * 3 / 10.0f;

    slideBarSoundPosition.x = boardPosition.x + board.width * scale / 5.0f;
    slideBarSoundPosition.y = boardPosition.y + board.height * scale * 5 / 10.0f;

    //manager.Init(4, 100.0f, 400.0f, 100.0f, RED, BLUE);
}

MenuManager::~MenuManager() {
    UnloadTexture(logo);
    UnloadTexture(board);
    //UnloadTexture(character);
    UnloadTexture(mario);
    UnloadTexture(luigi);
    UnloadTexture(toad);
    UnloadTexture(toadette);
}

void MenuManager::DrawMenu() {
    ClearBackground(SKYBLUE);
    const float width = static_cast<float>(UIManager::getInstance().screenWidth);
    const float height = static_cast<float>(UIManager::getInstance().screenHeight);
    Rectangle recLogo; recLogo.x = 0; recLogo.y = 0; recLogo.width = 886; recLogo.height = 352;
    float scale = 0.75;
    DrawTextureEx(logo, { (width - recLogo.width * scale) / 2, height / 5 }, 0, scale, WHITE); // center
    //thin_board.draw({ 300, 500 }, "Text");

    Font currentMenuFont = UIManager::getInstance().menuFont;
    if (!settingDialog && !exitDialog) {
        playBoard.draw({ width / 2.0f, height * 6.0f / 10.0f }, "Play");
        settingBoard.draw({ width / 2.0f, height * 7.0f / 10.0f }, "Setting");
        exitBoard.draw({ width / 2.0f, height * 8.0f / 10.0f }, "Exit");
        editingBoard.draw({ width / 2.0f, height * 9.0f / 10.0f }, "Editing");
    }
    /*
    if (select == 0 && !settingDialog && !exitDialog) {
        DrawTextEx(currentMenuFont, "Play", { width * 2 / 5, height * 2 / 3 }, 50, 10, YELLOW);
        DrawTextEx(currentMenuFont, "Setting", { width * 2 / 5 , height * 2 / 3 + 40 }, 50, 10, WHITE);
        DrawTextEx(currentMenuFont, "Exit", { width * 2 / 5 , height * 2 / 3 + 80 }, 50, 10, WHITE);
        DrawTextEx(currentMenuFont, "Editor Mode", { width * 2 / 5 , height * 2 / 3 + 120 }, 50, 10, WHITE);
    }
    else if (select == 1 && !settingDialog && !exitDialog) {
        DrawTextEx(currentMenuFont, "Play", { width * 2 / 5, height * 2 / 3 }, 50, 10, WHITE);
        DrawTextEx(currentMenuFont, "Setting", { width * 2 / 5 , height * 2 / 3 + 40 }, 50, 10, YELLOW);
        DrawTextEx(currentMenuFont, "Exit", { width * 2 / 5 , height * 2 / 3 + 80 }, 50, 10, WHITE);
        DrawTextEx(currentMenuFont, "Editor Mode", { width * 2 / 5 , height * 2 / 3 + 120 }, 50, 10, WHITE);
    }
    else if (select == 2 && !settingDialog && !exitDialog) {
        DrawTextEx(currentMenuFont, "Play", { width * 2 / 5, height * 2 / 3 }, 50, 10, WHITE);
        DrawTextEx(currentMenuFont, "Setting", { width * 2 / 5 , height * 2 / 3 + 40 }, 50, 10, WHITE);
        DrawTextEx(currentMenuFont, "Exit", { width * 2 / 5 , height * 2 / 3 + 80 }, 50, 10, YELLOW);
        DrawTextEx(currentMenuFont, "Editor Mode", { width * 2 / 5 , height * 2 / 3 + 120 }, 50, 10, WHITE);
    }
    else if (select == 3 && !settingDialog && !exitDialog) {
        DrawTextEx(currentMenuFont, "Play", { width * 2 / 5, height * 2 / 3 }, 50, 10, WHITE);
        DrawTextEx(currentMenuFont, "Setting", { width * 2 / 5 , height * 2 / 3 + 40 }, 50, 10, WHITE);
        DrawTextEx(currentMenuFont, "Exit", { width * 2 / 5 , height * 2 / 3 + 80 }, 50, 10, WHITE);
        DrawTextEx(currentMenuFont, "Editor Mode", { width * 2 / 5 , height * 2 / 3 + 120 }, 50, 10, YELLOW);
    }
    */
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
    /*
    if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_U)) && select != 0 && !settingDialog && !exitDialog) {
        select--;
        AudioManager::getInstance().PlaySoundEffect("click");

    }
    if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_D)) && select != 3 && !settingDialog && !exitDialog) {
        select++;
        AudioManager::getInstance().PlaySoundEffect("click");

    }

    if (IsKeyPressed(KEY_ENTER) && select == 1) {

    }
    if (IsKeyPressed(KEY_ENTER) && select == 2) {
        exitDialog = true;
    }
    if (IsKeyPressed(KEY_ENTER) && select == 3) {
        std::cout << "Editor mode\n";
    }
    */
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

        returnButton.draw(returnButtonPosition);

        DrawTextEx(currentMenuFont, "Music: ", { slideBarMusic.getBorder().x - 100, slideBarMusic.getBorder().y + slideBarMusic.getBorder().height / 2 - 10 }, 20, 4, BORDER_BROWN);
        slideBarMusic.DrawSlideBar(slideBarMusicPosition);

        DrawTextEx(currentMenuFont, "Sound: ", { slideBarSound.getBorder().x - 100, slideBarSound.getBorder().y + slideBarSound.getBorder().height / 2 - 10 }, 20, 4, BORDER_BROWN);
        slideBarSound.DrawSlideBar(slideBarSoundPosition);
    }
}
void MenuManager::HandleSetting() {
    Vector2 mousePos = GetMousePosition();
    if (!settingDialog && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && setting.checkCollision(mousePos)) {
        settingDialog = true;
    }

    if (settingDialog && IsKeyPressed(KEY_ENTER)) settingDialog = false;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && returnButton.checkCollision(mousePos)) {
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
        slideBarMusic.HandleSlideBar();
        slideBarSound.HandleSlideBar();
    }
}

void MenuManager::HandleSelecting() {
    if (IsKeyPressed(KEY_LEFT) && characterSelect != 0) {
        characterSelect--;
        AudioManager::getInstance().PlaySoundEffect("click");
    }
    if (IsKeyPressed(KEY_RIGHT) && characterSelect != 3) {
        characterSelect++;
        AudioManager::getInstance().PlaySoundEffect("click");
    }
}

void MenuManager::DrawSelecting() {
    Vector2 mousePos = GetMousePosition();
    Vector2 screen = { UIManager::getInstance().screenWidth, UIManager::getInstance().screenHeight };

    /*
    Parallelogram p11, p12;

    p11.DrawParallelogramGradient(
        { screen.x / 4.0f, screen.y / 4.0f },
        { screen.x / 2.0f, screen.y / 4.0f },
        { screen.x * 3.0f / 4.0f, screen.y / 2.0f },
        { screen.x / 2.0f, screen.y / 2.0f }, SKYBLUE, MARIO_COLOR);


    if (p11.CheckCollisionPointParallelogram(mousePos)) DrawText("Collide", 100, 100, 50, WHITE);
    */
    //manager.Draw();
    Vector2 textSize = MeasureTextEx(UIManager::getInstance().menuFont, "Select a player", 50, 10);
    DrawText("Press Enter", 50, 100, 25, BLACK);
    DrawTextEx(UIManager::getInstance().menuFont, "Select a player", { (screen.x - textSize.x) / 2.0f, screen.y / 5.0f }, 50, 10, WHITE);
    Vector2 marioPosition = { screen.x / 5.0f - mario.width / 2.0f, screen.y / 3.0f };
    Vector2 luigiPosition = { screen.x * 2.0f / 5.0f - luigi.width / 2.0f, screen.y / 3.0f };
    Vector2 toadPosition = { screen.x * 3.0f / 5.0f - toad.width / 2.0f, screen.y / 3.0f };
    Vector2 toadettePosition = { screen.x * 4.0f / 5.0f - toadette.width / 2.0f, screen.y / 3.0f };
    DrawTextureEx(mario, marioPosition, 0, 1.0f, WHITE);
    DrawTextureEx(luigi, luigiPosition, 0, 1.0f, WHITE);
    DrawTextureEx(toad, toadPosition, 0, 1.0f, WHITE);
    DrawTextureEx(toadette, toadettePosition, 0, 1.0f, WHITE);

    Vector2 marioText = MeasureTextEx(UIManager::getInstance().menuFont, "MARIO", 50, 10);
    Vector2 luigiText = MeasureTextEx(UIManager::getInstance().menuFont, "LUIGI", 50, 10);
    Vector2 toadText = MeasureTextEx(UIManager::getInstance().menuFont, "TOAD", 50, 10);
    Vector2 toadetteText = MeasureTextEx(UIManager::getInstance().menuFont, "TOADETTE", 50, 10);

    if (characterSelect == 0) {
        DrawTextEx(UIManager::getInstance().menuFont, "MARIO",
            { marioPosition.x + mario.width / 2.0f - marioText.x / 2.0f, marioPosition.y + mario.height }, 50, 10, YELLOW);
        DrawTextEx(UIManager::getInstance().menuFont, "LUIGI",
            { luigiPosition.x + luigi.width / 2.0f - luigiText.x / 2.0f, luigiPosition.y + luigi.height }, 50, 10, WHITE);
        DrawTextEx(UIManager::getInstance().menuFont, "TOAD",
            { toadPosition.x + toad.width / 2.0f - toadText.x / 2.0f, toadPosition.y + toad.height }, 50, 10, WHITE);
        DrawTextEx(UIManager::getInstance().menuFont, "TOADETTE",
            { toadettePosition.x + toadette.width / 2.0f - toadetteText.x / 2.0f, toadettePosition.y + toadette.height }, 50, 10, WHITE);
    }
    if (characterSelect == 1) {
        DrawTextEx(UIManager::getInstance().menuFont, "MARIO",
            { marioPosition.x + mario.width / 2.0f - marioText.x / 2.0f, marioPosition.y + mario.height }, 50, 10, WHITE);
        DrawTextEx(UIManager::getInstance().menuFont, "LUIGI",
            { luigiPosition.x + luigi.width / 2.0f - luigiText.x / 2.0f, luigiPosition.y + luigi.height }, 50, 10, YELLOW);
        DrawTextEx(UIManager::getInstance().menuFont, "TOAD",
            { toadPosition.x + toad.width / 2.0f - toadText.x / 2.0f, toadPosition.y + toad.height }, 50, 10, WHITE);
        DrawTextEx(UIManager::getInstance().menuFont, "TOADETTE",
            { toadettePosition.x + toadette.width / 2.0f - toadetteText.x / 2.0f, toadettePosition.y + toadette.height }, 50, 10, WHITE);
    }
    if (characterSelect == 2) {
        DrawTextEx(UIManager::getInstance().menuFont, "MARIO",
            { marioPosition.x + mario.width / 2.0f - marioText.x / 2.0f, marioPosition.y + mario.height }, 50, 10, WHITE);
        DrawTextEx(UIManager::getInstance().menuFont, "LUIGI",
            { luigiPosition.x + luigi.width / 2.0f - luigiText.x / 2.0f, luigiPosition.y + luigi.height }, 50, 10, WHITE);
        DrawTextEx(UIManager::getInstance().menuFont, "TOAD",
            { toadPosition.x + toad.width / 2.0f - toadText.x / 2.0f, toadPosition.y + toad.height }, 50, 10, YELLOW);
        DrawTextEx(UIManager::getInstance().menuFont, "TOADETTE",
            { toadettePosition.x + toadette.width / 2.0f - toadetteText.x / 2.0f, toadettePosition.y + toadette.height }, 50, 10, WHITE);
    }
    if (characterSelect == 3) {
        DrawTextEx(UIManager::getInstance().menuFont, "MARIO",
            { marioPosition.x + mario.width / 2.0f - marioText.x / 2.0f, marioPosition.y + mario.height }, 50, 10, WHITE);
        DrawTextEx(UIManager::getInstance().menuFont, "LUIGI",
            { luigiPosition.x + luigi.width / 2.0f - luigiText.x / 2.0f, luigiPosition.y + luigi.height }, 50, 10, WHITE);
        DrawTextEx(UIManager::getInstance().menuFont, "TOAD",
            { toadPosition.x + toad.width / 2.0f - toadText.x / 2.0f, toadPosition.y + toad.height }, 50, 10, WHITE);
        DrawTextEx(UIManager::getInstance().menuFont, "TOADETTE",
            { toadettePosition.x + toadette.width / 2.0f - toadetteText.x / 2.0f, toadettePosition.y + toadette.height }, 50, 10, YELLOW);
    }

}
void MenuManager::UpdateSelecting(float deltaTime) {
    //manager.Update(deltaTime);
}