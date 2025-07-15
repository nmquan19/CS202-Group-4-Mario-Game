 #pragma once
#include "../../Objects/CollectableObject.h"
#include "../../Objects/ObjectFactory.h"
#include <raylib.h>
#include "../../System/Interface.h"
#include "KoopaShellState.h"
#include <vector>
#include "../../Characters/Character.h"
#include <climits>
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
  Vector2 getSize() const {return size;};
  friend class KoopaShellIdleState;
  friend class KoopaShellMovingState;
  friend class KoopaShellCollectedState;
  friend class KoopaShellRevivingState;
  friend class KoopaShellKnockedState;
  void queueState(KoopaShellState* newState);
  void applyQueueState(); 
  bool isMovingState() const;  
private:
    bool isKnocked; 
      bool onGround; 
      float timer;
      Rectangle spriteBox;
      Vector2 velocity; 
	  KoopaShellState* currentState;
      KoopaShellState* pendingState; 
      Vector2 knockedVelocity; 
      float groundLevel = (float)INT_MAX;
};