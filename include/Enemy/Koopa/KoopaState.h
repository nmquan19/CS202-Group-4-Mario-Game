#pragma once
#include "../EnemyState.h"
#include "../Enemy.h"
class KoopaWanderingState : public EnemyState
{
public:
    virtual void exit(Enemy* enemy) override;
    virtual void enter(Enemy* enemy) override;
    virtual void checkCondition(Enemy* enemy) override;
    virtual void update(Enemy* enemy, float deltaTime) override;
    static KoopaWanderingState& GetInstance();
};
class KoopaStompedState : public EnemyState
{
public:
    virtual void exit(Enemy* enemy) override;
    virtual void enter(Enemy* enemy) override;
    virtual void checkCondition(Enemy* enemy) override;
    virtual void update(Enemy* enemy, float deltaTime) override;
    static KoopaStompedState& GetInstance();
};
class KoopaKnockState : public EnemyState
{
public:
    virtual void exit(Enemy* enemy) override;
    virtual void enter(Enemy* enemy) override;
    virtual void checkCondition(Enemy* enemy) override;
    virtual void update(Enemy* enemy, float deltaTime) override;
    static KoopaKnockState& GetInstance();
};