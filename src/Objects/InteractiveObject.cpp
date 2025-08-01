#include "../../include/Objects/InteractiveObject.h"
#include "../../include/System/Constant.h"

InteractiveObject::InteractiveObject(Vector2 position, Vector2 size) {
    this->position = position;
    this->hitBox = {position.x, position.y, size.x * Constants::TILE_SIZE, size.y * Constants::TILE_SIZE };
    this->size = size;
}

std::vector<Rectangle> InteractiveObject::getHitBox() const {
    return { hitBox };
}

ObjectCategory InteractiveObject::getObjectCategory() const {
    return ObjectCategory::INTERACTIVE;
}

std::vector<ObjectCategory> InteractiveObject::getCollisionTargets() const {
    return { ObjectCategory::CHARACTER };
}

void InteractiveObject::onCollision(std::shared_ptr<Object> other, Direction direction) {

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
