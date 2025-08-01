#pragma once
#include "../Boss.h"
#include "DryBowserSimState.h"
#include <memory>
#include "../BossPhaseState.h"
#include "../WorldState.h"
#include <vector>
#include "../SimulateState.h"
#include "../../../Objects/ObjectFactory.h"
#include "../../../System/Interface.h"
#include "../../../Enemy/Boss/DryBowser/DryBowserSimState.h"
#include <raylib.h>
#include <string>
enum class BowserAttackType {
    BasicAttack,
    SpinAttack,
    FireBreath,
    JumpAttack
};
class DryBowser : public Boss {
private:
    int HP;
    bool alive = true;
    WorldState currentWorld;
    DryBowserSimState simState;
    std::unique_ptr<BossPhaseState> currentPhase;
    float invulnerable_timer = 0.0f; 
    bool isWallSticking = false;  
  
public:
    DryBowser(Vector2 spawnPosition, Vector2 size = {2,2});

    // Game loop
    void update(float dt) override;
    void draw() override;

    // Collision & physics
    void handleCharacterCollision(std::shared_ptr<Object> other) override;
    void handleEnvironmentCollision(std::shared_ptr<Object> other) override;
    void checkCollision(const std::vector<std::shared_ptr<Object>>& candidates) override;
    void onCollision(std::shared_ptr<Object> other) override;
    std::vector<Rectangle> getHitBox() const override; // Override to provide multiple hitboxes
	std::string getCurAnimation() const { return curAniName; } 
    // Logic
    void takeDamage(int amount) override;
    void die() override;
    bool isAlive() const override;
	void setWallSticking(bool flag) { isWallSticking = flag; } 
    // Planning / AI interface
    void changePhase(std::unique_ptr<BossPhaseState> newPhase) override;
    const WorldState& getWorldState() const override;
    std::unique_ptr<BaseSimState> getCurrentSimState() const; 
    // Typing
    ObjectType getObjectType() const override;
    EnemyType getType() const override;
	void setAnimation(const std::string& animationName) override;
    void setTarget(Vector2 targetPos);
	void changeMoveState(std::shared_ptr<BossMoveState> moveState); 
    //
    
	///Behavior Tree implementation
    void walkToTarget();
    void attack() override;
    bool isAttacking(); 
	bool isNearTarget() const;
    void idle() override;
    bool isTurning() const override; 
    void walkTurn() override; 
    bool canUseSpin() const; 
    bool isTargetInRange(BowserAttackType type) const;
    void spinAttack(); 
    bool isInSpinAttack() const; 
	void spinAttackWindup();
    void spinAttackWinddown(); 
    void aerialAttack(); 
    void wallJump(); 
	int isNearWall() const;
    bool isBelowWall() const;  
    bool checkWallContact();
	void jumpFromWall() override;    
	bool getIsWallSticking() const { return isWallSticking; }
private:
    void updateWorldState();
};
