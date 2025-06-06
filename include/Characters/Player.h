#pragma once
#include "raylib.h"

class Player {
private:
    Rectangle rect;
    Vector2 velocity;
    bool onGround;

public:
    Player(Vector2 pos, Vector2 size);
    void Update(float deltaTime);
    void Draw() const;
    Rectangle GetRect() const;
};
