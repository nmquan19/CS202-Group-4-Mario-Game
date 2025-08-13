#include  "../../../include/Enemy/Boo/Boo.h"
#include "../../../include/System/TextureManager.h"
#include "../../../include/Enemy/Enemy.h"
#include "../../../include/Objects/ObjectFactory.h"
#include "../../../include/System/Interface.h"
#include "../../../include/Characters/Character.h"
#include "../../../include/Enemy/Boss/BehaviorTree.h"
#include "../../../include/Enemy/Boss/BT_Factory.h"
#include "../../../include/Game/GameContext.h"
#include "../../../include/System/Box2DWorldManager.h"
#include "../../../include/System/CameraSystem.h"
#include <raymath.h>
#include <raylib.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>
Boo::Boo(Vector2 startPos, Vector2 velocity, Vector2 accelleration) : Enemy(startPos, velocity, accelleration, TextureManager::boo_texture)
{
    root = BTFactory::getInstance().createTree(EnemyType::BOO);
    HP = 1;
    physicsBody->SetGravityScale(0.0f);
    setAnimation("Flying");
    initialPos = startPos;
} 
Boo::Boo(Vector2 startPos, Vector2 size) : Enemy(startPos, TextureManager::boo_texture, size)
{
    HP = 1;
    root = BTFactory::getInstance().createTree(EnemyType::BOO);
    physicsBody->SetGravityScale(0.0f);
    setAnimation("Flying");
    initialPos = startPos;


}

void Boo::onCollision(std::shared_ptr<Object> other, Direction dir) {
    switch (other->getObjectCategory()) {
    case ObjectCategory::CHARACTER:
        if (dir == Direction::UP) {
            setAnimation("Die");
            die();
        }
    case ObjectCategory::ENEMY:
    case ObjectCategory::INTERACTIVE:
    case ObjectCategory::BLOCK:
        handleEnvironmentCollision(other, dir);
        break;
    case ObjectCategory::PROJECTILE:
        setAnimation("Die");
        die();
        break;
    }
}

void Boo::draw() {
    Rectangle source = spritebox;
    Rectangle dest = hitbox;
    Vector2 origin = { 0, 0 };

    DrawCircle(targetPosition.x, targetPosition.y, 20, RED);
    DrawCircleLines(position.x, position.y, Constants::Boo::chaseRange, BLUE);
    if (curAniName == "Flying" || curAniName == "Patrol")
    {
        source.width *= -1;
    }
    if (!isFacingRight)
    {
        source.width *= -1;
    }
    DrawTexturePro(this->texture, source, dest, origin, 0.0f, WHITE);
}

