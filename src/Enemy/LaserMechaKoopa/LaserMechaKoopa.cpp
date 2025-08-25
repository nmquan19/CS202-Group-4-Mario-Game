
#include  "../../../include/Enemy/LaserMechaKoopa/LaserMechaKoopa.h"
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
#include  "../../../include/Enemy/LaserMechaKoopa/LaserBeam.h"
#include <raymath.h>
#include <raylib.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>
LaserMechaKoopa::LaserMechaKoopa(Vector2 startPos, Vector2 velocity, Vector2 accelleration) : Enemy(startPos, velocity, accelleration, TextureManager::enemyTextures)
{
    root = BTFactory::getInstance().createTree(EnemyType::LASER_MECHA_KOOPA);

    HP = 1;
    setAnimation("Idle");
    if (physicsBody) physicsBody->SetGravityScale(1.0f);
    direction = { 1,0 };
    spritebox = TextureManager::getInstance().Enemy_sprite_boxes[379];

}
LaserMechaKoopa::LaserMechaKoopa(Vector2 startPos, Vector2 size) : Enemy(startPos, TextureManager::enemyTextures, size)
{
    root = BTFactory::getInstance().createTree(EnemyType::LASER_MECHA_KOOPA);

    HP = 1;
    setAnimation("Idle");
    if (physicsBody) physicsBody->SetGravityScale(1.0f);
   
    direction = { 1,0 };
    spritebox = TextureManager::getInstance().Enemy_sprite_boxes[379];

}

void LaserMechaKoopa::onCollision(std::shared_ptr<Object> other, Direction dir) {
    switch (other->getObjectCategory()) {
    case ObjectCategory::CHARACTER:
        if (dir == Direction::UP) {
            setAnimation("Die");
            die();
        }
        break;
    case ObjectCategory::ENEMY:
        break; 
    case ObjectCategory::INTERACTIVE:
        break;
    case ObjectCategory::BLOCK:
        handleEnvironmentCollision(other, dir);
        break;
    case ObjectCategory::PROJECTILE:
        setAnimation("Die");
        die();
        break;
    }
}

void LaserMechaKoopa::draw() {
    Rectangle source = spritebox;
    Rectangle dest = hitbox;
    Vector2 origin = { 0, 0 };
    DrawTexturePro(this->texture, source, dest, origin, 0.0f, WHITE);
}
bool LaserMechaKoopa::canUseBasicAttack() const
{
    return attackCoolDown == 0;
}
void LaserMechaKoopa::handleEnvironmentCollision(std::shared_ptr<Object> other, Direction dir) {
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

void LaserMechaKoopa::die()
{
    root = nullptr;
    setActive(false);
}
void LaserMechaKoopa::takeDamage(int amount) {
    HP -= amount;
    if (HP <= 0) {
        die();
    }

}
void LaserMechaKoopa::update(float dt) {
    Vector2 playerPos = std::dynamic_pointer_cast<Character>(GameContext::getInstance().getCharacter())->getCenterPos();
    setTarget(playerPos);
    if (physicsBody) {
        b2Vec2 b2Pos = physicsBody->GetPosition();
        Vector2 bodyPos = Box2DWorldManager::b2ToRaylib(b2Pos);
        position.x = bodyPos.x - hitbox.width * 0.5f;
        position.y = bodyPos.y - hitbox.height * 0.5f;

        b2Vec2 b2Vel = physicsBody->GetLinearVelocity();
        velocity.y = Box2DWorldManager::b2ToRaylib(b2Vel).y;
        velocity.x = 0;
    }
    hitbox.x = position.x;
    hitbox.y = position.y;
    animController.update(dt);
    curFrame = animController.getCurrentFrame();
    spritebox = TextureManager::Enemy_sprite_boxes[curFrame];
    if (root) root->tick(this, dt);
    if (curAniName == "Attack" && animController.isFinished())
    {
        attackCoolDown = Constants::LaserMechaKoopa::attackCoolDown;
        setAnimation("Idle");
    }
    if (curAniName == "Die" && animController.isFinished()) {
        die();
        GameContext::getInstance().mark_for_deletion_Object(
            GameContext::getInstance().getSharedPtrFromRaw(this)
        );
    }
    if (physicsBody) {
        physicsBody->SetLinearVelocity(Box2DWorldManager::raylibToB2(velocity));
    }
    attackCoolDown = fmax(attackCoolDown - dt, 0);
}

EnemyType LaserMechaKoopa::getType() const {
    return EnemyType::LASER_MECHA_KOOPA;
}

ObjectType LaserMechaKoopa::getObjectType() const {
    return EnemyType::LASER_MECHA_KOOPA;
}

void LaserMechaKoopa::setAnimation(const std::string& aniName)
{
    if (aniName == "Idle") {
        animController.set(379, 382, 0.3, Easing::linear, false, true);
    }
    else if (aniName == "Attack")
    {
        animController.set(392, 397, Constants::LaserMechaKoopa::attackDuration, Easing::linear, false, false);
    }
    else if (aniName == "Die") {
        animController.set(384, 387, Constants::LaserMechaKoopa::dieDuration, Easing::linear, false, false);
    }
    curAniName = aniName;
}
void LaserMechaKoopa::attack() {
    setAnimation("Attack");
    isChargingLaser = true;
    isFiringLaser = false;
    laserTimer = 0.0f;
    Vector2 centerPos = { position.x + hitbox.width / 2, position.y + hitbox.height / 2 };

    Vector2 dir = Vector2Normalize(targetPosition - centerPos);
 
    GameContext::getInstance().addObject(
        ProjectileType::LASER_BEAM,
        { getCenter().x + hitbox.width / 2,position.y + 3*hitbox.height/2}, // spawn at center
        { 1, 1 },      // size placeholder, LaserBeam can override
        [dir](std::shared_ptr<Object> obj) {
            auto laser = std::dynamic_pointer_cast<LaserBeam>(obj);
            if (laser) {
                laser->setDirection(dir);
                laser->setChargeTime(0.5*Constants::LaserMechaKoopa::attackDuration);
                laser->setBeamDuration(0.5 * Constants::LaserMechaKoopa::attackDuration);
                laser->setMaxLength(Constants::LaserMechaKoopa::attackRange);
            }
        }
    );
}


bool LaserMechaKoopa::isAttacking()
{
    return curAniName == "Attack" && !animController.isFinished();
}
bool LaserMechaKoopa::isNearTarget() const 
{
    return Vector2Distance(targetPosition, position) <= Constants::LaserMechaKoopa::attackRange;
}