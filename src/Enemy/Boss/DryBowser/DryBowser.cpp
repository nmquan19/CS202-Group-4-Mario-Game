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
#include "../../../../include/Enemy/EnemyAI/EnemyNavigator.h"
#include "../../../../include/System/LightingSystem.h"
#include "../../../../include/System/CameraSystem.h"
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
    auto character = std::dynamic_pointer_cast<Character>(GameContext::getInstance().getCharacter());
    if (character) {
        setTarget(character->getCenterPos());
    }
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
    if (velocityController.isActiveAtFrame(curFrame)) {
        Vector2 multiplier = velocityController.getVelocityAtFrame(curFrame);
        Vector2 newVelocity = { direction.x * multiplier.x, direction.y * multiplier.y };

        if (physicsBody) {
            physicsBody->SetLinearVelocity(Box2DWorldManager::raylibToB2(newVelocity));
        }
    }
    updateWorldState();
    targetHitboxes = GameContext::getInstance().getCharacter()->getHitBox()[0];
    hitbox.x = position.x;
    hitbox.y = position.y;
    animController.update(dt);
    curFrame = animController.getCurrentFrame();
    if ((this->currentPhase->getCurMove() == "Jump"&& velocity.y <0) || this->currentPhase->getCurMove() == "Fall"|| this->currentPhase->getCurMove()=="AerialAttack")
    {
        this->onGround = false; 
    }  

    spritebox = TextureManager::DryBowser_sprite_boxes[curFrame];
    

    updateCooldowns(dt);
    if (currentPhase) currentPhase->update(this, dt);
    if (curAniName == "Die" && animController.isFinished())
    {
        GameContext::getInstance().mark_for_deletion_Object(GameContext::getInstance().getSharedPtrFromRaw(this));
    }
    if (curAniName == "Die")
    {
        velocity = { 0,0 };
    }
    if (physicsBody) {
        physicsBody->SetLinearVelocity(Box2DWorldManager::raylibToB2(velocity));
    }
    const FrameData* data = FrameDatabase::getInstance().getFrameData(this->getType(), curAniName, curFrame);
    if (data && lastTriggerFrame != curFrame) {
        for (auto& event : data->events) {
            if (event.eventType != EventType::None) {
                auto func = FrameEventHandlers::bind(event, this);
                if (func) {
                    func();
                }
            }
            lastTriggerFrame = curFrame;
        }
    }
}

