#include "../../../include/Enemy/Koopa/KoopaShell.h"
#include <raylib.h>
#include "../../../include/Objects/ObjectFactory.h"
#include "../../../include/Objects/CollectableObject.h"
#include "../../../include/System/TextureManager.h"
#include "../../../include/Enemy/Koopa/KoopaShellState.h"
#include <vector>
#include "../../../include/System/Interface.h"
#include "../../../include/Characters/Character.h"
#include "../../../include/System/Constant.h"
#include "../../../include/System/Box2DWorldManager.h"
#include <raymath.h>
#include <utility>
#include <memory>
#include <iostream>
#include "../../../include/Game/GameContext.h"

KoopaShell::KoopaShell(KoopaShellType type, Vector2 pos, Vector2 sz): type(type), spritebox({0,0,0,0}), CollectableObject(pos, sz, TextureManager::enemyTextures), aniSpeed(0.2f), aniTimer(0) {   
    physicsBody = Box2DWorldManager::getInstance().createEnemyBody(position, { sz.x * Constants::TILE_SIZE, sz.y * Constants::TILE_SIZE});
    if (physicsBody) {
        physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
        //for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
        //    b2Filter filter = fixture->GetFilterData();
        //    filter.maskBits = static_cast<uint16>(ObjectCategory::SHELL);
        //    filter.categoryBits = static_cast<uint16> (ObjectCategory::CHARACTER) | static_cast<uint16>(ObjectCategory::BLOCK) | static_cast<uint16>(ObjectCategory::PROJECTILE);
        //    fixture->SetFilterData(filter);
        //}
    }
    currentState = &KoopaShellIdleState::getInstance();
    currentState->enter(this);
}


void KoopaShell::onCollect(Character* player) {
    if (!player) return;
    std::cout << "On collect\n";
    TraceLog(LOG_INFO, "KoopaShell collected!");
    player->setHoldingProjectile(true);
    player->holdProjectile(*this);
	//this->changeState(&KoopaShellCollectedState::getInstance());
    player->holdProjectile(*this);
    player->setHoldingProjectile(true);
}

void KoopaShell::update(float deltaTime) {
    aniTimer += deltaTime; 
    //CollectableObject::update(deltaTime); 

	if(currentState)currentState->update(this, deltaTime);

    if (pendingState) {
        applyQueueState();
    }

    if (aniTimer >= aniSpeed) {
        curFrame += 1;
        aniTimer = 0;
    }
   
    if (physicsBody) {
        b2Vec2 b2Pos = physicsBody->GetPosition();
        Vector2 bodyPos = Box2DWorldManager::b2ToRaylib(b2Pos);
        position.x = bodyPos.x - hitBox.width * 0.5f;
        position.y = bodyPos.y - hitBox.height * 0.5f;
        hitBox.x = position.x;
        hitBox.y = position.y;
    }
}

void KoopaShell::setVelocity(Vector2 newVelocity) {
    velocity = newVelocity;
}

Vector2 KoopaShell::getVelocity() {
    return velocity;
}

void KoopaShell::onCollision(std::shared_ptr<Object> other, Direction dir) {
    if (currentState) {
        currentState->onCollision(this, other, dir);
    }
}

void KoopaShell::handleEnvironmentCollision(std::shared_ptr<Object> other, Direction dir) {

}

void KoopaShell::draw() {
    Rectangle sourceRec = this->spritebox;
    if (isKnocked)
    {
        sourceRec.height *= -1;
    }
    Vector2 origin = { 0, 0 };
    DrawTexturePro(this->texture, sourceRec, hitBox, origin, 0.0f, WHITE);
}

void KoopaShell::changeState(KoopaShellState* newState) {
    if (currentState == newState) {
        return;
    }

    if (currentState) {
        currentState->exit(this);
    }

    currentState = newState;

    if (newState) {
        currentState->enter(this);
    }
}
    
std::vector<ObjectCategory> KoopaShell::getCollisionTargets() const {
    return { ObjectCategory::CHARACTER, ObjectCategory::BLOCK, ObjectCategory::PROJECTILE};
}

bool KoopaShell::isMovingState() const
{
    return currentState == &KoopaShellMovingState::getInstance();  
}

ObjectCategory KoopaShell::getObjectCategory() const
{
    if (currentState)
    {
		return currentState->getObjectCategory();
    }
    return { };
}

void KoopaShell::queueState(KoopaShellState* newState) {
	pendingState = newState;
}

void KoopaShell::applyQueueState() {
   	changeState(pendingState);  
	pendingState = nullptr;  
}

std::vector<std::pair<int, int>> KoopaShell::getSpriteData() const {
    switch(type){
        case KoopaShellType::GREEN_KOOPA_SHELL:
            return { {47,47},{49,52},{47, 48}};
        default:
            return { { 55,55},{57,60}, {55,56} };
	}
}

//KoopaGreenShell::KoopaGreenShell(Vector2 pos, Vector2 sz) : KoopaShell(pos, sz) {
//    changeState(&KoopaShellIdleState::getInstance());
//}
//KoopaShellType KoopaGreenShell::getType() const {
//    return KoopaShellType::GREEN_KOOPA_SHELL;
//}
//KoopaRedShell::KoopaRedShell(Vector2 pos, Vector2 sz) : KoopaShell(pos, sz) {
//    changeState(&KoopaShellIdleState::getInstance());
//}
//KoopaShellType KoopaRedShell::getType() const {
//    return KoopaShellType::RED_KOOPA_SHELL;
//}
//ObjectType KoopaGreenShell::getObjectType() const {
//    return KoopaShellType::GREEN_KOOPA_SHELL;
//}
//ObjectType KoopaRedShell::getObjectType() const {
//    return KoopaShellType::RED_KOOPA_SHELL;
//}
//void KoopaShell::onRelease() {
//    GameContext::getInstance().addObject(this->getType(), this->position, { 0.75,0.75 });
//}   
//
//std::shared_ptr<KoopaShell> KoopaGreenShell::clone()const {
//    return std::make_shared<KoopaGreenShell>(*this);  
//}
//std::shared_ptr<KoopaShell> KoopaRedShell::clone() const {
//    return std::make_shared<KoopaRedShell>(*this); 
//}