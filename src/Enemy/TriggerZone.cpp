#include "../../include/Enemy/TriggerZone.h"
#include <raylib.h>
#include <vector>
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/System/Interface.h"
#include "../../include/Characters/Character.h"
#include "../../include/Objects/CollectableObject.h"
#include <raymath.h>
#include "../../include/System/Grid.h"
TriggerZone::TriggerZone(CollectableObject* ownerItem, Vector2 pos, Vector2 sz)
    : owner(ownerItem) {
    position = pos;
    size = sz*1.5;
    position.x = owner->getPosition().x + (owner->getHitBox().width - size.x * GridSystem::GRID_SIZE) / 2;
    position.y = owner->getPosition().y + (owner->getHitBox().height - size.y * GridSystem::GRID_SIZE) / 2;

    hitBox = Rectangle{ position.x, position.y, size.x * GridSystem::GRID_SIZE, size.y * GridSystem::GRID_SIZE };
    this->active = true;
     //PhysicsManager::getInstance().addObject(this); 
} 

void TriggerZone::update(float deltaTime) {
    position.x = owner->getPosition().x+ (owner->getHitBox().width - size.x*GridSystem::GRID_SIZE)/2 ;
    position.y = owner->getPosition().y + (owner->getHitBox().height - size.y*GridSystem::GRID_SIZE)/2;
	hitBox = Rectangle{ position.x, position.y, size.x * GridSystem::GRID_SIZE, size.y * GridSystem::GRID_SIZE };
}

void TriggerZone::draw() {
}

Rectangle TriggerZone::getHitBox() const {
    return hitBox;
}

ObjectCategory TriggerZone::getObjectCategory() const {
    return ObjectCategory::TRIGGER;
}

std::vector<ObjectCategory> TriggerZone::getCollisionTargets() const {
    return { ObjectCategory::CHARACTER};
}

void TriggerZone::checkCollision(const std::vector<std::shared_ptr<Object>>& candidates) {
    for (auto obj : candidates) {
        if (!obj->isActive()) continue;
        setCollided(true);
        if (IsKeyDown(KEY_E))
        {
            onCollision(obj);
        }
    }
}

void TriggerZone::onCollision(std::shared_ptr<Object> other) {
    if (!owner || !other) return;
    if (other->getObjectCategory() != ObjectCategory::CHARACTER) return;
    Character* player = dynamic_cast<Character*>(other.get());
    if (player) {
        owner->onCollect(player);
        owner->setActive(false);
        this->setActive(false); // disable trigger after use
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