void DryBowser::draw() {

    float selfBottomY = position.y + hitbox.height;
    float targetBottomY = targetPosition.y+targetHitboxes.height+5.0f;
    
    Rectangle source = spritebox;
    Rectangle dest = hitbox;
    if (!isFacingRight)
    {
        source.width *= -1;
    }
    Vector2 origin = { 0, 0 };
    DrawTexturePro(this->texture, source, dest, origin, 0.0f, WHITE);
    drawHealthBar();
}
void DryBowser::drawHealthBar()
{
    // --- Dark Souls style health bar ---
    float screenWidth = (float)GetScreenWidth();
    float screenHeight = (float)GetScreenHeight();

    // Health bar size & position
    float barWidth = screenWidth * 0.6f;   // 60% of screen width
    float barHeight = 30.0f;               // Thickness of the bar
    float barX = (screenWidth - barWidth) / 2.0f;
    float barY = 0 + 50.0f;     // 50px above the bottom

    float healthPercent = (float)HP / (float)100;
    if (healthPercent < 0.0f) healthPercent = 0.0f;

    // Background (dark gray)
    DrawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);

    // Fill (red for boss HP)
    DrawRectangle(barX, barY, barWidth * healthPercent, barHeight, RED);

    DrawRectangleLines(barX, barY, barWidth, barHeight, BLACK);

    const char* bossName = "Dry Bowser";
    int fontSize = 20;
    int textWidth = MeasureText(bossName, fontSize);
    DrawText(bossName, (int)(screenWidth - textWidth) / 2, (int)(barY - fontSize - 5), fontSize, WHITE);
}
void DryBowser::handleCharacterCollision(std::shared_ptr<Object> other) {
    
    std::vector<Rectangle> bowserBoxes = getHitBox();
    std::vector<Rectangle> otherBoxes = other->getHitBox();
    if (bowserBoxes.empty() || otherBoxes.empty()) return;
    Rectangle otherBox = otherBoxes[0];

    Rectangle bowserHead = bowserBoxes[1];
    
    if (CheckCollisionRecs(bowserHead, otherBoxes[0]) && (getCurAnimation() != "TakeDamage") || (getCurAnimation() == "TakeDamage" && currentPhase->isMoveFinished())) {
        takeDamage(25);
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
        if (isInSpinAttack()) {
            DrawText("IN SPIN ATTACK COLL", 240, 200, 20, GREEN);

            direction = { -direction.x, direction.y };
            b2Vec2 b2Vel = physicsBody->GetLinearVelocity();
            b2Vel.x *= -1;
            physicsBody->SetLinearVelocity(b2Vel);
        }

        break;  
    case Direction::RIGHT:
        // Wall collision
        if (isInSpinAttack()) {
            DrawText("IN SPIN ATTACK COLL", 240, 200, 20, GREEN);
            direction = { -direction.x, direction.y };
            b2Vec2 b2Vel = physicsBody->GetLinearVelocity();
            b2Vel.x *= -1;
            physicsBody->SetLinearVelocity(b2Vel);
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
    if (HP <= 0)
    {
        die(); 
    }
    else
    {
        currentPhase->changeMoveState(this, std::make_shared<DryBowserTakeDamageState>());
    }
    /*if (HP <= 50 && dynamic_cast<DryBowserPhase1BT*>(currentPhase.get())) {
        changePhase(std::make_unique<DryBowserPhase2GOAP>());
    }
    if (HP <= 0) {
        die();
    }*/
}

void DryBowser::die() {
    alive = false;
    velocity = { 0,0 };
    setAnimation("Die");
}

bool DryBowser::isAlive() const {
    return alive;
}
float DryBowser::getWalkVelocity() const
{
    return Constants::DryBowser::RUN_SPEED;
}
float DryBowser::getJumpVelocity() const
{
    return Constants::DryBowser::JUMP_VELOCITY;
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
    else if (aniName == "Die")
    {
        animController.set(656,643, 3.f, Easing::linear, true, false);
    }
    else if (aniName == "Taunt")
    {
        animController.set(659, 669, 1.5f, Easing::linear, false, false);
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
bool DryBowser::moveToTarget()
{
    return Enemy::moveToTarget();
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
    //std::cout << (onGround && isMoveOnCooldown("SpinAttack") == false && currentPhase->getCurMove() != "SpinAttack" && isTargetInRange(BowserAttackType::SpinAttack)) << "\n";
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
void DryBowser::taunt()
{
    setAnimation("Taunt");
    DistortionEffect shockwave;
    shockwave.center = getCenter(); 
    shockwave.time = 1.5f;       
    shockwave.strength = 1.f;          
    shockwave.radius = 1.f;      
    shockwave.type = DistortionType::ShockWave;
    LightingManager::getInstance().addDistortion(std::make_shared<DistortionEffect>(shockwave));
    GameCameraSystem::getInstance().shakeCurrentCamera(50, 0.9f);
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
    //const float gravity = 980.0f;
    //const float vy = Constants::DryBowser::JUMP_VELOCITY;
    //const float vx = Constants::DryBowser::RUN_SPEED;
    //const float dt = 0.05f; 

    //Rectangle hitBox = getHitBox()[0];
    //Vector2 start = { hitBox.x + hitBox.width / 2, hitBox.y + hitBox.height };

    //auto checkDirection = [&](float direction) -> bool {
    //    float t_air = (2.0f * vy) / gravity;
    //    for (float t = 0.0f; t <= t_air; t += dt) {
    //        float x = start.x + direction * vx * t;
    //        float y = start.y - (vy * t - 0.5f * gravity * t * t);

    //        Rectangle probe = { x - 4, y - 4, 8, 8 }; 

    //        //uto objs = PhysicsManager::getInstance().getObjectsInArea(probe);
    //        for (const auto& obj : objs) {
    //            if (obj->getObjectCategory() == ObjectCategory::BLOCK) {
    //                return false;
    //            }
    //        }
    //    }
    //    return true; 
    //    };

    //float maxJumpHeight = (vy * vy) / (2.0f * gravity);
    //float t_air = (2.0f * vy) / gravity;
    //float horizontalReach = vx * t_air;

    //Rectangle leftProbe = {
    //    hitBox.x - horizontalReach,
    //    hitBox.y - maxJumpHeight,
    //    horizontalReach,
    //    maxJumpHeight
    //};

    //Rectangle rightProbe = {
    //    hitBox.x + hitBox.width,
    //    hitBox.y - maxJumpHeight,
    //    horizontalReach,
    //    maxJumpHeight
    //};

    ////auto objsLeft = PhysicsManager::getInstance().getObjectsInArea(leftProbe);
    ////auto objsRight = PhysicsManager::getInstance().getObjectsInArea(rightProbe);

    //auto isWall = [](const std::shared_ptr<Object>& obj) {
    //    return obj->getObjectCategory() == ObjectCategory::BLOCK;
    //    };

    //if (std::any_of(objsLeft.begin(), objsLeft.end(), isWall)) {
    //    if (checkDirection(-1.0f)) return -1; // can reach left wall
    //}

    //if (std::any_of(objsRight.begin(), objsRight.end(), isWall)) {
    //    if (checkDirection(1.0f)) return 1; // can reach right wall
    //}

    return 0; 
}

bool DryBowser::isBelowWall() const {
    //const float gravity = 980.0f;
    //const float initialJumpVelocity = Constants::DryBowser::JUMP_VELOCITY;
    //const float horizontalTolerance = 30.0f; 

    //Rectangle hitBox = getHitBox()[0];
    //Vector2 start = {
    //    hitBox.x + hitBox.width / 2,
    //    hitBox.y + hitBox.height
    //};

    //float maxJumpHeight = (initialJumpVelocity * initialJumpVelocity) / (2.0f * gravity);
    //float airTime = (2.0f * initialJumpVelocity) / gravity;

    //const float dt = 0.05f;

    //for (float t = 0.0f; t <= airTime; t += dt) {
    //    float y = start.y - (initialJumpVelocity * t - 0.5f * gravity * t * t);
    //    float x = start.x; 

    //    Rectangle probe = {
    //        x - horizontalTolerance / 2.0f,
    //        y - 4.0f,
    //        horizontalTolerance,
    //        8.0f
    //    };

    //    auto objs = PhysicsManager::getInstance().getObjectsInArea(probe);
    //    for (const auto& obj : objs) {
    //        if (obj->getObjectCategory() == ObjectCategory::BLOCK) {
    //            return true;
    //        }
    //    }
    //}

    return false;   
}
bool DryBowser::checkWallContact() {
	bool hasTouchedWall = false; 
    //Rectangle hitBox = getHitBox()[0];
    //const float wallContactWidth = 2.0f;

    //Rectangle leftProbe = {
    //    hitBox.x - wallContactWidth,
    //    hitBox.y,
    //    wallContactWidth,
    //    hitBox.height
    //};

    //Rectangle rightProbe = {
    //    hitBox.x + hitBox.width,
    //    hitBox.y,
    //    wallContactWidth,
    //    hitBox.height
    //};

    //auto objsLeft = PhysicsManager::getInstance().getObjectsInArea(leftProbe);
    //auto objsRight = PhysicsManager::getInstance().getObjectsInArea(rightProbe);

    //auto isWall = [](const std::shared_ptr<Object>& obj) {
    //    return obj->getObjectCategory() == ObjectCategory::BLOCK;
    //    };

    //bool touchingLeftWall = std::any_of(objsLeft.begin(), objsLeft.end(), isWall);
    //bool touchingRightWall = std::any_of(objsRight.begin(), objsRight.end(), isWall);

    //hasTouchedWall = touchingLeftWall || touchingRightWall;
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
float DryBowser::jumpTo(Vector2 targetPos, bool apply)
{
    if (velocity.y <0 && !isOnGround())
    {
        return _FMAX;
    }
    float t = Enemy::jumpTo(targetPos, apply);
    this->currentPhase->changeMoveState(this, std::make_shared<DryBowserJumpingState>());
    return t; 
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
