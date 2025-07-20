// BehaviorTreeNodes.cpp
#include "../../../include/Enemy/Boss/BehaviorTree.h"
#include <functional>
#include <initializer_list>
#include <memory>

// SequenceNode Implementation
SequenceNode::SequenceNode(std::initializer_list<std::shared_ptr<BehaviorTreeNode>> nodes)
    : children(nodes) {
}

NodeStatus SequenceNode::tick(Boss* boss, float dt) {
    while (current < children.size()) {
        NodeStatus status = children[current]->tick(boss, dt);
        if (status == NodeStatus::Running)
            return NodeStatus::Running;
        if (status == NodeStatus::Failure) {
            current = 0;
            return NodeStatus::Failure;
        }
        ++current;
    }
    current = 0;
    return NodeStatus::Success;
}

// SelectorNode Implementation
SelectorNode::SelectorNode(std::initializer_list<std::shared_ptr<BehaviorTreeNode>> nodes)
    : children(nodes) {
}

NodeStatus SelectorNode::tick(Boss* boss, float dt) {
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
    return NodeStatus::Failure;
}

// ActionNode Implementation
ActionNode::ActionNode(std::function<NodeStatus(Boss*, float)> fn)
    : action(fn) {
}

NodeStatus ActionNode::tick(Boss* boss, float dt) {
    return action(boss, dt);
}
