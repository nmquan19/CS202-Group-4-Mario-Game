#pragma once
#include "../BossPhaseState.h"
#include "../GOAPPlanner.h"
#include "../BehaviorTree.h"
#include <memory>
#include <deque>
#include "../Boss.h"
#include "../BossMoveState.h"
#include <string>
#include <string>
class DryBowser;

class DryBowserPhase1BT : public BossPhaseState {
    std::shared_ptr<BehaviorTreeNode> root;
   std::shared_ptr<BossMoveState> currentState; 
public:
    DryBowserPhase1BT() {
        auto sequence = std::make_shared<SequenceNode>();

        auto walkNode = std::make_shared<WalkToTargetNode>();
        auto attackNode = std::make_shared<AttackNode>();

        sequence->addChild(walkNode);
        sequence->addChild(attackNode);

        root = sequence;
    }
     void enter(Boss* boss)
    {

    }
    
     void exit(Boss* boss)
     {

    }
     void changeMoveState(Boss* boss, std::shared_ptr<BossMoveState> moveState)
     {
         
         if(currentState) currentState->exit(boss);
         currentState = moveState; 
         if (currentState) currentState->enter(boss);
     }
    void update(Boss* boss, float dt) override {
        if(root)root->tick(boss, dt);
    }
    std::string getCurMove() const {
		return currentState ? currentState->getName() : "None"; 
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
    std::string getCurMove() const {
        return currentState ? currentState->getName() : "None";
    }
};