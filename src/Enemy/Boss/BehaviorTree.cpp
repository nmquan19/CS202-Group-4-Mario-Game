// BehaviorTreeNodes.cpp
#include "../../../include/Enemy/Boss/BehaviorTree.h"
#include "../../../include/Enemy/Enemy.h"
#include <memory>
#include <initializer_list>
#include <raylib.h>
#include <iostream>
#include "../../../include/Enemy/Boss/DryBowser/DryBowser.h"
// SequenceNode Implementation
SequenceNode::SequenceNode(std::initializer_list<std::shared_ptr<BehaviorTreeNode>> nodes)
    : children(nodes) {
}
void SequenceNode::reset() {
    current = 0;
    for (auto& child : children) {
        child->reset();
    }
}
void SequenceNode::addChild(std::shared_ptr<BehaviorTreeNode> child) {
    children.push_back(child);
}
NodeStatus SequenceNode::tick(Enemy* boss, float dt) {
    while (current < children.size()) {
        NodeStatus status = children[current]->tick(boss, dt);
        if (status == NodeStatus::Running)
        {
            return NodeStatus::Running;
        }
        if (status == NodeStatus::Failure) {
            current = 0;
            return NodeStatus::Failure;
        }
        ++current;
    }
    current = 0;
    reset();
    return NodeStatus::Success;
}
//ReactiveSequenceNode Implementation 
ReactiveSequenceNode::ReactiveSequenceNode(std::initializer_list<std::shared_ptr<BehaviorTreeNode>> nodes)
: children(nodes) {
}
void ReactiveSequenceNode::reset() {
    current = 0;
    for (auto& child : children) {
        child->reset();
    }
}
void ReactiveSequenceNode::addChild(std::shared_ptr<BehaviorTreeNode> child) {
    children.push_back(child);
}
NodeStatus ReactiveSequenceNode::tick(Enemy* boss, float dt) {
    for (auto& child : children) {
        NodeStatus status = child->tick(boss, dt);
        if (status != NodeStatus::Success) {
            return status; 
        }
    }
    return NodeStatus::Success;
}

// SelectorNode Implementation
SelectorNode::SelectorNode(std::initializer_list<std::shared_ptr<BehaviorTreeNode>> nodes)
    : children(nodes) {
}

NodeStatus SelectorNode::tick(Enemy* boss, float dt) {
    while (current < children.size()) {
        NodeStatus status = children[current]->tick(boss, dt);
        if (status == NodeStatus::Running)
            return NodeStatus::Running;
        if (status == NodeStatus::Success) {
            current = 0;
            return NodeStatus::Success;
        }
        ++current;
    }
    current = 0;
    reset(); 
    return NodeStatus::Failure;
}
void SelectorNode::addChild(std::shared_ptr<BehaviorTreeNode> child) {
    children.push_back(child);
}
void SelectorNode::reset() {
    current = 0;
    for (auto& child : children) {
        child->reset();
    }
}
// ActionNode Implementation

//ReactiveFallBackNode Implementation
ReactiveFallBackNode::ReactiveFallBackNode(std::initializer_list<std::shared_ptr<BehaviorTreeNode>> nodes)
    : children(nodes) {
}

NodeStatus  ReactiveFallBackNode::tick(Enemy* boss, float dt) {
    for (int i = 0; i < children.size(); i++)
    {
		NodeStatus status = children[i]->tick(boss, dt);
        switch(status)
        {
            case NodeStatus::Running:
                return NodeStatus::Running; 
                break; 
            case NodeStatus::Success:
                return NodeStatus::Success;
            case NodeStatus::Failure:
                continue; 
            default:
                break;
		}   
    }
    reset();
    return NodeStatus::Failure;
}
void ReactiveFallBackNode::addChild(std::shared_ptr<BehaviorTreeNode> child) {
    children.push_back(child);
}
void ReactiveFallBackNode::reset() {
    for (auto& child : children) {
        child->reset();
    }
}
// UtilitySelector Implementation
#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include <stdexcept>


namespace UtilityScorers {

    using ScorerFunc = std::function<float(BaseSimState&)>;

    static std::unordered_map<std::string, ScorerFunc> registry;

    void registerScorer(const std::string& name, ScorerFunc func) {
        registry[name] = std::move(func);
    }

