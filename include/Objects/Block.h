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

class Block_1_1_2Block : public Block {
public:
    Block_1_1_2Block(Vector2 gridPos);
};
class Block_1_1_3Block : public Block {
public:
    Block_1_1_3Block(Vector2 gridPos);
};

class Block_1_1_12Block : public Block {
public:
    Block_1_1_12Block(Vector2 gridPos);
};
class Block_1_1_13Block : public Block {
public:
    Block_1_1_13Block(Vector2 gridPos);
};
class Block_1_1_14Block : public Block {
public:
    Block_1_1_14Block(Vector2 gridPos);
};
class Block_1_1_15Block : public Block {
public:
    Block_1_1_15Block(Vector2 gridPos);
};
class Block_1_1_16Block : public Block {
public:
    Block_1_1_16Block(Vector2 gridPos);
};

class Block_1_2_7Block : public Block {
public:
    Block_1_2_7Block(Vector2 gridPos);
};
class Block_1_2_8Block : public Block {
public:
    Block_1_2_8Block(Vector2 gridPos);
};

class Block_1_2_12Block : public Block {
public:
    Block_1_2_12Block(Vector2 gridPos);
};
class Block_1_2_13Block : public Block {
public:
    Block_1_2_13Block(Vector2 gridPos);
};
class Block_1_2_14Block : public Block {
public:
    Block_1_2_14Block(Vector2 gridPos);
};
class Block_1_2_15Block : public Block {
public:
    Block_1_2_15Block(Vector2 gridPos);
};
class Block_1_2_16Block : public Block {
public:
    Block_1_2_16Block(Vector2 gridPos);
};

class Block_1_3_4Block : public Block {
public:
    Block_1_3_4Block(Vector2 gridPos);
};
class Block_1_3_5Block : public Block {
public:
    Block_1_3_5Block(Vector2 gridPos);
};
class Block_1_3_6Block : public Block {
public:
    Block_1_3_6Block(Vector2 gridPos);
};
class Block_1_3_7Block : public Block {
public:
    Block_1_3_7Block(Vector2 gridPos);
};
class Block_1_3_8Block : public Block {
public:
    Block_1_3_8Block(Vector2 gridPos);
};

class Block_1_3_15Block : public Block {
public:
    Block_1_3_15Block(Vector2 gridPos);
};
class Block_1_3_16Block : public Block {
public:
    Block_1_3_16Block(Vector2 gridPos);
};

class Block_1_4_4Block : public Block {
public:
    Block_1_4_4Block(Vector2 gridPos);
};
class Block_1_4_5Block : public Block {
public:
    Block_1_4_5Block(Vector2 gridPos);
};
class Block_1_4_6Block : public Block {
public:
    Block_1_4_6Block(Vector2 gridPos);
};
class Block_1_4_7Block : public Block {
public:
    Block_1_4_7Block(Vector2 gridPos);
};

class Block_1_4_11Block : public Block {
public:
    Block_1_4_11Block(Vector2 gridPos);
};
class Block_1_4_12Block : public Block {
public:
    Block_1_4_12Block(Vector2 gridPos);
};
class Block_1_4_13Block : public Block {
public:
    Block_1_4_13Block(Vector2 gridPos);
};
class Block_1_4_14Block : public Block {
public:
    Block_1_4_14Block(Vector2 gridPos);
};
class Block_1_4_15Block : public Block {
public:
    Block_1_4_15Block(Vector2 gridPos);
};
class Block_1_4_16Block : public Block {
public:
    Block_1_4_16Block(Vector2 gridPos);
};

class Block_1_5_4Block : public Block {
public:
    Block_1_5_4Block(Vector2 gridPos);
};
class Block_1_5_5Block : public Block {
public:
    Block_1_5_5Block(Vector2 gridPos);
};
class Block_1_5_6Block : public Block {
public:
    Block_1_5_6Block(Vector2 gridPos);
};

