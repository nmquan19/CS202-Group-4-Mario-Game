#pragma once
#include "../System/Interface.h"
#include "raylib.h"
#include "../Objects/ObjectFactory.h"
#include <climits>
#include <variant>
#include "../System/InterpolationController.h"

class Item : public Object, public IUpdatable {
public:
    Item(Vector2 startPos, Texture2D texture, Vector2 size);
    virtual ~Item();

    std::vector<Rectangle> getHitBox() const override;
    ObjectCategory getObjectCategory() const override;
    void onCollision(std::shared_ptr<Object> other) override;
    std::vector<ObjectCategory> getCollisionTargets() const override;
    void checkCollision(const std::vector<std::shared_ptr<Object>>& candidates) override;

    Vector2 getPosition() const override;
    void setPosition(Vector2 newPosition);
    bool isActive() const override;
    void setActive(bool flag) override;

	virtual void update(float deltaTime) override;
	void draw() override;

    bool isCollided() const override;
    void setCollided(bool) override;
    Vector2 getSize() const override;
    virtual ObjectType getObjectType() const override = 0 ;

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

class CoinItem : public Item {
public:
    CoinItem(Vector2 startPos, Texture2D texture, Vector2 size);

    ObjectType getObjectType() const override;
};

class MushroomItem : public Item {
public:
    MushroomItem(Vector2 startPos, Texture2D texture, Vector2 size);

    ObjectType getObjectType() const override;
};