    ScorerFunc get(const std::string& name) {
        auto it = registry.find(name);
        if (it == registry.end()) {
            throw std::runtime_error("Scorer not found: " + name);
        }
        return it->second;
    }

    void registerDefaults() {
        registerScorer("scoreSpinAttack", [](BaseSimState& state) -> float {
            auto* dryBowser = dynamic_cast<DryBowserSimState*>(&state);
            if (!dryBowser) return 0.0f;
            return (dryBowser->IsPlayerInSpinAttackRange && dryBowser->currentMove == "none") ? 0.8f : 0.0f;
            });

        registerScorer("scoreBasicMeleeAttack", [](BaseSimState& state) -> float {
            auto* dryBowser = dynamic_cast<DryBowserSimState*>(&state);
            if (!dryBowser || !dryBowser->isPlayerInMeleeRange) return 0.0f;

            float distance = Vector2Distance(dryBowser->bossPos, dryBowser->playerPos);

            const float minRange = 0.0f;
            const float maxRange = dryBowser->moveRange;

            float t = std::clamp((maxRange - distance) / maxRange, 0.0f, 1.0f);

            float eased = t * t * (3 - 2 * t); // Smoothstep easing

            return eased;
            });

        registerScorer("scoreJump", [](BaseSimState& state) -> float {
            auto* dryBowser = dynamic_cast<DryBowserSimState*>(&state);
            if (!dryBowser) return 0.0f;

            // Score higher when grounded and player is far
            if (!dryBowser->isOnGround) return 0.0f;

            float distance = Vector2Distance(dryBowser->bossPos, dryBowser->playerPos);
            float maxDistance = 600.0f; // max interest distance
            float t = std::clamp(distance / maxDistance, 0.0f, 1.0f);
            float eased = t * t * (3 - 2 * t); // smoothstep

            return eased * 0.8f;
            });
        registerScorer("scoreWallJump", [](BaseSimState& state) -> float {
            auto* dryBowser = dynamic_cast<DryBowserSimState*>(&state);
            if (!dryBowser) return 0.0f;
                
            return (!dryBowser->isInAir && dryBowser->isNearWall && dryBowser->isPlayerAtHigherGround) ? 0.9f : 0.0f;
            });
        registerScorer("scoreIdle", [](BaseSimState& state) -> float {
            return 0.05f;
            });
    }
}


NodeStatus UtilitySelector::tick(Enemy* e, float dt) {
    if (children.empty()) return NodeStatus::Failure;
	Boss* boss = dynamic_cast<Boss*>(e); 
    std::unique_ptr<BaseSimState> simState = boss->getCurrentSimState();
    if (!simState) return NodeStatus::Failure;
    int bestIndex = -1;
    float bestScore = -1.0f;
    for (size_t i = 0; i < children.size(); ++i) {
        float score = scorers[i](*simState);
        if (score > bestScore) {
            bestScore = score;
            bestIndex = static_cast<int>(i);
        }
    }

    if (bestIndex >= 0) {
        return children[bestIndex]->tick(boss, dt);
    }

    return NodeStatus::Failure;
}



// DecoratorNode Implementation
void DecoratorNode::reset() {
    if (child) {
        child->reset();
    }
}
DecoratorNode::DecoratorNode(std::shared_ptr<BehaviorTreeNode> childNode)
    : child(childNode) {}
std::shared_ptr<BehaviorTreeNode> DecoratorNode::getChild() const {
    return child; 
}
void DecoratorNode::setChild(std::shared_ptr<BehaviorTreeNode> childNode) {
    child = childNode; 
}
NodeStatus InverterNode::tick(Enemy* boss, float dt)
{
    switch (child->tick(boss, dt))
    {
        case NodeStatus::Success : 
         return NodeStatus::Failure; 
        case NodeStatus::Failure : 
         return NodeStatus::Success; 
    }
    return NodeStatus::Running; 
}
NodeStatus ForceSuccessNode::tick(Enemy* boss, float dt)
{
    child->tick(boss, dt);
    return NodeStatus::Success; 
}
NodeStatus ForceFailureNode::tick(Enemy* boss, float dt)
{
    child->tick(boss, dt);
    return NodeStatus::Failure; 
}
NodeStatus RepeatNode::tick(Enemy* boss, float dt)
{
    if (num_attempt >= num_cycles) {
        return NodeStatus::Success;
    }

    NodeStatus status = child->tick(boss, dt);

    if (status == NodeStatus::Running) {
        return NodeStatus::Running;
    }
    else if (status == NodeStatus::Failure) {
        return NodeStatus::Failure; 
    }
    else if (status == NodeStatus::Success) {
        num_attempt++;
        if (num_attempt >= num_cycles) {
            return NodeStatus::Success;
        }
        else {
            child->reset();  
            return NodeStatus::Running;
        }
    }

    return NodeStatus::Failure; // fallback, shouldn't hit
}