class Block_1_5_8Block : public Block {
public:
    Block_1_5_8Block(Vector2 gridPos);
};
class Block_1_5_9Block : public Block {
public:
    Block_1_5_9Block(Vector2 gridPos);
};
class Block_1_5_10Block : public Block {
public:
    Block_1_5_10Block(Vector2 gridPos);
};
class Block_1_5_11Block : public Block {
public:
    Block_1_5_11Block(Vector2 gridPos);
};
class Block_1_5_12Block : public Block {
public:
    Block_1_5_12Block(Vector2 gridPos);
};
class Block_1_5_13Block : public Block {
public:
    Block_1_5_13Block(Vector2 gridPos);
};
class Block_1_5_14Block : public Block {
public:
    Block_1_5_14Block(Vector2 gridPos);
};
class Block_1_5_15Block : public Block {
public:
    Block_1_5_15Block(Vector2 gridPos);
};
class Block_1_5_16Block : public Block {
public:
    Block_1_5_16Block(Vector2 gridPos);
};

class Block_1_6_5Block : public Block {
public:
    Block_1_6_5Block(Vector2 gridPos);
};
class Block_1_6_6Block : public Block {
public:
    Block_1_6_6Block(Vector2 gridPos);
};

class Block_1_7_1Block : public Block {
public:
    Block_1_7_1Block(Vector2 gridPos);
};
class Block_1_7_2Block : public Block {
public:
    Block_1_7_2Block(Vector2 gridPos);
};
class Block_1_7_3Block : public Block {
public:
    Block_1_7_3Block(Vector2 gridPos);
};
class Block_1_7_4Block : public Block {
public:
    Block_1_7_4Block(Vector2 gridPos);
};
class Block_1_7_5Block : public Block {
public:
    Block_1_7_5Block(Vector2 gridPos);
};
class Block_1_7_6Block : public Block {
public:
    Block_1_7_6Block(Vector2 gridPos);
};
class Block_1_7_7Block : public Block {
public:
    Block_1_7_7Block(Vector2 gridPos);
};

class Block_1_7_11Block : public Block {
public:
    Block_1_7_11Block(Vector2 gridPos);
};
class Block_1_7_12Block : public Block {
public:
    Block_1_7_12Block(Vector2 gridPos);
};
class Block_1_7_13Block : public Block {
public:
    Block_1_7_13Block(Vector2 gridPos);
};
class Block_1_7_14Block : public Block {
public:
    Block_1_7_14Block(Vector2 gridPos);
};
class Block_1_7_15Block : public Block {
public:
    Block_1_7_15Block(Vector2 gridPos);
};
class Block_1_7_16Block : public Block {
public:
    Block_1_7_16Block(Vector2 gridPos);
};

class Block_2_1_2Block : public Block {
public:
    Block_2_1_2Block(Vector2 gridPos);
};
class Block_2_1_3Block : public Block {
public:
    Block_2_1_3Block(Vector2 gridPos);
};

class Block_2_1_12Block : public Block {
public:
    Block_2_1_12Block(Vector2 gridPos);
};
class Block_2_1_13Block : public Block {
public:
    Block_2_1_13Block(Vector2 gridPos);
};
class Block_2_1_14Block : public Block {
public:
    Block_2_1_14Block(Vector2 gridPos);
};
class Block_2_1_15Block : public Block {
public:
    Block_2_1_15Block(Vector2 gridPos);
};
class Block_2_1_16Block : public Block {
public:
    Block_2_1_16Block(Vector2 gridPos);
};

class Block_2_2_7Block : public Block {
public:
    Block_2_2_7Block(Vector2 gridPos);
};
class Block_2_2_8Block : public Block {
public:
    Block_2_2_8Block(Vector2 gridPos);
};

class Block_2_2_12Block : public Block {
public:
    Block_2_2_12Block(Vector2 gridPos);
};
class Block_2_2_13Block : public Block {
public:
    Block_2_2_13Block(Vector2 gridPos);
};
class Block_2_2_14Block : public Block {
public:
    Block_2_2_14Block(Vector2 gridPos);
};
class Block_2_2_15Block : public Block {
public:
    Block_2_2_15Block(Vector2 gridPos);
};
class Block_2_2_16Block : public Block {
public:
    Block_2_2_16Block(Vector2 gridPos);
};

class Block_2_3_4Block : public Block {
public:
    Block_2_3_4Block(Vector2 gridPos);
};
class Block_2_3_5Block : public Block {
public:
    Block_2_3_5Block(Vector2 gridPos);
};
class Block_2_3_6Block : public Block {
public:
    Block_2_3_6Block(Vector2 gridPos);
};
class Block_2_3_7Block : public Block {
public:
    Block_2_3_7Block(Vector2 gridPos);
};
class Block_2_3_8Block : public Block {
public:
    Block_2_3_8Block(Vector2 gridPos);
};

