#include "../../../include/Enemy/Koopa/KoopaState.h"
#include "../../../include/Enemy/Koopa/Koopa.h"
#include "../../../include/Enemy/Enemy.h"
#include "../../../include/System/TextureManager.h"
#include <raylib.h>
#include "../../../include/Game/GameContext.h"
#include "../../../include/System/Interface.h"
#include <memory>
#include "../../../include/Objects/ObjectFactory.h"
#include "../../../include/Enemy/Koopa/KoopaShell.h"    
#include "../../../include/Enemy/Koopa/KoopaShellState.h"
#include "../../../include/System/Constant.h"
#include "../../../include/System/Box2dWorldManager.h"

void KoopaWanderingState::checkCondition(Enemy* enemy)
{

}

void KoopaWanderingState::enter(Enemy* enemy){
    Koopa* koopa = dynamic_cast<Koopa*>(enemy);
    float velDirection = (koopa->isFacingRight ? 1 : -1);
    b2Vec2 currentVel = koopa->physicsBody->GetLinearVelocity();

    if (koopa) {
        switch (koopa->getType()) {
           case EnemyType::GREEN_KOOPA:
                koopa->physicsBody->SetLinearVelocity(b2Vec2(velDirection * Box2DWorldManager::raylibToB2(Constants::GreenKoopa::WANDERING_SPEED), currentVel.y));
                break;
            case EnemyType::RED_KOOPA:
                koopa->physicsBody->SetLinearVelocity(b2Vec2(velDirection * Box2DWorldManager::raylibToB2(Constants::RedKoopa::WANDERING_SPEED), currentVel.y));
				break;
           default:
                break;
        }
        koopa->curFrame = 0;
        koopa->spritebox = TextureManager::Enemy_sprite_boxes[koopa->getSpriteData()[0].first];
    }
}

void KoopaWanderingState::exit(Enemy* enemy)
{
    Koopa* koopa = dynamic_cast<Koopa*>(enemy);
    if (koopa) {
        koopa->curFrame = koopa->getSpriteData()[0].second- koopa->getSpriteData()[0].first;
        koopa->spritebox = TextureManager::Enemy_sprite_boxes[koopa->getSpriteData()[0].first + koopa->curFrame];
    }
}

void KoopaWanderingState::update(Enemy* enemy, float deltaTime)
{
    Koopa* koopa = dynamic_cast<Koopa*>(enemy);
    if (koopa) {
        int num = koopa->getSpriteData()[0].second - koopa->getSpriteData()[0].first + 1;
        koopa->curFrame = koopa->curFrame % num;
        koopa->spritebox = TextureManager::Enemy_sprite_boxes[koopa->getSpriteData()[0].first + koopa->curFrame];
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
    exit(enemy);
}

void KoopaStompedState::checkCondition(Enemy* enemy) {

}

void KoopaStompedState::exit(Enemy* enemy) {
    Koopa* koopa = dynamic_cast<Koopa*>(enemy);
    if (koopa) {
        koopa->active = false;
        koopa->isalive = false;
		KoopaShellType type = KoopaShellType::GREEN_KOOPA_SHELL;

        switch (koopa->getType()) {
		case EnemyType::GREEN_KOOPA:
            type = KoopaShellType::GREEN_KOOPA_SHELL;
			break;
		case EnemyType::RED_KOOPA:
			type = KoopaShellType::RED_KOOPA_SHELL;
            break; 
        default:
            break;
        }

        b2Vec2 koopaPosition = koopa->physicsBody->GetPosition();
        b2Vec2 offSet = Box2DWorldManager::raylibToB2(Constants::KoopaShell::standardSize / 2);
        b2Vec2 spawnPosition = b2Vec2(koopaPosition.x - offSet.x, koopaPosition.y - offSet.y);

        GameContext::getInstance().addObject(type, Box2DWorldManager::b2ToRaylib(spawnPosition), { 0.75,0.75 });

        std::shared_ptr<Object> sharedKoopa = GameContext::getInstance().getSharedPtrFromRaw(koopa);
        if (sharedKoopa) {
            GameContext::getInstance().mark_for_deletion_Object(sharedKoopa);
        }
    }
}

void KoopaStompedState::update(Enemy* enemy, float deltaTime) {

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
        KoopaShellType type = KoopaShellType::GREEN_KOOPA_SHELL;

		float knockVelocity = Constants::GreenKoopa::KNOCK_VELOCITY;
        switch (koopa->getShellType())
        {
		case KoopaShellType::GREEN_KOOPA_SHELL:
            knockVelocity = Constants::GreenKoopa::KNOCK_VELOCITY;
            type = KoopaShellType::GREEN_KOOPA_SHELL;

			break;
        case KoopaShellType::RED_KOOPA_SHELL:
            knockVelocity = Constants::RedKoopa::KNOCK_VELOCITY;
            type = KoopaShellType::RED_KOOPA_SHELL;

			break;
        default:
            break;
        }
        koopa->velocity = { 0, knockVelocity };
        koopa->curFrame = 0;
        koopa->active = false;
        koopa->knockAnimation = true;
        koopa->spritebox = TextureManager::Enemy_sprite_boxes[koopa->getSpriteData()[0].first];
        Vector2 spawnPosition = { koopa->getPosition().x + (koopa->size.x * Constants::TILE_SIZE) / 2, koopa->getPosition().y + koopa->size.y * Constants::TILE_SIZE };
        GameContext::getInstance().addObject(type, spawnPosition, { 0.75,0.75 }, [](std::shared_ptr<Object> obj) {
            auto shell = std::dynamic_pointer_cast<KoopaShell>(obj);
            if (shell) {
                shell->changeState(&KoopaShellKnockedState::getInstance());
            }
            });
        koopa->changeState(nullptr);
    }
}

void KoopaKnockState::checkCondition(Enemy* enemy)
{
    
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
}
KoopaKnockState& KoopaKnockState::GetInstance()
{
    static KoopaKnockState instance;
    return instance;
}