void RepeatNode::reset() {
    num_attempt = 0;
    if (child) {
        child->reset();
    }
}
void RepeatUntilSuccessNode::reset() {
    num_attempt = 0;
    num_cycles = 0; 
    if (child) {
        child->reset();
    }
}
void RepeatUntilSuccessNode::setNumCycles(int cycles)  {
    num_cycles = cycles; 
}
int RepeatUntilSuccessNode::getNumCycles() const {
    return num_cycles; 
}
NodeStatus RepeatUntilSuccessNode::tick(Enemy* boss, float dt) 
{
    while (num_attempt >= num_cycles) {
        NodeStatus status = child->tick(boss, dt);
        if (status == NodeStatus::Running) {
            return NodeStatus::Running;
        }
        else if (status == NodeStatus::Success) {
            return NodeStatus::Failure;
        }
        else
        {
            num_attempt++;
        }

    }
    return NodeStatus::Success;
}
NodeStatus KeepRunningUntilFailureNode::tick(Enemy* boss, float dt) {
    if (!child) return NodeStatus::Failure;

    NodeStatus status = child->tick(boss, dt);

    if (status == NodeStatus::Failure) {
        return NodeStatus::Failure;
    }

    return NodeStatus::Running;
}
void RunOnceNode::reset() {
       hasRun = false;
       finalStatus = NodeStatus::Idle;
       if (child) child->reset();
    
}
NodeStatus RunOnceNode::tick(Enemy* boss, float dt){
    if (hasRun) {
        return thenSkip ? NodeStatus::Skipped : finalStatus;
    }
	if (!child) return NodeStatus::Failure;
    NodeStatus status = child->tick(boss, dt);
    if (status == NodeStatus::Success || status == NodeStatus::Failure) {
        finalStatus = status;
        hasRun = true;
    }

    return status;
}


// WalkToTargetNode.cpp
NodeStatus WalkToTargetNode::tick(Enemy* boss, float dt) {
    if (!boss) return NodeStatus::Failure;
    boss->walkToTarget();
    return NodeStatus::Success;
}
NodeStatus AttackNode::tick(Enemy* boss, float dt) {
    if (!boss) return NodeStatus::Failure;
    if( boss->isAttacking()) {
        return NodeStatus::Running;
	}
    if (boss->getCurAnimation() == "Attack" && boss->getAnimController().isFinished())
    {
        return NodeStatus::Success;
    }
    boss->attack();
    return NodeStatus::Running; 
}
NodeStatus IdleNode::tick(Enemy* boss, float dt) {
    boss->idle();
    return NodeStatus::Success;
}
NodeStatus WalkTurnNode::tick(Enemy* boss, float dt) {
    DrawText("IsWalkTurnNode", 100, 100, 20, RED);

    if (!boss) return NodeStatus::Failure;
    if (!started)
    {
        boss->walkTurn();
        started = true;
    }
    if(boss->isTurning())
    {
		return NodeStatus::Running;
    }
    return NodeStatus::Success;
}
void WalkTurnNode::reset() {
    started = false;
}
NodeStatus IsTargetInRangeNode::tick(Enemy* boss, float dt) {
    if (!boss) return NodeStatus::Failure;
    if (boss->isNearTarget()) {
        return NodeStatus::Success;
   }
    return NodeStatus::Failure;
}

#include <iostream>
NodeStatus NeedTurnNode::tick(Enemy* boss, float dt)
{
    if (!boss) return NodeStatus::Failure;

    float bossX = boss->getVelocity().x;
    float direction = boss->getDirection().x;
    bool isFacing = boss->FacingRight();
    if ((bossX>0 && !isFacing)||(bossX<0 && isFacing)){
        return NodeStatus::Success;
    }
    return NodeStatus::Failure;
}