class Block_2_3_15Block : public Block {
public:
    Block_2_3_15Block(Vector2 gridPos);
};
class Block_2_3_16Block : public Block {
public:
    Block_2_3_16Block(Vector2 gridPos);
};

class Block_2_4_4Block : public Block {
public:
    Block_2_4_4Block(Vector2 gridPos);
};
class Block_2_4_5Block : public Block {
public:
    Block_2_4_5Block(Vector2 gridPos);
};
class Block_2_4_6Block : public Block {
public:
    Block_2_4_6Block(Vector2 gridPos);
};
class Block_2_4_7Block : public Block {
public:
    Block_2_4_7Block(Vector2 gridPos);
};

class Block_2_4_11Block : public Block {
public:
    Block_2_4_11Block(Vector2 gridPos);
};
class Block_2_4_12Block : public Block {
public:
    Block_2_4_12Block(Vector2 gridPos);
};
class Block_2_4_13Block : public Block {
public:
    Block_2_4_13Block(Vector2 gridPos);
};
class Block_2_4_14Block : public Block {
public:
    Block_2_4_14Block(Vector2 gridPos);
};
class Block_2_4_15Block : public Block {
public:
    Block_2_4_15Block(Vector2 gridPos);
};
class Block_2_4_16Block : public Block {
public:
    Block_2_4_16Block(Vector2 gridPos);
};

class Block_2_5_4Block : public Block {
public:
    Block_2_5_4Block(Vector2 gridPos);
};
class Block_2_5_5Block : public Block {
public:
    Block_2_5_5Block(Vector2 gridPos);
};
class Block_2_5_6Block : public Block {
public:
    Block_2_5_6Block(Vector2 gridPos);
};

class Block_2_5_8Block : public Block {
public:
    Block_2_5_8Block(Vector2 gridPos);
};
class Block_2_5_9Block : public Block {
public:
    Block_2_5_9Block(Vector2 gridPos);
};
class Block_2_5_10Block : public Block {
public:
    Block_2_5_10Block(Vector2 gridPos);
};
class Block_2_5_11Block : public Block {
public:
    Block_2_5_11Block(Vector2 gridPos);
};
class Block_2_5_12Block : public Block {
public:
    Block_2_5_12Block(Vector2 gridPos);
};
class Block_2_5_13Block : public Block {
public:
    Block_2_5_13Block(Vector2 gridPos);
};
class Block_2_5_14Block : public Block {
public:
    Block_2_5_14Block(Vector2 gridPos);
};
class Block_2_5_15Block : public Block {
public:
    Block_2_5_15Block(Vector2 gridPos);
};
class Block_2_5_16Block : public Block {
public:
    Block_2_5_16Block(Vector2 gridPos);
};

class Block_2_6_5Block : public Block {
public:
    Block_2_6_5Block(Vector2 gridPos);
};
class Block_2_6_6Block : public Block {
public:
    Block_2_6_6Block(Vector2 gridPos);
};

class Block_2_7_1Block : public Block {
public:
    Block_2_7_1Block(Vector2 gridPos);
};
class Block_2_7_2Block : public Block {
public:
    Block_2_7_2Block(Vector2 gridPos);
};
class Block_2_7_3Block : public Block {
public:
    Block_2_7_3Block(Vector2 gridPos);
};
class Block_2_7_4Block : public Block {
public:
    Block_2_7_4Block(Vector2 gridPos);
};
class Block_2_7_5Block : public Block {
public:
    Block_2_7_5Block(Vector2 gridPos);
};
class Block_2_7_6Block : public Block {
public:
    Block_2_7_6Block(Vector2 gridPos);
};
class Block_2_7_7Block : public Block {
public:
    Block_2_7_7Block(Vector2 gridPos);
};

class Block_2_7_11Block : public Block {
public:
    Block_2_7_11Block(Vector2 gridPos);
};
class Block_2_7_12Block : public Block {
public:
    Block_2_7_12Block(Vector2 gridPos);
};
class Block_2_7_13Block : public Block {
public:
    Block_2_7_13Block(Vector2 gridPos);
};
class Block_2_7_14Block : public Block {
public:
    Block_2_7_14Block(Vector2 gridPos);
};
class Block_2_7_15Block : public Block {
public:
    Block_2_7_15Block(Vector2 gridPos);
};
class Block_2_7_16Block : public Block {
public:
    Block_2_7_16Block(Vector2 gridPos);
};
class Block_3_1_2Block : public Block {
public:
    Block_3_1_2Block(Vector2 gridPos);
};
class Block_3_1_3Block : public Block {
public:
    Block_3_1_3Block(Vector2 gridPos);
};

