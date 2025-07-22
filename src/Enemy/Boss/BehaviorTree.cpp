// BehaviorTreeNodes.cpp
#include "../../../include/Enemy/Boss/BehaviorTree.h"
#include "../../../include/Enemy/Enemy.h"
#include <memory>
#include <initializer_list>
#include <raylib.h>
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
            DrawText("Running", 200, 400, 20, RED);
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
         return NodeStatus::Running; 
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
    while (num_attempt < num_cycles) {
        NodeStatus status = child->tick(boss, dt);
        if (status == NodeStatus::Running) {
            return NodeStatus::Running;
        }
        else if(status == NodeStatus::Failure) {
            return NodeStatus::Failure;
        }else
        {
            num_attempt++; 
        }
        
    }
    return NodeStatus::Success; 
}
void RepeatNode::reset() {
    num_attempt = 0;
    num_cycles = 0; 
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
	//if (!child) return NodeStatus::Failure;
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

    if (boss->isNearTarget()) {
        return NodeStatus::Success;
    }
    boss->walkToTarget();
    return NodeStatus::Running;
}
NodeStatus AttackNode::tick(Enemy* boss, float dt) {
    if (!boss) return NodeStatus::Failure;
    DrawText("Attacking", 200, 200, 20, RED);
    boss->attack();
    if( boss->isAttacking()) {
        return NodeStatus::Running;
	}
    return NodeStatus::Success;
}
NodeStatus IdleNode::tick(Enemy* boss, float dt) {
    DrawText("Idling", 200, 200, 20, RED);
    boss->idle();
    return NodeStatus::Success;
}