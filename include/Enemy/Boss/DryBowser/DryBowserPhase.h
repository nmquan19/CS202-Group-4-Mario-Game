#pragma once
#include "../BossPhaseState.h"
#include "../GOAPPlanner.h"
#include "../BehaviorTree.h"
#include <memory>
#include <deque>
#include "../Boss.h"
#include "../BossMoveState.h"
class DryBowser;

class DryBowserPhase1BT : public BossPhaseState {
    std::shared_ptr<BehaviorTreeNode> root;
   std::shared_ptr<BossMoveState> currentState; 
public:
    DryBowserPhase1BT():root(nullptr), currentState(nullptr) {
    }
     void enter(Boss* boss)
    {

    }
    
     void exit(Boss* boss)
     {

    }
     void changeMoveState(Boss* boss, std::shared_ptr<BossMoveState> moveState)
     {
         currentState->exit(boss);
         currentState = moveState; 
         currentState->enter(boss);
     }
    void update(Boss* boss, float dt) override {
        if(root)root->tick(boss, dt);
    }
};

class DryBowserPhase2GOAP : public BossPhaseState {
    GOAPPlanner planner;
    std::deque<std::shared_ptr<BossAction>> plan;
    std::shared_ptr<BossMoveState> currentState;
public:
    void enter(Boss* boss) override {
    }
    void exit(Boss* boss) override
    {

    }
    void update(Boss* boss, float dt) override {
    }
    void changeMoveState(Boss* boss, std::shared_ptr<BossMoveState> moveState)
    {
        currentState->exit(boss);
        currentState = moveState;
        currentState->enter(boss);
    }
};