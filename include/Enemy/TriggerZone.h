#pragma once
#pragma once
#include <vector>
#include "../Objects/ObjectFactory.h"
#include "../System/Interface.h"
#include <raylib.h>

class CollectableObject;

class TriggerZone : public Object {
public:
    TriggerZone(CollectableObject* owner, Vector2 pos, Vector2 sz);
    void update(float deltaTime);
    void draw() override;
    Rectangle getHitBox() const override;
    ObjectCategory getObjectCategory() const override;
    std::vector<ObjectCategory> getCollisionTargets() const override;
    void checkCollision(const std::vector<Object*>& candidates) override;
    void onCollision(Object* other) override;

    void setActive(bool val) override;
    bool isActive() const override;
    void setCollided(bool val) override;
    bool isCollided() const override;

    Vector2 getPosition() const override;
    void setPosition(Vector2 newPos) override;

    ObjectType getObjectType() const override;
    Vector2 getSize() const override;
private:
    CollectableObject* owner;
	Rectangle hitBox;
};
