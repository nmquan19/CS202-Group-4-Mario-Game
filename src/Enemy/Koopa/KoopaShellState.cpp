#include "../../../include/Enemy/Koopa/KoopaShellState.h"
#include <vector>
#include "../../../include/Objects/ObjectFactory.h"
#include "../../../include/Enemy/Koopa/KoopaShell.h"
#include <raylib.h>
#include "../../../include/Game/GameContext.h"
#include "../../../include/System/Interface.h"
#include "../../../include/System/TextureManager.h"
#include "../../../include/System/Constant.h"
void KoopaShellIdleState::enter(KoopaShell* shell) {
    shell->setCollectable(true); 
    shell->velocity = { 0,0 }; 
    shell->timer = 0.0f; 
    shell->curFrame = 0;
    shell->spritebox = TextureManager::Enemy_sprite_boxes[shell->getSpriteData()[0].first];
}

void KoopaShellIdleState::exit(KoopaShell* shell) {

    shell->setCollectable(false);
    shell->curFrame = shell->getSpriteData()[0].second - shell->getSpriteData()[0].first;
}

void KoopaShellIdleState::update(KoopaShell* shell, float deltaTime) {
	shell->timer += deltaTime;
    shell->curFrame %= shell->getSpriteData()[0].second - shell->getSpriteData()[0].first +1 ;
    shell->spritebox = TextureManager::Enemy_sprite_boxes[shell->getSpriteData()[0].first + shell->curFrame];
    if (shell->timer > Constants::KoopaShell::IDLE_DURATION) { 
        shell->changeState(&KoopaShellRevivingState::getInstance());
    }
}
ObjectCategory KoopaShellIdleState::getObjectCategory() const {
    return ObjectCategory::SHELL;
}
//void KoopaShellIdleState::checkCollision(KoopaShell* shell, const std::vector<std::shared_ptr<Object>>& objects) {
//    for (auto obj : objects) {
//        {
//            switch (obj->getObjectCategory())
//            {
//       			case ObjectCategory::CHARACTER:
//                    shell->velocity = (obj->getPosition().x < shell->getPosition().x) ? Vector2{ Constants::KoopaShell::MOVING_SPEED,  shell->velocity.y} : Vector2{ -Constants::KoopaShell::MOVING_SPEED, shell->velocity.y};
//                    shell->changeState(&KoopaShellMovingState::getInstance());
//                    break; 
//                case ObjectCategory::BLOCK:
//                    shell->handleEnvironmentCollision(obj);
//                    break; 
//                case ObjectCategory::PROJECTILE:
//					shell->changeState(&KoopaShellKnockedState::getInstance());
//                    break; 
//                break;
//            }
//        }
//    }
//}
std::vector<ObjectCategory> KoopaShellIdleState::getCollisionTargets() const {
    return { ObjectCategory::CHARACTER, ObjectCategory::BLOCK, ObjectCategory::PROJECTILE };
}

void KoopaShellMovingState::enter(KoopaShell* shell) {

    shell->curFrame = 0; 
    shell->aniSpeed = 0.05;  
    shell->spritebox = TextureManager::Enemy_sprite_boxes[shell->getSpriteData()[1].first];
}

void KoopaShellMovingState::exit(KoopaShell* shell) {
	shell->velocity = { 0, 0 };
	shell->aniSpeed = 0.2;
    shell->curFrame = shell->getSpriteData()[0].second - shell->getSpriteData()[0].first;
    shell->spritebox = TextureManager::Enemy_sprite_boxes[shell->getSpriteData()[1].first + shell->curFrame];
}

void KoopaShellMovingState::update(KoopaShell* shell, float deltaTime) {
    shell->curFrame %= shell->getSpriteData()[1].second - shell->getSpriteData()[1].first+1;
    shell->spritebox = TextureManager::Enemy_sprite_boxes[shell->getSpriteData()[1].first + shell->curFrame];
}

//void KoopaShellMovingState::checkCollision(KoopaShell* shell, const std::vector<std::shared_ptr<Object>>& objects) {
//    for (auto obj : objects) {
//        {
//            switch (obj->getObjectCategory())
//            {
//            case ObjectCategory::CHARACTER:
//
//                if(shell->getCollidedPart(*obj) == (int)Direction::DOWN)
//                {
//					shell->velocity = { 0, 0 };
//                    shell->changeState(&KoopaShellIdleState::getInstance()); 
//                }
//                else if (shell->getCollidedPart(*obj) == (int)Direction::LEFT || shell->getCollidedPart(*obj) == (int)Direction::RIGHT)
//                {
//					std::shared_ptr<Character> character = std::dynamic_pointer_cast<Character>(obj);
//                    character->takeDamage(1); 
//                }
//                break;
//            case ObjectCategory::BLOCK:
//                shell->handleEnvironmentCollision(obj);
//                break; 
//            case ObjectCategory::PROJECTILE:
//                shell->queueState(&KoopaShellKnockedState::getInstance());
//                break;
//            case ObjectCategory::INTERACTIVE:
//                shell->handleEnvironmentCollision(obj);
//                break;
//            }
//        }
//    }
//}

ObjectCategory KoopaShellMovingState::getObjectCategory() const {
    return ObjectCategory::PROJECTILE;
}
std::vector<ObjectCategory> KoopaShellMovingState::getCollisionTargets() const {
    return { ObjectCategory::CHARACTER, ObjectCategory::BLOCK, ObjectCategory::PROJECTILE, ObjectCategory::INTERACTIVE };
}






void KoopaShellRevivingState::enter(KoopaShell* shell) {

    shell->curFrame = 0;
	shell->timer = 0.0f;
    shell->spritebox = TextureManager::Enemy_sprite_boxes[shell->getSpriteData()[2].second];
}

