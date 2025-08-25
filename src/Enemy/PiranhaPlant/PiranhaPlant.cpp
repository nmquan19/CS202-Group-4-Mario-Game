
#include  "../../../include/Enemy/PiranhaPlant/PiranhaPlant.h"
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
PiranhaPlant::PiranhaPlant(Vector2 startPos, Vector2 velocity, Vector2 accelleration) : Enemy(startPos, velocity, accelleration, TextureManager::enemyTextures)
{
    HP = 1;
    setAnimation("Attack");
    direction = { 1, 0 };
    initialPos = startPos;
    if (physicsBody) physicsBody->SetGravityScale(0.0f);
    if (physicsBody) {
        physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
        for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            b2Filter filter = fixture->GetFilterData();
            filter.maskBits = static_cast<uint16>(ObjectCategory::ENEMY);
            filter.categoryBits = static_cast<uint16> (ObjectCategory::CHARACTER) |
                static_cast<uint16>(ObjectCategory::PROJECTILE);
            fixture->SetFilterData(filter);
        }
    }
    spritebox = TextureManager::getInstance().Enemy_sprite_boxes[121];
}
PiranhaPlant::PiranhaPlant(Vector2 startPos, Vector2 size) : Enemy(startPos, TextureManager::enemyTextures, size)
{
    HP = 1;
    setAnimation("Attack");
    direction = { 1, 0 };
    initialPos = startPos;
    if (physicsBody) physicsBody->SetGravityScale(0.0f);
    if (physicsBody) {
        physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
        for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            b2Filter filter = fixture->GetFilterData();
            filter.maskBits = static_cast<uint16>(ObjectCategory::ENEMY);
            filter.categoryBits = static_cast<uint16> (ObjectCategory::CHARACTER)|
                static_cast<uint16>(ObjectCategory::PROJECTILE);
            fixture->SetFilterData(filter);
        }
    }
    spritebox = TextureManager::getInstance().Enemy_sprite_boxes[121];
}

void PiranhaPlant::onCollision(std::shared_ptr<Object> other, Direction dir) {
    switch (other->getObjectCategory()) {
    case ObjectCategory::PROJECTILE:
        setAnimation("Die");
        die();
        break;
    }
}

void PiranhaPlant::draw() {
    Rectangle source = spritebox;
    Rectangle dest = hitbox;
    Vector2 origin = { 0, 0 };
    DrawTexturePro(this->texture, source, dest, origin, 0.0f, WHITE);
}

void PiranhaPlant::handleEnvironmentCollision(std::shared_ptr<Object> other, Direction dir) {
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

void PiranhaPlant::die()
{
    setActive(false);
    root = nullptr;

}
void PiranhaPlant::takeDamage(int amount) {
    HP -= amount;
    if (HP <= 0) {
        die();
    }

}
void PiranhaPlant::update(float dt) {
    float moveSpeed = Constants::PiranhaPlant::moveSpeed;  
    float moveDistance = Constants::TILE_SIZE; 
    float pauseTime = Constants::PiranhaPlant::pausetime;

    float moveTime = moveDistance / moveSpeed;               
    float cycleTime = moveTime * 2.0f + pauseTime * 2.0f;     
    patrolTimer += dt;
    if (patrolTimer > cycleTime) {
        patrolTimer = 0.0f;
    }
    if (patrolTimer < moveTime) {
        velocity.y = -moveSpeed;
    }
    else if (patrolTimer < moveTime + pauseTime) {
        velocity.y = 0.0f;
    }
    else if (patrolTimer < moveTime + pauseTime + moveTime) {
        velocity.y = moveSpeed;
    }
    else {
        velocity.y = 0.0f;
    }
    if (physicsBody) {
        physicsBody->SetLinearVelocity(Box2DWorldManager::raylibToB2(velocity));
        b2Vec2 b2Pos = physicsBody->GetPosition();
        Vector2 bodyPos = Box2DWorldManager::b2ToRaylib(b2Pos);
        position.x = bodyPos.x - hitbox.width * 0.5f;
        position.y = bodyPos.y - hitbox.height * 0.5f;
    }
    hitbox.x = position.x;
    hitbox.y = position.y;
    animController.update(dt);
    curFrame = animController.getCurrentFrame();
    spritebox = TextureManager::Enemy_sprite_boxes[curFrame];
    if (curAniName == "Die" && animController.isFinished()) {
        die();
        GameContext::getInstance().mark_for_deletion_Object(
            GameContext::getInstance().getSharedPtrFromRaw(this)
        );
    }
}

EnemyType PiranhaPlant::getType() const {
    return EnemyType::PIRANHA_PLANT;
}

ObjectType PiranhaPlant::getObjectType() const {
    return EnemyType::PIRANHA_PLANT;
}

void PiranhaPlant::setAnimation(const std::string& aniName)
{
    if (aniName == "Attack") {
        animController.set(121, 122, 0.3, Easing::linear, false, true);
    }
    else if (aniName == "Die") {
        animController.set(123, 126, Constants::PiranhaPlant::dieDuration, Easing::linear, false, false);
    }
    curAniName = aniName;
}
void PiranhaPlant::attack() {
    setAnimation("Attack");
}
bool PiranhaPlant::isAttacking()
{
    return curAniName == "Attack" && !animController.isFinished();
}
