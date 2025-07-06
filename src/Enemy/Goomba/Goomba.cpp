#include  "../../../include/Enemy/Goomba/Goomba.h"
#include "../../../include/System/Resources.h"
#include <raymath.h>
#include "../../../include/Enemy/Enemy.h"
#include "../../../include/Enemy/Goomba/GoombaState.h"
#include "../../../include/Objects/ObjectFactory.h"
#include "../../../include/System/Interface.h"
#include <raylib.h>
#include <vector>
Goomba::Goomba(Vector2 startPos,Vector2 velocity, Vector2 accelleration): Enemy(startPos, velocity,accelleration, Resources::Enemy_Textures )
{    
    type = EnemyType::GOOMBA;
    scale = 5; 
    num_sprites =  {{0,1},{2,3},{5,5}};
    numSprites = {2,2,1};
    max_numSprites  = 2; 
    changeState(&GoombaWanderingState::GetInstance());
    hitbox = Rectangle({position.x,position.y,(float)spritebox.width*scale,(float)spritebox.height*scale});
}
Goomba::Goomba(Vector2 startPos, float scale) : Enemy(startPos, Resources::Enemy_Textures,scale)
{
    type = EnemyType::GOOMBA;
    num_sprites = { {0,1},{2,3},{5,5} };
    numSprites = { 2,2,1 };
    max_numSprites = 2;
    changeState(&GoombaWanderingState::GetInstance());
    hitbox = Rectangle({ position.x,position.y,(float)spritebox.width * scale,(float)spritebox.height * scale });
}

void Goomba::onCollision(Object* other) {
   
	if (other->getObjectCategory() == ObjectCategory::CHARACTER) {
         DrawText("HITTED",200,200,20,RED);
         this->changeState(&GoombaStompedState::GetInstance());
    }
}
void Goomba::checkCollision(const std::vector<Object*>& candidates)
{
    for (auto* candidate : candidates) {
        switch (candidate->getObjectCategory()) {
        case ObjectCategory::BLOCK:
            handleEnvironmentCollision(candidate);
            break;
        case ObjectCategory::PROJECTILE:
            // implement
            break;
        case ObjectCategory::CHARACTER:
            // implement
            this->changeState(&GoombaStompedState::GetInstance());

            break;
        }
    }
}
void Goomba::update(float deltaTime) {
    // Find Mario (player) in the physics manager
    Enemy::update(deltaTime);
    currentState->update(this, deltaTime);
    currentState->checkCondition(this);
    hitbox.x = position.x;
    hitbox.y = position.y;
    // Keep enemies within screen bounds
    if (position.x < 0)
    {
        position.x = 0;
        velocity.x *= -1;
    }
    if (position.x > 1920 - hitbox.width)
    {
        position.x = 1920 - hitbox.width;
        velocity.x *= -1;
    }

}