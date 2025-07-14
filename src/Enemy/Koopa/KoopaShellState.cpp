#include "../../../include/Enemy/Koopa/KoopaShellState.h"
#include <vector>
#include "../../../include/Objects/ObjectFactory.h"
#include "../../../include/Enemy/Koopa/KoopaShell.h"
#include <raylib.h>
#include "../../include/Game/GameContext.h"
#include "../../../include/System/Interface.h"
void KoopaShellIdleState::enter(KoopaShell* shell) {
    shell->velocity = { 0,0 }; 
    shell->timer = 0.0f; 
}

void KoopaShellIdleState::exit(KoopaShell* shell) {
    // Nothing for now
}

void KoopaShellIdleState::update(KoopaShell* shell, float deltaTime) {
	shell->timer += deltaTime;
    DrawText("Koopa Shell Idle State", 300, 300, 20, RED);
    if (shell->timer > 6.0f) { 
        shell->changeState(&KoopaShellRevivingState::getInstance());
    }
}
ObjectCategory KoopaShellIdleState::getObjectCategory() const {
    return ObjectCategory::SHELL;
}
void KoopaShellIdleState::checkCollision(KoopaShell* shell, const std::vector<Object*>& objects) {
    for (auto* obj : objects) {
        {
            switch (obj->getObjectCategory())
            {
       			case ObjectCategory::CHARACTER:
                    DrawText("Collided", 200, 200, 20,RED);
                    shell->velocity = (obj->getPosition().x < shell->getPosition().x) ? Vector2{ 100,  shell->velocity.y} : Vector2{ -100, shell->velocity.y};
                    shell->changeState(&KoopaShellMovingState::getInstance());
                    break; 
                case ObjectCategory::BLOCK:
                    //DrawText("COLLIDED2", 200, 200, 20, RED);
                    shell->handleEnvironmentCollision(obj);
                    break; 
                case ObjectCategory::PROJECTILE:
					shell->changeState(&KoopaShellKnockedState::getInstance());
                    break; 
                break;
            }
        }
    }
}
std::vector<ObjectCategory> KoopaShellIdleState::getCollisionTargets() const {
    return { ObjectCategory::CHARACTER, ObjectCategory::BLOCK, ObjectCategory::PROJECTILE };
}

void KoopaShellMovingState::enter(KoopaShell* shell) {
       
}

void KoopaShellMovingState::exit(KoopaShell* shell) {
	shell->velocity = { 0, 0 };
}

void KoopaShellMovingState::update(KoopaShell* shell, float deltaTime) {
	DrawText("Koopa Shell Moving State", 300, 300, 20, RED);
}

void KoopaShellMovingState::checkCollision(KoopaShell* shell, const std::vector<Object*>& objects) {
    for (auto* obj : objects) {
        {
            switch (obj->getObjectCategory())
            {
            case ObjectCategory::CHARACTER:

                if(shell->getCollidedPart(*obj) == (int)Direction::DOWN)
                {
					shell->velocity = { 0, 0 };
                    shell->changeState(&KoopaShellIdleState::getInstance()); 
                }
                break;
            case ObjectCategory::BLOCK:
                shell->handleEnvironmentCollision(obj);
                break; 
            case ObjectCategory::PROJECTILE:
                shell->queueState(&KoopaShellKnockedState::getInstance());
                break;
            }
        }
    }
}
ObjectCategory KoopaShellMovingState::getObjectCategory() const {
    return ObjectCategory::PROJECTILE;
}
std::vector<ObjectCategory> KoopaShellMovingState::getCollisionTargets() const {
    return { ObjectCategory::CHARACTER, ObjectCategory::BLOCK, ObjectCategory::PROJECTILE};
}






void KoopaShellRevivingState::enter(KoopaShell* shell) {

}

void KoopaShellRevivingState::exit(KoopaShell* shell) {
    shell->active = false; 
    shell->velocity = { 0, 0 };
	shell->timer = 0.0f; 
    GameContext::getInstance().addObject(EnemyType::KOOPA, shell->getPosition(), { 1,1 });
}

void KoopaShellRevivingState::update(KoopaShell* shell, float deltaTime) {
	shell->timer += deltaTime;
    if(shell->timer > 3.0f) { 
		DrawText("REVIVED", 300, 300, 20, RED);
        shell->changeState(nullptr);
    }
    else
    {
        DrawText("REVIVING", 300, 300, 20, RED);
    }
}

void KoopaShellRevivingState::checkCollision(KoopaShell* shell, const std::vector<Object*>& objects) {
    for (auto* obj : objects) {
        {
            switch (obj->getObjectCategory())
            {
            case ObjectCategory::CHARACTER:
                if (shell->getCollidedPart(*obj) == (int)Direction::UP)
                {
                    shell->velocity = { 0, 0 };
                    shell->changeState(&KoopaShellIdleState::getInstance());
                }
                if (shell->getCollidedPart(*obj) == (int)Direction::RIGHT || shell->getCollidedPart(*obj) == (int)Direction::LEFT)
                {
                    shell->velocity = (obj->getPosition().x < shell->getPosition().x) ? Vector2{ 100, 0 } : Vector2{ -100, 0 };
                    shell->changeState(&KoopaShellMovingState::getInstance());
                }
                break;
            case ObjectCategory::BLOCK:
                shell->handleEnvironmentCollision(obj);
                break; 
            case ObjectCategory::PROJECTILE:
                shell->changeState(&KoopaShellKnockedState::getInstance());
                break;
            }
        }
    }
}
ObjectCategory KoopaShellRevivingState::getObjectCategory() const {
    return ObjectCategory::BLOCK;
}
std::vector<ObjectCategory> KoopaShellRevivingState::getCollisionTargets() const {
    return { ObjectCategory::CHARACTER, ObjectCategory::BLOCK, ObjectCategory::PROJECTILE };
}




void KoopaShellCollectedState::enter(KoopaShell* shell) {

}

void KoopaShellCollectedState::exit(KoopaShell* shell) {
    shell->changeState(&KoopaShellMovingState::getInstance());
}

void KoopaShellCollectedState::update(KoopaShell* shell, float deltaTime) {

}

void KoopaShellCollectedState::checkCollision(KoopaShell* shell, const std::vector<Object*>& objects) {
  
}
ObjectCategory KoopaShellCollectedState::getObjectCategory() const {
    return ObjectCategory::ITEM;
}
std::vector<ObjectCategory> KoopaShellCollectedState::getCollisionTargets() const {
    return {};
}



void KoopaShellKnockedState::enter(KoopaShell* shell) {
    shell->velocity = shell->knockedVelocity;
}

void KoopaShellKnockedState::exit(KoopaShell* shell) {
	shell->active = false; 
}

void KoopaShellKnockedState::update(KoopaShell* shell, float deltaTime) {

    if(shell->getPosition().y >= GetScreenHeight()) {
        shell->changeState(nullptr);
	}
}

void KoopaShellKnockedState::checkCollision(KoopaShell* shell, const std::vector<Object*>& objects) {

}
ObjectCategory KoopaShellKnockedState::getObjectCategory() const {
    return ObjectCategory::ITEM;
}
std::vector<ObjectCategory> KoopaShellKnockedState::getCollisionTargets() const {
    return {};
}