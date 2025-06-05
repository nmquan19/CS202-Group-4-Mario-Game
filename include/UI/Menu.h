#pragma once

#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>
#include "UI.h"
#include "SoundEffect.h"

class MenuManager : public UIManager {
private:
    Texture2D logo;
    //Texture2D startButton;
    bool isActive;
    AudioManager audioManager;

public:
    bool select;
    MenuManager();
    ~MenuManager();

    void HandleInput();
    void DrawMenu();
};