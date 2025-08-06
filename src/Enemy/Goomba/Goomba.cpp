#include  "../../../include/Enemy/Goomba/Goomba.h"
#include "../../../include/System/TextureManager.h"
#include "../../../include/Enemy/Enemy.h"
#include "../../../include/Enemy/Goomba/GoombaState.h"
#include "../../../include/Objects/ObjectFactory.h"
#include "../../../include/System/Interface.h"
#include "../../../include/Characters/Character.h"
#include <raymath.h>
#include <raylib.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>

Goomba::Goomba(Vector2 startPos,Vector2 velocity, Vector2 accelleration): Enemy(startPos, velocity,accelleration, TextureManager::enemyTextures )
{    
    HP = 1;
    scale = 5; 
	stompedAnimation = false;
    changeState(&GoombaWanderingState::GetInstance());
}
Goomba::Goomba(Vector2 startPos, Vector2 size) : Enemy(startPos,TextureManager::enemyTextures, size)
{   
    stompedAnimation = false;

    changeState(&GoombaWanderingState::GetInstance());
}

void Goomba::onCollision(std::shared_ptr<Object> other, Direction dir) {
    switch (other->getObjectCategory()) {
    case ObjectCategory::INTERACTIVE:
    case ObjectCategory::BLOCK:
        handleEnvironmentCollision(other, dir);
        break;
    case ObjectCategory::PROJECTILE:
        this->changeState(&GoombaKnockState::GetInstance());
        break;
    case ObjectCategory::CHARACTER:
        if (dir == Direction::UP) { 
            this->changeState(&GoombaStompedState::GetInstance());
        } 
        break;
    }
}

void Goomba::draw() {  
    Rectangle source = spritebox;
    Rectangle dest = hitbox;  
    if (knockAnimation && velocity.y > 0) {
        source.height *= -1;
    }else if(stompedAnimation)
    {   
        dest.y += dest.height * 0.5f;
        dest.height *= 0.5f; 
    }
    Vector2 origin = { 0, 0 };
    DrawTexturePro(this->texture, source, dest, origin, 0.0f, WHITE);
}

void Goomba::handleEnvironmentCollision(std::shared_ptr<Object> other, Direction dir) {
    b2Vec2 currentVel = this->physicsBody->GetLinearVelocity();
    switch (dir) {
    case Direction::LEFT:
        isFacingRight = true;
        physicsBody->SetLinearVelocity(b2Vec2(abs(currentVel.x), currentVel.y));
        break;
    case Direction::RIGHT:
        physicsBody->SetLinearVelocity(b2Vec2(-abs(currentVel.x), currentVel.y));
        isFacingRight = false;
        break;
    }
}

void Goomba::die()
{
    setActive(false);
}

void Goomba::takeDamage(int amount) {
    HP -= amount;
    this->changeState(&GoombaStompedState::GetInstance());
    if (HP <= 0) {
        die();
    }
}

void Goomba::update(float deltaTime) {
    Enemy::update(deltaTime);

    if (currentState)
    {
        currentState->update(this, deltaTime);
        currentState->checkCondition(this);
    }
}

EnemyType Goomba::getType() const {
    return EnemyType::GOOMBA;
}

ObjectType Goomba::getObjectType() const {
    return EnemyType::GOOMBA;
}