#include "../../../../include/Enemy/Boss/DryBowser/DryBowser.h"
#include <memory>
#include <vector>
#include "../../../../include/Enemy/Boss/BossPhaseState.h"
#include "../../../../include/System/TextureManager.h"
#include <raylib.h>
#include "../../../../include/Enemy/Boss/DryBowser/DryBowserSimState.h"

#include "../../../../include/Enemy/Boss/Boss.h"
#include "../../include/Enemy/Boss/DryBowser/DryBowser.h"
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
    setAnimation("BasicAttack");
    currentPhase = std::make_unique<DryBowserPhase1BT>();
    currentPhase->enter(this);
}

void DryBowser::update(float dt) {
    Enemy::update(dt); 
    updateWorldState();
	hitbox.x = position.x;
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        targetPosition = { mousePos.x, mousePos.y };
    }
    else if (IsKeyPressed(KEY_SPACE)) {
        targetPosition = { 800, 100 }; 
	}
    hitbox.y = position.y; 
	animController.update(dt);  
    curFrame = animController.getCurrentFrame();
    if (velocityController.isActiveAtFrame(curFrame))
    {
		Vector2 mulplier = velocityController.getVelocityAtFrame(curFrame);
        velocity = { velocity.x * mulplier.x,velocity.y * mulplier.y };
    }
    spritebox = TextureManager::DryBowser_sprite_boxes[curFrame] ; 
    const FrameData* data = FrameDatabase::getInstance().getFrameData(this->getType(), curAniName, curFrame);
    if (data)
    {
        for(auto& event : data->events) {
            if (event.eventType != EventType::None) {
                auto func = FrameEventHandlers::bind(event, this);
                if (func)
                {
                    func();
                }
            }
		}       
    }
    if (currentPhase) currentPhase->update(this, dt);
}

void DryBowser::draw() {

    Enemy::draw(); 
	DrawText(TextFormat("NearTarget: %s", isNearTarget() ? "Yes" : "No"), 300, 300, 20, RED);
	DrawRectangle(targetPosition.x, targetPosition.y, 50,50, RED);
}

void DryBowser::handleCharacterCollision(std::shared_ptr<Object> other) {
}

void DryBowser::handleEnvironmentCollision(std::shared_ptr<Object> other) {


}

void DryBowser::checkCollision(const std::vector<std::shared_ptr<Object>>& candidates) {

    for (auto obj : candidates) {
    }
}

void DryBowser::onCollision(std::shared_ptr<Object> other) {

}

std::vector<Rectangle> DryBowser::getHitBox() const {
    std::vector<Rectangle> hitboxes;
    
    hitboxes.push_back(hitbox);
    
    Rectangle headBox = {
        position.x + hitbox.width * 0.55f,
        position.y + hitbox.height * 0.2f,
        hitbox.width * 0.3f,
        hitbox.height * 0.6f
    };
    hitboxes.push_back(headBox);

    return hitboxes;
}

void DryBowser::takeDamage(int amount) {
    HP -= amount;
    simState.bossHP = HP;
    //currentPhase->changeMoveState(this, std::make_shared<DryBowserTakeDamageState>());
    if (HP <= 50 && dynamic_cast<DryBowserPhase1BT*>(currentPhase.get())) {
        changePhase(std::make_unique<DryBowserPhase2GOAP>());
    }
    if (HP <= 0) {
        die();
    }
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
        animController.set(73, 86, Constants::DryBowser::BASIC_ATTACK_DURATION, Easing::easeOut,false,false);
    }
    else if (aniName == "Run")
    {
        animController.set(32, 40, 2.f, Easing::linear, false, true);
    }
    curAniName = aniName;
}


void DryBowser::walkToTarget() {

    
    if (currentPhase)
    {
        if(currentPhase->getCurMove() !="Run")
           currentPhase->changeMoveState(this, std::make_shared<DryBowserRunningState>());
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
	DrawText(TextFormat("Ended: %s", animController.isFinished() ? "Yes" : "No"), 200, 300, 20, RED);
    return  currentPhase->getCurMove() == "MeleeAttack1" && !animController.isFinished();
}
bool DryBowser::isNearTarget() const
{
	return abs(targetPosition.x - position.x) < 100 && abs(targetPosition.y - position.y)< 100;
}
void DryBowser::idle() {
    if (currentPhase)
    {
        if (currentPhase->getCurMove() != "Idle")
            currentPhase->changeMoveState(this, std::make_shared<DryBowserStandingState>());
    }
}