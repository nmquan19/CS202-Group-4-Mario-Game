#include "..\..\include\Enemy\Enemy.h"
#include <raymath.h>
#include <vector>
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/System/Interface.h"
#include <raylib.h>
#include "../../include/Enemy/EnemyState.h"
#include <algorithm>
#include "../../include/System/Grid.h"
#include "../../include/System/Box2DWorldManager.h"
#include <utility>

Enemy::Enemy(Vector2 startPos, Vector2 velocity, Vector2 accelleration,Texture2D texture) : position(startPos), active(true), velocity(velocity), accelleration(accelleration), texture(texture), aniTimer(0), aniSpeed(0.2f), collided(false) {
    isalive = true;
    hitbox = { position.x, position.y,  size.x * GridSystem::GRID_SIZE,
        size.y * GridSystem::GRID_SIZE};
    currentState = nullptr; 
    physicsBody = Box2DWorldManager::getInstance().createEnemyBody(position, { hitbox.width, hitbox.height });
    if (physicsBody) {
        physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
    }
}
Enemy::Enemy(Vector2 startPos,  Texture2D texture, Vector2 size) : position(startPos), active(true), velocity({0,0}), accelleration({0,0}), texture(texture), aniTimer(0), aniSpeed(0.2f), collided(false) {
    this->size = size; 
	isalive = true;
    this->spritebox = { 0, 0, 32, 32}; 
    hitbox = {position.x, position.y,  size.x * GridSystem::GRID_SIZE,
        size.y * GridSystem::GRID_SIZE };
    currentState = nullptr;
    physicsBody = Box2DWorldManager::getInstance().createEnemyBody(position, { hitbox.width, hitbox.height });
    if (physicsBody) {
        physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
    }
}
Enemy::~Enemy() {
    if (physicsBody) {
        Box2DWorldManager::getInstance().destroyBody(physicsBody);
        physicsBody = nullptr;
    }
}

std::vector<Rectangle> Enemy::getHitBox() const {
    return {hitbox};
}
std::vector<ObjectCategory> Enemy::getCollisionTargets() const 
{
	return { ObjectCategory::CHARACTER, ObjectCategory::BLOCK, ObjectCategory::PROJECTILE};
}
void Enemy::applyGravity(float deltaTime) {
    // Box2D handles gravity automatically, but we can apply additional forces if needed
    if (!onGround && physicsBody) {
        // Box2D already applies gravity, but we can add custom gravity if needed
        // For now, let Box2D handle it with its built-in gravity
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
    
    // Sync position and velocity with Box2D physics body
    if (physicsBody) {
        // Get position from Box2D
        b2Vec2 b2Pos = physicsBody->GetPosition();
        Vector2 bodyPos = Box2DWorldManager::b2ToRaylib(b2Pos);
        position.x = bodyPos.x - hitbox.width * 0.5f;
        position.y = bodyPos.y - hitbox.height * 0.5f;
        
        // Get velocity from Box2D
        b2Vec2 b2Vel = physicsBody->GetLinearVelocity();
        velocity = Box2DWorldManager::b2ToRaylib(b2Vel);
        
        // Update hitbox position
        hitbox.x = position.x;
        hitbox.y = position.y;
        
        // Check if on ground based on velocity and position
        onGround = (abs(velocity.y) < 0.1f && getBottom() >= groundLevel - 5.0f);
    } else {
        // Fallback to manual physics if no Box2D body
        position += velocity * deltaTime;
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
void Enemy::flipDirection()
{
    isFacingRight ^= 1; 
}
bool Enemy::isAlive() const {
    return isalive;
}
void Enemy::onCollision(std::shared_ptr<Object> other, Direction dir) {
    if (other->getObjectCategory() == ObjectCategory::CHARACTER) {
        // Character collision handled by derived classes via handleCharacterCollision
        // No direct velocity modification here since Box2D handles the physics
    }
    if (other->getObjectCategory() == ObjectCategory::ENEMY) {
        // Enemy-to-enemy collision - Box2D handles the physical response
        // We can add game logic effects here if needed
    }
    if (other->getObjectCategory() == ObjectCategory::BLOCK) {
        handleEnvironmentCollision(other, dir);
    }
    if (other->getObjectCategory() == ObjectCategory::PROJECTILE) {
        // Handle projectile collision - implementation depends on derived class
    }
}
void Enemy::handleEnvironmentCollision(std::shared_ptr<Object> other, Direction dir) {
    // With Box2D, physical collision response is handled automatically
    // We only need to handle game logic responses here
    
    switch (dir) {
    case Direction::DOWN:
        onGround = true;
        groundLevel = other->getPosition().y; // Store ground level for reference
        break;
    case Direction::UP:
        // Hit ceiling - Box2D handles the physical response
        break;
    case Direction::LEFT:
    case Direction::RIGHT:
        // Hit wall - for some enemies this might trigger direction change
        // This depends on the specific enemy type, so it's handled in derived classes
        break;
    }
}
bool Enemy::isActive() const {
	return active;
}
void Enemy::setVelocity(Vector2 newVelocity) {
    velocity = newVelocity;
    if (physicsBody) {
        physicsBody->SetLinearVelocity(Box2DWorldManager::raylibToB2(newVelocity));
    }
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
    if (physicsBody) {
        physicsBody->SetTransform(Box2DWorldManager::raylibToB2(newPosition), physicsBody->GetAngle());
    }
}
Vector2 Enemy::getDirection() const {
    return direction;
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

Vector2 Enemy::getSize() const {
    switch(getType()) {
        case EnemyType::GOOMBA:
            return Constants::Goomba::standardSize;
        case EnemyType::GREEN_KOOPA:
            return  Constants::GreenKoopa::standardSize;
        case EnemyType::RED_KOOPA:
			return Constants::RedKoopa::standardSize;
        default:
            return {1, 1};
    }
}