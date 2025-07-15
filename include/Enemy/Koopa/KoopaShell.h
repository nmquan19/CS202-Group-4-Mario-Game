 #pragma once
#include "../../Objects/CollectableObject.h"
#include "../../Objects/ObjectFactory.h"
#include <raylib.h>
#include "../../System/Interface.h"
#include "KoopaShellState.h"
#include <vector>
#include "../../Characters/Character.h"
#include <climits>
#include <utility>
class KoopaShell : public CollectableObject, public IMovable{    
    public:
     KoopaShell(Vector2 pos, Vector2 sz);
     void onCollect(Character* player) override; 
     void update(float deltaTime) override;
     void draw() override;
     void setVelocity(Vector2 newVelocity) override;
     Vector2 getVelocity() override;
	 ObjectCategory getObjectCategory() const override;
     std::vector<ObjectCategory> getCollisionTargets() const override;
     virtual void handleEnvironmentCollision(Object* other);
	 void applyGravity(float deltaTime);
	 void onCollision(Object* other) override;  
	 void changeState(KoopaShellState* newState); 
     void checkCollision(const std::vector<Object*>& candidates) override;
	 friend class KoopaShellIdleState;
	 friend class KoopaShellMovingState;
	 friend class KoopaShellCollectedState;
	 friend class KoopaShellRevivingState;
     friend class KoopaShellKnockedState;
	 void queueState(KoopaShellState* newState);
     void applyQueueState(); 
	 bool isMovingState() const;  
     virtual KoopaShellType getType() const =0; 
     std::vector<std::pair<int, int>> getSpriteData() const;   
     Vector2 getSize() const { return size; };
private:
      float aniTimer;
	  float aniSpeed;    
      bool isKnocked; 
      bool onGround; 
      float timer;
      Rectangle spritebox;
      Vector2 velocity; 
	  KoopaShellState* currentState;
      KoopaShellState* pendingState; 
      float groundLevel = (float)INT_MAX;
	  int curFrame = 0;
};

class KoopaGreenShell : public KoopaShell {
    public:
        KoopaGreenShell(Vector2 pos, Vector2 sz);

        KoopaShellType getType() const ;
}; 

class KoopaRedShell : public KoopaShell {
public:

    KoopaRedShell(Vector2 pos, Vector2 sz);

    KoopaShellType getType() const ;
};