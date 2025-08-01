#include  "../../../include/Enemy/Koopa/Koopa.h"
#include "../../../include/System/TextureManager.h"
#include <raymath.h>
#include "../../../include/Enemy/Enemy.h"
#include "../../../include/Enemy/Koopa/KoopaState.h"
#include "../../../include/Objects/ObjectFactory.h"
#include "../../../include/System/Interface.h"
#include "../../../include/System/Constant.h"
#include <raylib.h>
#include <vector>
#include <algorithm>
#include "../../../include/Enemy/LedgeDetector.h"
#include "../../../include/System/PhysicsManager.h"
#include <memory>
#include <utility>
Koopa::Koopa(Vector2 startPos, Vector2 velocity, Vector2 accelleration) : Enemy(startPos, velocity, accelleration, TextureManager::enemyTextures)
{
    HP = 1;
    stompedAnimation = false;
    isFacingRight = velocity.x > 0;

    ledgeDetector  =  std::make_shared<LedgeDetector>(1000.0f);
    PhysicsManager::getInstance().addObject(ledgeDetector);
}
Koopa::~Koopa()
{
	
    if (ledgeDetector) {
        PhysicsManager::getInstance().markForDeletion(std::move(ledgeDetector));
    }
    

}
Koopa::Koopa(Vector2 startPos, Vector2 size) : Enemy(startPos, TextureManager::enemyTextures, size), ledgeDetector(std::make_shared<LedgeDetector>(10.0f))
{
    stompedAnimation = false;
    isFacingRight = velocity.x >= 0;
    ledgeDetector = std::make_shared<LedgeDetector>(10.0f);
    PhysicsManager::getInstance().addObject(ledgeDetector);
}

void Koopa::onCollision(std::shared_ptr<Object> other, Direction dir) {
    switch (other->getObjectCategory()) {
    case ObjectCategory::BLOCK:
        handleEnvironmentCollision(other, dir);
        break;
    case ObjectCategory::PROJECTILE:
        this->changeState(&KoopaKnockState::GetInstance());
        break;
    case ObjectCategory::CHARACTER:
        if (dir == Direction::UP) { // Character jumped on Koopa from above
            this->changeState(&KoopaStompedState::GetInstance());
        } else {
            // Side collision - character should take damage
        }
        break;
    }
}
void Koopa::draw() {

    Rectangle source = spritebox;
    Rectangle dest = hitbox;
    if (knockAnimation && velocity.y > 0) {
        source.height *= -1;
    }
    else if (stompedAnimation)
    {
        dest.y += dest.height * 0.5f;
        dest.height *= 0.5f;
    }
    if(isFacingRight)
    {
		source.width *= -1;
    }
    Vector2 origin = { 0, 0 };
    DrawTexturePro(this->texture, source, dest, origin, 0.0f, WHITE);
}

void Koopa::handleEnvironmentCollision(std::shared_ptr<Object> other, Direction dir) {
    Enemy::handleEnvironmentCollision(other, dir); // Call base class implementation
    
    switch (dir) {
    case Direction::DOWN:
        // Already handled in base class
        break;
    case Direction::UP:
        // Hit ceiling
        break;
    case Direction::LEFT:
        if (physicsBody) {
            b2Vec2 currentVel = physicsBody->GetLinearVelocity();
            physicsBody->SetLinearVelocity(b2Vec2(-currentVel.x, currentVel.y));
        } else {
            velocity.x *= -1.0f;
        }
        isFacingRight = false;
        break;
    case Direction::RIGHT:
        if (physicsBody) {
            b2Vec2 currentVel = physicsBody->GetLinearVelocity();
            physicsBody->SetLinearVelocity(b2Vec2(-currentVel.x, currentVel.y));
        } else {
            velocity.x *= -1.0f;
        }
        isFacingRight = true;
        break;
    }
}

void Koopa::die()   
{
}
void Koopa::takeDamage(int amount) {
    HP -= amount;
    this->changeState(&KoopaStompedState::GetInstance());
    if (HP <= 0) {
        die();
    }
}

void Koopa::update(float deltaTime) {

    if (!ledgeDetector->isNearLedge())
    {
        isFacingRight ^= 1;
        if (physicsBody) {
            b2Vec2 currentVel = physicsBody->GetLinearVelocity();
            physicsBody->SetLinearVelocity(b2Vec2(-currentVel.x, currentVel.y));
        } else {
            velocity.x *= -1;
        }
    }
    ledgeDetector->update(this, deltaTime);

    Enemy::update(deltaTime);
    if (!stompedAnimation) {
        applyGravity(deltaTime);
    }
    if (currentState)
    {
        currentState->update(this, deltaTime);
        currentState->checkCondition(this);
    }
    
    // Update hitbox position (already handled in Enemy::update for Box2D)
    if (!physicsBody) {
        // Fallback for manual physics bounds checking
        hitbox.x = position.x;
        hitbox.y = position.y;
        if (position.x < 0)
        {
            position.x = 0;
            velocity.x *= -1;
            isFacingRight = true; 
        }
        if (position.x > 1920 - hitbox.width)
        {
            position.x = 1920 - hitbox.width;
            velocity.x *= -1;
            isFacingRight = false;
        }
    }
    // Note: Bounds checking now handled by Box2D world boundaries or wall blocks
}

GreenKoopa::GreenKoopa(Vector2 startPos, Vector2 size) : Koopa(startPos, size) {
    changeState(&KoopaWanderingState::GetInstance());

 }
GreenKoopa::GreenKoopa(Vector2 startPos, Vector2 velocity, Vector2 accelleration) : Koopa(startPos, velocity, accelleration) {
    changeState(&KoopaWanderingState::GetInstance());

}
EnemyType GreenKoopa::getType() const {
    return EnemyType::GREEN_KOOPA;
}

RedKoopa::RedKoopa(Vector2 startPos, Vector2 size) : Koopa(startPos, size) {
    changeState(&KoopaWanderingState::GetInstance());

}
RedKoopa::RedKoopa(Vector2 startPos, Vector2 velocity, Vector2 accelleration) : Koopa(startPos, velocity, accelleration) {
    changeState(&KoopaWanderingState::GetInstance());

}
EnemyType RedKoopa::getType() const {
    return EnemyType::RED_KOOPA;
}
KoopaShellType Koopa::getShellType() const {
    switch (getType()) {
    case EnemyType::GREEN_KOOPA:
        return KoopaShellType::GREEN_KOOPA_SHELL;
    case EnemyType::RED_KOOPA:
        return KoopaShellType::RED_KOOPA_SHELL;
    default:
        return KoopaShellType::GREEN_KOOPA_SHELL; 
    }
}
ObjectType RedKoopa::getObjectType() const {
    return getType();
}

ObjectType GreenKoopa::getObjectType() const {
    return getType();
}
