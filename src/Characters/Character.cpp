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

Character::Character(Vector2 startPosition, const CharacterStats& stats, const std::vector<std::vector<Rectangle>>& stateFrameData, CharacterType type, Vector2 size) 
	: characterType(type), hp(1), projectile(nullptr), holdingProjectile(false), invincibleTimer(0), reviveTimer(0),
	facingRight(true), currentFrame(0), currentStateRow(0), aniTimer(0), aniSpeed(0.2f) {

	this->stateFrameData = stateFrameData;
	this->spriteSheet = TextureManager::getInstance().getCharacterTexture();
	this->speed = stats.baseSpeed;
	this->jumpVel = stats.jumpVel;
	this->size = size;

	if (!stateFrameData.empty() && !stateFrameData[0].empty()) {
		setCurrentStateRow(0);
	}

	physicsBody = Box2DWorldManager::getInstance().createCharacterBody(startPosition, { size.x * Constants::TILE_SIZE, size.y * Constants::TILE_SIZE });
	if (physicsBody) {
		physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
		for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
			b2Filter filter = fixture->GetFilterData();
			filter.maskBits = static_cast<uint16>(ObjectCategory::CHARACTER);
			filter.categoryBits = static_cast<uint16>(ObjectCategory::BLOCK) | static_cast<uint16>(ObjectCategory::ENEMY) |
				static_cast<uint16>(ObjectCategory::INTERACTIVE) | static_cast<uint16>(ObjectCategory::ITEM) |
				static_cast<uint16>(ObjectCategory::SHELL) | static_cast<uint16>(ObjectCategory::PROJECTILE);
			fixture->SetFilterData(filter);
		}
	}

	powerState = PowerState::SMALL;
    currentState = &IdleState::getInstance();
    currentState->enter(this);
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

	aniTimer += deltaTime;
	if(aniTimer >= aniSpeed){
		currentFrame = (currentFrame + 1) % stateFrameData[currentStateRow].size();
		spriteRec = getCurrentStateFrame();
		aniTimer = 0;
	}

	if(invincibleTimer > 0) {
		invincibleTimer -= deltaTime;		
	}

	handleProjectile(deltaTime);
}

void Character::draw() {
	Rectangle sourceRec = spriteRec;

	if (!isFacingRight()) {
		sourceRec.width = -sourceRec.width;
	}

	Vector2 centerPosition = Box2DWorldManager::b2ToRaylib(this->physicsBody->GetPosition());
	Vector2 drawPosition = { centerPosition.x - size.x * Constants::TILE_SIZE * 0.5f, 
		centerPosition.y - size.y * Constants::TILE_SIZE * 0.5f };
	Rectangle reference = stateFrameData[0][0];

	Rectangle destRec = {
		drawPosition.x,
		drawPosition.y + size.y * Constants::TILE_SIZE - spriteRec.height / reference.height * size.y * Constants::TILE_SIZE,
		spriteRec.width / reference.width * size.x * Constants::TILE_SIZE,
		spriteRec.height / reference.height * size.y * Constants::TILE_SIZE
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

bool Character::isOnGround() const{
	return groundContactCount > 0;
}

void Character::jump(){
	if (isOnGround()) {
		b2Vec2 currentVel = this->physicsBody->GetLinearVelocity();
		float mass = this->physicsBody->GetMass();
		this->physicsBody->ApplyLinearImpulseToCenter(b2Vec2(0, mass * (-jumpVel - currentVel.y)), true);
		groundContactCount = 0;
	}
}

void Character::addGroundContact() {
	groundContactCount++;
}

void Character::removeGroundContact() {
	groundContactCount--;
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

std::vector<Rectangle> Character::getHitBox() const {
	Vector2 centerPosition = Box2DWorldManager::b2ToRaylib(this->physicsBody->GetPosition());
	Rectangle hitbox = {
		centerPosition.x - size.x * Constants::TILE_SIZE * 0.5f,
		centerPosition.y - size.y * Constants::TILE_SIZE * 0.5f,
		size.x * Constants::TILE_SIZE,
		size.y * Constants::TILE_SIZE
	};
	return {hitbox};
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
	case ObjectCategory::PROJECTILE:
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
		invincibleTimer = Constants::Character::INVINCIBLE_TIME_AFTER_STRIKE;
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
}

void Character::handleEnemyCollision(std::shared_ptr<Object> other, Direction direction) {
	b2Vec2 currentVel = this->physicsBody->GetLinearVelocity();
	if (direction == Direction::DOWN) {
		float mass = physicsBody->GetMass();
		physicsBody->ApplyLinearImpulseToCenter(b2Vec2(0, mass * (-Constants::Character::BOUNCE_AFTER_STRIKE_VELOCITY - currentVel.y)), true);
		changeState(JumpingState::getInstance());
		invincibleTimer = Constants::Character::INVINCIBLE_TIME_AFTER_STRIKE;
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

void Character::handleSpringCollision(std::shared_ptr<Object> other, Direction direction) {
	b2Vec2 currentVel = this->physicsBody->GetLinearVelocity();
	if (direction == Direction::DOWN) {
		float mass = this->physicsBody->GetMass();
		this->physicsBody->ApplyLinearImpulseToCenter(b2Vec2(0, mass * (-Constants::Spring::BOUNCE_VELOCITY - currentVel.y)), true);
		changeState(JumpingState::getInstance());
	}
}




