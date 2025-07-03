#pragma once
#include "../EnemyState.h"
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

class GoombaJumpingState:  public EnemyState
{ 
    public :  
     virtual void exit(Enemy* enemy) override; 
    virtual void enter(Enemy* enemy) override;
    virtual void checkCondition(Enemy* enemy) override; 
    virtual void update(Enemy* enemy ,float deltaTime) override; 
    static GoombaJumpingState& GetInstance() ;  
}; 