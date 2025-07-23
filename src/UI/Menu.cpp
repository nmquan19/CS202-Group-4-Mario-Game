#include "../../include/UI/Menu.h"

MenuManager::MenuManager() {
    isActive = true;

    logo = LoadTexture("./assets/Super Mario Bros.png");
    select = 0;
    settingDialog = false;
    exitDialog = false;

    exit = false;
    board = LoadTexture("./assets/button/board.png");

    check.load("./assets/button/check.png");
    check_selected.load("./assets/button/check_selected.png");

    cross.load("./assets/button/cross.png");
    cross_selected.load("./assets/button/cross_selected.png");

    setting.load("./assets/button/setting.png");
    setting_selected.load("./assets/button/setting_selected.png");

    returnButton.load("./assets/button/return.png");
    returnButton_selected.load("./assets/button/return_selected.png");

    home.load("./assets/button/home.png");
    play.load("./assets/button/play.png");

    boardPosition.x = (static_cast<float>(screenWidth) - board.width * scale) / 2;
    boardPosition.y = (static_cast<float>(screenHeight) - board.height * scale) / 2;

    crossPosition.x = boardPosition.x + board.width * scale / 2 - 50 - 100;
    crossPosition.y = boardPosition.y + board.height * scale / 2;

    checkPosition.x = boardPosition.x + board.width * scale / 2 - 50 + 100;
    checkPosition.y = boardPosition.y + board.height * scale / 2;

    settingPosition.x = GetScreenWidth() * 9 / 10.0f;
    settingPosition.y = GetScreenHeight() / 10.0f;

    returnButtonPosition.x = boardPosition.x + board.width * scale / 2.0f - 50;
    returnButtonPosition.y = boardPosition.y + board.height * scale * 3 / 4.0f;

    slideBarMusicPosition.x = boardPosition.x + board.width * scale / 5.0f;
    slideBarMusicPosition.y = boardPosition.y + board.height * scale * 3 / 10.0f;

    slideBarSoundPosition.x = boardPosition.x + board.width * scale / 5.0f;
    slideBarSoundPosition.y = boardPosition.y + board.height * scale * 5 / 10.0f;
}

MenuManager::~MenuManager() {
    UnloadFont(menuFont);
    UnloadTexture(logo);
    UnloadTexture(board);
}

void MenuManager::DrawMenu() {
    ClearBackground(SKYBLUE);
    float width = static_cast<float>(screenWidth);
    float height = static_cast<float>(screenHeight);
    Rectangle recLogo; recLogo.x = 0; recLogo.y = 0; recLogo.width = 886; recLogo.height = 352;
    float scale = 0.75;
    DrawTextureEx(logo, { (width - recLogo.width * scale) / 2, height / 5 }, 0, scale, WHITE); // center
    if (select == 0 && !settingDialog && !exitDialog) {
        DrawTextEx(menuFont, "Play", { width * 2 / 5, height * 2 / 3 }, 50, 10, YELLOW);
        DrawTextEx(menuFont, "Setting", { width * 2 / 5 , height * 2 / 3 + 40 }, 50, 10, WHITE);
        DrawTextEx(menuFont, "Exit", { width * 2 / 5 , height * 2 / 3 + 80 }, 50, 10, WHITE);
        DrawTextEx(menuFont, "Editor Mode", { width * 2 / 5 , height * 2 / 3 + 120 }, 50, 10, WHITE);
    }
    else if (select == 1 && !settingDialog && !exitDialog) {
        DrawTextEx(menuFont, "Play", { width * 2 / 5, height * 2 / 3 }, 50, 10, WHITE);
        DrawTextEx(menuFont, "Setting", { width * 2 / 5 , height * 2 / 3 + 40 }, 50, 10, YELLOW);
        DrawTextEx(menuFont, "Exit", { width * 2 / 5 , height * 2 / 3 + 80 }, 50, 10, WHITE);
        DrawTextEx(menuFont, "Editor Mode", { width * 2 / 5 , height * 2 / 3 + 120 }, 50, 10, WHITE);
    }
    else if (select == 2 && !settingDialog && !exitDialog) {
        DrawTextEx(menuFont, "Play", { width * 2 / 5, height * 2 / 3 }, 50, 10, WHITE);
        DrawTextEx(menuFont, "Setting", { width * 2 / 5 , height * 2 / 3 + 40 }, 50, 10, WHITE);
        DrawTextEx(menuFont, "Exit", { width * 2 / 5 , height * 2 / 3 + 80 }, 50, 10, YELLOW);
        DrawTextEx(menuFont, "Editor Mode", { width * 2 / 5 , height * 2 / 3 + 120 }, 50, 10, WHITE);
    }
    else if (select == 3 && !settingDialog && !exitDialog) {
        DrawTextEx(menuFont, "Play", { width * 2 / 5, height * 2 / 3 }, 50, 10, WHITE);
        DrawTextEx(menuFont, "Setting", { width * 2 / 5 , height * 2 / 3 + 40 }, 50, 10, WHITE);
        DrawTextEx(menuFont, "Exit", { width * 2 / 5 , height * 2 / 3 + 80 }, 50, 10, WHITE);
        DrawTextEx(menuFont, "Editor Mode", { width * 2 / 5 , height * 2 / 3 + 120 }, 50, 10, YELLOW);
    }
    if (settingDialog) {
        DrawSetting();
    }
    if (exitDialog) {
        DrawExit();
    }
}

