#pragma once
#include "../EnemyState.h"
#include "../Enemy.h"
class GoombaWanderingState : public EnemyState
{ 
    public : 
    virtual void exit(Enemy* enemy) override; 
    virtual void enter(Enemy* enemy) override;
    virtual void checkCondition(Enemy* enemy) override; 
    virtual void update(Enemy* enemy ,float deltaTime) override; 
    static GoombaWanderingState& GetInstance() ;  
};  
class GoombaStompedState:  public EnemyState
{ 
    public : 
     virtual void exit(Enemy* enemy) override; 
    virtual void enter(Enemy* enemy) override;
    virtual void checkCondition(Enemy* enemy) override; 
    virtual void update(Enemy* enemy ,float deltaTime) override; 
    static GoombaStompedState& GetInstance() ;  
}; 
class GoombaKnockState:  public EnemyState
{ 
    public :  
     virtual void exit(Enemy* enemy) override; 
    virtual void enter(Enemy* enemy) override;
    virtual void checkCondition(Enemy* enemy) override; 
    virtual void update(Enemy* enemy ,float deltaTime) override; 
    static GoombaKnockState& GetInstance() ;  
}; 