class Block_3_1_12Block : public Block {
public:
    Block_3_1_12Block(Vector2 gridPos);
};
class Block_3_1_13Block : public Block {
public:
    Block_3_1_13Block(Vector2 gridPos);
};
class Block_3_1_14Block : public Block {
public:
    Block_3_1_14Block(Vector2 gridPos);
};
class Block_3_1_15Block : public Block {
public:
    Block_3_1_15Block(Vector2 gridPos);
};
class Block_3_1_16Block : public Block {
public:
    Block_3_1_16Block(Vector2 gridPos);
};

class Block_3_2_7Block : public Block {
public:
    Block_3_2_7Block(Vector2 gridPos);
};
class Block_3_2_8Block : public Block {
public:
    Block_3_2_8Block(Vector2 gridPos);
};

class Block_3_2_12Block : public Block {
public:
    Block_3_2_12Block(Vector2 gridPos);
};
class Block_3_2_13Block : public Block {
public:
    Block_3_2_13Block(Vector2 gridPos);
};
class Block_3_2_14Block : public Block {
public:
    Block_3_2_14Block(Vector2 gridPos);
};
class Block_3_2_15Block : public Block {
public:
    Block_3_2_15Block(Vector2 gridPos);
};
class Block_3_2_16Block : public Block {
public:
    Block_3_2_16Block(Vector2 gridPos);
};

class Block_3_3_4Block : public Block {
public:
    Block_3_3_4Block(Vector2 gridPos);
};
class Block_3_3_5Block : public Block {
public:
    Block_3_3_5Block(Vector2 gridPos);
};
class Block_3_3_6Block : public Block {
public:
    Block_3_3_6Block(Vector2 gridPos);
};
class Block_3_3_7Block : public Block {
public:
    Block_3_3_7Block(Vector2 gridPos);
};
class Block_3_3_8Block : public Block {
public:
    Block_3_3_8Block(Vector2 gridPos);
};

class Block_3_3_15Block : public Block {
public:
    Block_3_3_15Block(Vector2 gridPos);
};
class Block_3_3_16Block : public Block {
public:
    Block_3_3_16Block(Vector2 gridPos);
};

class Block_3_4_4Block : public Block {
public:
    Block_3_4_4Block(Vector2 gridPos);
};
class Block_3_4_5Block : public Block {
public:
    Block_3_4_5Block(Vector2 gridPos);
};
class Block_3_4_6Block : public Block {
public:
    Block_3_4_6Block(Vector2 gridPos);
};
class Block_3_4_7Block : public Block {
public:
    Block_3_4_7Block(Vector2 gridPos);
};

class Block_3_4_11Block : public Block {
public:
    Block_3_4_11Block(Vector2 gridPos);
};
class Block_3_4_12Block : public Block {
public:
    Block_3_4_12Block(Vector2 gridPos);
};
class Block_3_4_13Block : public Block {
public:
    Block_3_4_13Block(Vector2 gridPos);
};
class Block_3_4_14Block : public Block {
public:
    Block_3_4_14Block(Vector2 gridPos);
};
class Block_3_4_15Block : public Block {
public:
    Block_3_4_15Block(Vector2 gridPos);
};
class Block_3_4_16Block : public Block {
public:
    Block_3_4_16Block(Vector2 gridPos);
};

class Block_3_5_4Block : public Block {
public:
    Block_3_5_4Block(Vector2 gridPos);
};
class Block_3_5_5Block : public Block {
public:
    Block_3_5_5Block(Vector2 gridPos);
};
class Block_3_5_6Block : public Block {
public:
    Block_3_5_6Block(Vector2 gridPos);
};

class Block_3_5_8Block : public Block {
public:
    Block_3_5_8Block(Vector2 gridPos);
};
class Block_3_5_9Block : public Block {
public:
    Block_3_5_9Block(Vector2 gridPos);
};
class Block_3_5_10Block : public Block {
public:
    Block_3_5_10Block(Vector2 gridPos);
};
class Block_3_5_11Block : public Block {
public:
    Block_3_5_11Block(Vector2 gridPos);
};
class Block_3_5_12Block : public Block {
public:
    Block_3_5_12Block(Vector2 gridPos);
};
class Block_3_5_13Block : public Block {
public:
    Block_3_5_13Block(Vector2 gridPos);
};
class Block_3_5_14Block : public Block {
public:
    Block_3_5_14Block(Vector2 gridPos);
};
class Block_3_5_15Block : public Block {
public:
    Block_3_5_15Block(Vector2 gridPos);
};
class Block_3_5_16Block : public Block {
public:
    Block_3_5_16Block(Vector2 gridPos);
};

