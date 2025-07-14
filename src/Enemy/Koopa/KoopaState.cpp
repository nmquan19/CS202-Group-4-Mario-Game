#include "../../../include/Enemy/Koopa/KoopaState.h"
#include "../../../include/Enemy/Koopa/Koopa.h"
#include "../../../include/Enemy/Enemy.h"
#include "../../../include/Enemy/EnemyState.h"
#include "../../../include/System/TextureManager.h"
#include <raylib.h>
#include "../../../include/Game/GameContext.h"
#include "../../../include/System/Interface.h"
#include <memory>
#include "../../../include/Objects/ObjectFactory.h"
#include <iostream>
void KoopaWanderingState::checkCondition(Enemy* enemy)  
{  
    
}
void KoopaWanderingState::enter(Enemy* enemy)  
{  
    Koopa* koopa = dynamic_cast<Koopa*>(enemy);  
    if (koopa) {  
        koopa->velocity.x = 500;  
        koopa->curFrame = 0;  
        koopa->spritebox = TextureManager::Enemy_sprite_boxes[koopa->num_sprites[0].first];  
    }  
}  

void KoopaWanderingState::exit(Enemy* enemy)  
{  
    Koopa* koopa = dynamic_cast<Koopa*>(enemy);  
    if (koopa) {  
        koopa->curFrame = koopa->numSprites[0] - 1;  
        koopa->spritebox = TextureManager::Enemy_sprite_boxes[koopa->num_sprites[0].first + koopa->curFrame];  
    }  
}  

void KoopaWanderingState::update(Enemy* enemy, float deltaTime)  
{  
    Koopa* koopa = dynamic_cast<Koopa*>(enemy);  
    if (koopa) {  
        int num = koopa->numSprites[Idle];  
        koopa->curFrame = koopa->curFrame % num;  
        koopa->spritebox = TextureManager::Enemy_sprite_boxes[koopa->num_sprites[0].first + koopa->curFrame];  
    }  
}  
KoopaWanderingState& KoopaWanderingState::GetInstance()  
{  
    static KoopaWanderingState instance;  
    return instance;  
}
void KoopaStompedState::enter(Enemy* enemy)  
{  
    Koopa* koopa = dynamic_cast<Koopa*>(enemy);  
    koopa->changeState(&KoopaWanderingState::GetInstance()); 
}  

void KoopaStompedState::checkCondition(Enemy* enemy)  
{  
}  

void KoopaStompedState::exit(Enemy* enemy)  
{  
    Koopa* koopa = dynamic_cast<Koopa*>(enemy);  
    if (koopa) {  
		koopa->active = false;
        koopa->isalive = false; 
        GameContext::getInstance().addObject(KoopaShellType::GREEN_KOOPA_SHELL, koopa->getPosition(), { 1,1 }); 
        std::shared_ptr<Object> sharedKoopa = GameContext::getInstance().getSharedPtrFromRaw(koopa);
        if (sharedKoopa) {
            std::cout << "AAA" << "\n";
     
            GameContext::getInstance().mark_for_deletion_Object(sharedKoopa);
        }
    }  
}  

void KoopaStompedState::update(Enemy* enemy, float deltaTime)  
{  
    Koopa* koopa = dynamic_cast<Koopa*>(enemy);  
    if (koopa) {  
        int num = koopa->numSprites[0];  
        koopa->curFrame = koopa->curFrame % num;  
        koopa->spritebox = TextureManager::Enemy_sprite_boxes[koopa->num_sprites[0].first + koopa->curFrame];  
        koopa->stompedTime += deltaTime;  
    }  
}  
KoopaStompedState& KoopaStompedState::GetInstance()  
{  
    static KoopaStompedState instance;  
    return instance;  
}
void KoopaKnockState::enter(Enemy* enemy)  
{  
    Koopa* koopa = dynamic_cast<Koopa*>(enemy);  
    if (koopa) {  
        koopa->velocity = { 0, koopa->knockVelocity };  
        koopa->curFrame = 0;  
        koopa->active = false;  
        koopa->stompedAnimation = true;  
        koopa->spritebox = TextureManager::Enemy_sprite_boxes[koopa->num_sprites[0].first];  
    }  
}  

void KoopaKnockState::checkCondition(Enemy* enemy)  
{  
    Koopa* koopa = dynamic_cast<Koopa*>(enemy);  
    if (koopa && koopa->position.y >= GetScreenHeight()) {  
        koopa->knockAnimation = false;  
        koopa->active = false;  
        koopa->changeState(nullptr);  
    }  
}  

void KoopaKnockState::exit(Enemy* enemy)
{
    Koopa* koopa = dynamic_cast<Koopa*>(enemy);
    if (koopa) {
        koopa->isalive = false;

        std::shared_ptr<Object> sharedKoopa = GameContext::getInstance().getSharedPtrFromRaw(koopa);
        if (sharedKoopa) {
            GameContext::getInstance().mark_for_deletion_Object(sharedKoopa);
        }
    }
}


void KoopaKnockState::update(Enemy* enemy, float deltaTime)  
{  
    Koopa* koopa = dynamic_cast<Koopa*>(enemy);  
    if (koopa) {  
        int num = koopa->numSprites[0];  
        koopa->curFrame = koopa->curFrame % num;  
        koopa->spritebox = TextureManager::Enemy_sprite_boxes[koopa->num_sprites[0].first + koopa->curFrame];  
    }  
}
KoopaKnockState& KoopaKnockState::GetInstance()  
{  
    static KoopaKnockState instance;  
    return instance;  
}