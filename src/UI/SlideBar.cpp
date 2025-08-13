#include "../../include/UI/SlideBar.h"

SlideBar::SlideBar() {
    scale = 0.1f;
    percent = 50.0f;
    thickness = UIManager::getInstance().screenWidth / 23.0f * scale;
    position = { 0,0 };
    border = { 0,0,0,0 };
    dragging = false;
}

SlideBar::~SlideBar() {}

float SlideBar::getValue() {
    return percent;
}

Rectangle SlideBar::getBorder() {
    return border;
}

void SlideBar::HandleSlideBar() {

    Vector2 mousePos = GetMousePosition();
    Rectangle inner = { border.x + thickness, border.y + thickness, border.width - 2 * thickness, border.height - 2 * thickness };

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, inner)) {
        dragging = true;
    }

    if (dragging) {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            dragging = false;
        }
        float localX = mousePos.x - inner.x;
        if (localX < 0) localX = 0;
        if (localX > inner.width) localX = inner.width;

        percent = localX / inner.width * 100.0f;
    }
}

void SlideBar::DrawSlideBar(Vector2 pos) {
    position = pos;
    border = { pos.x, pos.y, 640.0f / 23.0f * 130.0f * scale, 640.0f / 23.0f * 18.0f * scale };
    DrawRectangleRec(border, BORDER_BROWN);

    Vector2 innerVec = { border.x + thickness, border.y + thickness };
    Rectangle inner = { innerVec.x, innerVec.y, border.width - 2 * thickness, border.height - 2 * thickness };
    DrawRectangleRec(inner, LIGHT_YELLOW);

    Rectangle value = { innerVec.x, innerVec.y, inner.width * percent / 100.0f, inner.height };
    DrawRectangleRec(value, CYBER_YELLOW);
    DrawTextEx(UIManager::getInstance().menuFont, TextFormat("%.0f%%", percent), { border.x + border.width + 10, border.y + border.height / 2 - 10 }, 20, 4, BORDER_BROWN);

    HandleSlideBar();
}