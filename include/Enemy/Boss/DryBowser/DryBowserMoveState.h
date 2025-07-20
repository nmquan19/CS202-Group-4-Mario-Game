#pragma once
#include "../BossMoveState.h"
#include "../../../System/Constant.h"
#include <string>
#include "../Boss.h"
#include <memory>
#include <vector>
class DryBowserWallJumpMoveState : public BossMoveState {
private:
    float timer = 0.0f;
	float friction = Constants::DryBowser::WallJumpFriction; 
public:
    void enter(Boss* boss) override;
    void update(Boss* boss, float dt) override;
    void exit(Boss* boss) override;
    bool isFinished() const override;
    bool canBeInterrupted() override ;
    std::string getName() const;
};
class DryBowserStandingState : public BossMoveState {
    private:
  public:
    void enter(Boss* boss) override;
    void update(Boss* boss, float dt) override;
    void exit(Boss* boss) override;
    bool isFinished() const override;
    bool canBeInterrupted();
    std::string getName() const;
};
class DryBowserRunningState : public BossMoveState{
public:
    void enter(Boss* boss) override;
    void update(Boss* boss, float dt) override;
    void exit(Boss* boss) override;
    bool isFinished() const override;
    std::vector<std::string> getAllowedTransitions() const;
    bool canBeInterrupted();
    bool canTransitionTo(const std::string& nextStateName);
    std::string getName() const;
    std::shared_ptr<BossMoveState> getForcedTransition(Boss* boss);
};
class DryBowserWalkTurnState : public BossMoveState
{   
    private:         
       float timer = 0.0f;
       float turnDuration = Constants::DryBowser::WalkTurnDuration; 
    public:
        void enter(Boss* boss) override;
        void update(Boss* boss, float dt) override;
        void exit(Boss* boss) override;
        bool isFinished() const override;
        std::vector<std::string> getAllowedTransitions() const;
        bool canBeInterrupted();
        bool canTransitionTo(const std::string& nextStateName);
        std::string getName() const;
        std::shared_ptr<BossMoveState> getForcedTransition(Boss* boss);
};
class DryBowserJumpingState : public BossMoveState {
   private: 
	float jumpVeclocity = Constants::DryBowser::JUMP_VELOCITY;   
  public:
    void enter(Boss* boss) override;
    void update(Boss* boss, float dt) override;
    void exit(Boss* boss) override;
    bool isFinished() const override;
    std::vector<std::string> getAllowedTransitions() const;
    bool canBeInterrupted();
    bool canTransitionTo(const std::string& nextStateName);
    std::string getName() const;
    std::shared_ptr<BossMoveState> getForcedTransition(Boss* boss);
};
class DryBowserMeleeAttack1State : public BossMoveState {
private:
    float attackDuration = 0.5f; 
	float timer = 0.0f; 
	float attackRange = Constants::DryBowser::BASIC_ATTACK_RANGE;
  public:
    void enter(Boss* boss) override;
    void update(Boss* boss, float dt) override;
    void exit(Boss* boss) override;
    bool isFinished() const override;
    std::vector<std::string> getAllowedTransitions() const;
    bool canBeInterrupted();
    bool canTransitionTo(const std::string& nextStateName);
    std::string getName() const;
    std::shared_ptr<BossMoveState> getForcedTransition(Boss* boss);
};
//class DryBowserTakeDamageState : public BossMoveState {
//private:
//    float takeDamageDuration = 0.1f;
//    float timer = 0.0f;
//public:
//    void enter(Boss* boss) override;
//    void update(Boss* boss, float dt) override;
//    void exit(Boss* boss) override;
//    bool isFinished() const override;
//    std::vector<std::string> getAllowedTransitions() const;
//    bool canBeInterrupted();
//    bool canTransitionTo(const std::string& nextStateName);
//    std::string getName() const;
//    std::shared_ptr<BossMoveState> getForcedTransition(Boss* boss);
//};
