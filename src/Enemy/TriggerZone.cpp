#include "../../include/Enemy/TriggerZone.h"
#include <raylib.h>
#include <vector>
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/System/Interface.h"
#include "../../include/Characters/Character.h"
#include "../../include/Objects/CollectableObject.h"
#include <raymath.h>
#include <iostream>
#include "../../include/System/Grid.h"
#include "../../include/System/Box2DWorldManager.h"
TriggerZone::TriggerZone(CollectableObject* ownerItem, Vector2 pos, Vector2 sz)
    : owner(ownerItem) {
    position = pos;
    size = sz*2;
    hitBox = Rectangle{ position.x, position.y, size.x * GridSystem::GRID_SIZE, size.y * GridSystem::GRID_SIZE };
    position.x = owner->getPosition().x + (owner->getHitBox()[0].width - size.x * GridSystem::GRID_SIZE) / 2;
    position.y = owner->getPosition().y + (owner->getHitBox()[0].height - size.y * GridSystem::GRID_SIZE) / 2;
    physicsBody = Box2DWorldManager::getInstance().createRectangleBody(position, { hitBox.width, hitBox.height });
    if (physicsBody) {
        physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
        for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext())
        {
            b2Filter filter = fixture->GetFilterData();
            filter.maskBits = static_cast<uint16>(ObjectCategory::TRIGGER);
            fixture->SetSensor(true);
        }
    }
    this->active = true;

} 

void TriggerZone::update(float deltaTime) {

    if (!isActive())
    {
        return;
    }
    if (owner) {
        std::vector<Rectangle> ownerHitBoxes = owner->getHitBox();
        if (!ownerHitBoxes.empty()) {
            Rectangle ownerHitBox = ownerHitBoxes[0];

            Vector2 zoneCenter;
            zoneCenter.x = owner->getPosition().x + ownerHitBox.width * 0.5f;
            zoneCenter.y = owner->getPosition().y + ownerHitBox.height * 0.5f;

            zoneCenter.x += (ownerHitBox.width - size.x * GridSystem::GRID_SIZE) * 0.5f;
            zoneCenter.y += (ownerHitBox.height - size.y * GridSystem::GRID_SIZE) * 0.5f;

            if (physicsBody) {
                b2Vec2 b2Pos = Box2DWorldManager::raylibToB2(zoneCenter);
                physicsBody->SetTransform(b2Pos, physicsBody->GetAngle());
            }

            position.x = zoneCenter.x - size.x * GridSystem::GRID_SIZE * 0.5f;
            position.y = zoneCenter.y - size.y * GridSystem::GRID_SIZE * 0.5f;

            hitBox = Rectangle{
                position.x,
                position.y,
                size.x * GridSystem::GRID_SIZE,
                size.y * GridSystem::GRID_SIZE
            };
        }
    }
}


void TriggerZone::draw() {
}

std::vector<Rectangle> TriggerZone::getHitBox() const {
    return {hitBox};
}

ObjectCategory TriggerZone::getObjectCategory() const {
    return ObjectCategory::TRIGGER;
}

std::vector<ObjectCategory> TriggerZone::getCollisionTargets() const {
    return { ObjectCategory::CHARACTER};
}

void TriggerZone::onCollision(std::shared_ptr<Object> other, Direction direction) {
    if (!owner->isCollectable()) return;
    setCollided(true);
    if (IsKeyPressed(KEY_E))
    {
        if (!owner || !other) return;
        if (other->getObjectCategory() != ObjectCategory::CHARACTER) return;
        Character* player = dynamic_cast<Character*>(other.get());
        if (player) {
            owner->onCollect(player);
            owner->setActive(false);
            this->setActive(false); // disable trigger after use
        }
    }
}

void TriggerZone::setActive(bool val) { active = val; }
bool TriggerZone::isActive() const { return active; }

void TriggerZone::setCollided(bool val) { collided = val; }
bool TriggerZone::isCollided() const { return collided; }

Vector2 TriggerZone::getPosition() const { return position; }
void TriggerZone::setPosition(Vector2 newPos) { position = newPos; }

ObjectType TriggerZone::getObjectType() const {
    return TriggerType::ZONE;
}

Vector2 TriggerZone::getSize() const {
    return size;
}