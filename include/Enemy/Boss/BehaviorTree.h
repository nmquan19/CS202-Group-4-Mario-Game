#pragma once
#include <memory>
#include <vector>
#include <initializer_list>


class Enemy;
enum class NodeStatus{
    Success,
    Failure,
    Running,
    Idle,
    Skipped
};  

class BehaviorTreeNode {
public:
    virtual ~BehaviorTreeNode() = default;
    virtual NodeStatus tick(Enemy* enemy, float dt) = 0;
    virtual void reset() = 0;   
};
class SequenceNode : public BehaviorTreeNode {
private:
    std::vector<std::shared_ptr<BehaviorTreeNode>> children;
    size_t current = 0;

public:
	SequenceNode() = default; 
    SequenceNode(std::initializer_list<std::shared_ptr<BehaviorTreeNode>> nodes);
    NodeStatus tick(Enemy* enemy, float dt) override;
    void addChild(std::shared_ptr<BehaviorTreeNode> child);
    void reset() override;  
};

class SelectorNode : public BehaviorTreeNode {
private:
    std::vector<std::shared_ptr<BehaviorTreeNode>> children;
    size_t current = 0;

public:
    SelectorNode(std::initializer_list<std::shared_ptr<BehaviorTreeNode>> nodes);
    NodeStatus tick(Enemy* enemy, float dt) override;
    void reset() override; 
};



class DecoratorNode : public BehaviorTreeNode
{
protected :  
    std::shared_ptr<BehaviorTreeNode> child; 
public : 
    DecoratorNode(std::shared_ptr<BehaviorTreeNode> childNode);  
	std::shared_ptr<BehaviorTreeNode> getChild() const;
    virtual NodeStatus tick(Enemy* boss, float dt) override = 0;
    virtual void reset() override;
};

class InverterNode : public DecoratorNode {
public:
    InverterNode(std::shared_ptr<BehaviorTreeNode> childNode)
		: DecoratorNode(childNode) {
	}
    NodeStatus tick(Enemy* boss, float dt) override;
}; 
class ForceSuccessNode : public DecoratorNode
{
public: 
    ForceSuccessNode(std::shared_ptr<BehaviorTreeNode> childNode)
        : DecoratorNode(childNode) {
	}   
    NodeStatus tick(Enemy* boss, float dt); 
};
class ForceFailureNode : public DecoratorNode
{
public:
    ForceFailureNode(std::shared_ptr<BehaviorTreeNode> childNode)
        : DecoratorNode(childNode) {
    } 
    NodeStatus tick(Enemy* boss, float dt) override;
}; 
class RepeatNode : public DecoratorNode
{
private: 
    int num_cycles;  
    int num_attempt = 0; 
public:
    RepeatNode(std::shared_ptr<BehaviorTreeNode> childNode, int cycles = 1)
        : DecoratorNode(childNode), num_cycles(cycles),num_attempt(0) {}
    
	int getNumCycles() const { return num_cycles; }
	void setNumCycles(int cycles) { num_cycles = cycles; }
    NodeStatus tick(Enemy* boss, float dt) override;
    void reset() override; 
}; 
class RepeatUntilSuccessNode : public DecoratorNode
{
private: 
	int num_attempt = 0;
	int num_cycles = 0; 
public:
    RepeatUntilSuccessNode(std::shared_ptr<BehaviorTreeNode> childNode)
        : DecoratorNode(childNode) {
    }
    int getNumCycles() const;
    void setNumCycles(int cycles);
    NodeStatus tick(Enemy* boss, float dt) override;
    void reset() override;
};
class KeepRunningUntilFailureNode : public DecoratorNode
{
public:
    KeepRunningUntilFailureNode(std::shared_ptr<BehaviorTreeNode> childNode)
        : DecoratorNode(childNode) {
    }
    NodeStatus tick(Enemy* boss, float dt) override;
}; 
class RunOnceNode : public BehaviorTreeNode {
private:
    std::shared_ptr<BehaviorTreeNode> child;
    bool hasRun = false;
    NodeStatus finalStatus = NodeStatus::Idle;
    bool thenSkip = true;

public:
    RunOnceNode(std::shared_ptr<BehaviorTreeNode> child, bool thenSkip = true)
        : child(child), thenSkip(thenSkip) {
    }

    NodeStatus tick(Enemy* boss, float dt) override; 

    void reset() override;
};


class ActionNode : public BehaviorTreeNode {
protected: 

public:
	ActionNode() :BehaviorTreeNode(){}
    virtual NodeStatus tick(Enemy* boss, float dt) override = 0;
    virtual void reset() override {}; 
};

class WalkToTargetNode : public ActionNode {
public : 
	WalkToTargetNode() :ActionNode() {}
	NodeStatus tick(Enemy* boss, float dt) override; 
}; 
class AttackNode : public ActionNode {
public: 
    AttackNode() :ActionNode() {}
    NodeStatus tick(Enemy* boss, float dt) override;
};

