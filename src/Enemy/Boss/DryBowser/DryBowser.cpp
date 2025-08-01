#include "../../../../include/Enemy/Boss/DryBowser/DryBowser.h"
#include <memory>
#include <vector>
#include "../../../../include/Enemy/Boss/BossPhaseState.h"
#include "../../../../include/System/TextureManager.h"
#include <raylib.h>
#include "../../../../include/Enemy/Boss/DryBowser/DryBowserSimState.h"

#include "../../../../include/Enemy/Boss/Boss.h"
#include "../../../../include/Enemy/Boss/DryBowser/DryBowser.h"
#include "../../../../include/System/InterpolationController.h"
#include "../../../../include/Enemy/Boss/DryBowser/DryBowserPhase.h"
#include "../../../../include/Enemy/Boss/DryBowser/DryBowserMoveState.h"
#include "../../../../include/Enemy/Enemy.h"
#include <cstdlib>
#include <string>
#include <utility>
#include "../../../../include/Enemy/Boss/SimulateState.h"
#include "../../../../include/Enemy/Boss/WorldState.h"
#include "../../../../include/Objects/ObjectFactory.h"
#include "../../../../include/System/Interface.h"
#include "../../../../include/System/Constant.h"
#include "../../../../include/System/FrameData.h"
#include "../../../../include/Game/GameContext.h"
#include "../../../../include/System/Box2DWorldManager.h"
DryBowser::DryBowser(Vector2 spawnPosition, Vector2 size) :Boss(spawnPosition,size,TextureManager::DryBowser_texture){
    HP = 100;
    alive = true;
    simState.bossPos = spawnPosition;
    simState.bossVelocity = { 0, 0 };
    simState.playerPos = { 0, 0 };
    currentWorld = {
        {"IsOnGround", true},
        {"PlayerInRange", false},
        {"CanAttack", true},
        {"IsNearWall", false}
    };
    spritebox = TextureManager::DryBowser_sprite_boxes[2];
    //setAnimation("BasicAttack");
    currentPhase = std::make_unique<DryBowserPhase1BT>();
    currentPhase->enter(this);
  
    float headPos_x = isFacingRight ? position.x + hitbox.width * 0.6f : position.x + hitbox.width * 0.2f;
    Rectangle headBox = {
       headPos_x,
       position.y - hitbox.height * 0.2f,
       hitbox.width * 0.3f,
       hitbox.height * 0.4f
    };
	direction = { 1, 0 }; // Default direction to the right
    hitboxes.push_back(headBox);
    hitboxes.push_back(hitbox);
}
#include <iostream>
void DryBowser::update(float dt) {
    // First, sync with Box2D physics body if available
    if (physicsBody) {
        // Get position and velocity from Box2D
        b2Vec2 b2Pos = physicsBody->GetPosition();
        Vector2 bodyPos = Box2DWorldManager::b2ToRaylib(b2Pos);
        position.x = bodyPos.x - hitbox.width * 0.5f;
        position.y = bodyPos.y - hitbox.height * 0.5f;
        
        b2Vec2 b2Vel = physicsBody->GetLinearVelocity();
        velocity = Box2DWorldManager::b2ToRaylib(b2Vel);
    }
    
    // Handle screen boundaries
    if (position.x < 0) {
        position.x = 0;
        if (isInSpinAttack()) {
            direction = { -direction.x, direction.y };
            if (physicsBody) {
                physicsBody->SetLinearVelocity(Box2DWorldManager::raylibToB2({-velocity.x, velocity.y}));
            }
        }
    }
    if (position.x > 1920 - hitbox.width) {
        position.x = 1920 - hitbox.width;
        if (isInSpinAttack()) {
            direction = { -direction.x, direction.y };
            if (physicsBody) {
                physicsBody->SetLinearVelocity(Box2DWorldManager::raylibToB2({-velocity.x, velocity.y}));
            }
        }
    }
    
    updateWorldState();
    setTarget(GameContext::getInstance().getCharacter()->getPosition());
    hitbox.x = position.x;
    hitbox.y = position.y;
    
    animController.update(dt);
    curFrame = animController.getCurrentFrame();
    
    // Ground collision detection (for non-Box2D fallback)
    if (!physicsBody && getBottom() <= groundLevel) {
        DrawText(TextFormat("Drybowser Position: x = %f, y = %f, Ground Level z = %f", position.x, position.y, groundLevel), 200, 400, 20, BLACK);
        onGround = true;
        if (velocity.y > 0) {
            velocity.y = 0;
        }
    } 
    groundLevel = INT_MIN;
    
    // Apply frame-based velocity
    if (velocityController.isActiveAtFrame(curFrame)) {
        Vector2 multiplier = velocityController.getVelocityAtFrame(curFrame);
        Vector2 newVelocity = { direction.x * multiplier.x, direction.y * multiplier.y };
        
        if (physicsBody) {
            physicsBody->SetLinearVelocity(Box2DWorldManager::raylibToB2(newVelocity));
        }
    }
    
    DrawText(TextFormat("Drybowser Velocity: x = %f, y = %f", velocity.x, velocity.y), 200, 250, 20, BLACK);
    spritebox = TextureManager::DryBowser_sprite_boxes[curFrame];
    
    const FrameData* data = FrameDatabase::getInstance().getFrameData(this->getType(), curAniName, curFrame);
    if (data) {
        for(auto& event : data->events) {
            if (event.eventType != EventType::None) {
                DrawText(TextFormat("Event Triggered:%d, %s", data->frameIndex, event.payload.c_str()), 200, 300, 20, RED);
                auto func = FrameEventHandlers::bind(event, this);
                if (func) {
                    func();
                }
            }
        }
    }
    updateCooldowns(dt);
    if (currentPhase) currentPhase->update(this, dt);
}

