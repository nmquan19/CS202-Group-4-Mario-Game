#include "..\..\include\Characters\Character.h"
#include "..\..\include\Characters\IdleState.h"
#include "..\..\include\Characters\MovingState.h"
#include "..\..\include\Characters\JumpingState.h"
#include "../../include/System/TextureManager.h"
#include "../../include/Enemy/Koopa/KoopaShell.h"
#include "../../include/System/Constant.h"
#include "../../include/System/PhysicsManager.h"
#include "../../include/System/PhysicsBody.h"
#include "../../include/Game/GameContext.h"
#include <iostream>
#include <string>
#include <algorithm>

Character::Character(Vector2 startPosition, const CharacterStats& stats, const std::vector<std::vector<Rectangle>>& stateFrameData, CharacterType type, float scale)
	: characterType(type), velocity({ 0, 0 }), scale(scale), hp(5), projectile(nullptr), holdingProjectile(false),
	invincibleTimer(0), invincibleTime(2.0f),
	facingRight(true), currentFrame(0), currentStateRow(0), aniTimer(0), aniSpeed(0.2f) {

	this->position = startPosition;
	this->speed = stats.baseSpeed;
	this->jumpForce = stats.jumpForce;
	this->gravity = Constants::GRAVITY;
	this->stateFrameData = stateFrameData;
	this->spriteSheet = TextureManager::getInstance().getCharacterTexture();

    if (!stateFrameData.empty() && !stateFrameData[0].empty()) {
        setCurrentStateRow(0);
    }

    currentState = &IdleState::getInstance();
    currentState->enter(this);
}

Character::~Character() {
	
}

void Character::changeState(ICharacterState& newState) {
	if (currentState) {
		currentState->exit(this);
	}
	currentState = &newState;
	currentState->enter(this);
}

void Character::update(float deltaTime) {
	if (currentState) {
		currentState->update(this, deltaTime);
	}

	aniTimer += deltaTime;
	if(aniTimer >= aniSpeed){
		currentFrame = (currentFrame + 1) % stateFrameData[currentStateRow].size();
		spriteRec = getCurrentStateFrame();
		updateHitBox();
		aniTimer = 0;
	}

	if(invincibleTimer > 0) {
		invincibleTimer -= deltaTime;		
	}

	if(holdingProjectile && projectile != nullptr) {
		if(IsKeyPressed(KEY_X)) {
			Vector2 throwDirection = isFacingRight() ? Vector2{300.0f, -100.0f} : Vector2{-300.0f, -100.0f};
			
			// Get physics body and apply force
			auto physicsBody = PhysicsManager::getInstance().getPhysicsBody(
				GameContext::getInstance().getSharedPtrFromRaw(projectile)
			);
			if (physicsBody) {
				physicsBody->setVelocity(throwDirection);
			}
			
			setHoldingProjectile(false);
			projectile = nullptr;
		}
	}

	if(projectile) {
		projectile->update(deltaTime);
	}
	
	// Note: Physics (gravity, position updates) are now handled by Box2D
	// We just need to sync our position with the physics body
	auto physicsBody = PhysicsManager::getInstance().getPhysicsBody(
		GameContext::getInstance().getSharedPtrFromRaw(this)
	);
	if (physicsBody) {
		position = physicsBody->getPosition();
		velocity = physicsBody->getVelocity();
	}
}

void Character::draw() {
	Rectangle sourceRec = spriteRec;

	if(!isFacingRight()){
		sourceRec.width = -sourceRec.width;
	}

	Rectangle destRec = {
		position.x,
		position.y,
		abs(sourceRec.width) * scale,
		sourceRec.height * scale
	};

	DrawTexturePro(spriteSheet, sourceRec, destRec, {0, 0}, 0.0f, WHITE);
	std::string s = "hp: " + std::to_string(hp);
	DrawText(s.c_str(), 20, 460, 25, BLACK);

	if(holdingProjectile && projectile != nullptr) {
		DrawText("holding projectile", 20, 540, 25, BLACK);
		projectile->draw();
	}
}