void KoopaShellRevivingState::exit(KoopaShell* shell) {
	shell->timer = 0.0f; 
}

void KoopaShellRevivingState::update(KoopaShell* shell, float deltaTime) {
	shell->timer += deltaTime;
    if(shell->timer > Constants::KoopaShell::REVIVE_DURATION) { 
        EnemyType type;  
        switch (shell->getType()) {
            case KoopaShellType::GREEN_KOOPA_SHELL:
                type = EnemyType::GREEN_KOOPA;
				break;
            case KoopaShellType::RED_KOOPA_SHELL:
                type = EnemyType::RED_KOOPA;
                break;
            default:
                type = EnemyType::GREEN_KOOPA; 
				break;
        }
        Vector2 spawnPosition = { shell->getPosition().x + (shell->size.x * Constants::TILE_SIZE) / 2, shell->getPosition().y + shell->size.y * Constants::TILE_SIZE };

        GameContext::getInstance().addObject(type, spawnPosition, { 0.75,1 });
        //GameContext::getInstance().mark_for_deletion_Object(GameContext::getInstance().getSharedPtrFromRaw(shell));
		shell->active = false;
        shell->changeState(nullptr);
    }
}

//void KoopaShellRevivingState::checkCollision(KoopaShell* shell, const std::vector<std::shared_ptr<Object>>& objects) {
//    for (auto obj : objects) {
//        {
//            switch (obj->getObjectCategory())
//            {
//            case ObjectCategory::CHARACTER:
//                if (shell->getCollidedPart(*obj) == (int)Direction::UP)
//                {
//                    shell->velocity = { 0, 0 };
//                    shell->changeState(&KoopaShellIdleState::getInstance());
//                }
//                if (shell->getCollidedPart(*obj) == (int)Direction::RIGHT || shell->getCollidedPart(*obj) == (int)Direction::LEFT)
//                {
//                    shell->velocity = (obj->getPosition().x < shell->getPosition().x) ? Vector2{ Constants::KoopaShell::MOVING_SPEED, 0 } : Vector2{ -Constants::KoopaShell::MOVING_SPEED, 0 };
//                    shell->changeState(&KoopaShellMovingState::getInstance());
//                }
//                break;
//            case ObjectCategory::BLOCK:
//                shell->handleEnvironmentCollision(obj);
//                break; 
//            case ObjectCategory::PROJECTILE:
//                shell->changeState(&KoopaShellKnockedState::getInstance());
//                break;
//            }
//        }
//    }
//}
ObjectCategory KoopaShellRevivingState::getObjectCategory() const {
    return ObjectCategory::BLOCK;
}
std::vector<ObjectCategory> KoopaShellRevivingState::getCollisionTargets() const {
    return { ObjectCategory::CHARACTER, ObjectCategory::BLOCK, ObjectCategory::PROJECTILE };
}




void KoopaShellCollectedState::enter(KoopaShell* shell) {
    shell->curFrame = 0;
    shell->spritebox = TextureManager::Enemy_sprite_boxes[shell->getSpriteData()[0].first];
}

void KoopaShellCollectedState::exit(KoopaShell* shell) {
	shell->spritebox = { 0,0, 0, 0 };
    
    //shell->changeState(&KoopaShellMovingState::getInstance());
}

void KoopaShellCollectedState::update(KoopaShell* shell, float deltaTime) {

    shell->curFrame %= shell->getSpriteData()[0].second - shell->getSpriteData()[0].first + 1;
    shell->spritebox = TextureManager::Enemy_sprite_boxes[shell->getSpriteData()[0].first + shell->curFrame];
}

//void KoopaShellCollectedState::checkCollision(KoopaShell* shell, const std::vector<std::shared_ptr<Object>>& objects) {
//  
//}

ObjectCategory KoopaShellCollectedState::getObjectCategory() const {
    return ObjectCategory::ITEM;
}
std::vector<ObjectCategory> KoopaShellCollectedState::getCollisionTargets() const {
    return {};
}



void KoopaShellKnockedState::enter(KoopaShell* shell) {
	float knockVelocity = 0.0f;
    switch (shell->getType())
    {
    case KoopaShellType::GREEN_KOOPA_SHELL:
        knockVelocity = Constants::GreenKoopa::KNOCK_VELOCITY;

        break;
    case KoopaShellType::RED_KOOPA_SHELL:
        knockVelocity = Constants::RedKoopa::KNOCK_VELOCITY;
        break;
    default:
        break;
    }
    shell->velocity = { 0,knockVelocity};
    shell->curFrame = 0;
    shell->spritebox = TextureManager::Enemy_sprite_boxes[shell->getSpriteData()[0].first]; 
    shell->isKnocked = true; 
}

void KoopaShellKnockedState::exit(KoopaShell* shell) {
	shell->active = false; 
}

void KoopaShellKnockedState::update(KoopaShell* shell, float deltaTime) {

    if(shell->getPosition().y >= GetScreenHeight()) {
        shell->changeState(nullptr);
	}
    shell->curFrame %= shell->getSpriteData()[0].second - shell->getSpriteData()[0].first + 1;
    shell->spritebox = TextureManager::Enemy_sprite_boxes[shell->getSpriteData()[0].first + shell->curFrame];
}

//void KoopaShellKnockedState::checkCollision(KoopaShell* shell, const std::vector<std::shared_ptr<Object>>& objects) {
//
//}

ObjectCategory KoopaShellKnockedState::getObjectCategory() const {
    return ObjectCategory::BACKGROUND;
}
std::vector<ObjectCategory> KoopaShellKnockedState::getCollisionTargets() const {
    return {};
}