#pragma once
#include <string>
#include "../BossPhaseState.h"
#include "../GOAPPlanner.h"
#include "../BehaviorTree.h"
#include <memory>
#include "../Boss.h"
#include "../BossMoveState.h"

class DryBowser; 
class DryBowserPhase1BT : public BossPhaseState {
private: 
    std::shared_ptr<BehaviorTreeNode> root;
   std::shared_ptr<BossMoveState> currentState; 
public:
    DryBowserPhase1BT();
    void enter(Boss* boss);
    void exit(Boss* boss);
    void changeMoveState(Boss* boss, std::shared_ptr<BossMoveState> moveState);
    void update(Boss* boss, float dt) override;
    std::string getCurMove() const;
};
class DryBowserPhase2GOAP : public BossPhaseState {
    GOAPPlanner planner;
    std::shared_ptr<BossMoveState> currentState;
  /*  std::deque<std::shared_ptr<BossAction>> plan;
    */
public:
    void enter(Boss* boss) override;
    void exit(Boss* boss) override;
    void update(Boss* boss, float dt) override;
    void changeMoveState(Boss* boss, std::shared_ptr<BossMoveState> moveState);
    std::string getCurMove() const;
};