class Block_3_6_5Block : public Block {
public:
    Block_3_6_5Block(Vector2 gridPos);
};
class Block_3_6_6Block : public Block {
public:
    Block_3_6_6Block(Vector2 gridPos);
};

class Block_3_7_1Block : public Block {
public:
    Block_3_7_1Block(Vector2 gridPos);
};
class Block_3_7_2Block : public Block {
public:
    Block_3_7_2Block(Vector2 gridPos);
};
class Block_3_7_3Block : public Block {
public:
    Block_3_7_3Block(Vector2 gridPos);
};
class Block_3_7_4Block : public Block {
public:
    Block_3_7_4Block(Vector2 gridPos);
};
class Block_3_7_5Block : public Block {
public:
    Block_3_7_5Block(Vector2 gridPos);
};
class Block_3_7_6Block : public Block {
public:
    Block_3_7_6Block(Vector2 gridPos);
};
class Block_3_7_7Block : public Block {
public:
    Block_3_7_7Block(Vector2 gridPos);
};

class Block_3_7_11Block : public Block {
public:
    Block_3_7_11Block(Vector2 gridPos);
};
class Block_3_7_12Block : public Block {
public:
    Block_3_7_12Block(Vector2 gridPos);
};
class Block_3_7_13Block : public Block {
public:
    Block_3_7_13Block(Vector2 gridPos);
};
class Block_3_7_14Block : public Block {
public:
    Block_3_7_14Block(Vector2 gridPos);
};
class Block_3_7_15Block : public Block {
public:
    Block_3_7_15Block(Vector2 gridPos);
};
class Block_3_7_16Block : public Block {
public:
    Block_3_7_16Block(Vector2 gridPos);
};
class Block_4_1_2Block : public Block {
public:
    Block_4_1_2Block(Vector2 gridPos);
};
class Block_4_1_3Block : public Block {
public:
    Block_4_1_3Block(Vector2 gridPos);
};

class Block_4_1_12Block : public Block {
public:
    Block_4_1_12Block(Vector2 gridPos);
};
class Block_4_1_13Block : public Block {
public:
    Block_4_1_13Block(Vector2 gridPos);
};
class Block_4_1_14Block : public Block {
public:
    Block_4_1_14Block(Vector2 gridPos);
};
class Block_4_1_15Block : public Block {
public:
    Block_4_1_15Block(Vector2 gridPos);
};
class Block_4_1_16Block : public Block {
public:
    Block_4_1_16Block(Vector2 gridPos);
};

class Block_4_2_7Block : public Block {
public:
    Block_4_2_7Block(Vector2 gridPos);
};
class Block_4_2_8Block : public Block {
public:
    Block_4_2_8Block(Vector2 gridPos);
};

class Block_4_2_12Block : public Block {
public:
    Block_4_2_12Block(Vector2 gridPos);
};
class Block_4_2_13Block : public Block {
public:
    Block_4_2_13Block(Vector2 gridPos);
};
class Block_4_2_14Block : public Block {
public:
    Block_4_2_14Block(Vector2 gridPos);
};
class Block_4_2_15Block : public Block {
public:
    Block_4_2_15Block(Vector2 gridPos);
};
class Block_4_2_16Block : public Block {
public:
    Block_4_2_16Block(Vector2 gridPos);
};

class Block_4_3_4Block : public Block {
public:
    Block_4_3_4Block(Vector2 gridPos);
};
class Block_4_3_5Block : public Block {
public:
    Block_4_3_5Block(Vector2 gridPos);
};
class Block_4_3_6Block : public Block {
public:
    Block_4_3_6Block(Vector2 gridPos);
};
class Block_4_3_7Block : public Block {
public:
    Block_4_3_7Block(Vector2 gridPos);
};
class Block_4_3_8Block : public Block {
public:
    Block_4_3_8Block(Vector2 gridPos);
};

