#pragma once

#include "ObjectFactory.h"
#include "../System/Interface.h"

class Block : public Object, public IUpdatable {
public:
    Block(Vector2 gridPos, BlockType type, Vector2);
    virtual ~Block();

    void draw() override;

    std::vector<Rectangle> getHitBox() const override;
    ObjectCategory getObjectCategory() const override;
    std::vector<ObjectCategory> getCollisionTargets() const override;
    void onCollision(std::shared_ptr<Object> other, Direction direction) override;

    void update(float deltaTime);

    bool isActive() const override;
    void setActive(bool flag) override;
    bool isCollided() const override;
    void setCollided(bool flag) override;
    Vector2 getPosition() const override;
    void setPosition(Vector2 newPos) override;
    ObjectType getObjectType() const override;
    Vector2 getSize() const override;

    bool isSolid() const;
    BlockType getType() const;
protected:
    Vector2 position;
    Vector2 gridPosition;
    Rectangle hitbox;
    BlockType blockType;
    bool solid;

    bool isMoving;
    float resetTimer = 0.0f;
    const float RESET_TIME = 0.1f;
    Vector2 originalPos;

    bool pendingImpulse;
    float impulseForce;

};

class GroundBlock : public Block {
public:
    GroundBlock(Vector2 gridPos);
};

class BrickBlock : public Block {
public:
    BrickBlock(Vector2 gridPos);
};