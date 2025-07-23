#pragma once

#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>
#include "UI.h"
#include "SoundEffect.h"
#include "Button.h"
#include "SlideBar.h"

class MenuManager : public UIManager {
private:
    Texture2D logo, board;
    Button check, cross, setting, returnButton, home, play;
    Button check_selected, cross_selected, setting_selected, returnButton_selected;
    Vector2 boardPosition, crossPosition, checkPosition, settingPosition, returnButtonPosition;

    float scale = 0.2f;
    bool isActive;
    AudioManager audioManager;
public:
    SlideBar slideBarMusic, slideBarSound;
    Vector2 slideBarMusicPosition, slideBarSoundPosition;
    int select;
    bool settingDialog, exitDialog, exit;
    MenuManager();
    ~MenuManager();

    void HandleInput();
    void DrawMenu();

    bool HandleExit();
    void DrawExit();

    void DrawSetting();
    void HandleSetting();
};
