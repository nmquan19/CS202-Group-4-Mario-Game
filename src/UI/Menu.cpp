#include "../../include/UI/Menu.h"

MenuManager::MenuManager() {
    isActive = true;
    
    logo = LoadTexture("./assets/Super Mario Bros.png");
    //startButton = LoadTexture("resources/textures/start_button.png");
    select = true;
    audioManager.LoadSoundEffect("click", "./assets/sound/click.wav");
}

MenuManager::~MenuManager() {
    UnloadFont(menuFont);
    UnloadTexture(logo);
    //UnloadTexture(startButton);
}

void MenuManager::DrawMenu() {
    ClearBackground(SKYBLUE);
    float width = static_cast<float>(screenWidth);
    float height = static_cast<float>(screenHeight);
    Rectangle recLogo; recLogo.x = 0; recLogo.y = 0; recLogo.width = 886; recLogo.height = 352;
    float scale = 0.75;
    DrawTextureEx(logo, { (width - recLogo.width * scale) / 2, height / 5 }, 0, scale, WHITE); // center
    if (select) {
        DrawTextEx(menuFont, "1 player", { width * 2 / 5, height * 2 / 3 }, 40, 8, YELLOW);
        DrawTextEx(menuFont, "2 players", { width * 2 / 5 , height * 3 / 4 }, 40, 8, WHITE);
    }
    else {
        DrawTextEx(menuFont, "1 player", { width * 2 / 5, height * 2 / 3 }, 40, 8, WHITE);
        DrawTextEx(menuFont, "2 players", { width * 2 / 5 , height * 3 / 4 }, 40, 8, YELLOW);
    }
    
}

void MenuManager::HandleInput() {
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_U) ) {
        audioManager.PlaySoundEffect("click");
        select = true;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_D)) {
        audioManager.PlaySoundEffect("click");
        select = false;
    }
}

