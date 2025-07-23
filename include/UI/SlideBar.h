#pragma once

#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>
#include "UI.h"
#include "SoundEffect.h"

#define LIGHT_YELLOW CLITERAL(Color){254, 242, 0, 255}

#define CYBER_YELLOW CLITERAL(Color){255, 194, 14, 255}

#define BORDER_BROWN CLITERAL(Color){189, 163, 62, 255}

class SlideBar : public UIManager {
private:
    float percent;
    float thickness;
    float scale;
    Vector2 position;
    Rectangle border;
    bool dragging;

public:
    SlideBar();
    ~SlideBar();

    float getValue();
    Rectangle getBorder();

    void HandleSlideBar();
    void DrawSlideBar(Vector2 pos);
};