NodeStatus CanUseSpinNode::tick(Enemy* boss, float dt) {
    DryBowser* dryBowser = dynamic_cast<DryBowser*>(boss);
    if (!dryBowser) return NodeStatus::Failure;
    if (dryBowser->canUseSpin()) {
        return NodeStatus::Success;
    }
    return NodeStatus::Failure;
}
NodeStatus SpinAttackNode::tick(Enemy* boss, float dt) {
    DrawText("IsSpinAttackNode", 100, 100, 20, RED);

    DryBowser* dryBowser = dynamic_cast<DryBowser*>(boss);
    if (!dryBowser) return NodeStatus::Failure;
    if (!started)
    {
        dryBowser->spinAttack();
		started = true;
    }
    if (dryBowser->isInSpinAttack()) {
        return NodeStatus::Running;
    }
    return NodeStatus::Success;
}
void SpinAttackNode::reset() {
    started = false;
}
NodeStatus CanUseMeleeAttack1Node::tick(Enemy* boss, float dt) {
    DryBowser* dryBowser = dynamic_cast<DryBowser*>(boss);
    if (!dryBowser) return NodeStatus::Failure;
    if (dryBowser->isNearTarget() && !dryBowser->isAttacking() && !dryBowser->isMoveOnCooldown("MeleeAttack1")){
        return NodeStatus::Success;
    }
    return NodeStatus::Failure;
}

NodeStatus SpinAttackWindupNode::tick(Enemy* boss, float dt) {
    DryBowser* dryBowser = dynamic_cast<DryBowser*>(boss);
    if (!boss) return NodeStatus::Failure;
    if (dryBowser->getCurAnimation() != "SpinAttackWindup")
    {
        dryBowser->setAnimation("SpinAttackWindup");
        return NodeStatus::Running;
    }
   if(dryBowser->getAnimController().isFinished()) {
        return NodeStatus::Success;
   }
   return NodeStatus::Running;
}

NodeStatus SpinAttackWinddownNode::tick(Enemy* boss, float dt) {
    DryBowser* dryBowser = dynamic_cast<DryBowser*>(boss);
    if (!boss) return NodeStatus::Failure;
    if (dryBowser->getCurAnimation() != "SpinAttackWinddown")
    {
        dryBowser->setAnimation("SpinAttackWinddown");
        return NodeStatus::Running;
    }
    if (dryBowser->getAnimController().isFinished()) {
        return NodeStatus::Success;
    }
    return NodeStatus::Running;
}

NodeStatus IsTakingDamageNode::tick(Enemy* boss, float dt) {
    if (!boss) return NodeStatus::Failure;
    DryBowser* dryBowser = dynamic_cast<DryBowser*>(boss);
    if(dryBowser&& dryBowser->getCurAnimation()=="TakeDamage" && !dryBowser->getAnimController().isFinished()) {
        return NodeStatus::Success;
	}
    return NodeStatus::Failure;
}

NodeStatus IsInIntroNode::tick(Enemy* boss, float dt) {
    DryBowser* dryBowser = dynamic_cast<DryBowser*>(boss);
    if (!dryBowser) return NodeStatus::Failure;
    if ((dryBowser->getCurAnimation() == "Intro")||((dryBowser->getCurAnimation() == "Taunt")&& !dryBowser->getAnimController().isFinished())){
        return NodeStatus::Running;
    }
	return NodeStatus::Success;
}



NodeStatus IsInAirNode::tick(Enemy* boss, float dt) {

    if (!boss->isOnGround()) {
        return NodeStatus::Success;
    }
    return NodeStatus::Failure;
}

NodeStatus IsFallingNode::tick(Enemy* boss, float dt) {

    if (!boss->isOnGround()&& boss->getVelocity().y> 0) {
        return NodeStatus::Success;
    }
    return NodeStatus::Failure;
}

NodeStatus IsPlayerHigherNode::tick(Enemy* boss, float dt) {
    if (!boss) return NodeStatus::Failure;
    if (boss->isPlayerAtHigherGround()) {
        return NodeStatus::Success;
    }
    return NodeStatus::Failure;
}
NodeStatus CanWallJumpNode::tick(Enemy* boss, float dt) {
    if (boss->canWallJump()) {
        return NodeStatus::Success;
    }
    return NodeStatus::Failure;
}
NodeStatus WallJumpNode::tick(Enemy* boss, float dt) {
    if (!boss) return NodeStatus::Failure;
   
    if (!boss) return NodeStatus::Failure;
   
    if (!boss->isInWallJump())
    {
        boss->wallJump(); 
        return NodeStatus::Running; 
    }
    if (boss->getAnimController().isFinished()){
        return NodeStatus::Success;
    }
    return NodeStatus::Running;
}

