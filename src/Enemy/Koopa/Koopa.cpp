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
#include "../../../include/System/PhysicsFactory.h"
#include <memory>
#include <utility>
Koopa::Koopa(Vector2 startPos, Vector2 velocity, Vector2 accelleration) : Enemy(startPos, velocity, accelleration, TextureManager::enemyTextures)
{
    stompedAnimation = false;
    isFacingRight = velocity.x > 0;

    ledgeDetector = std::make_shared<LedgeDetector>(1000.0f);
    // Create physics body for ledge detector using trigger body
    auto ledgeBodyInfo = PhysicsFactory::createTriggerBody(startPos, {1.0f, 10.0f});
    PhysicsManager::getInstance().addObject(ledgeDetector, ledgeBodyInfo);
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
    // Create physics body for ledge detector using trigger body
    auto ledgeBodyInfo = PhysicsFactory::createTriggerBody(startPos, {1.0f, 10.0f});
    PhysicsManager::getInstance().addObject(ledgeDetector, ledgeBodyInfo);
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
    // Physics system now handles collision detection through ContactListener
    // This method can be simplified or used for additional game logic
    for (auto candidate : candidates) {
        switch (candidate->getObjectCategory()) {
        case ObjectCategory::BLOCK:
            // Environment collision is handled by physics system
            break;
        case ObjectCategory::PROJECTILE:
            // Projectile collision is handled by physics system
            this->changeState(&KoopaKnockState::GetInstance());
            break;
        case ObjectCategory::CHARACTER:
            // Character collision is handled by physics system
            break;
        }
    }
}
void Koopa::handleCharacterCollision(std::shared_ptr<Object> other) {
    // Character collision is now handled by the ContactListener in the physics system
    // This method can be used for additional game logic if needed
    // The actual collision response (stomping, damage, etc.) is handled in ContactListener
}

void Koopa::handleEnvironmentCollision(std::shared_ptr<Object> other) {
    // Environment collision is now handled by the physics system
    // The physics system will automatically handle:
    // - Collision detection and response
    // - Position correction
    // - Velocity changes for bouncing off walls
    // - Ground detection
    
    // If you need custom behavior, you can check collision normals from the ContactListener
    // or add game-specific logic here that doesn't interfere with physics
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
        setVelocity(velocity); // Update physics body velocity
    }
    ledgeDetector->update(this, deltaTime);

    Enemy::update(deltaTime);
    
    // Gravity is now handled by the physics system, so we don't need to apply it manually
    // unless the object is stomped and should be static
    if (!stompedAnimation) {
        // Let physics handle gravity
    }
    
    if (currentState)
    {
        currentState->update(this, deltaTime);
        currentState->checkCondition(this);
    }
    
    // Position is now updated by the physics system in Enemy::update()
    // Boundary checking should ideally be handled by world boundaries in physics system
    // But we can keep this as a fallback
    if (position.x < 0)
    {
        position.x = 0;
        velocity.x *= -1;
        isFacingRight = true;
        setVelocity(velocity); // Update physics body
    }
    if (position.x > 1920 - hitbox.width)
    {
        position.x = 1920 - hitbox.width;
        velocity.x *= -1;
        isFacingRight = false;
        setVelocity(velocity); // Update physics body
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
