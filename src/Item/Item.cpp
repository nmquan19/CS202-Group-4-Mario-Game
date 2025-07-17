#include "../../include/Item/Item.h"
#include "../../include/System/PhysicsManager.h"
#include "../../include/System/Grid.h"
#include <raymath.h>

#include <memory> // Include this header for std::enable_shared_from_this

Item::Item(Vector2 startPos, Texture2D texture, Vector2 size)
    : position(startPos), texture(texture), active(true), scale(1.0f),
    velocity({ 0, 0 }), accelleration({ 0, 0 }), aniTimer(0), aniSpeed(0.2f), curFrame(0) {



    this->spritebox = { 0, 0, 32, 32 };
    this->hitbox = { position.x, position.y, size.x * GridSystem::GRID_SIZE, size.y * GridSystem::GRID_SIZE };
}


Item::~Item() {
    // PhysicsManager::getInstance().markForDeletion(this);
}


void Item::update(float deltaTime) {
    aniTimer += deltaTime;
    if (aniTimer >= aniSpeed) {
        curFrame = (curFrame + 1) % 4; // Giả định item có 4 frame animation
        aniTimer = 0;
    }

    position += velocity * deltaTime;
    hitbox.x = position.x;
    hitbox.y = position.y;
}

void Item::draw() {
    Rectangle sourceRec = this->spritebox;
    DrawTexturePro(texture, sourceRec, hitbox, { 0,0 }, 0.0f, WHITE);
}

Rectangle Item::getHitBox() const {
    return hitbox;
}

ObjectCategory Item::getObjectCategory() const {
    return ObjectCategory::ITEM;
}

std::vector<ObjectCategory> Item::getCollisionTargets() const {
    return { ObjectCategory::CHARACTER };
}

void Item::onCollision(Object* other) {
    // Ví dụ: Khi va chạm với nhân vật thì biến mất
    if (other->getObjectCategory() == ObjectCategory::CHARACTER) {
        active = false;
    }
}

void Item::checkCollision(const std::vector<Object*>& candidates) {
    for (Object* obj : candidates) {
        if (!obj->isActive()) continue;

        for (ObjectCategory targetCat : getCollisionTargets()) {
            if (obj->getObjectCategory() == targetCat && CheckCollisionRecs(hitbox, obj->getHitBox())) {
                onCollision(obj);
            }
        }
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

CoinItem::CoinItem(Vector2 startPos, Texture2D texture, Vector2 size)
    : Item(startPos, texture, size) {
}

std::variant<CharacterType, BlockType, EnemyType, KoopaShellType, TriggerType, ItemType> CoinItem::getObjectType() const {
    return ItemType::COIN;
}

MushroomItem::MushroomItem(Vector2 startPos, Texture2D texture, Vector2 size)
    : Item(startPos, texture, size) {
}

std::variant<CharacterType, BlockType, EnemyType, KoopaShellType, TriggerType, ItemType> MushroomItem::getObjectType() const {
    return ItemType::MUSHROOM;  
}

