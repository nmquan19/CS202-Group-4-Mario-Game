#include "../Enemy.h"
#include <vector>
#include "../../Objects/ObjectFactory.h"
#include <raylib.h>
class GoombaWanderingState;  
class Goomba: public Enemy
{
    public : 
      Goomba(Vector2 startPos,Vector2 velocity, Vector2 accelleration);   
      friend class GoombaWanderingState;
      friend class GoombaStompedState;
      friend class GoombaJumpingState;
      void onCollision(Object* other) override;
      void update(float deltaTime); 
      void checkCollision(const std::vector<Object*>& candidates) override;
}; 
