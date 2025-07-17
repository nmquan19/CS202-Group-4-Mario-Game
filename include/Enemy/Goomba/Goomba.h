#pragma once
#include "../Enemy.h"
#include <vector>
#include "../../Objects/ObjectFactory.h"
#include <raylib.h>
#include "../../System/Interface.h"
class Goomba: public Enemy
{
    private: 
        bool stompedAnimation = false;
		bool knockAnimation = false;
		float stompedTime = 0.0f; 
    public : 
	  Goomba(Vector2 startPos, Vector2 size);
      Goomba(Vector2 startPos,Vector2 velocity, Vector2 accelleration);   
      friend class GoombaWanderingState;
      friend class GoombaStompedState;
      friend class GoombaKnockState;

      void handleEnvironmentCollision(std::shared_ptr<Object> other) override;
      void onCollision(std::shared_ptr<Object> other) override;
      void update(float deltaTime); 
	  void draw() override;
      void checkCollision(const std::vector<std::shared_ptr<Object>>& candidates) override;
	  void handleCharacterCollision(std::shared_ptr<Object> other);
      void die() override;  
      void takeDamage(int damage) override;
	  ObjectType getObjectType() const override;
      EnemyType getType() const override;
}; 