void DryBowser::draw() {

	//DrawRectangle(targetPosition.x, targetPosition.y, 50,50, RED);
    Rectangle source = spritebox;
    Rectangle dest = hitbox;
    if (!isFacingRight)
    {
        source.width *= -1;
    }
    Vector2 origin = { 0, 0 };
	DrawText(TextFormat("DryBowser HP: %d", HP), 200, 300, 40, BLACK);
    DrawTexturePro(this->texture, source, dest, origin, 0.0f, WHITE);
}

void DryBowser::handleCharacterCollision(std::shared_ptr<Object> other) {
    
    std::vector<Rectangle> bowserBoxes = getHitBox();
    std::vector<Rectangle> otherBoxes = other->getHitBox();
    if (bowserBoxes.empty() || otherBoxes.empty()) return;
    Rectangle otherBox = otherBoxes[0];

    Rectangle bowserHead = bowserBoxes[1];
    
    if (CheckCollisionRecs(bowserHead, otherBoxes[0]) && (getCurAnimation() != "TakeDamage") || (getCurAnimation() != "TakeDamage" && currentPhase->isMoveFinished())) {
        takeDamage(25);
        currentPhase->changeMoveState(this, std::make_shared<DryBowserTakeDamageState>());    
        return;
    }
    Rectangle bowserBox = bowserBoxes[0];
    if (!CheckCollisionRecs(bowserBox, otherBox)) return;
    

}

void DryBowser::handleEnvironmentCollision(std::shared_ptr<Object> other, Direction dir) {
    // With Box2D, we should NOT manually adjust positions during collision callbacks
    // Box2D handles the physical collision response automatically
    // We only handle game logic here
    
    switch (dir) {
    case Direction::DOWN:
        // Ground collision - Box2D will stop the falling automatically
        onGround = true;
        if (other && !other->getHitBox().empty()) {
            groundLevel = other->getHitBox()[0].y;
        }
        // Don't manually set position or velocity - Box2D handles this
        break;
        
    case Direction::LEFT:
    case Direction::RIGHT:
        // Wall collision
        if (isInSpinAttack()) {
            direction = { -direction.x, direction.y };
            // Don't call setVelocity during collision - it causes the crash
            // The velocity change will be applied in the next update cycle
        }
        break;
        
    case Direction::UP:
        // Ceiling collision - Box2D handles stopping upward movement
        break;
    }
}

