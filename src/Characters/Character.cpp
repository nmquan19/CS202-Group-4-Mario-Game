#include "..\..\include\Characters\Character.h"
#include "..\..\include\Characters\IdleState.h"
#include "..\..\include\Characters\MovingState.h"
#include "..\..\include\Characters\JumpingState.h"
#include "../../include/System/TextureManager.h"
#include <iostream>
#include <string>

Character::Character(Vector2 startPosition, const CharacterStats& stats, const std::vector<std::vector<Rectangle>>& stateFrameData, CharacterType type, float scale)
	: characterType(type), velocity({ 0, 0 }), scale(scale), hp(5), heldProjectile(nullptr), isHoldingProjectile(false),
	invincibleTimer(0), invincibleTime(2.0f),
	facingRight(true), currentFrame(0), currentStateRow(0), aniTimer(0), aniSpeed(0.2f) {

	this->position = startPosition;
	this->speed = stats.baseSpeed;
	this->jumpForce = stats.jumpForce;
	//this->gravity = stats.gravity;
	this->gravity = 490.0f;
	this->stateFrameData = stateFrameData;
	this->spriteSheet = TextureManager::getInstance().getCharacterTexture(type);

    if (!stateFrameData.empty() && !stateFrameData[0].empty()) {
        setCurrentStateRow(0);
    }

    currentState = &IdleState::getInstance();
    currentState->enter(this);

    PhysicsManager::getInstance().addObject(this);
}

Character::~Character() {
	PhysicsManager::getInstance().markForDeletion(this);
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
	
	applyGravity(deltaTime);
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
	
	if (!onGround) {
        return;
    }

    Rectangle groundCheckBox = {
        position.x + 5,
        position.y + (spriteRec.height * scale),
        (spriteRec.width * scale) - 10,
        5.0f
    };

    std::vector<Object*> nearbyObjects = PhysicsManager::getInstance().getObjectsInArea(groundCheckBox);
    
    bool stillOnGround = false;
    for (auto* obj : nearbyObjects) {
        if (obj != this && obj->getObjectCategory() == ObjectCategory::BLOCK) {
            if (CheckCollisionRecs(groundCheckBox, obj->getHitBox())) {
                stillOnGround = true;
                break;
            }
        }
    }
    
    if (!stillOnGround) {
        setOnGround(false);
    }
}

void Character::draw() {
	Rectangle sourceRec = spriteRec;

	if(isFacingRight()){
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
	DrawText(s.c_str(), 20, 460, 20, BLACK);
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

ObjectCategory Character::getObjectCategory() const {
	return ObjectCategory::CHARACTER;
}

std::vector<ObjectCategory> Character::getCollisionTargets() const {
	return { ObjectCategory::BLOCK, ObjectCategory::ITEM, ObjectCategory::ENEMY };
}

void Character::checkCollision(const std::vector<Object*>& candidates) {
	for (auto* candidate : candidates) {
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

void Character::onCollision(Object* other) {
	
}

void Character::handleEnvironmentCollision(Object* other) {
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

void Character::handleEnemyCollision(Object* other) {
    Rectangle characterHitbox = getHitBox();
    Rectangle otherHitbox = other->getHitBox();

    float overlapLeft = (characterHitbox.x + characterHitbox.width) - otherHitbox.x;
    float overlapRight = (otherHitbox.x + otherHitbox.width) - characterHitbox.x;
    float overlapTop = (characterHitbox.y + characterHitbox.height) - otherHitbox.y;
    float overlapBottom = (otherHitbox.y + otherHitbox.height) - characterHitbox.y;

    const float MIN_OVERLAP = 2.0f;

    if (overlapLeft <= MIN_OVERLAP || overlapRight <= MIN_OVERLAP || overlapTop <= MIN_OVERLAP || overlapBottom <= MIN_OVERLAP) {
        return;
    }

    float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

    if (minOverlap == overlapTop) {
        DrawText("deal damage", 50, 200, 30, BLACK);
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
	return alive;
}

void Character::die() {

}