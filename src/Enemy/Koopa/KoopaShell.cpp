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
#include <raymath.h>
#include <utility>
#include <memory>
#include <iostream>
#include "../../../include/Game/GameContext.h"
KoopaShell::KoopaShell(Vector2 pos, Vector2 sz): spritebox({0,0,0,0}), velocity({0,0}), CollectableObject(pos, sz, TextureManager::enemyTextures), aniSpeed(0.2f), aniTimer(0) {   
	
}


void KoopaShell::onCollect(Character* player) {
    if (!player) return;
    std::cout << "On collect\n";
    TraceLog(LOG_INFO, "KoopaShell collected!");
    player->setHoldingProjectile(true);
    player->holdProjectile(*this);
	this->changeState(&KoopaShellCollectedState::getInstance());
    player->holdProjectile(*this);
    player->setHoldingProjectile(true);
}

void KoopaShell::update(float deltaTime) {
    aniTimer += deltaTime; 
    CollectableObject::update(deltaTime); 

	if(currentState)currentState->update(this, deltaTime);
    if (pendingState) {
        applyQueueState();
    }
	DrawText(TextFormat("KoopaShell pos %f",position.x), 100, 100, 20, RED);
	DrawText(TextFormat("KoopaShell velocity %f", velocity.x), 100, 120, 20, RED);
    position += velocity * deltaTime;
    applyGravity(deltaTime);
    if (aniTimer >= aniSpeed) {
        curFrame += 1;
        aniTimer = 0;
    }
   
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
    if (position.x < 0)
    {
        position.x = 0;
        velocity.x *= -1;
       
    }
    if (position.x > 1920 - hitBox.width)
    {
        position.x = 1920 - hitBox.width;
        velocity.x *= -1;
    }
}
void KoopaShell::setVelocity(Vector2 newVelocity) {
    velocity = newVelocity;
}
Vector2 KoopaShell::getVelocity() {
    return velocity;
}
void KoopaShell::onCollision(std::shared_ptr<Object> other, Direction dir) {
    
}

void KoopaShell::draw() {
    if (isActive()) {
        Rectangle sourceRec = this->spritebox;
        if (isKnocked)
        {
            sourceRec.height *= -1;
        }
        Vector2 origin = { 0, 0 };
        std::vector<Rectangle> hitBoxes = getHitBox();
        if (!hitBoxes.empty()) {
            DrawTexturePro(this->texture, sourceRec, hitBoxes[0], origin, 0.0f, WHITE);
        }
    }
}

void KoopaShell::changeState(KoopaShellState* newState) {
    if (currentState) currentState->exit(this);
    currentState = newState;
   if(currentState) currentState->enter(this);
}

void KoopaShell ::applyGravity(float deltaTime) {
    if (!onGround) {
        velocity.y += Constants::GRAVITY * deltaTime;
    }
}
void KoopaShell::handleEnvironmentCollision(std::shared_ptr<Object> other) {
    std::vector<Rectangle> koopaBoxes = getHitBox();
    std::vector<Rectangle> otherBoxes = other->getHitBox();
    
    if (koopaBoxes.empty() || otherBoxes.empty()) return;
    
    Rectangle koopaBox = koopaBoxes[0];
    Rectangle otherBox = otherBoxes[0];
    if (!CheckCollisionRecs(koopaBox, otherBox)) return;

    // Calculate previous position
    Vector2 prevPos = position - velocity*0.16;

    Rectangle prevBox = {
        prevPos.x,
        prevPos.y,
        koopaBox.width,
        koopaBox.height
    };

    bool wasAbove = (prevBox.y + prevBox.height) <= otherBox.y;
    bool wasBelow = prevBox.y >= (otherBox.y + otherBox.height);
    bool wasLeft = (prevBox.x + prevBox.width) <= otherBox.x;
    bool wasRight = prevBox.x >= (otherBox.x + otherBox.width);

    const float snapOffset = 0.01f; 

    if (wasAbove && velocity.y >= 0) {
        // Landed on top
        position.y = otherBox.y - koopaBox.height - snapOffset;
        velocity.y = 0;
        onGround = true;
    }
    else if (wasBelow && velocity.y <= 0) {
        position.y = otherBox.y + otherBox.height + snapOffset;
        velocity.y = 0;
    }
    else if (wasLeft && velocity.x >= 0) {
        // Hit from left
        position.x = otherBox.x - koopaBox.width - snapOffset;
        velocity.x *= -1;
    }
    else if (wasRight && velocity.x <= 0) {
        // Hit from right
        position.x = otherBox.x + otherBox.width + snapOffset;
        velocity.x *= -1;
    }
}
    
std::vector<ObjectCategory> KoopaShell::getCollisionTargets() const {
	if (!currentState) return {};
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
std::vector<std::pair<int, int>> KoopaShell::getSpriteData() const {
    switch(getType()){
        case KoopaShellType::GREEN_KOOPA_SHELL:
            return { {47,47},{49,52},{47, 48}};
        default:
            return { { 55,55},{57,60}, {55,56} };
	}
}


KoopaGreenShell::KoopaGreenShell(Vector2 pos, Vector2 sz) : KoopaShell(pos, sz) {
    changeState(&KoopaShellIdleState::getInstance());
}
KoopaShellType KoopaGreenShell::getType() const {
    return KoopaShellType::GREEN_KOOPA_SHELL;
}
KoopaRedShell::KoopaRedShell(Vector2 pos, Vector2 sz) : KoopaShell(pos, sz) {
    changeState(&KoopaShellIdleState::getInstance());
}
KoopaShellType KoopaRedShell::getType() const {
    return KoopaShellType::RED_KOOPA_SHELL;
}
ObjectType KoopaGreenShell::getObjectType() const {
    return KoopaShellType::GREEN_KOOPA_SHELL;
}
ObjectType KoopaRedShell::getObjectType() const {
    return KoopaShellType::RED_KOOPA_SHELL;
}
void KoopaShell::onRelease() {
    GameContext::getInstance().addObject(this->getType(), this->position, { 0.75,0.75 });
}   

std::shared_ptr<KoopaShell> KoopaGreenShell::clone()const {
    return std::make_shared<KoopaGreenShell>(*this);  
}
std::shared_ptr<KoopaShell> KoopaRedShell::clone() const {
    return std::make_shared<KoopaRedShell>(*this); 
}