void DryBowser::onCollision(std::shared_ptr<Object> other, Direction dir) {
    switch (other->getObjectCategory()) {
        case ObjectCategory::BLOCK:
            handleEnvironmentCollision(other, dir);
            break;
        case ObjectCategory::PROJECTILE:
            // implement
            break;
        case ObjectCategory::CHARACTER:
            handleCharacterCollision(other);
            break;
        }
}

std::vector<Rectangle> DryBowser::getHitBox() const {
    std::vector<Rectangle> hitboxes;
    
    hitboxes.push_back(hitbox);
   

	float headPos_x =  isFacingRight ? position.x + hitbox.width * 0.6f : position.x + hitbox.width * 0.2f;
    Rectangle headBox = {
       headPos_x,
       position.y - hitbox.height * 0.2f,
       hitbox.width * 0.3f,
       hitbox.height * 0.4f
    };
    if (isInSpinAttack())
    {
        headBox = {};
    }
    
    hitboxes.push_back(headBox);

    return hitboxes;
}

void DryBowser::takeDamage(int amount) {
    HP -= amount;
    simState.bossHP = HP;
    //currentPhase->changeMoveState(this, std::make_shared<DryBowserTakeDamageState>());
    /*if (HP <= 50 && dynamic_cast<DryBowserPhase1BT*>(currentPhase.get())) {
        changePhase(std::make_unique<DryBowserPhase2GOAP>());
    }
    if (HP <= 0) {
        die();
    }*/
}

void DryBowser::die() {
    alive = false;
}

bool DryBowser::isAlive() const {
    return alive;
}

void DryBowser::changePhase(std::unique_ptr<BossPhaseState> newPhase) {
    if (currentPhase) currentPhase->exit(this);
    currentPhase = std::move(newPhase);
    currentPhase->enter(this);
}

const WorldState& DryBowser::getWorldState() const {
    return currentWorld;
}

std::unique_ptr<BaseSimState> DryBowser::getCurrentSimState() const {
    return std::make_unique<DryBowserSimState>(simState);
}

ObjectType DryBowser::getObjectType() const {
    return getType();
}

EnemyType DryBowser::getType() const {
    return EnemyType::DRY_BOWSER;
}

void DryBowser::updateWorldState() {
    currentWorld["IsOnGround"] = simState.isOnGround;
    currentWorld["IsNearWall"] = simState.isNearWall;
    currentWorld["CanAttack"] = simState.canAttack;

    float dx = std::abs(simState.bossPos.x - simState.playerPos.x);
    currentWorld["PlayerInRange"] = dx <= simState.moveRange;
}
void DryBowser::setAnimation(const std::string& aniName) {
    curFrame = 0;
    if (aniName == "Idle") {
        animController.set(2, 5, 0.5f,Easing::linear, false, true);
    }
    else if (aniName == "Walk") {
        animController.set(18, 25, 0.25f, Easing::linear, false, true);
    }
    else if (aniName == "MeleeAttack1") {
        animController.set(71, 83, Constants::DryBowser::BASIC_ATTACK_DURATION, Easing::easeOut,false,false);
    }
    else if (aniName == "Run")
    {
        animController.set(32, 41, 2.f, Easing::linear, false, true);
    }
    else if (aniName == "WalkTurn")
    {
        animController.set(29, 26, Constants::DryBowser::WalkTurnDuration, Easing::linear, true, false); 
    }
    else if (aniName == "SpinAttack")
    {
        animController.set(389, 396, Constants::DryBowser::SPIN_ATTACK_DURATION, Easing::easeInOut, false, false);
    }
    else if (aniName == "SpinAttackWindup")
    {
        animController.set(385, 388, Constants::DryBowser::SPIN_ATTACK_WINDUP_DURATION, Easing::linear, false, false);
    }
    else if (aniName == "SpinAttackWinddown")
    {
        animController.set(397, 399, Constants::DryBowser::SPIN_ATTACK_WINDDOWN_DURATION, Easing::linear, false, false);
    }
    else if (aniName == "TakeDamage")
    {
		animController.set(575, 580, Constants::DryBowser::TAKE_DAMAGE_DURATION, Easing::linear, false, false);
    }
    curAniName = aniName;
}