void MenuManager::HandleInput() {
    if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_U)) && select != 0 && !settingDialog && !exitDialog) {
        select--;
        audioManager.PlaySoundEffect("click");

    }
    if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_D)) && select != 3 && !settingDialog && !exitDialog) {
        select++;
        audioManager.PlaySoundEffect("click");

    }
    if (IsKeyPressed(KEY_ENTER) && select == 1) {
        settingDialog = true;
    }
    if (IsKeyPressed(KEY_ENTER) && select == 2) {
        exitDialog = true;
    }
    if (IsKeyPressed(KEY_ENTER) && select == 3) {
        std::cout << "Editor mode\n";
    }
    if (settingDialog && !exitDialog) {
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && returnButton.checkCollision(mousePos)) {
            settingDialog = false;
        }

    }
    if (exitDialog && !settingDialog) {
        bool b = HandleExit();
    }
}
void MenuManager::DrawExit() {
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 1.0f - 0.5f));
    DrawTextureEx(board, boardPosition, 0, scale, WHITE);
    Vector2 textSize = MeasureTextEx(menuFont, "Are you sure want to exit?", 40, 1);
    Vector2 textPosition = { boardPosition.x + (board.width * scale - textSize.x) / 2, boardPosition.y + 100 };
    DrawTextEx(menuFont, "Are you sure want to exit?", textPosition, 40, 1, BROWN);

    Vector2 mousePos = GetMousePosition();
    cross.draw(crossPosition);
    if (cross.checkCollision(mousePos)) cross_selected.draw(crossPosition);

    check.draw(checkPosition);
    if (check.checkCollision(mousePos)) check_selected.draw(checkPosition);
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
    if (setting.checkCollision(mousePos)) setting_selected.draw(settingPosition);

    if (settingDialog) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 1.0f - 0.5f));
        DrawTextureEx(board, boardPosition, 0, scale, WHITE);
        Vector2 textSize = MeasureTextEx(menuFont, "Setting", 40, 1);
        Vector2 textPosition = { boardPosition.x + (board.width * scale - textSize.x) / 2, boardPosition.y + 60 };
        DrawTextEx(menuFont, "Setting", textPosition, 40, 1, BROWN);

        returnButton.draw(returnButtonPosition);
        if (returnButton.checkCollision(mousePos)) returnButton_selected.draw(returnButtonPosition);

        DrawTextEx(menuFont, "Music: ", { slideBarMusic.getBorder().x - 100, slideBarMusic.getBorder().y + slideBarMusic.getBorder().height / 2 - 10 }, 20, 4, BORDER_BROWN);
        slideBarMusic.DrawSlideBar(slideBarMusicPosition);

        DrawTextEx(menuFont, "Sound: ", { slideBarSound.getBorder().x - 100, slideBarSound.getBorder().y + slideBarSound.getBorder().height / 2 - 10 }, 20, 4, BORDER_BROWN);
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