NodeStatus JumpNode::tick(Enemy* boss, float dt) {
    if (!boss) return NodeStatus::Failure;

    DryBowser* drybowser = dynamic_cast<DryBowser*>(boss);
    if (drybowser) {
        bool canReach = drybowser->canReachPlayerHeight();
        float distance = std::abs(drybowser->getPosition().x - drybowser->getTargetPos().x);
        bool inRange = distance <= Constants::DryBowser::AERIAL_ATTACK_RANGE;
        bool isOnCoolDown = drybowser->isMoveOnCooldown("AerialAttack");
        if (!canReach || !inRange || isOnCoolDown) {
            DrawText(TextFormat("JumpNode Fail: canReach=%d, inRange=%d", canReach, inRange), 400, 200, 20, BLUE);
            return NodeStatus::Failure;
        }
    }

    if (boss->isJumping()) {
        if (drybowser && drybowser->isOnGround()) {
            return NodeStatus::Running; 
        }
        if (boss->getAnimController().isFinished()) {
            return NodeStatus::Success;
        }
        return NodeStatus::Running;
    }
    boss->jump();
    return NodeStatus::Running;
}


NodeStatus AerialAttackNode::tick(Enemy* boss, float dt) {
    DryBowser* dryBowser = dynamic_cast<DryBowser*>(boss);
    if (!boss) return NodeStatus::Failure;

    if (dryBowser->getCurAnimation() != "AerialAttack") 
    {
        dryBowser->setGravity(0.0f);
        dryBowser->aerialAttack();
        return NodeStatus::Running;
    }
    if (dryBowser->getAnimController().isFinished()) {
        dryBowser->setGravity(1.0f);
        return NodeStatus::Success;

    }
    return NodeStatus::Running;
}

NodeStatus IsInWallJumpNode::tick(Enemy* boss, float dt) {
    DryBowser* dryBowser = dynamic_cast<DryBowser*>(boss);
    if (!boss) return NodeStatus::Failure;
    if (dryBowser->getIsWallSticking())
    {
        return NodeStatus::Success;
    }
    return NodeStatus::Failure;
}

NodeStatus CanReachPlayerHeightNode::tick(Enemy* boss, float dt)
{
    DryBowser* dryBowser = dynamic_cast<DryBowser*>(boss);
    if (dryBowser->getIsWallSticking())
    {
        return NodeStatus::Success;
    }
    return NodeStatus::Failure;
}
NodeStatus CanUseAerialAttackNode::tick(Enemy* boss, float dt) {
    DryBowser* dryBowser = dynamic_cast<DryBowser*>(boss);
    if (!dryBowser) return NodeStatus::Failure;
    if (dryBowser->canUseAerialAttack()) {
        return NodeStatus::Success;
    }
    return NodeStatus::Failure;
}
NodeStatus MoveToTargetNode::tick(Enemy* boss, float dt)
{
    if (!boss) return NodeStatus::Failure; 
    if (boss->moveToTarget())
    {
        return NodeStatus::Success;
    }
    return NodeStatus::Failure;
}

NodeStatus CanUseBasicAttackNode::tick(Enemy* boss, float dt) {
 
    if (boss->canUseBasicAttack()) {
        return NodeStatus::Success;
    }
    return NodeStatus::Failure;
}

NodeStatus RetreatNode::tick(Enemy* boss, float dt) {
    if (!boss) return NodeStatus::Failure;
    if (boss->isRetreating()) {
        return NodeStatus::Running;
    }
    if (boss->getCurAnimation() == "Retreat" && boss->getAnimController().isFinished())
    {
        return NodeStatus::Success;
    }
    boss->retreat();
    return NodeStatus::Running;
}


NodeStatus PatrolNode::tick(Enemy* boss, float dt) {
    if (!boss) return NodeStatus::Failure;
    if (boss->isPatrolling()) {
        return NodeStatus::Running;
    }
    boss->patrol();
    return NodeStatus::Running;
}


NodeStatus IsInChaseRangeNode::tick(Enemy* boss, float dt) {

    if (boss->isInChaseRange()) {
        return NodeStatus::Success;
    }
    return NodeStatus::Failure;
}