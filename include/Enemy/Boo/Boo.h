#pragma once
#include "../Enemy.h"
#include "../../../include/Enemy/Boss/BehaviorTree.h"

class Boo : public Enemy
{
private:
    bool isReturning = false ;
    std::shared_ptr<BehaviorTreeNode> root;
    float retreatTimer = 0.0f;
    float patrolTimer = 0.0f; 
    Vector2 initialPos; 
    
public:
    Boo(Vector2 startPos, Vector2 size);
    Boo(Vector2 startPos, Vector2 velocity, Vector2 accelleration);

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
    ObjectType getObjectType() const override;
    EnemyType getType() const override;
    NodeType getTraverseType() override; 
};
