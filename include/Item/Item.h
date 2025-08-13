#pragma once
#include "../System/Interface.h"
#include "raylib.h"
#include "../Objects/ObjectFactory.h"
#include <climits>
#include <variant>
#include "../System/InterpolationController.h"
#include "Anima.h"

class Item : public Object, public IUpdatable {
public:
    Item();
    Item(Vector2 startPos);
    virtual ~Item();

    std::vector<Rectangle> getHitBox() const override;
    ObjectCategory getObjectCategory() const override;
    void onCollision(std::shared_ptr<Object> other, Direction direction) override;
    std::vector<ObjectCategory> getCollisionTargets() const override;

    Vector2 getPosition() const override;
    void setPosition(Vector2 newPosition);
    bool isActive() const override;
    void setActive(bool flag) override;

    virtual void update(float deltaTime) override;
    void draw() override;
    void draw(Texture2D texture);

    bool isCollided() const override;
    void setCollided(bool) override;
    Vector2 getSize() const override;
    virtual ObjectType getObjectType() const override;
    void HarmonicOscillationMove(float amplitude, float frequency, float deltaTime);
    void StarShapeMove(Vector2 center, float deltaTime, float frequency); 
    virtual void Move(double dt);

    void CircleMove(Vector2 center, float radius, float speed, float deltaTime);
protected:
    Vector2 position;
    Vector2 velocity;
    Vector2 accelleration;

    bool active;
    //Texture2D texture;
    Rectangle spritebox;
    Rectangle hitbox;
    float scale;
    float aniTimer, aniSpeed;
    int curFrame;

    float gravity = 600.0f;
    const float groundY = 400.0f;
    Anima* animation;
    ItemType type;
    float totalTime;
};





