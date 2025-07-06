#pragma once
#include "ObjectFactory.h"
#include "../System/Interface.h"

class Projectile : public Object, public IUpdatable, public IMovable {
public:
    Projectile(Vector2 position, int direction, float scale = 1.0f);
    virtual ~Projectile();

    ObjectCategory getObjectCategory() const override;

    Rectangle getHitBox() const override;
    std::vector<ObjectCategory> getCollisionTargets() const override;
    void checkCollision(const std::vector<Object*>& candidates) override;
    void onCollision(Object* other) override;

    void update(float deltaTime) override;
    void draw() override;

    void setVelocity(Vector2 newVelocity) override;
    Vector2 getVelocity() override;
    void setPosition(Vector2 newPos) override;
    Vector2 getPosition() const override;
    void setActive(bool flag) override;
    bool isActive() const override;
    void setCollided(bool flag) override;
    bool isCollided() const override;

protected:
    virtual void handleEnvironmentCollision(Object* other);
    virtual void handleCharacterCollision(Object* other) {}
    virtual void handleEnemyCollision(Object* other);

private:
    float scale;
    Vector2 position;
    Vector2 velocity;
    float speed;

    float aniTimer;
    float aniSpeed;

    Texture2D spriteSheet;
    Rectangle spriteRec;
    int currentFrame;
    std::vector<Rectangle> frameData;
};