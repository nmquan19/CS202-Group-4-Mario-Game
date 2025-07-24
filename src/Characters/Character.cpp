#include "..\..\include\Characters\Character.h"
#include "..\..\include\Characters\IdleState.h"
#include "..\..\include\Characters\MovingState.h"
#include "..\..\include\Characters\JumpingState.h"
#include "../../include/System/TextureManager.h"
#include "../../include/Enemy/Koopa/KoopaShell.h"
#include "../../include/System/Constant.h"
#include "../../include/Enemy/Enemy.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <variant>
#include <memory>
Character::Character(Vector2 startPosition, const CharacterStats& stats, const std::vector<std::vector<Rectangle>>& stateFrameData, CharacterType type, float scale)
	: characterType(type), velocity({ 0, 0 }), scale(scale), hp(5), projectile(nullptr), holdingProjectile(false),
	invincibleTimer(0),
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

	handleProjectile(deltaTime);
	
	applyGravity(deltaTime);
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
	
	handleGroundCheck();
}

void Character::draw() {
	Rectangle sourceRec = spriteRec;

	if (!isFacingRight()) {
		sourceRec.width = -sourceRec.width;
	}

	Rectangle destRec = {
		position.x,
		position.y,
		abs(sourceRec.width) * scale,
		sourceRec.height * scale
	};

	DrawTexturePro(spriteSheet, sourceRec, destRec, { 0, 0 }, 0.0f, WHITE);

	if(holdingProjectile && projectile != nullptr) {
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
		velocity.y = -jumpForce;
		setOnGround(false);
	}
}

void Character::applyGravity(float deltaTime){
	if(!onGround){
		velocity.y += gravity * deltaTime;
	}
}

void Character::setVelocity(Vector2 newVelocity){
	velocity = newVelocity;
}

