#include "../../include/Objects/InteractiveObject.h"

InteractiveObject::InteractiveObject(Vector2 position, Vector2 size) {
    this->position = position;
    this->hitBox = {position.x, position.y, size.x, size.y};
}

void InteractiveObject::update(float deltaTime) {

}

void InteractiveObject::draw() {

}

Rectangle InteractiveObject::getHitBox() const {
    return hitBox;
}

ObjectCategory InteractiveObject::getObjectCategory() const {
    return ObjectCategory::INTERACTIVE;
}

std::vector<ObjectCategory> InteractiveObject::getCollisionTargets() const {

}

void InteractiveObject::checkCollision(const std::vector<std::shared_ptr<Object>>& candidates) {

}

void InteractiveObject::onCollision(std::shared_ptr<Object> other) {

}

void InteractiveObject::setActive(bool flag) {
    active = flag;
}

bool InteractiveObject::isActive() const {
    return active;
}

void InteractiveObject::setCollided(bool flag) {
    collided = flag;
}

bool InteractiveObject::isCollided() const {
    return collided;
}

Vector2 InteractiveObject::getPosition() const {
    return position;
}

void InteractiveObject::setPosition(Vector2 newPos) {
    position = newPos;
}
