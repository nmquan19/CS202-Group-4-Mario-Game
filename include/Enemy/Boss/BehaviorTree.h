#pragma once
#include <memory>
#include <vector>
#include <initializer_list>
#include <functional>
#include <string>
#include "SimulateState.h"
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
	std::vector<std::shared_ptr<BehaviorTreeNode>> getChildren() const { return children; }
    void reset() override;  
};
class ReactiveSequenceNode : public BehaviorTreeNode {
private:
    std::vector<std::shared_ptr<BehaviorTreeNode>> children;
    size_t current = 0;

public:
    ReactiveSequenceNode() = default;
    ReactiveSequenceNode(std::initializer_list<std::shared_ptr<BehaviorTreeNode>> nodes);
    NodeStatus tick(Enemy* enemy, float dt) override;
    void addChild(std::shared_ptr<BehaviorTreeNode> child);
    std::vector<std::shared_ptr<BehaviorTreeNode>> getChildren() const { return children; }
    void reset() override;
};
class SelectorNode : public BehaviorTreeNode {
private:
    std::vector<std::shared_ptr<BehaviorTreeNode>> children;
    size_t current = 0;

public:
	SelectorNode() = default;
    SelectorNode(std::initializer_list<std::shared_ptr<BehaviorTreeNode>> nodes);
    NodeStatus tick(Enemy* enemy, float dt) override;
    void addChild(std::shared_ptr<BehaviorTreeNode> child);
    void reset() override; 
    std::vector<std::shared_ptr<BehaviorTreeNode>> getChildren() const { return children; }

};

class ReactiveFallBackNode : public BehaviorTreeNode {
private:
    std::vector<std::shared_ptr<BehaviorTreeNode>> children;

public:
    ReactiveFallBackNode() = default;
    ReactiveFallBackNode(std::initializer_list<std::shared_ptr<BehaviorTreeNode>> nodes);
    NodeStatus tick(Enemy* enemy, float dt) override;
    void addChild(std::shared_ptr<BehaviorTreeNode> child);
    void reset() override;
    std::vector<std::shared_ptr<BehaviorTreeNode>> getChildren() const { return children; }

};

namespace UtilityScorers {

    using ScorerFunc = std::function<float(BaseSimState&)>;
    extern std::unordered_map<std::string, ScorerFunc> registry;
    void registerScorer(const std::string& name, ScorerFunc func);
    ScorerFunc get(const std::string& name);
    void registerDefaults();
}
class UtilitySelector : public BehaviorTreeNode {
public:
    using ScorerFunc = std::function<float(BaseSimState&)>;

    UtilitySelector() = default;

    NodeStatus tick(Enemy* boss, float dt) override;
    void reset() override;

    void addChild(std::shared_ptr<BehaviorTreeNode> child);
    void addScorer(ScorerFunc scorer);

    std::vector<std::shared_ptr<BehaviorTreeNode>> getChildren() const { return children; }

private:
    std::vector<std::shared_ptr<BehaviorTreeNode>> children;
    std::vector<ScorerFunc> scorers;
};


class DecoratorNode : public BehaviorTreeNode
{
protected :  
    std::shared_ptr<BehaviorTreeNode> child; 
public : 
    DecoratorNode(std::shared_ptr<BehaviorTreeNode> childNode);  
	virtual std::shared_ptr<BehaviorTreeNode> getChild() const;
    void setChild(std::shared_ptr<BehaviorTreeNode> childNode);
    virtual NodeStatus tick(Enemy* boss, float dt) override = 0;
    virtual void reset() override;
};

