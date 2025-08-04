#include "../../../include/Enemy/Goomba/GoombaState.h"
#include "../../../include/Enemy/Goomba/Goomba.h"
#include "../../../include/Enemy/Enemy.h"
#include "../../../include/Enemy/EnemyState.h"
#include "../../../include/System/TextureManager.h"
#include "../../../include/Game/GameContext.h"
#include "../../../include/System/Constant.h"
#include "../../../include/System/Box2DWorldManager.h"
#include <raylib.h>

void GoombaWanderingState::checkCondition(Enemy* enemy)
{

}

void GoombaWanderingState::enter(Enemy* enemy)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);

    b2Vec2 currentVel;
    if (goomba->physicsBody) {
        goomba->physicsBody->GetLinearVelocity();
        goomba->physicsBody->SetLinearVelocity(b2Vec2(-Box2DWorldManager::raylibToB2(Constants::Goomba::WANDERING_SPEED), currentVel.y));
    }
    goomba->curFrame = 0;

    goomba->spritebox = TextureManager::Enemy_sprite_boxes[goomba->getSpriteData()[0].first];
}

void GoombaWanderingState::exit(Enemy* enemy)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    goomba->curFrame = (goomba->getSpriteData()[Idle].second - goomba->getSpriteData()[Idle].first) ;
    goomba->spritebox = TextureManager::Enemy_sprite_boxes[goomba->getSpriteData()[0].first + goomba->curFrame];
}

void GoombaWanderingState::update(Enemy* enemy, float deltaTime)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    int num = goomba->getSpriteData()[Idle].second - goomba->getSpriteData()[Idle].first+1;
    goomba->curFrame = goomba->curFrame % num;
    goomba->spritebox = TextureManager::Enemy_sprite_boxes[goomba->getSpriteData()[0].first + goomba->curFrame];
}

GoombaWanderingState& GoombaWanderingState::GetInstance() {
    static GoombaWanderingState instance;
    return instance;
}

void GoombaStompedState::enter(Enemy* enemy)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    goomba->physicsBody->SetLinearVelocity(b2Vec2(0, 0));
    goomba->curFrame = 0;
    goomba->active = false;
    goomba->stompedAnimation = true;
    goomba->stompedTime = 0.0f;
    goomba->spritebox = TextureManager::Enemy_sprite_boxes[goomba->getSpriteData()[2].first];

}

void GoombaStompedState::checkCondition(Enemy* enemy)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    if (goomba->stompedTime >= Constants::Goomba::STOMPED_DURATION)
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
    std::cout << goomba->stompedTime << std::endl;
    GameContext::getInstance().mark_for_deletion_Object(GameContext::getInstance().getSharedPtrFromRaw(goomba));
}

void GoombaStompedState::update(Enemy* enemy, float deltaTime)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    int num = goomba->getSpriteData()[2].second - goomba->getSpriteData()[2].first + 1;
    goomba->curFrame = goomba->curFrame % num;
    goomba->spritebox = TextureManager::Enemy_sprite_boxes[goomba->getSpriteData()[2].first + goomba->curFrame];
    goomba->stompedTime += deltaTime;
}

GoombaStompedState& GoombaStompedState::GetInstance() {
    static GoombaStompedState instance;
    return instance;
}

void GoombaKnockState::enter(Enemy* enemy)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    goomba->velocity = { 0, Constants::Goomba::KNOCK_VELOCITY };
    goomba->curFrame = 0;
    goomba->active = false;
	goomba->knockAnimation = true;
    goomba->spritebox = TextureManager::Enemy_sprite_boxes[goomba->getSpriteData()[2].first];
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
	GameContext::getInstance().mark_for_deletion_Object(GameContext::getInstance().getSharedPtrFromRaw(goomba));
}
void GoombaKnockState::update(Enemy* enemy, float deltaTime)
{
    Goomba* goomba = dynamic_cast<Goomba*>(enemy);
    int num = goomba->getSpriteData()[0].second - goomba->getSpriteData()[0].first + 1;
    goomba->curFrame = goomba->curFrame % num;
    goomba->spritebox = TextureManager::Enemy_sprite_boxes[goomba->getSpriteData()[0].first + goomba->curFrame];
}
GoombaKnockState& GoombaKnockState::GetInstance() {
    static GoombaKnockState instance;
    return instance;
}

