#include "../../../include/Enemy/Koopa/KoopaShell.h"
#include <raylib.h>
#include "../../../include/Objects/ObjectFactory.h"
#include "../../../include/Objects/CollectableObject.h"
#include "../../../include/System/TextureManager.h"
#include "../../../include/Enemy/Koopa/KoopaShellState.h"
#include <vector>
#include "../../../include/System/Interface.h"
#include "../../../include/Characters/Character.h"
#include <raymath.h>
KoopaShell::KoopaShell(Vector2 pos, Vector2 sz): spriteBox({0,0,0,0}), velocity({0,0}), CollectableObject(pos, sz, TextureManager::enemyTextures) {   
    changeState(&KoopaShellIdleState::getInstance());
	knockedVelocity = { 0, -600};
}

void KoopaShell::onCollect(Character* player) {
    if (!player) return;
    TraceLog(LOG_INFO, "KoopaShell collected!");
	this->changeState(&KoopaShellCollectedState::getInstance());
}

void KoopaShell::update(float deltaTime) {

    CollectableObject::update(deltaTime); 
	if(currentState)currentState->update(this, deltaTime);
    if (pendingState) {
        applyQueueState();
    }
	position += velocity*deltaTime; 
    applyGravity(deltaTime);
    if (position.y + hitBox.height >= groundLevel) {
		position.y = groundLevel - hitBox.height;
        onGround = true;
        if (velocity.y > 0) {
            velocity.y = 0;
        }
    }
    else {
        onGround = false;
    }
  
}
void KoopaShell::setVelocity(Vector2 newVelocity) {
    velocity = newVelocity;
}
Vector2 KoopaShell::getVelocity() {
    return velocity;
}
void KoopaShell::onCollision(Object* other) {
    
}

void KoopaShell::draw() {
    Rectangle sourceRec = this->spriteBox;
    if (isKnocked)
    {
        sourceRec.height *= -1; 
    }
    Vector2 origin = { 0, 0 };
    DrawTexturePro(this->texture, sourceRec, getHitBox(), origin, 0.0f, WHITE);
}

void KoopaShell::changeState(KoopaShellState* newState) {
    if (currentState) currentState->exit(this);
    currentState = newState;
   if(currentState) currentState->enter(this);
}

void KoopaShell::checkCollision(const std::vector<Object*>& candidates)
{
	if (currentState == nullptr) return;
	currentState->checkCollision(this, candidates);
}

void KoopaShell ::applyGravity(float deltaTime) {
    if (!onGround) {
        velocity.y += 980 * deltaTime;
    }
}
void KoopaShell::handleEnvironmentCollision(Object* other) {
	Rectangle otherHitBox = other->getHitBox();
	Rectangle playerHitBox = getHitBox();
    int minOverlap = getCollidedPart(*other);
    if (minOverlap == (int)Direction::UP) {
        position.y = otherHitBox.y - playerHitBox.height;
        velocity.y = 0;
        onGround = true;
    }
    else if (minOverlap == (int)Direction::DOWN) {
        position.y = otherHitBox.y + otherHitBox.height;
        if (velocity.y > 0) {
            velocity.y = 0;
        }
    }
    else if (minOverlap ==(int)Direction::LEFT){
        position.x = otherHitBox.x - playerHitBox.width;
        velocity.x *= -1;
    }
    else if (minOverlap == (int)Direction::RIGHT){
        position.x = otherHitBox.x + otherHitBox.width;
        velocity.x *= -1;
    }
}
    
std::vector<ObjectCategory> KoopaShell::getCollisionTargets() const {    
	return currentState->getCollisionTargets();
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
void KoopaShell::applyQueueState()
{
   	changeState(pendingState);  
	pendingState = nullptr;  
}