#include "../../include/Objects/Projectile.h"
#include "../../include/System/PhysicsManager.h"
#include "../../include/Enemy/Enemy.h"
#include <iostream>

Projectile::Projectile(Vector2 position, int direction, float scale) : position(position), scale(scale), speed(200.0f) {
    velocity = Vector2{speed * direction, 0};
    spriteSheet = LoadTexture("assets/fireball_sprite.png");
    frameData = {{15, 9, 8, 8}, {50, 9, 8, 8}, {85, 9, 8, 8}, {120, 9, 8, 8}};
    currentFrame = 0;
    spriteRec = frameData[0];
    aniTimer = 0.0f;
    aniSpeed = 0.1f;
    PhysicsManager::getInstance().addObject(this);
}

Projectile::~Projectile() {
    UnloadTexture(spriteSheet);
    PhysicsManager::getInstance().markForDeletion(this);
}

ObjectCategory Projectile::getObjectCategory() const {
    return ObjectCategory::PROJECTILE;
}

Rectangle Projectile::getHitBox() const {
    return {position.x, position.y, spriteRec.width * scale, spriteRec.height * scale};
}

std::vector<ObjectCategory> Projectile::getCollisionTargets() const {
    return {ObjectCategory::BLOCK, ObjectCategory::ENEMY, ObjectCategory::CHARACTER};
}

void Projectile::checkCollision(const std::vector<Object*>& candidates) {
    for(auto candidate : candidates) {
        switch(candidate->getObjectCategory()) {
            case ObjectCategory::BLOCK:
                handleEnvironmentCollision(candidate);
                break;
            case ObjectCategory::CHARACTER:
                handleCharacterCollision(candidate);
                break;
            case ObjectCategory::ENEMY:
                handleEnemyCollision(candidate);
                break;
        }
    }
}

void Projectile::onCollision(Object* object) {
    if(object->getObjectCategory() == ObjectCategory::CHARACTER) {
        object->setActive(false);
    }
}

void Projectile::update(float deltaTime) {
    aniTimer += deltaTime;
    if(aniTimer >= aniSpeed) {
        currentFrame = (currentFrame + 1) % frameData.size();
        spriteRec = frameData[currentFrame];
        aniTimer = 0;
    }

    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
}

void Projectile::draw() {
    if(!active) return;

    Rectangle source = spriteRec;

    if(velocity.x < 0) {
        source.width = -source.width;
    }

    Rectangle dest = {position.x, position.y, abs(spriteRec.width) * scale, spriteRec.height * scale};

    DrawTexturePro(spriteSheet, source, dest, {0, 0}, 0.0f, WHITE);

    DrawText(TextFormat("position.x: %d, position.y: %d", position.x, position.y), 50, 200, 20, BLACK);
}

void Projectile::setVelocity(Vector2 newVel) {
    velocity = newVel;
}

Vector2 Projectile::getVelocity() {
    return velocity;
}

void Projectile::setPosition(Vector2 newPos) {
    position = newPos;
}

Vector2 Projectile::getPosition() const {
    return position;
}

void Projectile::setActive(bool flag) {
    active = flag;
}

bool Projectile::isActive() const {
    return active;
}

void Projectile::setCollided(bool flag) {
    collided = flag;
}

bool Projectile::isCollided() const {
    return collided;
}

void Projectile::handleEnvironmentCollision(Object* other) {
    setActive(false);
}

void Projectile::handleEnemyCollision(Object* other) {
    //setActive(false);
}