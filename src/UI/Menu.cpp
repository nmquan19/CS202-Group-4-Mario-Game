#include "../../include/UI/Menu.h"

MenuManager::MenuManager() {
    isActive = true;

    logo = LoadTexture("./assets/Super Mario Bros.png");
    //startButton = LoadTexture("resources/textures/start_button.png");
    select = 0;
    dialog = false;
    exit = false;
    audioManager.LoadSoundEffect("click", "./assets/sound/click.wav");
    board = LoadTexture("./assets/button/board.png");
    check.load("./assets/button/check.png");
    cross.load("./assets/button/cross.png");
    setting.load("./assets/button/setting.png");
    returnButton.load("./assets/button/return.png");

    boardPosition.x = (static_cast<float>(screenWidth) - board.width * scale) / 2;
    boardPosition.y = (static_cast<float>(screenHeight) - board.height * scale) / 2;

    crossPosition.x = boardPosition.x + board.width * scale / 2 - 50 - 100;
    crossPosition.y = boardPosition.y + board.height * scale / 2;

    checkPosition.x = boardPosition.x + board.width * scale / 2 - 50 + 100;
    checkPosition.y = boardPosition.y + board.height * scale / 2;

    settingPosition.x = GetScreenWidth() * 9 / 10;
    settingPosition.y = GetScreenHeight() / 10;

    returnButtonPosition.x = boardPosition.x + board.width * scale / 2 - 50;
    returnButtonPosition.y = boardPosition.y + board.height * scale / 2;

    slideBarPosition.x = boardPosition.x + board.width * scale / 4;
    slideBarPosition.y = boardPosition.y + board.height * scale / 4;
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
    if (select == 0 && !dialog) {
        DrawTextEx(menuFont, "Play", { width * 2 / 5, height * 2 / 3 }, 50, 10, YELLOW);
        DrawTextEx(menuFont, "Setting", { width * 2 / 5 , height * 2 / 3 + 40 }, 50, 10, WHITE);
        DrawTextEx(menuFont, "Exit", { width * 2 / 5 , height * 2 / 3 + 80 }, 50, 10, WHITE);
        DrawTextEx(menuFont, "Editor Mode", { width * 2 / 5 , height * 2 / 3 + 120 }, 50, 10, WHITE);
    }
    else if (select == 1 && !dialog) {
        DrawTextEx(menuFont, "Play", { width * 2 / 5, height * 2 / 3 }, 50, 10, WHITE);
        DrawTextEx(menuFont, "Setting", { width * 2 / 5 , height * 2 / 3 + 40 }, 50, 10, YELLOW);
        DrawTextEx(menuFont, "Exit", { width * 2 / 5 , height * 2 / 3 + 80 }, 50, 10, WHITE);
        DrawTextEx(menuFont, "Editor Mode", { width * 2 / 5 , height * 2 / 3 + 120 }, 50, 10, WHITE);
    }
    else if (select == 2 && !dialog) {
        DrawTextEx(menuFont, "Play", { width * 2 / 5, height * 2 / 3 }, 50, 10, WHITE);
        DrawTextEx(menuFont, "Setting", { width * 2 / 5 , height * 2 / 3 + 40 }, 50, 10, WHITE);
        DrawTextEx(menuFont, "Exit", { width * 2 / 5 , height * 2 / 3 + 80 }, 50, 10, YELLOW);
        DrawTextEx(menuFont, "Editor Mode", { width * 2 / 5 , height * 2 / 3 + 120 }, 50, 10, WHITE);
    }
    else if (select == 3 && !dialog) {
        DrawTextEx(menuFont, "Play", { width * 2 / 5, height * 2 / 3 }, 50, 10, WHITE);
        DrawTextEx(menuFont, "Setting", { width * 2 / 5 , height * 2 / 3 + 40 }, 50, 10, WHITE);
        DrawTextEx(menuFont, "Exit", { width * 2 / 5 , height * 2 / 3 + 80 }, 50, 10, WHITE);
        DrawTextEx(menuFont, "Editor Mode", { width * 2 / 5 , height * 2 / 3 + 120 }, 50, 10, YELLOW);
    }
    if (dialog) {
        DrawExit();
    }
}

void MenuManager::HandleInput() {
    if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_U)) && select != 0 && !dialog) {
        audioManager.PlaySoundEffect("click");
        select--;
    }
    if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_D)) && select != 3 && !dialog) {
        audioManager.PlaySoundEffect("click");
        select++;
    }
    if (IsKeyPressed(KEY_ENTER) && select == 2) {
        dialog = true;
    }
    if (IsKeyPressed(KEY_ENTER) && select == 3) {
        std::cout << "Editor mode\n";
    }
    if (dialog) {
        bool b = HandleExit();
    }
}
void MenuManager::DrawExit() {
    float spaceCrossCheck = 100;

    DrawTextureEx(board, boardPosition, 0, scale, WHITE);
    Vector2 textPosition = { boardPosition.x + 30, boardPosition.y + 80 };
    DrawTextEx(menuFont, "Are you sure want to exit?", textPosition, 40, 1, BROWN);
    cross.draw(crossPosition);
    check.draw(checkPosition);
}
bool MenuManager::HandleExit() {
    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && check.checkCollision(mousePos)) {
        return true;
    }
    else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && cross.checkCollision(mousePos)) {
        dialog = false;
    }
    return false;
}
void MenuManager::DrawSetting() {
    setting.draw(settingPosition);
    if (dialog) {
        DrawTextureEx(board, boardPosition, 0, scale, WHITE);
        returnButton.draw(returnButtonPosition);
        slideBar.DrawSlideBar(slideBarPosition);
    }

}
void MenuManager::HandleSetting() {
    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && setting.checkCollision(mousePos)) {
        dialog = true;
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && returnButton.checkCollision(mousePos)) {
        dialog = false;
    }
}
