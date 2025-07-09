#include "../../../include/Enemy/Goomba/GoombaState.h"
#include "../../../include/Enemy/Goomba/Goomba.h"
#include "../../../include/Enemy/Enemy.h"
#include "../../../include/Enemy/EnemyState.h"
#include "../../../include/System/TextureManager.h"
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
    goomba->spritebox = TextureManager::Enemy_sprite_boxes[goomba->num_sprites[0].first];
}
void GoombaWanderingState::exit(Enemy* enemy)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    goomba->curFrame = goomba->numSprites[0] - 1;
    goomba->spritebox = TextureManager::Enemy_sprite_boxes[goomba->num_sprites[0].first + goomba->curFrame];
}
void GoombaWanderingState::update(Enemy* enemy, float deltaTime)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    int num = goomba->numSprites[Idle];
    goomba->curFrame = goomba->curFrame % num;
    goomba->spritebox = TextureManager::Enemy_sprite_boxes[goomba->num_sprites[0].first + goomba->curFrame];
}

GoombaWanderingState& GoombaWanderingState::GetInstance() {
    static GoombaWanderingState instance;
    return instance;
}

void GoombaStompedState::enter(Enemy* enemy)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    goomba->velocity = {0,0};
    goomba->curFrame = 0;
    goomba->active = false; 
    goomba->stompedAnimation = true; 
    goomba->spritebox = TextureManager::Enemy_sprite_boxes[goomba->num_sprites[2].first];
}
void GoombaStompedState::checkCondition(Enemy* enemy)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    if (goomba->stompedTime >= goomba->stompedDuration) 
    {
        goomba->stompedAnimation = false;
        goomba->active = false; 
        goomba->changeState(nullptr);
    }
}
void GoombaStompedState::exit(Enemy* enemy)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    goomba->isalive = false; 
}
void GoombaStompedState::update(Enemy* enemy, float deltaTime)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    int num = goomba->numSprites[2];
    goomba->curFrame = goomba->curFrame % num;
    goomba->spritebox = TextureManager::Enemy_sprite_boxes[goomba->num_sprites[2].first + goomba->curFrame];
	goomba->stompedTime += deltaTime; 
}
GoombaStompedState& GoombaStompedState::GetInstance() {
    static GoombaStompedState instance;
    return instance;
}

void GoombaKnockState::enter(Enemy* enemy)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    goomba->velocity = { 0, goomba->knockVelocity };
    goomba->curFrame = 0;
    goomba->active = false;
    goomba->stompedAnimation = true;
    goomba->spritebox = TextureManager::Enemy_sprite_boxes[goomba->num_sprites[2].first];
}
void GoombaKnockState::checkCondition(Enemy* enemy)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    if (goomba->position.y >= GetScreenHeight())
    {

        goomba->knockAnimation = false;
        //goomba->velocity = { 0, 0 };
        goomba->active = false;
        goomba->changeState(nullptr);
    }
}
void GoombaKnockState::exit(Enemy* enemy)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    goomba->isalive = false;
}
void GoombaKnockState::update(Enemy* enemy, float deltaTime)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    int num = goomba->numSprites[2];
    goomba->curFrame = goomba->curFrame % num;
    goomba->spritebox = TextureManager::Enemy_sprite_boxes[goomba->num_sprites[2].first + goomba->curFrame];
}
GoombaKnockState& GoombaKnockState::GetInstance() {
    static GoombaKnockState instance;
    return instance;
}

