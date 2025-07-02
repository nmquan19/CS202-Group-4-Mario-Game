#include "../Enemy.h"
#include "GoombaState.h"
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
}; 