Rectangle Character::getCurrentStateFrame() const{
	if(currentStateRow < stateFrameData.size() && currentFrame < stateFrameData[currentStateRow].size()){
		return stateFrameData[currentStateRow][currentFrame];
	}
	return {0, 0, 0, 0};
}

int Character::getCurrentStateRow() const {
	return currentStateRow;
}

void Character::setSpriteRec(Rectangle newRec){
	spriteRec = newRec;
}

void Character::setCurrentStateRow(int newRow){
	if(newRow < stateFrameData.size()){
		currentStateRow = newRow;
		currentFrame = 0;
		spriteRec = getCurrentStateFrame();
		updateHitBox();
	} 
}

void Character::setFrame(int newFrame){
	currentFrame = newFrame;
}

void Character::setAniTime(float newTime){
	aniTimer = newTime;
}

void Character::setAniSpeed(float newSpeed){
	aniSpeed = newSpeed;
}

void Character::setOnGround(bool flag){
	onGround = flag;
}

bool Character::isOnGround() const{
	return onGround;
}

void Character::jump(){
	if(onGround){
		// Use Box2D physics for jumping
		auto physicsBody = PhysicsManager::getInstance().getPhysicsBody(
			GameContext::getInstance().getSharedPtrFromRaw(this)
		);
		if (physicsBody) {
			Vector2 currentVel = physicsBody->getVelocity();
			currentVel.y = -jumpForce;
			physicsBody->setVelocity(currentVel);
		}
		setOnGround(false);
	}
}

void Character::applyGravity(float deltaTime){
	// Gravity is now handled by Box2D physics world
	// This method is kept for compatibility but does nothing
}

void Character::setVelocity(Vector2 newVelocity){
	velocity = newVelocity;
	
	// Update Box2D physics body
	auto physicsBody = PhysicsManager::getInstance().getPhysicsBody(
		GameContext::getInstance().getSharedPtrFromRaw(this)
	);
	if (physicsBody) {
		physicsBody->setVelocity(newVelocity);
	}
}

Vector2 Character::getVelocity(){
	// Get velocity from Box2D physics body if available
	auto physicsBody = PhysicsManager::getInstance().getPhysicsBody(
		GameContext::getInstance().getSharedPtrFromRaw(this)
	);
	if (physicsBody) {
		velocity = physicsBody->getVelocity();
	}
	return velocity;
}

void Character::setSpeed(float newSpeed){
	speed = newSpeed;
}

float Character::getSpeed(){
	return speed;
}

void Character::setPosition(Vector2 newPosition){
	position = newPosition;
	
	// Update Box2D physics body
	auto physicsBody = PhysicsManager::getInstance().getPhysicsBody(
		GameContext::getInstance().getSharedPtrFromRaw(this)
	);
	if (physicsBody) {
		physicsBody->setPosition(newPosition);
	}
}

Vector2 Character::getPosition() const {
	return position;
}

bool Character::isFacingRight() const {
	return facingRight;
}

void Character::setFacingRight(bool flag) {
	facingRight = flag;
}

void Character::updateHitBox(){
	hitBoxWidth = spriteRec.width * scale;
	hitBoxHeight = spriteRec.height * scale;
}

Rectangle Character::getHitBox() const {
	return {position.x, position.y, hitBoxWidth, hitBoxHeight};
}

bool Character::isActive() const {
	return active;
}

void Character::setActive(bool flag) {
	active = flag;
}

bool Character::isCollided() const {
	return collided;
}

void Character::setCollided(bool flag) {
	collided = flag;
}

ObjectType Character::getObjectType() const {
	return characterType;
}

Vector2 Character::getSize() const {
	return size;
}

ObjectCategory Character::getObjectCategory() const {
	return ObjectCategory::CHARACTER;
}

std::vector<ObjectCategory> Character::getCollisionTargets() const {
	return { ObjectCategory::BLOCK, ObjectCategory::ITEM, ObjectCategory::ENEMY };
}

