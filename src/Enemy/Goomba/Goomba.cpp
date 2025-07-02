#include  "../../../include/Enemy/Goomba/Goomba.h"
#include "../../../include/System/Resources.h"
#include "../../include/System/PhysicsManager.h"
#include <raymath.h>
#include "../../../include/Enemy/Enemy.h"
#include "../../../include/Enemy/Goomba/GoombaState.h"
#include "../../../include/Objects/ObjectFactory.h"
#include "../../../include/System/Interface.h"
#include <raylib.h>
Goomba::Goomba(Vector2 startPos,Vector2 velocity, Vector2 accelleration): Enemy(startPos, velocity,accelleration, Resources::Enemy_Textures )
{    
    num_sprites =  {{0,1},{2,3},{5,5}};
    numSprites = {2,2,1};
    max_numSprites  = 2; 
    changeState(&GoombaWanderingState::GetInstance());
    hitbox = Rectangle({position.x,position.y,(float)spritebox.width*5,(float)spritebox.height*5});
}


void Goomba::onCollision(Object* other) {
   
	if (other->getObjectCategory() == ObjectCategory::CHARACTER) {
         DrawText("HITTED",200,200,20,RED);
         this->changeState(&GoombaStompedState::GetInstance());
    }
}
void Goomba::update(float deltaTime) {
    // Find Mario (player) in the physics manager
    auto players = PhysicsManager::getInstance().getObjectsInLayer(ObjectCategory::CHARACTER);
    currentState->update(this, deltaTime);
    currentState->checkCondition(this);
    position += velocity;
    hitbox.x = position.x;
    hitbox.y = position.y;
    // Keep enemies within screen bounds
    if (position.x < 0)
    {
        position.x = 0;
        velocity *= -1;
    }
    if (position.y < 0) position.y = 0;
    if (position.x > 1920 - hitbox.width)
    {
        position.x = 1920 - hitbox.width;
        velocity *= -1;
    }

    if (position.y > 1080 - hitBoxHeight) position.y = 1080 - hitBoxHeight;
}