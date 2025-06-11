#include  "../../../include/Enemy/Goomba/Goomba.h"
#include "../../../include/System/Resources.h"
#include <iostream>
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