void Character::checkCollision(const std::vector<std::shared_ptr<Object>>& candidates) {
	for (auto candidate : candidates) {
		switch(candidate->getObjectCategory()) {
			case ObjectCategory::ENEMY:
				handleEnemyCollision(candidate);
				takeDamage(1);
				break;
			case ObjectCategory::BLOCK:
				handleEnvironmentCollision(candidate);
				break;
			case ObjectCategory::PROJECTILE:
				// implement
				//position =  Vector2{position.x - 50, position.y - 20};
				break;
			case ObjectCategory::ITEM:
				// implement 
				break;
		}
	}
}


void Character::onCollision(std::shared_ptr<Object> other) {}

void Character::handleEnvironmentCollision(std::shared_ptr<Object> other) {
    Rectangle playerHitBox = getHitBox();
    Rectangle otherHitBox = other->getHitBox();

    float overlapLeft = (playerHitBox.x + playerHitBox.width) - otherHitBox.x;
    float overlapRight = (otherHitBox.x + otherHitBox.width) - playerHitBox.x;
    float overlapTop = (playerHitBox.y + playerHitBox.height) - otherHitBox.y;
    float overlapBottom = (otherHitBox.y + otherHitBox.height) - playerHitBox.y;
    
	const float MIN_OVERLAP = 2.0f;

	if(overlapTop < MIN_OVERLAP && overlapBottom < MIN_OVERLAP && overlapLeft < MIN_OVERLAP && overlapRight < MIN_OVERLAP) {
		return;
	}
	float minOverlap = std::min({overlapTop, overlapBottom, overlapLeft, overlapRight});

	if(minOverlap == overlapTop) {
		position.y = otherHitBox.y - playerHitBox.height;
		velocity.y = 0;
		setOnGround(true);
	}
	else if(minOverlap == overlapBottom) {
		position.y = otherHitBox.y + otherHitBox.height;
		if(velocity.y < 0) {
			velocity.y = 0;
		}
	}
	else if(minOverlap == overlapLeft && overlapLeft >= MIN_OVERLAP) {
		position.x = otherHitBox.x - playerHitBox.width;
		velocity.x = 0;
	}
	else if (minOverlap == overlapRight && overlapRight >= MIN_OVERLAP) {
		position.x = otherHitBox.x + otherHitBox.width;
		velocity.x = 0;
	}
}

void Character::handleEnemyCollision(std::shared_ptr<Object> other) {
    Rectangle characterHitbox = getHitBox();
    Rectangle otherHitbox = other->getHitBox();

    float overlapLeft = (characterHitbox.x + characterHitbox.width) - otherHitbox.x;
    float overlapRight = (otherHitbox.x + otherHitbox.width) - characterHitbox.x;
    float overlapTop = (characterHitbox.y + characterHitbox.height) - otherHitbox.y;
    float overlapBottom = (otherHitbox.y + otherHitbox.height) - characterHitbox.y;

    float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

    if (minOverlap == overlapTop) {
        DrawText("deal damage", 50, 200, 30, BLACK);
		invincibleTimer = 0.2f;
        velocity.y = -200.0f;
        setOnGround(false);
    }
    else {
		DrawText("got hit", 50, 200, 30, BLACK);
    }
}

float Character::getBottom() const {
	return position.y + spriteRec.height * scale;
}

float Character::getWidth() const {
	return spriteRec.width * scale;
}

float Character::getHeight() const {
	return spriteRec.height * scale;
}

float Character::getCenterX() const {
	return position.x + (spriteRec.width * scale) / 2;
}

float Character::getCenterY() const {
	return position.y + (spriteRec.height * scale) / 2;
}

Vector2 Character::getCenter() const {
	return Vector2{getCenterX(), getCenterY()};
}

void Character::takeDamage(int amount) {
	if (invincibleTimer > 0) {
		return;
	}
	hp -= amount;
	invincibleTimer = invincibleTime;
}

bool Character::isAlive() const {
	return hp > 0;
}

void Character::die() {

}

void Character::setHoldingProjectile(bool flag) {
	holdingProjectile = flag;
}

bool Character::isHoldingProjectile() const {
	return holdingProjectile;
}

void Character::holdProjectile(KoopaShell& p) {
	projectile = &p;
}