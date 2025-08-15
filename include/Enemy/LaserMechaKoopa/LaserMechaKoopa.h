#pragma once
#pragma once
#include "../Enemy.h"
#include "../../../include/Enemy/Boss/BehaviorTree.h"
class LaserMechaKoopa : public Enemy
{
private:
    bool isReturning = false;
    float attackCoolDown = 0.0f;
    float attackTimer = 0.0f;
public:
    LaserMechaKoopa(Vector2 startPos, Vector2 size);
    LaserMechaKoopa(Vector2 startPos, Vector2 velocity, Vector2 accelleration);
    std::shared_ptr<BehaviorTreeNode> root;

    void handleEnvironmentCollision(std::shared_ptr<Object> other, Direction dir) override;
    void onCollision(std::shared_ptr<Object> other, Direction dir) override;
    void update(float deltaTime);
    void attack()override;
    void draw() override;
    void die() override;
    void setAnimation(const std::string& s) override;
    void takeDamage(int damage) override;
    bool isAttacking() override;
    bool isNearTarget()const override; 
    bool canUseBasicAttack() const override;
 
    ObjectType getObjectType() const override;
    EnemyType getType() const override;
};
