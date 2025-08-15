
#include  "../../../include/Enemy/Bob-ombs/Bob-ombs.h"
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
Bob_ombs::Bob_ombs(Vector2 startPos, Vector2 velocity, Vector2 accelleration) : Enemy(startPos, velocity, accelleration, TextureManager::enemyTextures)
{
    root = BTFactory::getInstance().createTree(EnemyType::BOB_OMBS);
    HP = 1;
    setAnimation("Patrol");
    direction = { 1, 0 };
    initialPos = startPos;
}
Bob_ombs::Bob_ombs(Vector2 startPos, Vector2 size) : Enemy(startPos, TextureManager::enemyTextures, size)
{
    HP = 1;
    root = BTFactory::getInstance().createTree(EnemyType::BOB_OMBS);
    setAnimation("Patrol");
    direction = { 1, 0 };

    initialPos = startPos;
}

void Bob_ombs::onCollision(std::shared_ptr<Object> other, Direction dir) {
    switch (other->getObjectCategory()) {
    case ObjectCategory::CHARACTER:
        if (dir == Direction::UP) {
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

void Bob_ombs::draw() {
    Rectangle source = spritebox;
    Rectangle dest = hitbox;
    Vector2 origin = { 0, 0 };
    DrawText(curAniName.c_str(), 100, 100, 20, RED);
    DrawCircle(targetPosition.x, targetPosition.y, 200, YELLOW);
    if (!isFacingRight)
    {
        source.width *= -1;
    }
    for (const auto& node : currentPath)
    {
        node->draw(YELLOW);
    }
    DrawTexturePro(this->texture, source, dest, origin, 0.0f, WHITE);
}

void Bob_ombs::handleEnvironmentCollision(std::shared_ptr<Object> other, Direction dir) {
    b2Vec2 currentVel = this->physicsBody->GetLinearVelocity();
    switch (dir) {
    case Direction::LEFT:
        isFacingRight = true;
        physicsBody->SetLinearVelocity(b2Vec2(abs(currentVel.x), currentVel.y));
        direction.x = (isFacingRight ? 1 : -1);
        break;
    case Direction::RIGHT:
        physicsBody->SetLinearVelocity(b2Vec2(-abs(currentVel.x), currentVel.y));
        direction.x = (isFacingRight ? 1 : -1);
        isFacingRight = false;
        break;
    }
}

void Bob_ombs::die()
{
    setActive(false);
    root = nullptr;

}
void Bob_ombs::patrol()
{

    setAnimation("Patrol");
    patrolTimer = 0;
}
float Bob_ombs::getWalkVelocity() const
{
    return Constants::Bob_ombs::chaseSpeed;
}
void Bob_ombs::takeDamage(int amount) {
    HP -= amount;
    if (HP <= 0) {
        die();
    }

}
void Bob_ombs::update(float dt) {

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
    spritebox = TextureManager::Enemy_sprite_boxes[curFrame];
    if (!((getVelocity().x > 0) ^ isFacingRight)) {
        flipDirection();
    }
    if (isAttacking())
    {
        attackTimer += dt; 
        moveToTarget();
        setAnimation("Attack");
    }
    if (attackTimer >= Constants::Bob_ombs::attackDuration || Vector2Distance(targetPosition,position) <= Constants
        ::Bob_ombs::bombRange)
    {
        setAnimation("Die");
        root = nullptr;
        attackTimer = 0; 
    }

    if (root) if (getVelocity().x == 0)
    {
        Vector2 dir = Vector2Normalize(Vector2Subtract(targetPosition, getPosition()));
        if (!((dir.x > 0) ^ isFacingRight))
        {
            flipDirection();
        }
    }
    {
        if (root) root->tick(this, dt);
    }
    if (isPatrolling())
    {

        velocity.x = direction.x * Constants::Bob_ombs::runSpeed;
    }
    if (curAniName == "Die" && animController.isFinished())
    {
        die();
        GameContext::getInstance().mark_for_deletion_Object(GameContext::getInstance().getSharedPtrFromRaw(this));
    }
    if (curAniName == "Die")
    {
        this->hitbox.width *= 1.04;
        this->hitbox.height *= 1.04;
        velocity = { 0,0 };
    }
    if (physicsBody) {
        physicsBody->SetLinearVelocity(Box2DWorldManager::raylibToB2(velocity));
    }


}

EnemyType Bob_ombs::getType() const {
    return EnemyType::BOB_OMBS;
}

ObjectType Bob_ombs::getObjectType() const {
    return EnemyType::BOB_OMBS;
}
NodeType Bob_ombs::getTraverseType()
{
    return NodeType::Ground;
}
bool Bob_ombs::isNearTarget() const
{
    return Vector2Distance(position, targetPosition) <= Constants::Bob_ombs::attackRange;
}
void Bob_ombs::setAnimation(const std::string& aniName)
{
    if (aniName == "Run") {
        animController.set(157, 158, 0.5f, Easing::linear, false, true);
    }
    else  if (aniName == "Patrol") {
        animController.set(157, 158, 0.5f, Easing::linear, false, true);
    }
    else if (aniName == "Jump") {
        animController.set(159, 159, 2.f, Easing::linear, false, false);
    }
    else if (aniName == "Attack") {
        animController.set(161, 162,0.1, Easing::linear, false, true);
    }
    else if (aniName == "Die") {
        animController.set(161, 162, Constants::Bob_ombs::dieDuration, Easing::linear, false, false);
    }
    curAniName = aniName;
}
void Bob_ombs::attack() {
    setAnimation("Attack");
}
bool Bob_ombs::isAttacking()
{
    return curAniName == "Attack" && !animController.isFinished();
}
void Bob_ombs::retreat() {
    setAnimation("Retreat");
}

bool Bob_ombs::isRetreating()
{
    return curAniName == "Retreat" && !animController.isFinished();
}
bool Bob_ombs::isPatrolling()
{
    return curAniName == "Patrol" && !animController.isFinished();
}
bool Bob_ombs::isInChaseRange()
{
    return Vector2Distance(targetPosition, position) <= Constants::Bob_ombs::chaseRange;
}
float Bob_ombs::getJumpVelocity() const 
{
    return Constants::Bob_ombs::jumpVelocity;
}
float Bob_ombs::jumpTo(Vector2 position, bool apply)
{
    if (apply)
    {
        setAnimation("Jump");
    }
    return Enemy::jumpTo(position, apply);
}