void Boo::handleEnvironmentCollision(std::shared_ptr<Object> other, Direction dir) {
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

void Boo::die()
{
    setActive(false);
    root = nullptr; 

}
void Boo::patrol()
{
   
    setAnimation("Patrol");
    patrolTimer = 0;
}
float Boo::getWalkVelocity() const
{
    return Constants::Boo::flySpeed;
}
void Boo::takeDamage(int amount) {
    HP -= amount;
    if (HP <= 0) {
        die();
}

    }
void Boo::update(float dt) {
    
    Vector2 playerPos = std::dynamic_pointer_cast<Character>(GameContext::getInstance().getCharacter())->getCenterPos();
    
    
    setTarget(playerPos);
    
    if (physicsBody) {
        // Get position and velocity from Box2D
        b2Vec2 b2Pos = physicsBody->GetPosition();
        Vector2 bodyPos = Box2DWorldManager::b2ToRaylib(b2Pos);
        position.x = bodyPos.x - hitbox.width * 0.5f;
        position.y = bodyPos.y - hitbox.height * 0.5f;

        b2Vec2 b2Vel = physicsBody->GetLinearVelocity();
        velocity = Box2DWorldManager::b2ToRaylib(b2Vel);
    }
    if (velocityController.isActiveAtFrame(curFrame)) {
        Vector2 multiplier = velocityController.getVelocityAtFrame(curFrame);
        Vector2 newVelocity = { direction.x * multiplier.x, direction.y * multiplier.y };
        if (physicsBody) {
            physicsBody->SetLinearVelocity(Box2DWorldManager::raylibToB2(newVelocity));
        }
    }
    targetHitboxes = GameContext::getInstance().getCharacter()->getHitBox()[0];
    hitbox.x = position.x;
    hitbox.y = position.y;
    animController.update(dt);
    curFrame = animController.getCurrentFrame();
    spritebox = TextureManager::boo_sprite_boxes[curFrame];
   
    if (!((getVelocity().x > 0)^ isFacingRight)){
        flipDirection();
       
    }
   
    if (root) if (getVelocity().x == 0)
    {
        Vector2 dir = Vector2Normalize(Vector2Subtract(targetPosition,getPosition()));
        if (!((dir.x > 0) ^ isFacingRight))
        {
            flipDirection();
        }
    }
    {
       if(root) root->tick(this, dt);
    }
    if (isRetreating())
    {
        float gravity = 1000.0f;
        float retreatRange = Constants::Boo::attackRange +50;   
        float retreatDuration = Constants::Boo::retreatDuration; 

        Vector2 awayDir = Vector2Normalize(getPosition() - targetPosition);

        velocity.x = awayDir.x * (retreatRange / retreatDuration);

        
        if (retreatTimer == 0.0f) {
            velocity.y = 0.0f; 
        }
        velocity.y += (-gravity) * dt;

        retreatTimer += dt;
        if (retreatTimer >= retreatDuration) {
            retreatTimer = 0.0f;
            velocity = { 0.0f, 0.0f };
        }
    }
    if (isPatrolling())
    {
        // increment timer
        float patrolPeriod = 2 * Constants::Boo::patrolDuration;

        const float TWO_PI = 2.0f * 3.14159265358979323846f;
        if (patrolTimer > patrolPeriod) patrolTimer = fmodf(patrolTimer, patrolPeriod);
        float omega = TWO_PI / patrolPeriod;
        if (std::fabs(position.x - initialPos.x) > Constants::Boo::flySpeed / omega &&
            targetPosition != initialPos && !isReturning)
        {
            Vector2 dir = Vector2Normalize(Vector2Subtract(initialPos, position));
            velocity = dir * Constants::Boo::flySpeed;
            isReturning = true;
        }
        else if (!isReturning)
        {
            float phi = 0.0f;
            float vx = Constants::Boo::flySpeed * cosf(omega * patrolTimer + phi);
            velocity.y = 0; 
            velocity.x = vx;
        }
        if (Vector2Distance(position, initialPos) < 5.0f)
        {
            isReturning = false;
        }

    }
    if (curAniName == "Die" && animController.isFinished())
    {
        GameContext::getInstance().mark_for_deletion_Object(GameContext::getInstance().getSharedPtrFromRaw(this));
    }
    if (physicsBody) {
        physicsBody->SetLinearVelocity(Box2DWorldManager::raylibToB2(velocity));
    }


}

EnemyType Boo::getType() const {
    return EnemyType::BOO;
}

ObjectType Boo::getObjectType() const {
    return EnemyType::BOO;
}
NodeType Boo::getTraverseType()
{
    return NodeType::Flying;
}
bool Boo::isNearTarget() const
{
    return Vector2Distance(position, targetPosition) <= Constants::Boo::attackRange;
}
void Boo::setAnimation(const std::string& aniName)
{
    if (aniName == "Flying") {
        animController.set(98, 102, 0.5f, Easing::linear, false, true);
    }
    else if (aniName == "WaitForAttack") {
        animController.set(4, 10, 2.f, Easing::linear, false,false);
    }
    else if (aniName == "Attack") {
        animController.set(121, 125, Constants::Boo::attackDuration, Easing::easeOut, false, false);
    }
    else if (aniName == "Retreat") {
        animController.set(98, 102, Constants::Boo::retreatDuration, Easing::linear, false, false);
    }
    else if (aniName == "Patrol") {
        animController.set(98, 102, Constants::Boo::patrolDuration, Easing::linear, false, true);
    }
    else if (aniName == "Die") {
        animController.set(166, 170, Constants::Boo::dieDuration, Easing::linear, false, false);
    }
    curAniName = aniName;
}
void Boo::attack() {
    if(curAniName != "WaitForAttack") 
    {
        velocity = { 0,0 };
        setAnimation("WaitForAttack");
    }
    if (curAniName == "WaitForAttack" && !animController.isFinished())
    {
        return; 
    }
    setAnimation("Attack");
    Vector2 dir = Vector2Normalize(targetPosition - getPosition()); 
    float travelDistance = Constants::Boo::attackRange;

    travelDistance += 50.0f; 

    velocity = dir * (travelDistance / Constants::Boo::attackDuration);

}
bool Boo::isAttacking()
{

    return curAniName == "Attack" && !animController.isFinished();
}
void Boo::retreat() {
    setAnimation("Retreat");    
    retreatTimer = 0;
}

bool Boo::isRetreating()
{
    return curAniName == "Retreat" && !animController.isFinished();
}
bool Boo::isPatrolling()
{
    return curAniName == "Patrol" && !animController.isFinished();
}
bool Boo::isInChaseRange()
{
    return Vector2Distance(targetPosition, position) <= Constants::Boo::chaseRange;
}