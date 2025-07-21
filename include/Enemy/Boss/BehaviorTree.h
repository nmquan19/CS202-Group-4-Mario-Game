#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <initializer_list>
class Boss;

enum class NodeStatus {
    Success,
    Failure,
    Running
};  

class BehaviorTreeNode {
public:
    virtual ~BehaviorTreeNode() = default;
    virtual NodeStatus tick(Boss* boss, float dt) = 0;
};
class SequenceNode : public BehaviorTreeNode {
private:
    std::vector<std::shared_ptr<BehaviorTreeNode>> children;
    size_t current = 0;

public:
    SequenceNode(std::initializer_list<std::shared_ptr<BehaviorTreeNode>> nodes);
    NodeStatus tick(Boss* boss, float dt) override;
};

// SelectorNode
class SelectorNode : public BehaviorTreeNode {
private:
    std::vector<std::shared_ptr<BehaviorTreeNode>> children;
    size_t current = 0;

public:
    SelectorNode(std::initializer_list<std::shared_ptr<BehaviorTreeNode>> nodes);
    NodeStatus tick(Boss* boss, float dt) override;
};

// ActionNode
class ActionNode : public BehaviorTreeNode {
private:
    std::function<NodeStatus(Boss*, float)> action;

public:
    ActionNode(std::function<NodeStatus(Boss*, float)> fn);
    NodeStatus tick(Boss* boss, float dt) override;
};
