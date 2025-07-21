#pragma once
#include "ObjectFactory.h" 
#include "../System/Interface.h"
#include <raylib.h>
#include <vector>
#include <memory>

class TriggerZone;
class Character;

class CollectableObject : public Object, public IUpdatable {
public:
    CollectableObject(Vector2 pos, Vector2 sz, Texture2D texture);
    virtual ~CollectableObject();

    virtual void onCollect(Character* 
    ) = 0;

    virtual void update(float deltaTime) override;
    virtual void draw() override;

    Rectangle getHitBox() const override;
    virtual ObjectCategory getObjectCategory() const override;
    virtual std::vector<ObjectCategory> getCollisionTargets() const override;
    virtual void checkCollision(const std::vector<std::shared_ptr<Object>>&) override;
    void applyGravity(float deltaTime);

    void setActive(bool val) override;
    bool isActive() const override;

    void setCollided(bool val) override;
    bool isCollided() const override;

    Vector2 getPosition() const override;
    void setPosition(Vector2 newPos) override;  

    ObjectType getObjectType() const;
protected:
    Texture2D texture;   
    ItemType type = ItemType::COIN;
	Rectangle hitBox;
    std::shared_ptr<TriggerZone> triggerZone = nullptr;
};
