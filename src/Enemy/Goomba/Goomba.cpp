#include  "../../../include/Enemy/Goomba/Goomba.h"
#include "../../../include/System/TextureManager.h"
#include <raymath.h>
#include "../../../include/Enemy/Enemy.h"
#include "../../../include/Enemy/Goomba/GoombaState.h"
#include "../../../include/Objects/ObjectFactory.h"
#include "../../../include/System/Interface.h"
#include "../../../include/Characters/Character.h"
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
    case ObjectCategory::BLOCK:
        handleEnvironmentCollision(other, dir);
        break;
    case ObjectCategory::PROJECTILE:
        this->changeState(&GoombaKnockState::GetInstance());
        break;
    case ObjectCategory::CHARACTER:
        if (dir == Direction::UP) { // Character jumped on Goomba from above
            DrawText("STOMPED", 200, 200, 20, RED);
            this->changeState(&GoombaStompedState::GetInstance());
        } else {
            DrawText("CHARACTER COLLISION", 200, 200, 20, RED);
            // Side collision - character should take damage
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
    Enemy::handleEnvironmentCollision(other, dir); // Call base class implementation
    
    switch (dir) {
    case Direction::DOWN:
        // Already handled in base class
        break;
    case Direction::LEFT:
    case Direction::RIGHT:
        // Goomba bounces off walls - reverse direction
        if (physicsBody) {
            b2Vec2 currentVel = physicsBody->GetLinearVelocity();
            physicsBody->SetLinearVelocity(b2Vec2(-currentVel.x, currentVel.y));
        } else {
            velocity.x *= -1.0f;
        }
        break;
    case Direction::UP:
        // Hit ceiling
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
        }
        if (position.x > 1920 - hitbox.width)
        {
            position.x = 1920 - hitbox.width;
            velocity.x *= -1;
        }
    }
    // Note: Bounds checking now handled by Box2D world boundaries or wall blocks
}
EnemyType Goomba::getType() const {
    return EnemyType::GOOMBA;
}
ObjectType Goomba::getObjectType() const {
    return EnemyType::GOOMBA;
}