class InverterNode : public DecoratorNode {
public:
	InverterNode() :DecoratorNode(nullptr) {}
    InverterNode(std::shared_ptr<BehaviorTreeNode> childNode)
		: DecoratorNode(childNode) {
	}
    NodeStatus tick(Enemy* boss, float dt) override;
}; 
class ForceSuccessNode : public DecoratorNode
{
public: 
	ForceSuccessNode() = default;
    ForceSuccessNode(std::shared_ptr<BehaviorTreeNode> childNode)
        : DecoratorNode(childNode) {
	}   
    NodeStatus tick(Enemy* boss, float dt); 
};
class ForceFailureNode : public DecoratorNode
{
public:
	ForceFailureNode() = default;
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
	RepeatNode() : DecoratorNode(nullptr), num_cycles(1), num_attempt(0) {}
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
	RepeatUntilSuccessNode() : DecoratorNode(nullptr), num_attempt(0), num_cycles(0) {}
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
	KeepRunningUntilFailureNode() : DecoratorNode(nullptr) {}
    KeepRunningUntilFailureNode(std::shared_ptr<BehaviorTreeNode> childNode)
        : DecoratorNode(childNode) {
    }
    NodeStatus tick(Enemy* boss, float dt) override;
}; 
class RunOnceNode : public DecoratorNode {
private:
    bool hasRun = false;
    NodeStatus finalStatus = NodeStatus::Idle;
    bool thenSkip = true;

public:
	RunOnceNode() : DecoratorNode(nullptr), thenSkip(true) {}
    RunOnceNode(std::shared_ptr<BehaviorTreeNode> child, bool thenSkip = true)
        : DecoratorNode(child), thenSkip(thenSkip) {
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
class CanUseMeleeAttack1Node : public ActionNode {
public:
    CanUseMeleeAttack1Node() : ActionNode() {}
    NodeStatus tick(Enemy* boss, float dt) override;
};
class IdleNode : public ActionNode {
public:
    IdleNode() :ActionNode() {}
    NodeStatus tick(Enemy* boss, float dt) override;
};
class WalkTurnNode : public ActionNode {
private: 
    bool started = false; 
public:
    WalkTurnNode() : ActionNode() {}
    NodeStatus tick(Enemy* boss, float dt) override;
    void reset() override;
};
class IsTargetInRangeNode : public ActionNode {
public:
    IsTargetInRangeNode() : ActionNode() {}
    NodeStatus tick(Enemy* boss, float dt) override;
};
class NeedTurnNode : public ActionNode
{
public: 
	NeedTurnNode() : ActionNode() {}
	NodeStatus tick(Enemy* boss, float dt) override;
};
class CanUseSpinNode :public ActionNode {
public:
    CanUseSpinNode() : ActionNode() {}
	NodeStatus tick(Enemy* boss, float dt) override;
}; 
class SpinAttackNode : public ActionNode {
private:
	bool started = false;
public:
    SpinAttackNode() : ActionNode() {}
    NodeStatus tick(Enemy* boss, float dt) override;
    void reset() override; 
}; 
class SpinAttackWindupNode : public ActionNode {
public:
    SpinAttackWindupNode() : ActionNode() {}
    NodeStatus tick(Enemy* boss, float dt) override;
};

class SpinAttackWinddownNode : public ActionNode {
public:
    SpinAttackWinddownNode() : ActionNode() {}
    NodeStatus tick(Enemy* boss, float dt) override;
};
class IsTakingDamageNode : public ActionNode {
public:
    IsTakingDamageNode() : ActionNode() {}
	NodeStatus tick(Enemy* boss, float dt) override;    
};

class IsPlayerHigherNode : public ActionNode {
public:
    IsPlayerHigherNode() : ActionNode() {}
    NodeStatus tick(Enemy* boss, float dt) override;
};

class IsFallingNode : public ActionNode {
    public:
    IsFallingNode() : ActionNode() {}
    NodeStatus tick(Enemy* boss, float dt) override;
};
class IsInIntroNode : public ActionNode {
public:
    IsInIntroNode() : ActionNode() {}
	NodeStatus tick(Enemy* boss, float dt) override;
};
class CanWallJumpNode : public ActionNode {
public:
    CanWallJumpNode() : ActionNode() {}
    NodeStatus tick(Enemy* boss, float dt) override;

};
class WallJumpNode : public ActionNode {
public:
    WallJumpNode() : ActionNode() {}
    NodeStatus tick(Enemy* boss, float dt) override;
}; 
class JumpNode : public ActionNode {
public:
    JumpNode() : ActionNode() {}
    NodeStatus tick(Enemy* boss, float dt) override;
};
class IsInAirNode : public ActionNode {
public:
    IsInAirNode() : ActionNode() {}
    NodeStatus tick(Enemy* boss, float dt) override; 
}; 
class AerialAttackNode : public ActionNode
{
public: 
    AerialAttackNode(): ActionNode() {}
    NodeStatus tick(Enemy* boss, float dt) override;  
};
class IsInWallJumpNode : public ActionNode {
public:
    IsInWallJumpNode() : ActionNode() {}
    NodeStatus tick(Enemy* boss, float dt) override;
};
class CanReachPlayerHeightNode : public ActionNode
{
    CanReachPlayerHeightNode(): ActionNode(){}
    NodeStatus tick(Enemy* boss, float dt) override;
};
class CanUseAerialAttackNode : public ActionNode
{
public:
    CanUseAerialAttackNode() : ActionNode() {}
    NodeStatus tick(Enemy* boss, float dt) override;
};

class MoveToTargetNode : public ActionNode
{
public:
    MoveToTargetNode() : ActionNode(){}
    NodeStatus tick(Enemy* boss, float dt) override;
};