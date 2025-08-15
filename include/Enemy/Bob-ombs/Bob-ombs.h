#pragma once
#include "../Enemy.h"
#include "../../../include/Enemy/Boss/BehaviorTree.h"
#include "../../Enemy/LedgeDetector.h"
class Bob_ombs : public Enemy
{
private:
    bool isReturning = false ;
    std::shared_ptr<BehaviorTreeNode> root;
    float retreatTimer = 0.0f;
    float patrolTimer = 0.0f; 
    float attackTimer = 0.0f;
    std::shared_ptr<LedgeDetector> ledgeDetector;

    Vector2 initialPos; 
    
public:
    Bob_ombs(Vector2 startPos, Vector2 size);
    Bob_ombs(Vector2 startPos, Vector2 velocity, Vector2 accelleration);

    void handleEnvironmentCollision(std::shared_ptr<Object> other, Direction dir) override;
    void onCollision(std::shared_ptr<Object> other, Direction dir) override;
    void update(float deltaTime);
    void attack()override; 
    void draw() override;
    void die() override;   
    void patrol() override; 
    bool isPatrolling() override; 
    void retreat() override; 
    void setAnimation(const std::string& s) override;  
    bool isNearTarget() const override; 
    void takeDamage(int damage) override;
    float getWalkVelocity() const override; 
    bool isAttacking() override; 
    bool isRetreating() override;
    bool isInChaseRange() override;
    float getJumpVelocity() const override;

    float jumpTo(Vector2 position, bool apply) override;
    ObjectType getObjectType() const override;
    EnemyType getType() const override;
    NodeType getTraverseType() override; 
};