Vector2 Character::getVelocity(){
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

std::vector<Rectangle> Character::getHitBox() const {
	return {{position.x, position.y, hitBoxWidth, hitBoxHeight}};
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
	return { ObjectCategory::BLOCK, ObjectCategory::ITEM, ObjectCategory::ENEMY, ObjectCategory::INTERACTIVE, ObjectCategory::SHELL };
}

void Character::checkCollision(const std::vector<std::shared_ptr<Object>>& candidates) {
	for (auto candidate : candidates) {
		switch(candidate->getObjectCategory()) {
			case ObjectCategory::ENEMY:
				handleEnemyCollision(candidate);
				break;
			case ObjectCategory::BLOCK:
				handleEnvironmentCollision(candidate);
				break;
			case ObjectCategory::INTERACTIVE:
				handleInteractiveCollision(candidate);
				break;
			case ObjectCategory::SHELL:
				// implement
				break;
			case ObjectCategory::ITEM:
				// implement 
				break;
		}
	}
}


void Character::onCollision(std::shared_ptr<Object> other) {

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
	invincibleTimer = 1.0f;
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

void Character::handleProjectile(float deltaTime) {
	if (holdingProjectile && projectile != nullptr) {
		if (IsKeyPressed(KEY_X)) {
			projectile->setPosition(Vector2{ this->position.x + (this->isFacingRight() ? this->getWidth() : -20.0f), this->getCenterY() });
			projectile->onRelease();
			projectile->changeState(nullptr);
			projectile = nullptr;
		}
	}

	if (projectile) {
		projectile->update(deltaTime);
	}
}

void Character::handleGroundCheck() {
	if (!onGround) {
		return;
	}

	Rectangle groundCheckBox = {
		position.x + 5,
		position.y + (spriteRec.height * scale),
		(spriteRec.width * scale) - 10,
		1.0f
	};

	std::vector<std::shared_ptr<Object>> nearbyObjects = PhysicsManager::getInstance().getObjectsInArea(groundCheckBox);

	bool stillOnGround = false;
	for (auto obj : nearbyObjects) {
		if (obj.get() != this && obj->getObjectCategory() == ObjectCategory::BLOCK) {
			if (CheckCollisionRecs(groundCheckBox, obj->getHitBox()[0])) {
				stillOnGround = true;
				break;
			}
		}
	}

	if (!stillOnGround) {
		setOnGround(false);
	}
}

void Character::handleEnvironmentCollision(std::shared_ptr<Object> other) {
	std::vector<Rectangle> playerHitBoxes = getHitBox();
	std::vector<Rectangle> otherHitBoxes = other->getHitBox();

	if (playerHitBoxes.empty() || otherHitBoxes.empty()) return;

	Rectangle playerHitBox = playerHitBoxes[0];
	Rectangle otherHitBox = otherHitBoxes[0];

	float overlapLeft = (playerHitBox.x + playerHitBox.width) - otherHitBox.x;
	float overlapRight = (otherHitBox.x + otherHitBox.width) - playerHitBox.x;
	float overlapTop = (playerHitBox.y + playerHitBox.height) - otherHitBox.y;
	float overlapBottom = (otherHitBox.y + otherHitBox.height) - playerHitBox.y;

	float minOverlap = std::min({ overlapTop, overlapBottom, overlapLeft, overlapRight });

	if (minOverlap == overlapLeft) {
		position.x = otherHitBox.x - playerHitBox.width;
		velocity.x = 0;
	}
	else if (minOverlap == overlapRight) {
		position.x = otherHitBox.x + otherHitBox.width;
		velocity.x = 0;
	}
	else if (minOverlap == overlapTop) {
		position.y = otherHitBox.y - playerHitBox.height;
		velocity.y = 0;
		setOnGround(true);
	}
	else if (minOverlap == overlapBottom) {
		position.y = otherHitBox.y + otherHitBox.height;
		if (velocity.y < 0) {
			velocity.y = 0;
		}
	}
}

void Character::handleEnemyCollision(std::shared_ptr<Object> other) {
	std::vector<Rectangle> characterHitboxes = getHitBox();
	std::vector<Rectangle> otherHitboxes = other->getHitBox();

	if (characterHitboxes.empty() || otherHitboxes.empty()) return;

	Rectangle characterHitbox = characterHitboxes[0];
	Rectangle otherHitbox = otherHitboxes[0];

	float overlapLeft = (characterHitbox.x + characterHitbox.width) - otherHitbox.x;
	float overlapRight = (otherHitbox.x + otherHitbox.width) - characterHitbox.x;
	float overlapTop = (characterHitbox.y + characterHitbox.height) - otherHitbox.y;
	float overlapBottom = (otherHitbox.y + otherHitbox.height) - characterHitbox.y;
	float minOverlap = std::min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

	if (minOverlap == overlapTop && velocity.y > 0) {
		invincibleTimer = 0.2f;
		velocity.y = Constants::Character::BOUNCE_AFTER_STRIKE_VELOCITY;
		setOnGround(false);
		auto enemy = dynamic_cast<Enemy*>(other.get());
		enemy->takeDamage(1);
	}
	else {
		takeDamage(1);
		DrawText("got hit", 50, 200, 30, BLACK);
	}
}

void Character::handleInteractiveCollision(std::shared_ptr<Object> other) {
    ObjectType objectType = other->getObjectType();
    if (auto* interactiveType = std::get_if<InteractiveType>(&objectType)) {
        switch (*interactiveType) {
		case InteractiveType::SPRING:	
			handleSpringCollision(std::dynamic_pointer_cast<Spring>(other));
        }
    }
}

void Character::handleSpringCollision(std::shared_ptr<Spring> other) {
	std::vector<Rectangle> characterHitboxes = getHitBox();
	std::vector<Rectangle> otherHitboxes = other->getHitBox();

	if (characterHitboxes.empty() || otherHitboxes.empty()) return;

	Rectangle characterHitbox = characterHitboxes[0];
	Rectangle otherHitbox = otherHitboxes[0];

	float overlapLeft = (characterHitbox.x + characterHitbox.width) - otherHitbox.x;
	float overlapRight = (otherHitbox.x + otherHitbox.width) - characterHitbox.x;
	float overlapTop = (characterHitbox.y + characterHitbox.height) - otherHitbox.y;
	float overlapBottom = (otherHitbox.y + otherHitbox.height) - characterHitbox.y;
	float minOverlap = std::min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

	if (minOverlap == overlapTop && velocity.y > 0) {
		velocity.y = Constants::Spring::BOUNCE_VELOCITY;
		changeState(JumpingState::getInstance());
		other->setBouncing(true);
		other->setAniTimer(0.0f);
		other->setBounceTimer(0.0f);
	}
	else if (minOverlap == overlapBottom) {
		position.y = otherHitbox.y + otherHitbox.height;
		if (velocity.y < 0) {
			velocity.y = 0;
		}
	}
	else if (minOverlap == overlapLeft) {
		position.x = otherHitbox.x - characterHitbox.width;
		velocity.x = 0;
	}
	else if (minOverlap == overlapRight) {
		position.x = otherHitbox.x + otherHitbox.width;
		velocity.x = 0;
	}
}