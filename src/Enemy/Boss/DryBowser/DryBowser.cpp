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
    canReachPlayerHeight();
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
    targetHitboxes = GameContext::getInstance().getCharacter()->getHitBox()[0];
    hitbox.x = position.x;
    hitbox.y = position.y;
    
    animController.update(dt);
    curFrame = animController.getCurrentFrame();
    if ((this->currentPhase->getCurMove() == "Jump"&& velocity.y <0) || this->currentPhase->getCurMove() == "Fall"|| this->currentPhase->getCurMove()=="AerialAttack")
    {
        this->onGround = false; 
    }
    // Ground collision detection (for non-Box2D fallback)
    /*if (!physicsBody && getBottom() <= groundLevel) {
        DrawText(TextFormat("Drybowser Position: x = %f, y = %f, Ground Level z = %f", position.x, position.y, groundLevel), 200, 400, 20, BLACK);
        onGround = true;
        if (velocity.y > 0) {
            velocity.y = 0;
        }
    } */
    //groundLevel = INT_MIN;
    
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
    if (data&&lastTriggerFrame != curFrame) {
        for(auto& event : data->events) {
            if (event.eventType != EventType::None) {
                DrawText(TextFormat("Event Triggered:%d, %s", data->frameIndex, event.payload.c_str()), 200, 300, 20, RED);
                auto func = FrameEventHandlers::bind(event, this);
                if (func) {
                    func();
                }
            }
            lastTriggerFrame = curFrame; 
        }
    }
    updateCooldowns(dt);
    if (currentPhase) currentPhase->update(this, dt);

}

