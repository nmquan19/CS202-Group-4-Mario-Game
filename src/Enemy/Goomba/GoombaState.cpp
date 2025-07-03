#include "../../../include/Enemy/Goomba/GoombaState.h"
#include "../../../include/Enemy/Goomba/Goomba.h"
#include "../../../include/System/PhysicsManager.h"
#include <cstdlib>  
#include "../../../include/System/Resources.h"
#include "../../../include/Enemy/Enemy.h"
#include "../../../include/Enemy/EnemyState.h"
#include <raylib.h>
void GoombaWanderingState::checkCondition(Enemy* enemy)
{

}
void GoombaWanderingState::enter(Enemy* enemy)
{

    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    goomba->velocity.x = 500; 
    goomba->curFrame = 0; 
//    std::cout<<goomba->num_sprites[0].first<<"\n";
    goomba->spritebox =  Resources::Enemy_sprite_boxes[goomba->num_sprites[0].first];
}
void GoombaWanderingState::exit(Enemy* enemy)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    goomba->curFrame =  goomba->numSprites[0]-1;
    goomba->spritebox =  Resources::Enemy_sprite_boxes[goomba->num_sprites[0].first+goomba->curFrame];
}
void GoombaWanderingState::update(Enemy* enemy,float deltaTime)
{   
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    int num =  goomba->numSprites[Idle];
    goomba->curFrame  =   goomba->curFrame%num; 
    goomba->spritebox =  Resources::Enemy_sprite_boxes[goomba->num_sprites[0].first+goomba->curFrame];
} 

GoombaWanderingState& GoombaWanderingState::GetInstance(){
    static GoombaWanderingState instance; 
    return instance;  
}

void GoombaStompedState::enter(Enemy* enemy)
{
    
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    goomba->velocity = {0,0};
    goomba->active = false;
    goomba->curFrame = 0; 
    goomba->spritebox =  Resources::Enemy_sprite_boxes[goomba->num_sprites[2].first];
}
void GoombaStompedState::checkCondition(Enemy* enemy)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
     if(goomba->aniTimer >= goomba->aniSpeed-0.1)
     {
          goomba->changeState(&GoombaWanderingState::GetInstance());
     }

}
void GoombaStompedState::exit(Enemy* enemy)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    goomba->active = true;
    PhysicsManager::getInstance().addObject(goomba);
}
void GoombaStompedState::update(Enemy* enemy,float deltaTime)
{   
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    int num =  goomba->numSprites[2];
    goomba->curFrame  =   goomba->curFrame%num; 
    goomba->spritebox =  Resources::Enemy_sprite_boxes[goomba->num_sprites[2].first+goomba->curFrame];
} 
GoombaStompedState& GoombaStompedState::GetInstance(){
    static GoombaStompedState instance; 
    return instance;  
}





void GoombaJumpingState::checkCondition(Enemy* enemy)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    int chance = rand() % 100;
    if (chance == 0) {
        goomba->changeState(&GoombaWanderingState::GetInstance());
    }
}
void GoombaJumpingState::enter(Enemy* enemy)
{
    
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);

    goomba->velocity = {5,0};
    int rowFrames =  goomba->numSprites[1];
    float frameHeight = goomba->texture.height/3;
    float frameWidth = goomba->texture.width/goomba->max_numSprites;
    goomba->curFrame = 0; 
    goomba->spritebox =  Rectangle({goomba->curFrame*frameWidth,1*frameHeight, frameWidth, frameHeight});
}
void GoombaJumpingState::exit(Enemy* enemy)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    goomba->curFrame =  goomba->numSprites[1]-1;
    goomba->spritebox.x = (float)goomba->curFrame*(float)goomba->texture.width/goomba->max_numSprites;
}
void GoombaJumpingState::update(Enemy* enemy,float deltaTime)
{   
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    int num =  goomba->numSprites[1];
    goomba->curFrame  =   goomba->curFrame%num; 
    goomba->spritebox.x = (float)goomba->curFrame*(float)goomba->texture.width/goomba->max_numSprites;
} 

GoombaJumpingState& GoombaJumpingState::GetInstance(){
    static GoombaJumpingState instance; 
    return instance;  
}