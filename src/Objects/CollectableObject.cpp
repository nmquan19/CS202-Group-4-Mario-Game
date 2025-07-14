#include "../../include/Objects/CollectableObject.h"
#include <vector>
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/System/Interface.h"
#include "../../include/Enemy/TriggerZone.h"
#include "../../include/System/PhysicsManager.h"
#include <raylib.h>
#include "../../include/System/Grid.h"
#include <memory>

CollectableObject::CollectableObject(Vector2 pos, Vector2 sz,Texture2D texture): texture(texture)
{
    position = pos;
    size = sz;
	hitBox = Rectangle{ pos.x, pos.y, sz.x*GridSystem::GRID_SIZE, sz.y * GridSystem::GRID_SIZE };
    triggerZone = std::make_unique<TriggerZone>(this, pos, sz);
    this->active = true; 
    PhysicsManager::getInstance().addObject(this);
}

CollectableObject::~CollectableObject() {
    this->active = false;
}
void CollectableObject::update(float deltaTime) {
    if (triggerZone) {
        triggerZone->update(deltaTime);
    }
   
    hitBox.x = position.x;
	hitBox.y = position.y;
}

void CollectableObject::draw() {


}

Rectangle CollectableObject::getHitBox() const {
    return hitBox;
}

ObjectCategory CollectableObject::getObjectCategory() const {
    return ObjectCategory::ITEM;
}

std::vector<ObjectCategory> CollectableObject::getCollisionTargets() const {
    return {};
}

void CollectableObject::checkCollision(const std::vector<Object*>&) {
}

void CollectableObject::setActive(bool val) {
    active = val;
    if (triggerZone) triggerZone->setActive(val);
}

bool CollectableObject::isActive() const {
    return active;
}

void CollectableObject::setCollided(bool val) {
    collided = val;
}

bool CollectableObject::isCollided() const {
    return collided;
}

Vector2 CollectableObject::getPosition() const {
    return position;
}

void CollectableObject::setPosition(Vector2 newPos) {
    position = newPos;
    if (triggerZone) triggerZone->setPosition(newPos);
}
