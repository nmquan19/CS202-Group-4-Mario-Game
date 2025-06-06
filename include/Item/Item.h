#pragma once
#include "raylib.h"

class Item {
protected:
    Vector2 position;
    Vector2 velocity;
    Texture2D texture;
    float scale;

public:
    Item(Vector2 pos, const char* imagePath, float scale = 0.1f);
    virtual ~Item();
    virtual void Update(float deltaTime) = 0;
    virtual void Draw() const;
    virtual Rectangle GetRect() const;
};



//Coin
class Coin : public Item {
private:
    bool isCollected;

public:
    Coin(Vector2 pos);
    void Update(float deltaTime) override;
    void Draw() const override;
    Rectangle GetRect() const override;
    bool Collected() const;
    void Collect();
};




//Mushroom
class Mushroom : public Item {
public:
    Mushroom(Vector2 pos);
    void Update(float deltaTime) override;
    Rectangle GetRect() const override;
};
