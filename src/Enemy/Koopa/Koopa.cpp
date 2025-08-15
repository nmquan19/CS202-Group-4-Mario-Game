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
#include <memory>
#include <utility>
Koopa::Koopa(Vector2 startPos, Vector2 velocity, Vector2 accelleration) : Enemy(startPos, velocity, accelleration, TextureManager::enemyTextures), ledgeDetector(/*std::make_shared<LedgeDetector>(1000.f)*/nullptr)
{
    HP = 1;
    stompedAnimation = false;
    isFacingRight = false;
    //ledgeDetector = nullptr;
}
Koopa::~Koopa() {	
    
}

Koopa::Koopa(Vector2 startPos, Vector2 size) : Enemy(startPos, TextureManager::enemyTextures, size), ledgeDetector(/*std::make_shared<LedgeDetector>(1000.f)*/nullptr)
{
    stompedAnimation = false;
    isFacingRight = false;
    //ledgeDetector = nullptr;
}

void Koopa::onCollision(std::shared_ptr<Object> other, Direction dir) {
    switch (other->getObjectCategory()) {
    case ObjectCategory::CHARACTER:
        if (dir == Direction::UP) {
            this->changeState(&KoopaStompedState::GetInstance());
        }
    case ObjectCategory::ENEMY:
    case ObjectCategory::INTERACTIVE:
    case ObjectCategory::BLOCK:
        handleEnvironmentCollision(other, dir);
        break;
    case ObjectCategory::PROJECTILE:
        this->changeState(&KoopaKnockState::GetInstance());
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
    b2Vec2 currentVel = physicsBody->GetLinearVelocity();
    switch (dir) {
    case Direction::LEFT:
        isFacingRight = true;
        this->physicsBody->SetLinearVelocity(b2Vec2(abs(currentVel.x), currentVel.y));
        break;
    case Direction::RIGHT:
        this->physicsBody->SetLinearVelocity(b2Vec2(-abs(currentVel.x), currentVel.y));
        isFacingRight = false;
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
    if (ledgeDetector) {
        if (!ledgeDetector->isNearLedge())
        {
            isFacingRight ^= 1;
            b2Vec2 currentVel = physicsBody->GetLinearVelocity();
            physicsBody->SetLinearVelocity(b2Vec2(-currentVel.x, currentVel.y));
        }
        ledgeDetector->update(this, deltaTime);
    }

    Enemy::update(deltaTime);

    if (currentState) {
        currentState->update(this, deltaTime);
        currentState->checkCondition(this);
    }
}

GreenKoopa::GreenKoopa(Vector2 startPos, Vector2 size) : Koopa(startPos, size) {
    changeState(&KoopaWanderingState::GetInstance());
    spritebox = TextureManager::Enemy_sprite_boxes[45];
 }
GreenKoopa::GreenKoopa(Vector2 startPos, Vector2 velocity, Vector2 accelleration) : Koopa(startPos, velocity, accelleration) {
    changeState(&KoopaWanderingState::GetInstance());
    spritebox = TextureManager::Enemy_sprite_boxes[45];
}
EnemyType GreenKoopa::getType() const {
    return EnemyType::GREEN_KOOPA;
}

RedKoopa::RedKoopa(Vector2 startPos, Vector2 size) : Koopa(startPos, size) {
    changeState(&KoopaWanderingState::GetInstance());
    spritebox = TextureManager::Enemy_sprite_boxes[53];
}
RedKoopa::RedKoopa(Vector2 startPos, Vector2 velocity, Vector2 accelleration) : Koopa(startPos, velocity, accelleration) {
    changeState(&KoopaWanderingState::GetInstance());
    spritebox = TextureManager::Enemy_sprite_boxes[53];
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
