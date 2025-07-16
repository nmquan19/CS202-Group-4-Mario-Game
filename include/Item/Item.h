#pragma once
#include "../System/Interface.h"
#include "raylib.h"
#include "../Objects/ObjectFactory.h"
#include <climits>

class Item : public Object, public IUpdatable {
public:
    Item(Vector2 startPos, Texture2D texture, Vector2 size);
    ~Item();

    void update(float deltaTime) override;
    void draw() override;

    Rectangle getHitBox() const override;
    ObjectCategory getObjectCategory() const override;
    void onCollision(Object* other) override;
    std::vector<ObjectCategory> getCollisionTargets() const override;
    void checkCollision(const std::vector<Object*>& candidates) override;

    Vector2 getPosition() const override;
    void setPosition(Vector2 newPosition);
    bool isActive() const override;
    void setActive(bool flag) override;

protected:
    Vector2 position;
    Vector2 velocity;
    Vector2 accelleration;

    bool active;
    Texture2D texture;
    Rectangle spritebox;
    Rectangle hitbox;
    float scale;
    float aniTimer, aniSpeed;
    int curFrame;

    ItemType type; // Giả định bạn có enum ItemType
};
