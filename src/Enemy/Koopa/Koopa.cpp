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

void Koopa::onCollision(std::shared_ptr<Object> other) {

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

void Koopa::checkCollision(const std::vector<std::shared_ptr<Object>>& candidates)
{
    for (auto candidate : candidates) {
        switch (candidate->getObjectCategory()) {
        case ObjectCategory::BLOCK:
            handleEnvironmentCollision(candidate);
            break;
        case ObjectCategory::PROJECTILE:
            // implement
            this->changeState(&KoopaKnockState::GetInstance());
            break;
        }
    }
}

void Koopa::handleEnvironmentCollision(std::shared_ptr<Object> other) {
      std::vector<Rectangle> koopaBoxes = getHitBox();
    std::vector<Rectangle> otherBoxes = other->getHitBox();
    
    if (koopaBoxes.empty() || otherBoxes.empty()) return;
    
    Rectangle koopaBox = koopaBoxes[0];
    Rectangle otherBox = otherBoxes[0];
    if (!CheckCollisionRecs(koopaBox, otherBox)) return;

    // Calculate previous position
    Vector2 prevPos = position - velocity;

    Rectangle prevBox = {
        prevPos.x,
        prevPos.y,
        koopaBox.width,
        koopaBox.height
    };

    bool wasAbove = (prevBox.y + prevBox.height) <= otherBox.y;
    bool wasBelow = prevBox.y >= (otherBox.y + otherBox.height);
    bool wasLeft = (prevBox.x + prevBox.width) <= otherBox.x;
    bool wasRight = prevBox.x >= (otherBox.x + otherBox.width);

    const float snapOffset = 0.01f;  // Prevent sinking into edges

    if (wasAbove && velocity.y >= 0) {
        // Landed on top
        position.y = otherBox.y - koopaBox.height - snapOffset;
        velocity.y = 0;
        onGround = true;
    }
    else if (wasBelow && velocity.y <= 0) {
        // Hit from below
        position.y = otherBox.y + otherBox.height + snapOffset;
        velocity.y = 0;
    }
    else if (wasLeft && velocity.x >= 0) {
        // Hit from left
        position.x = otherBox.x - koopaBox.width - snapOffset;
        velocity.x *= -1;
        isFacingRight = false;
    }
    else if (wasRight && velocity.x <= 0) {
        // Hit from right
        position.x = otherBox.x + otherBox.width + snapOffset;
        velocity.x *= -1;
        isFacingRight = true;
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
