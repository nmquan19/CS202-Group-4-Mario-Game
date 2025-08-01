#include "..\..\include\Characters\Character.h"
#include "..\..\include\Characters\IdleState.h"
#include "..\..\include\Characters\MovingState.h"
#include "..\..\include\Characters\JumpingState.h"
#include "../../include/Characters/StunnedState.h"
#include "../../include/Characters/KnockedState.h"
#include "../../include/System/TextureManager.h"
#include "../../include/Enemy/Koopa/KoopaShell.h"
#include "../../include/System/Constant.h"
#include "../../include/Enemy/Enemy.h"
#include "../../include//Item/Item.h"
#include "../../include/System/Box2DWorldManager.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <variant>
#include <memory>
Character::Character(Vector2 startPosition, const CharacterStats& stats, const std::vector<std::vector<Rectangle>>& stateFrameData, CharacterType type, float scale)
	: characterType(type), velocity({ 0, 0 }), scale(scale), hp(1), projectile(nullptr), holdingProjectile(false),
	invincibleTimer(0), reviveTimer(0), 
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

	// Create Box2D body with proper hitbox size
	physicsBody = Box2DWorldManager::getInstance().createCharacterBody(position, {spriteRec.width * scale, spriteRec.height * scale});
	if (physicsBody) {
		physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
	}
}

Character::~Character() {
	if (physicsBody) {
		Box2DWorldManager::getInstance().destroyBody(physicsBody);
		physicsBody = nullptr;
	}
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

	// Update animation
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

	if (reviveTimer > 0) {
		reviveTimer -= deltaTime;
	}

	handleProjectile(deltaTime);

	// Sync with Box2D physics body
	if (physicsBody) {
		// Apply the current velocity to the physics body
		physicsBody->SetLinearVelocity(Box2DWorldManager::raylibToB2(velocity));
		// Update position from physics body
		auto newPos = Box2DWorldManager::b2ToRaylib(physicsBody->GetPosition());
		position = {newPos.x - hitBoxWidth/2.0f, newPos.y - hitBoxHeight/2.0f};
		// Update velocity from physics body
		velocity = Box2DWorldManager::b2ToRaylib(physicsBody->GetLinearVelocity());
	}
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
	if(canJump()){
		b2Vec2 currentVel = physicsBody->GetLinearVelocity();
		physicsBody->SetLinearVelocity(b2Vec2(currentVel.x, -Box2DWorldManager::raylibToB2(jumpForce)));
		setOnGround(false);
		groundContactCount = 0; // Reset ground contacts when jumping
	}
}

void Character::addGroundContact() {
	groundContactCount++;
	if (groundContactCount > 0) {
		setOnGround(true);
	}
}

void Character::removeGroundContact() {
	groundContactCount--;
	if (groundContactCount <= 0) {
		groundContactCount = 0;
		setOnGround(false);
	}
}

bool Character::canJump() const {
	return (groundContactCount > 0) && physicsBody;
}

void Character::setVelocity(Vector2 newVelocity){
	velocity = newVelocity;
	if (physicsBody) {
		physicsBody->SetLinearVelocity(Box2DWorldManager::raylibToB2(newVelocity));
	}
}

Vector2 Character::getVelocity(){
	if (physicsBody) {
		velocity = Box2DWorldManager::b2ToRaylib(physicsBody->GetLinearVelocity());
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
	if (physicsBody) {
		physicsBody->SetTransform(Box2DWorldManager::raylibToB2(newPosition), physicsBody->GetAngle());
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
	if (currentState == &StunnedState::getInstance()) {
		position.y = lastBottomY - hitBoxHeight;
	}
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

void Character::onCollision(std::shared_ptr<Object> other, Direction direction) {
	switch (other->getObjectCategory()) {
	case ObjectCategory::ENEMY:
		handleEnemyCollision(other, direction);
		break;
	case ObjectCategory::BLOCK:
		handleEnvironmentCollision(other, direction);
		break;
	case ObjectCategory::INTERACTIVE:
		handleInteractiveCollision(other, direction);
		break;
	case ObjectCategory::SHELL:
		// implement
		break;
	case ObjectCategory::ITEM:
		// implement 
		break;
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
	changeState(StunnedState::getInstance());
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

void Character::handleEnvironmentCollision(std::shared_ptr<Object> other, Direction direction) {
	if (direction == Direction::DOWN) {
		addGroundContact();
	}
	else if (direction == Direction::UP) {
		if (physicsBody) {
			b2Vec2 vel = physicsBody->GetLinearVelocity();
			if (vel.y < 0) { // Moving upward
				physicsBody->SetLinearVelocity(b2Vec2(vel.x, 0));
			}
		}
	}
}

void Character::handleEnemyCollision(std::shared_ptr<Object> other, Direction direction) {
	if (direction == Direction::DOWN && velocity.y > 0) {
		// Bouncing off enemy from above
		invincibleTimer = 0.2f;
		if (physicsBody) {
			physicsBody->SetLinearVelocity(b2Vec2(physicsBody->GetLinearVelocity().x, 
				-Box2DWorldManager::raylibToB2(Constants::Character::BOUNCE_AFTER_STRIKE_VELOCITY)));
		}

		setOnGround(false);
		if (auto enemy = dynamic_cast<Enemy*>(other.get())) {
			enemy->takeDamage(1);
		}
	}
	else {
		takeDamage(1);
	}
}

void Character::handleInteractiveCollision(std::shared_ptr<Object> other, Direction direction) {
    ObjectType objectType = other->getObjectType();
    if (auto* interactiveType = std::get_if<InteractiveType>(&objectType)) {
        switch (*interactiveType) {
		case InteractiveType::SPRING:	
			handleSpringCollision(std::dynamic_pointer_cast<Spring>(other), direction);
        }
    }
}

void Character::handleSpringCollision(std::shared_ptr<Spring> other, Direction direction) {
	if (direction == Direction::DOWN && velocity.y > 0) {
		velocity.y = Constants::Spring::BOUNCE_VELOCITY;
		changeState(JumpingState::getInstance());
		other->setBouncing(true);
		other->setAniTimer(0.0f);
		other->setBounceTimer(0.0f);
	}
}




