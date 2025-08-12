#pragma once
#include "ObjectFactory.h"
#include "../System/Interface.h"

class InteractiveObject : public Object, public IUpdatable {
public:
    InteractiveObject(Vector2 position, Vector2 size);
    virtual ~InteractiveObject() = default;

    virtual void update(float deltaTime) override = 0;
    virtual void draw() override = 0;

    std::vector<Rectangle> getHitBox() const override;
    virtual ObjectCategory getObjectCategory() const override;
    virtual std::vector<ObjectCategory> getCollisionTargets() const override;
    virtual void onCollision(std::shared_ptr<Object> other, Direction direction) override = 0;

    void setActive(bool flag) override;
    bool isActive() const override;

    void setCollided(bool flag) override;
    bool isCollided() const override;

    Vector2 getPosition() const override;
    void setPosition(Vector2 newPos) override;

    virtual ObjectType getObjectType() const = 0;

protected:
    Rectangle hitBox;
};