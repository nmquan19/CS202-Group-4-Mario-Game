#include "..\..\include\Enemy\Enemy.h"
#include <raymath.h>
#include <vector>
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/System/Interface.h"
#include <raylib.h>
#include "../../include/Enemy/EnemyState.h"
#include <algorithm>
#include "../../include/System/Grid.h"
#include <utility>
Enemy::Enemy(Vector2 startPos, Vector2 velocity, Vector2 accelleration,Texture2D texture) : position(startPos), active(true), velocity(velocity), accelleration(accelleration), texture(texture), aniTimer(0), aniSpeed(0.2f) {
	isalive = true;
    size = { 1,1 };
    hitbox = { position.x, position.y,  size.x * GridSystem::GRID_SIZE,
        size.y * GridSystem::GRID_SIZE};
    currentState = nullptr; 
    // PhysicsManager::getInstance().addObject(this);

}
Enemy::Enemy(Vector2 startPos,  Texture2D texture, Vector2 size) : position(startPos), active(true), velocity({0,0}), accelleration({0,0}), texture(texture), aniTimer(0), aniSpeed(0.2f) {
    this->size = size; 
	isalive = true;
    this->spritebox = { 0, 0, 32, 32}; 
    hitbox = {position.x, position.y,  size.x * GridSystem::GRID_SIZE,
        size.y * GridSystem::GRID_SIZE };
    currentState = nullptr;
    // PhysicsManager::getInstance().addObject(this);
}
Enemy::~Enemy() {
    // PhysicsManager::getInstance().markForDeletion(this);
}

Rectangle Enemy::getHitBox() const {
    return hitbox;
}
std::vector<ObjectCategory> Enemy::getCollisionTargets() const 
{
	return { ObjectCategory::CHARACTER, ObjectCategory::ENEMY, ObjectCategory::BLOCK, ObjectCategory::PROJECTILE };
}
void Enemy::applyGravity(float deltaTime) {
    if (!onGround) {
        velocity.y +=  980* deltaTime;
    }
}
void Enemy::update(float deltaTime)
{
    aniTimer += deltaTime;
    DrawText(TextFormat("curFrame: x =%f", curFrame), 700, 700, 12, BLACK);
    if (aniTimer >= aniSpeed) {
        curFrame += 1;
        aniTimer = 0;
    }
    position += velocity* deltaTime;
    if (getBottom() >= groundLevel) {
        position.y = groundLevel - (spritebox.height * scale);
        onGround = true;  
        if (velocity.y > 0) {
            velocity.y = 0;
        }
    }
    else {
        onGround = false; 
    }
}

void Enemy::draw() {
    Rectangle sourceRec = this->spritebox;      
    Vector2 origin = { 0, 0 };
    DrawTexturePro(this->texture, sourceRec, hitbox, origin, 0.0f, WHITE);
}

ObjectCategory Enemy::getObjectCategory() const {
	return ObjectCategory::ENEMY;
}
void Enemy::changeState(EnemyState* other)
{      
     if(currentState) currentState->exit(this);
      currentState =  other; 
      if(other)   currentState->enter(this);
}
bool Enemy::isAlive() const {
    return isalive;
}
void Enemy::onCollision(Object* other) {
	if (other->getObjectCategory() == ObjectCategory::CHARACTER) {
            // velocity = {0,0} ;  
            // accelleration = {0,0};
    }
    if (other->getObjectCategory() == ObjectCategory::ENEMY) {
        // Push enemy away from Mario slightly to prevent sticking
        Rectangle otherHitBox = other->getHitBox();
        Vector2 pushDirection = {
            position.x - otherHitBox.x,
            position.y - otherHitBox.y
        };

        // Normalize and apply small push
        float length = pushDirection.x * pushDirection.x + pushDirection.y * pushDirection.y;
        if (length > 0) {
            pushDirection.x /= length;
            pushDirection.y /= length;
            position.x += pushDirection.x * 500.0f; // Small push away
            position.y += pushDirection.y * 500.0f;
        }
    }
}
void Enemy::handleEnvironmentCollision(Object* other) {
    Rectangle playerHitBox = getHitBox();
    Rectangle otherHitBox = other->getHitBox();

    if (!CheckCollisionRecs(playerHitBox, otherHitBox)) {
        return;
    }

    // Calculate overlap amounts for each direction
    float overlapLeft = (playerHitBox.x + playerHitBox.width) - otherHitBox.x;
    float overlapRight = (otherHitBox.x + otherHitBox.width) - playerHitBox.x;
    float overlapTop = (playerHitBox.y + playerHitBox.height) - otherHitBox.y;
    float overlapBottom = (otherHitBox.y + otherHitBox.height) - playerHitBox.y;

    const float MIN_OVERLAP = 2.0f;

    if (overlapTop < MIN_OVERLAP && overlapBottom < MIN_OVERLAP && overlapLeft < MIN_OVERLAP && overlapRight < MIN_OVERLAP) {
        return;
    }

    float minOverlap = std::min({ overlapTop, overlapBottom, overlapLeft, overlapRight });

    if (minOverlap == overlapTop) {
        position.y = otherHitBox.y - playerHitBox.height;
        velocity.y = 0;
        onGround = true; 
    }
    else if (minOverlap == overlapBottom) {
        position.y = otherHitBox.y + otherHitBox.height;
        if (velocity.y < 0) {
            velocity.y = 0;
        }
    }
    else if (minOverlap == overlapLeft && overlapLeft >= MIN_OVERLAP) {
        position.x = otherHitBox.x - playerHitBox.width;
        velocity.x *=-1;
    }
    else if (minOverlap == overlapRight && overlapRight >= MIN_OVERLAP) {
        position.x = otherHitBox.x + otherHitBox.width;
        velocity.x *= -1;
    }
}
bool Enemy::isActive() const {
	return active;
}
void Enemy::setVelocity(Vector2 newVelocity) {
    velocity = newVelocity;
}

Vector2 Enemy::getVelocity() {
    return velocity;
}

void Enemy::setSpeed(float newSpeed) {
    speed = newSpeed;
}

float Enemy::getSpeed() {
    return speed;
}

void Enemy::setPosition(Vector2 newPosition) {
    position = newPosition;
}

Vector2 Enemy::getPosition() const {
    return position;
}


void Enemy::setActive(bool flag) {
    active = flag;
}

bool Enemy::isCollided() const {
    return collided;
}

void Enemy::setCollided(bool flag) {
    collided = flag;
}


float Enemy::getWidth() const {
    return spritebox.width * scale;
}

float Enemy::getHeight() const {
    return spritebox.height * scale;
}

float Enemy::getCenterX() const {
    return position.x + (spritebox.width * scale) / 2;
}
bool Enemy::FacingRight() const
{
    return isFacingRight;
}
float Enemy::getBottom() const {
    return position.y + hitbox.height;
}
float Enemy::getCenterY() const {
    return position.y + (spritebox.height * scale) / 2;
}

Vector2 Enemy::getCenter() const {
    return Vector2{ getCenterX(), getCenterY() };
}

std::vector<std::pair<int, int>> Enemy::getSpriteData() {
   switch(getType()) {
        case EnemyType::GOOMBA:
            return { {0,1},{2,3},{5,5} }; 
        case EnemyType::GREEN_KOOPA:
            return  {{45,46}};
        case EnemyType::RED_KOOPA:
			return { {53,54} };
        default:
            return {};
   }
}