void DryBowser::draw() {

	//DrawRectangle(targetPosition.x, targetPosition.y, 50,50, RED);
    DrawCircleLines(hitbox.x + hitbox.width / 2, hitbox.y + hitbox.height / 2, Constants::DryBowser::AERIAL_ATTACK_RANGE, BLUE);
    float selfBottomY = position.y + hitbox.height;
    float targetBottomY = targetPosition.y+targetHitboxes.height+5.0f;

    DrawLine(0, selfBottomY, 1000, selfBottomY, RED);
    DrawText("DryBowser Bottom", 10, selfBottomY - 15, 20, RED);

    DrawLine(0, targetBottomY, 1000, targetBottomY, GREEN);
    DrawText("Target Bottom + 10", 10, targetBottomY - 15, 20, GREEN);
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
        animController.set(31, 41, 2.f, Easing::linear, false, true);
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
    else if (aniName == "Intro")
    {
		animController.set(643, 656, Constants::DryBowser::INTRO_DURATION, Easing::linear, false, false);
    }
    else if (aniName == "Jump")
    {
        animController.set(50, 54, fabs((5.0f / 3.0f)*Constants::DryBowser::JUMP_VELOCITY/Constants::GRAVITY), Easing::linear, false, false);
    }
    else if (aniName == "AerialAttack")
    {
        animController.set(94, 107, Constants::DryBowser::AERIAL_ATTACK_DURATION, Easing::easeDip, false, false);
    }
    else if (aniName == "WallSticking")
    {
        animController.set(67, 67, 0.0f, Easing::linear, false, true);
    }
    else if (aniName == "WallJump")
    {
        animController.set(67, 69, Constants::DryBowser::WALL_JUMP_DURATION, Easing::linear, false, false);
	}
    else if (aniName == "Fall")
    {
        animController.set(55, 57, 0.5f, Easing::linear, false, false);
    }
    else
    {
        std::cerr << "Unknown animation name: " << aniName << std::endl;
        return;
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
        return (abs(targetPosition.x - position.x) <= Constants::DryBowser::SPIN_ATTACK_RANGE) && !isPlayerAtHigherGround(); 
    }
    else if (type == BowserAttackType::FireBreath) {
        return false;
    }
    else if (type == BowserAttackType::JumpAttack) {
        return (abs(targetPosition.x - position.x) <= Constants::DryBowser::AERIAL_ATTACK_RANGE) && !isPlayerAtHigherGround();
        return false;
    }
    return false;
}
bool DryBowser::canUseSpin() const
{
    std::cout << (onGround && isMoveOnCooldown("SpinAttack") == false && currentPhase->getCurMove() != "SpinAttack" && isTargetInRange(BowserAttackType::SpinAttack)) << "\n";
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
void DryBowser::changeMoveState(std::shared_ptr<BossMoveState> moveState) {
    if (currentPhase) {
        currentPhase->changeMoveState(this, moveState);
    }
}
void DryBowser::aerialAttack()
{
    this->setCooldown("AerialAttack", Constants::DryBowser::AERIAL_ATTACK_COOLDOWN);
    this->setAnimation("AerialAttack");
 
}
bool DryBowser::canUseAerialAttack() const
{
   return !onGround && isMoveOnCooldown("AerialAttack") == false && currentPhase->getCurMove() != "AerialAttack";
}

int DryBowser::isNearWall() const {
    const float gravity = 980.0f;
    const float vy = Constants::DryBowser::JUMP_VELOCITY;
    const float vx = Constants::DryBowser::RUN_SPEED;
    const float dt = 0.05f; 

    Rectangle hitBox = getHitBox()[0];
    Vector2 start = { hitBox.x + hitBox.width / 2, hitBox.y + hitBox.height };

    auto checkDirection = [&](float direction) -> bool {
        float t_air = (2.0f * vy) / gravity;
        for (float t = 0.0f; t <= t_air; t += dt) {
            float x = start.x + direction * vx * t;
            float y = start.y - (vy * t - 0.5f * gravity * t * t);

            Rectangle probe = { x - 4, y - 4, 8, 8 }; 

            auto objs = PhysicsManager::getInstance().getObjectsInArea(probe);
            for (const auto& obj : objs) {
                if (obj->getObjectCategory() == ObjectCategory::BLOCK) {
                    return false;
                }
            }
        }
        return true; 
        };

    float maxJumpHeight = (vy * vy) / (2.0f * gravity);
    float t_air = (2.0f * vy) / gravity;
    float horizontalReach = vx * t_air;

    Rectangle leftProbe = {
        hitBox.x - horizontalReach,
        hitBox.y - maxJumpHeight,
        horizontalReach,
        maxJumpHeight
    };

    Rectangle rightProbe = {
        hitBox.x + hitBox.width,
        hitBox.y - maxJumpHeight,
        horizontalReach,
        maxJumpHeight
    };

    auto objsLeft = PhysicsManager::getInstance().getObjectsInArea(leftProbe);
    auto objsRight = PhysicsManager::getInstance().getObjectsInArea(rightProbe);

    auto isWall = [](const std::shared_ptr<Object>& obj) {
        return obj->getObjectCategory() == ObjectCategory::BLOCK;
        };

    if (std::any_of(objsLeft.begin(), objsLeft.end(), isWall)) {
        if (checkDirection(-1.0f)) return -1; // can reach left wall
    }

    if (std::any_of(objsRight.begin(), objsRight.end(), isWall)) {
        if (checkDirection(1.0f)) return 1; // can reach right wall
    }

    return 0; 
}

bool DryBowser::isBelowWall() const {
    const float gravity = 980.0f;
    const float initialJumpVelocity = Constants::DryBowser::JUMP_VELOCITY;
    const float horizontalTolerance = 30.0f; 

    Rectangle hitBox = getHitBox()[0];
    Vector2 start = {
        hitBox.x + hitBox.width / 2,
        hitBox.y + hitBox.height
    };

    float maxJumpHeight = (initialJumpVelocity * initialJumpVelocity) / (2.0f * gravity);
    float airTime = (2.0f * initialJumpVelocity) / gravity;

    const float dt = 0.05f;

    for (float t = 0.0f; t <= airTime; t += dt) {
        float y = start.y - (initialJumpVelocity * t - 0.5f * gravity * t * t);
        float x = start.x; 

        Rectangle probe = {
            x - horizontalTolerance / 2.0f,
            y - 4.0f,
            horizontalTolerance,
            8.0f
        };

        auto objs = PhysicsManager::getInstance().getObjectsInArea(probe);
        for (const auto& obj : objs) {
            if (obj->getObjectCategory() == ObjectCategory::BLOCK) {
                return true;
            }
        }
    }

    return false;   
}
bool DryBowser::checkWallContact() {
	bool hasTouchedWall = false; 
    Rectangle hitBox = getHitBox()[0];
    const float wallContactWidth = 2.0f;

    Rectangle leftProbe = {
        hitBox.x - wallContactWidth,
        hitBox.y,
        wallContactWidth,
        hitBox.height
    };

    Rectangle rightProbe = {
        hitBox.x + hitBox.width,
        hitBox.y,
        wallContactWidth,
        hitBox.height
    };

    auto objsLeft = PhysicsManager::getInstance().getObjectsInArea(leftProbe);
    auto objsRight = PhysicsManager::getInstance().getObjectsInArea(rightProbe);

    auto isWall = [](const std::shared_ptr<Object>& obj) {
        return obj->getObjectCategory() == ObjectCategory::BLOCK;
        };

    bool touchingLeftWall = std::any_of(objsLeft.begin(), objsLeft.end(), isWall);
    bool touchingRightWall = std::any_of(objsRight.begin(), objsRight.end(), isWall);

    hasTouchedWall = touchingLeftWall || touchingRightWall;
    return hasTouchedWall; 
}

void DryBowser::wallJump() {
    if (!isOnGround()|| canWallJump() == 0) return; 
	this->currentPhase->changeMoveState(this, std::make_shared<DryBowserWallJumpMoveState>());   
}
void DryBowser::jumpFromWall() {

	this->setAnimation("WallJump");
	this->setWallSticking(false);
}
void DryBowser::jump()
{
    this->currentPhase->changeMoveState(this, std::make_shared<DryBowserJumpingState>());
}
bool DryBowser::isJumping() const
{
    return (this->currentPhase->getCurMove()=="Jump");
}
void DryBowser::setGravity(float scale) {
    this->physicsBody->SetGravityScale(scale);
}
bool DryBowser::canReachPlayerHeight()
{
    float bowserY = this->getPosition().y; // DryBowser's current Y position
    float playerY = GameContext::getInstance().getCharacter()->getPosition().y; 
    float g = 980.f;
    float v = Constants::DryBowser::JUMP_VELOCITY;
    float maxJumpHeight = (v * v) / (2 * g);    
    DrawText(TextFormat("CanReach %d", (bowserY - maxJumpHeight) <= playerY), 300, 200, 20, RED);
    DrawLine(0, bowserY - maxJumpHeight, 1000, bowserY - maxJumpHeight, BLUE);
   return (bowserY - maxJumpHeight) <= playerY;
}