class Block_4_3_15Block : public Block {
public:
    Block_4_3_15Block(Vector2 gridPos);
};
class Block_4_3_16Block : public Block {
public:
    Block_4_3_16Block(Vector2 gridPos);
};

class Block_4_4_4Block : public Block {
public:
    Block_4_4_4Block(Vector2 gridPos);
};
class Block_4_4_5Block : public Block {
public:
    Block_4_4_5Block(Vector2 gridPos);
};
class Block_4_4_6Block : public Block {
public:
    Block_4_4_6Block(Vector2 gridPos);
};
class Block_4_4_7Block : public Block {
public:
    Block_4_4_7Block(Vector2 gridPos);
};

class Block_4_4_11Block : public Block {
public:
    Block_4_4_11Block(Vector2 gridPos);
};
class Block_4_4_12Block : public Block {
public:
    Block_4_4_12Block(Vector2 gridPos);
};
class Block_4_4_13Block : public Block {
public:
    Block_4_4_13Block(Vector2 gridPos);
};
class Block_4_4_14Block : public Block {
public:
    Block_4_4_14Block(Vector2 gridPos);
};
class Block_4_4_15Block : public Block {
public:
    Block_4_4_15Block(Vector2 gridPos);
};
class Block_4_4_16Block : public Block {
public:
    Block_4_4_16Block(Vector2 gridPos);
};

class Block_4_5_4Block : public Block {
public:
    Block_4_5_4Block(Vector2 gridPos);
};
class Block_4_5_5Block : public Block {
public:
    Block_4_5_5Block(Vector2 gridPos);
};
class Block_4_5_6Block : public Block {
public:
    Block_4_5_6Block(Vector2 gridPos);
};

class Block_4_5_8Block : public Block {
public:
    Block_4_5_8Block(Vector2 gridPos);
};
class Block_4_5_9Block : public Block {
public:
    Block_4_5_9Block(Vector2 gridPos);
};
class Block_4_5_10Block : public Block {
public:
    Block_4_5_10Block(Vector2 gridPos);
};
class Block_4_5_11Block : public Block {
public:
    Block_4_5_11Block(Vector2 gridPos);
};
class Block_4_5_12Block : public Block {
public:
    Block_4_5_12Block(Vector2 gridPos);
};
class Block_4_5_13Block : public Block {
public:
    Block_4_5_13Block(Vector2 gridPos);
};
class Block_4_5_14Block : public Block {
public:
    Block_4_5_14Block(Vector2 gridPos);
};
class Block_4_5_15Block : public Block {
public:
    Block_4_5_15Block(Vector2 gridPos);
};
class Block_4_5_16Block : public Block {
public:
    Block_4_5_16Block(Vector2 gridPos);
};

class Block_4_6_5Block : public Block {
public:
    Block_4_6_5Block(Vector2 gridPos);
};
class Block_4_6_6Block : public Block {
public:
    Block_4_6_6Block(Vector2 gridPos);
};

class Block_4_7_1Block : public Block {
public:
    Block_4_7_1Block(Vector2 gridPos);
};
class Block_4_7_2Block : public Block {
public:
    Block_4_7_2Block(Vector2 gridPos);
};
class Block_4_7_3Block : public Block {
public:
    Block_4_7_3Block(Vector2 gridPos);
};
class Block_4_7_4Block : public Block {
public:
    Block_4_7_4Block(Vector2 gridPos);
};
class Block_4_7_5Block : public Block {
public:
    Block_4_7_5Block(Vector2 gridPos);
};
class Block_4_7_6Block : public Block {
public:
    Block_4_7_6Block(Vector2 gridPos);
};
class Block_4_7_7Block : public Block {
public:
    Block_4_7_7Block(Vector2 gridPos);
};

class Block_4_7_11Block : public Block {
public:
    Block_4_7_11Block(Vector2 gridPos);
};
class Block_4_7_12Block : public Block {
public:
    Block_4_7_12Block(Vector2 gridPos);
};
class Block_4_7_13Block : public Block {
public:
    Block_4_7_13Block(Vector2 gridPos);
};
class Block_4_7_14Block : public Block {
public:
    Block_4_7_14Block(Vector2 gridPos);
};
class Block_4_7_15Block : public Block {
public:
    Block_4_7_15Block(Vector2 gridPos);
};
class Block_4_7_16Block : public Block {
public:
    Block_4_7_16Block(Vector2 gridPos);
};