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
#include <memory>
class KoopaShell : public CollectableObject, public IMovable, public ISavable{    
    public:
     KoopaShell(KoopaShellType type, Vector2 pos, Vector2 sz);
     void onCollect(Character* player) override; 
     void update(float deltaTime) override;
     void draw() override;
     void setVelocity(Vector2 newVelocity) override;
     Vector2 getVelocity() override;
	 ObjectCategory getObjectCategory() const override;
     std::vector<ObjectCategory> getCollisionTargets() const override;
     virtual void handleEnvironmentCollision(std::shared_ptr<Object> other, Direction dir);
	 void onCollision(std::shared_ptr<Object> other, Direction dir) override;  
	 void changeState(KoopaShellState* newState); 
	 friend class KoopaShellIdleState;
	 friend class KoopaShellMovingState;
	 friend class KoopaShellCollectedState;
	 friend class KoopaShellRevivingState;
     friend class KoopaShellKnockedState;
	 void queueState(KoopaShellState* newState);
     void applyQueueState(); 
	 bool isMovingState() const;  
     virtual KoopaShellType getType() const {
         return type;
     };
     std::vector<std::pair<int, int>> getSpriteData() const;   
     void onRelease() {}
     Vector2 getSize() const { return size; };

     nlohmann::json toJson() const override;
     void fromJson(const nlohmann::json& data) override;
     std::string getSaveType() const override;

private:
      float aniTimer;
	  float aniSpeed;    
      bool isKnocked; 
      bool onGround; 
      float timer;
      float activeTimer;    
      Rectangle spritebox;
      Vector2 velocity; 
	  KoopaShellState* currentState;
      KoopaShellState* pendingState; 
      float groundLevel = (float)INT_MAX;
      KoopaShellType type;

	  int curFrame = 0;
};