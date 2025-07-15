#include  "../../../include/Enemy/Koopa/Koopa.h"
#include "../../../include/System/TextureManager.h"
#include <raymath.h>
#include "../../../include/Enemy/Enemy.h"
#include "../../../include/Enemy/Koopa/KoopaState.h"
#include "../../../include/Objects/ObjectFactory.h"
#include "../../../include/System/Interface.h"
#include <raylib.h>
#include <vector>
#include <algorithm>
#include "../../../include/Enemy/LedgeDetector.h"
#include "../../../include/System/PhysicsManager.h"
#include <memory>
#include <utility>
Koopa::Koopa(Vector2 startPos, Vector2 velocity, Vector2 accelleration) : Enemy(startPos, velocity, accelleration, TextureManager::enemyTextures)
{
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

void Koopa::onCollision(Object* other) {

    if (other->getObjectCategory() == ObjectCategory::CHARACTER) {
        this->changeState(&KoopaStompedState::GetInstance());
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

void Koopa::checkCollision(const std::vector<Object*>& candidates)
{
    for (auto* candidate : candidates) {
        switch (candidate->getObjectCategory()) {
        case ObjectCategory::BLOCK:
            handleEnvironmentCollision(candidate);
            break;
        case ObjectCategory::PROJECTILE:
            // implement
            this->changeState(&KoopaKnockState::GetInstance());
            break;
        case ObjectCategory::CHARACTER:
            handleCharacterCollision(candidate);
            break;
        }
    }
}
void Koopa::handleCharacterCollision(Object* other) {
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
        this->changeState(&KoopaStompedState::GetInstance());
    }
}

void Koopa::handleEnvironmentCollision(Object* other) {
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
    const float MIN_OVERLAP = 0.1f;
    if (overlapTop < MIN_OVERLAP && overlapBottom < MIN_OVERLAP && overlapLeft < MIN_OVERLAP && overlapRight < MIN_OVERLAP) {
        return;
    }
  /*  const float MIN_OVERLAP = 2.0f;

    if (overlapTop < MIN_OVERLAP && overlapBottom < MIN_OVERLAP && overlapLeft < MIN_OVERLAP && overlapRight < MIN_OVERLAP) {
        return;
    }*/

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
		isFacingRight = false;
    }
    else if (minOverlap == overlapRight && overlapRight >= MIN_OVERLAP) {
        position.x = otherHitBox.x + otherHitBox.width;
        velocity.x *= -1;
		isFacingRight = true;
    }
}
void Koopa::die()   
{
}
void Koopa::takeDamage(int amount) {

}

void Koopa::update(float deltaTime) {

    if (!ledgeDetector->isNearLedge())
    {
        isFacingRight ^= 1;
        velocity.x *= -1;
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