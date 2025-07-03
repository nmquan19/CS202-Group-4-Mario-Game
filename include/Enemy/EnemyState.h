#pragma once
#include "Enemy.h"
class Enemy; 
enum EState{ 
  Idle = 0, 
  Moving, 
  Jumping,
  Attacking
};  
class EnemyState
{
  public : 
    virtual void exit(Enemy* enemy) = 0; 
    virtual void enter(Enemy* enemy)= 0;
    virtual void checkCondition(Enemy* enemy)= 0; 
    virtual void update(Enemy* enemy ,float deltaTime) =  0;  
}; 
