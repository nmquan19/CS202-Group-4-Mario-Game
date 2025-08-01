#include "../../include/Item/Item.h"
#include "../../include/System/PhysicsManager.h"
#include "../../include/System/Grid.h"
#include "../../include/System/TextureManager.h"
#include "../../include/Game/GameContext.h"
#include <raymath.h>
#include <memory> // Include this header for std::enable_shared_from_this

Item::Item() {
	
}

Item::Item(Vector2 startPos)
    : position(startPos), active(true), scale(1.0f),
    velocity({ 0, 0 }),
    animation(new Anima(0, 3, 0.08f))
{

    this->spritebox = { 0, 0, 32, 32 };
    this->size = { 1, 1};
    this->hitbox = { position.x, position.y, size.x * GridSystem::GRID_SIZE, size.y * GridSystem::GRID_SIZE };
}

Item::~Item() {
    // PhysicsManager::getInstance().markForDeletion(this);
    delete animation;
}

void Item::update(float deltaTime) {
    /*velocity.y += gravity * deltaTime;
    position.y += velocity.y * deltaTime;
    if (position.y >= groundY) {
        position.y = groundY;
        velocity.y = 0;
    }*/
	//std::cout << deltaTime << " " << velocity.y << " " << position.y << std::endl;
    animation->Update();
}

void Item::draw() {
    // Draw the item using the animation frames
    DrawTexturePro(
        TextureManager::getInstance().getItemTexture(),
        animation->GetFrame(),
        hitbox,
        { 0, 0 },
        0,
        WHITE
    );
    DrawText(TextFormat("hitbot: %f %f", hitbox.width, hitbox.height), 50, 50, 50, BLACK);
}

void Item::draw(Texture2D texture) {
    DrawTexturePro(
        texture,
        animation->GetFrame(),
        { position.x, position.y, 32, 32 },
        { 0, 0 },
        0,
        WHITE
    );
}

std::vector<Rectangle> Item::getHitBox() const {
    return { hitbox }; // Đưa Rectangle đơn vào trong vector
}

ObjectCategory Item::getObjectCategory() const {
    return ObjectCategory::ITEM;
}

std::vector<ObjectCategory> Item::getCollisionTargets() const {
    return { ObjectCategory::CHARACTER };
}

void Item::onCollision(std::shared_ptr<Object> other, Direction direction) {
    if (other->getObjectCategory() == ObjectCategory::CHARACTER) {
        setActive(false);
        GameContext::getInstance().mark_for_deletion_Object(GameContext::getInstance().getSharedPtrFromRaw(this));
    }
}

Vector2 Item::getPosition() const {
    return position;
}

void Item::setPosition(Vector2 newPosition) {
    position = newPosition;
    hitbox.x = position.x;
    hitbox.y = position.y;
}

bool Item::isActive() const {
    return active;
}

void Item::setActive(bool flag) {
    active = flag;
}

bool Item::isCollided() const {
    return collided;
}

void Item::setCollided(bool value) {
    collided = value;
}

Vector2 Item::getSize() const {
    return size;
}

ObjectType Item::getObjectType() const {
    return type;
}



