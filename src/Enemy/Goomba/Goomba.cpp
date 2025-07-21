#include  "../../../include/Enemy/Goomba/Goomba.h"
#include "../../../include/System/TextureManager.h"
#include <raymath.h>
#include "../../../include/Enemy/Enemy.h"
#include "../../../include/Enemy/Goomba/GoombaState.h"
#include "../../../include/Objects/ObjectFactory.h"
#include "../../../include/System/Interface.h"
#include <raylib.h>
#include <vector>
#include <algorithm>
#include <memory>
Goomba::Goomba(Vector2 startPos,Vector2 velocity, Vector2 accelleration): Enemy(startPos, velocity,accelleration, TextureManager::enemyTextures )
{    
    scale = 5; 
	stompedAnimation = false;
    changeState(&GoombaWanderingState::GetInstance());
}
Goomba::Goomba(Vector2 startPos, Vector2 size) : Enemy(startPos,TextureManager::enemyTextures, size)
{
    stompedAnimation = false;

    changeState(&GoombaWanderingState::GetInstance());
}

void Goomba::onCollision(std::shared_ptr<Object> other) {
   
	if (other->getObjectCategory() == ObjectCategory::CHARACTER) {
         DrawText("HITTED",200,200,20,RED);
         this->changeState(&GoombaStompedState::GetInstance());
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

void Goomba::checkCollision(const std::vector<std::shared_ptr<Object>>& candidates)
{   
    for (auto candidate : candidates) {
        switch (candidate->getObjectCategory()) {
        case ObjectCategory::BLOCK:
            handleEnvironmentCollision(candidate);
            break;
        case ObjectCategory::PROJECTILE:
			this->changeState(&GoombaKnockState::GetInstance());
            break;
        case ObjectCategory::CHARACTER:
			handleCharacterCollision(candidate);
            break;
        }
    }
}
void Goomba::handleCharacterCollision(std::shared_ptr<Object> other) {
    Rectangle playerHitBox = getHitBox();
    Rectangle otherHitBox = other->getHitBox();

    if (!CheckCollisionRecs(playerHitBox, otherHitBox)) return;

    float overlapLeft = (playerHitBox.x + playerHitBox.width) - otherHitBox.x;
    float overlapRight = (otherHitBox.x + otherHitBox.width) - playerHitBox.x;
    float overlapTop = (playerHitBox.y + playerHitBox.height) - otherHitBox.y;
    float overlapBottom = (otherHitBox.y + otherHitBox.height) - playerHitBox.y;
      
    float minOverlap = std::min({ overlapTop, overlapBottom, overlapLeft, overlapRight });
    if (minOverlap == overlapBottom) {
        die(); 
		this->changeState(&GoombaStompedState::GetInstance());
    }
}

void Goomba::handleEnvironmentCollision(std::shared_ptr<Object> other) {
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
        velocity.x *= -1;
    }
    else if (minOverlap == overlapRight && overlapRight >= MIN_OVERLAP) {
        position.x = otherHitBox.x + otherHitBox.width;
        velocity.x *= -1;
    }
}
void Goomba::die()
{
}
void Goomba::takeDamage(int amount) {
 
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
EnemyType Goomba::getType() const {
    return EnemyType::GOOMBA;
}
ObjectType Goomba::getObjectType() const {
    return EnemyType::GOOMBA;
}