void DryBowser::walkToTarget() {

    if (currentPhase)
    {
		direction = { (targetPosition.x - position.x)>0? 1.f : -1, 1 };
        if (currentPhase->getCurMove() != "Run")
        {
            currentPhase->changeMoveState(this, std::make_shared<DryBowserRunningState>());
        }
    }
}
void DryBowser::attack() {
    if (currentPhase)
    {
        if (currentPhase->getCurMove() != "MeleeAttack1")
            currentPhase->changeMoveState(this, std::make_shared<DryBowserMeleeAttack1State>());
    }
}
bool DryBowser::isAttacking()
{
    return  currentPhase->getCurMove() == "MeleeAttack1" && !currentPhase->isMoveFinished();
}
bool DryBowser::isNearTarget() const
{
    Vector2 Nposition = { position.x + (isFacingRight? 1:-1)*hitbox.width, position.y + hitbox.height }; 
	float distance = Vector2Distance(Nposition, targetPosition);
        DrawText(TextFormat("Distance to Target: %f",Nposition.x - targetPosition.x), 100, 100, 20, RED);

	return distance  < Constants::DryBowser::BASIC_ATTACK_RANGE;
}
void DryBowser::idle() {
    if (currentPhase)
    {
        if (currentPhase->getCurMove() != "Idle")
            currentPhase->changeMoveState(this, std::make_shared<DryBowserStandingState>());
    }
}
bool DryBowser::isTurning() const
{
   
    return (currentPhase->getCurMove() == "WalkTurn" && !currentPhase->isMoveFinished());
}
void DryBowser::walkTurn()
{
    if (currentPhase)
    {
        if (!isTurning())
            currentPhase->changeMoveState(this, std::make_shared<DryBowserWalkTurnState>());
    }
}
bool DryBowser::isTargetInRange(BowserAttackType type) const
{
    if (type == BowserAttackType::BasicAttack) {
        return isNearTarget();
    }
    else if (type == BowserAttackType::SpinAttack) {
        return abs(targetPosition.x - position.x) <= Constants::DryBowser::SPIN_ATTACK_RANGE; 
    }
    else if (type == BowserAttackType::FireBreath) {
        return false;
    }
    else if (type == BowserAttackType::JumpAttack) {
        // Implement jump attack range check
        return false;
    }
    return false;
}
bool DryBowser::canUseSpin() const
{
	DrawText(TextFormat("Spin Attack Cooldown: %s", isMoveOnCooldown("SpinAttack") ? "On Cooldown" : "Ready"), 100, 150, 20, RED);
	DrawText(TextFormat("On Ground: %s", onGround ? "Yes" : "No"), 100, 200, 20, RED);
    return onGround && isMoveOnCooldown("SpinAttack") == false && currentPhase->getCurMove() != "SpinAttack" && isTargetInRange(BowserAttackType::SpinAttack); 
}

bool DryBowser::isInSpinAttack() const{
    return currentPhase->getCurMove() == "SpinAttack" && !currentPhase->isMoveFinished();
}
void DryBowser::spinAttack() {
    if (currentPhase)
    {
		if (currentPhase->getCurMove() != "SpinAttack" || (currentPhase->getCurMove() == "SpinAttack" && currentPhase->isMoveFinished()))
        {
            currentPhase->changeMoveState(this, std::make_shared<DryBowserSpinAttackState>());
        }
    }
}
void DryBowser::spinAttackWindup()
{
	setAnimation("SpinAttackWindup");
}
void DryBowser::spinAttackWinddown()
{
	setAnimation("SpinAttackWinddown");
}

void DryBowser::setTarget(Vector2 targetPos)
{
    this->targetPosition = targetPos; 
}