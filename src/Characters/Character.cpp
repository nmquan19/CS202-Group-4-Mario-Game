#include "..\..\include\Characters\Character.h"
#include "..\..\include\Characters\IdleState.h"
#include "..\..\include\Characters\MovingState.h"
#include "..\..\include\Characters\JumpingState.h"
#include "../../include/Characters/StunnedState.h"
#include "../../include/Characters/KnockedState.h"
#include "../../include/Characters/FireTransformState.h"
#include "../../include/Characters/SuperTransformState.h"
#include "../../include/Characters/SmallTransformState.h"
#include "../../include/Characters/StarTransformState.h"
#include "../../include/Characters/RefreshPowerState.h"
#include "../../include/System/TextureManager.h"
#include "../../include/Enemy/Koopa/KoopaShell.h"
#include "../../include/System/Constant.h"
#include "../../include/Enemy/Enemy.h"
#include "../../include/Item/Item.h"
#include "../../include/System/Box2DWorldManager.h"
#include "../../include/Game/GameContext.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <variant>
#include <memory>
#include <cmath>

// Helper function to create rainbow colors
Color getRainbowColor(float time, float speed = 1.0f, float saturation = 1.0f, float value = 1.0f) {
	float hue = fmod(time * speed, 360.0f);
	
	// Convert HSV to RGB
	float c = value * saturation;
	float x = c * (1 - abs(fmod(hue / 60.0f, 2) - 1));
	float m = value - c;
	
	float r, g, b;
	if (hue >= 0 && hue < 60) {
		r = c; g = x; b = 0;
	} else if (hue >= 60 && hue < 120) {
		r = x; g = c; b = 0;
	} else if (hue >= 120 && hue < 180) {
		r = 0; g = c; b = x;
	} else if (hue >= 180 && hue < 240) {
		r = 0; g = x; b = c;
	} else if (hue >= 240 && hue < 300) {
		r = x; g = 0; b = c;
	} else {
		r = c; g = 0; b = x;
	}
	
	return Color{
		(unsigned char)((r + m) * 255),
		(unsigned char)((g + m) * 255),
		(unsigned char)((b + m) * 255),
		255
	};
}

Character::Character(Vector2 startPosition, const CharacterStats& stats, const std::vector<std::vector<Rectangle>>& stateFrameData, CharacterType type, PlayerID id, Vector2 size) 
	: characterType(type), id(id), hp(1), invincibleTimer(0), platform(nullptr),
	reviveTimer(0), facingRight(true), currentFrame(0), currentStateRow(0), aniTimer(0), aniSpeed(0.2f),
	rainbowEffect(false), rainbowTimer(0.0f) {

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
			filter.categoryBits = static_cast<uint16>(ObjectCategory::BLOCK) | static_cast<uint16>(ObjectCategory::CHARACTER) |
				static_cast<uint16>(ObjectCategory::ENEMY) | static_cast<uint16>(ObjectCategory::INTERACTIVE) | 
				static_cast<uint16>(ObjectCategory::ITEM) | static_cast<uint16>(ObjectCategory::SHELL) | 
				static_cast<uint16>(ObjectCategory::PROJECTILE)|static_cast<uint16>(ObjectCategory::TRIGGER);
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
		InputState input = InputState::fromPlayer(this->id);
		currentState->handleInput(this, input);
		currentState->checkTransitions(this, input);
		currentState->update(this, deltaTime);
	}

	aniTimer += deltaTime;
	if(aniTimer >= aniSpeed){
		currentFrame = (currentFrame + 1) % stateFrameData[currentStateRow].size();
		spriteRec = getCurrentStateFrame();
		aniTimer = 0;
	}

	if (rainbowEffect) {
		rainbowTimer += deltaTime;
	}
	else {
		rainbowTimer = 0.0f;
	}

	if(invincibleTimer > 0) {
		invincibleTimer -= deltaTime;		
	}

	if (isOnPlatform()) {
		b2Vec2 platformVel = platform->getPhysicsBody()->GetLinearVelocity();
		b2Vec2 characterVel = physicsBody->GetLinearVelocity();
		if (currentState == &IdleState::getInstance()) {
			physicsBody->SetLinearVelocity(b2Vec2(platformVel.x, characterVel.y));
		}
	}

	gridPosition = GridSystem::getGridCoord(getPosition());	
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

	// Rainbow effect
	Color tintColor = WHITE;
	
	if (rainbowEffect) {
		tintColor = getRainbowColor(rainbowTimer * 60.0f, 3.0f); // Speed = 3.0f
	}

	DrawTexturePro(spriteSheet, sourceRec, destRec, { 0, 0 }, 0.0f, tintColor);
	
	Vector2 indicatorSize = MeasureTextEx(GetFontDefault(), id == PlayerID::PLAYER_01 ? "P1" : "P2", 20.0f, 1.5f);
	Vector2 textPosition = { destRec.x + destRec.width * 0.5f - indicatorSize.x * 0.5f, destRec.y - indicatorSize.y * 0.5f - 10.0f };
	DrawTextPro(GetFontDefault(),
		id == PlayerID::PLAYER_01 ? "P1" : "P2",
		textPosition,
		{ 0, 0 },
		0.0f,
		20.0f,
		1.5f,
		BLACK
	);
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

bool Character::isOnPlatform() const {
	return platform;
}

void Character::jump(){
	if (isOnGround()) {
		b2Vec2 currentVel = this->physicsBody->GetLinearVelocity();
		float mass = this->physicsBody->GetMass();
		this->physicsBody->ApplyLinearImpulseToCenter(b2Vec2(0, mass * (-jumpVel - currentVel.y)), true);
		groundContactCount = 0;
		if (powerState == PowerState::SMALL) {
			AudioManager::getInstance().PlaySoundEffect("jump_small");
		}
		else {
			AudioManager::getInstance().PlaySoundEffect("jump_super");
		}
	}
}

void Character::addGroundContact() {
	groundContactCount++;
}

void Character::removeGroundContact() {
	groundContactCount--;
}

void Character::setPlatform(std::shared_ptr<MovingPlatform> platform) {
	this->platform = platform.get();
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
	Vector2 centerPosition = getCenterPos();
	Vector2 topLeftPosition = {
		centerPosition.x - size.x * Constants::TILE_SIZE * 0.5f,
		centerPosition.y - size.y * Constants::TILE_SIZE * 0.5f
	};
	return topLeftPosition;
}
Vector2 Character::getCenterPos()const
{
	return Box2DWorldManager::b2ToRaylib(this->physicsBody->GetPosition());
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
		takeDamage(1);
		break;
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
		handleItemCollision(other, direction);
		break;
	}
}

PowerState Character::getPowerState() const {
	return powerState;
}

void Character::takeDamage(int amount) {
	if (invincibleTimer > 0 || powerState == PowerState::STAR) {
		return;
	}
	if (powerState == PowerState::SMALL) {
		hp -= amount;
	}
	changeState(StunnedState::getInstance());
}

bool Character::isAlive() const {
	return hp > 0;
}

void Character::die() {

}

void Character::handleEnvironmentCollision(std::shared_ptr<Object> other, Direction direction) {
	if (direction == Direction::DOWN) {
		addGroundContact();
	}
}

void Character::handleEnemyCollision(std::shared_ptr<Object> other, Direction direction) {
	b2Vec2 currentVel = this->physicsBody->GetLinearVelocity();
	if (direction == Direction::DOWN) {
		UIManager::getInstance().addScore();
		AudioManager::getInstance().PlaySoundEffect("stomp");
		float mass = physicsBody->GetMass();
		physicsBody->ApplyLinearImpulseToCenter(b2Vec2(0, mass * (-Constants::Character::BOUNCE_AFTER_STRIKE_VELOCITY*1.2 - currentVel.y)), true);
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
			break;
		case InteractiveType::MOVING_PLATFORM:
			handleMovingPlatformCollision(std::dynamic_pointer_cast<MovingPlatform>(other), direction);
			break;
		case InteractiveType::FIRE_BAR:
			takeDamage(1);
			break;
		case InteractiveType::FIRE_BAR_BASE:
			if (direction == Direction::DOWN) {
				addGroundContact();
			}
			break;
        }
    }
}

void Character::handleSpringCollision(std::shared_ptr<Spring> other, Direction direction) {
	b2Vec2 currentVel = this->physicsBody->GetLinearVelocity();
	if (direction == Direction::DOWN) {
		float mass = this->physicsBody->GetMass();
		this->physicsBody->ApplyLinearImpulseToCenter(b2Vec2(0, mass * (-Constants::Spring::BOUNCE_VELOCITY - currentVel.y)), true);
		changeState(JumpingState::getInstance());
	}
}

void Character::handleMovingPlatformCollision(std::shared_ptr<MovingPlatform> other, Direction direction) {
	if (direction == Direction::DOWN) {
		addGroundContact();
		setPlatform(other);
	}
}

void Character::handleItemCollision(std::shared_ptr<Object> other, Direction direction) {
	ObjectType objectType = other->getObjectType();
	if (auto* itemType = std::get_if<ItemType>(&objectType)) {
		switch (*itemType) {
		case ItemType::COIN:
			AudioManager::getInstance().PlaySoundEffect("point");
			UIManager::getInstance().addCoin();
			UIManager::getInstance().addScore();
			break;
		case ItemType::FIRE_FLOWER:
			if (powerState == PowerState::FIRE) {
				changeState(RefreshPowerState::getInstance());
			}
			else if (powerState == PowerState::SMALL || powerState == PowerState::SUPER) {
				changeState(FireTransformState::getInstance());
			}
			break;
		case ItemType::MUSHROOM:
			if (powerState == PowerState::SUPER) {
				changeState(RefreshPowerState::getInstance());
			}
			else if (powerState == PowerState::SMALL) {
				changeState(SuperTransformState::getInstance());
			}
			break;
		case ItemType::ONE_UP:
			AudioManager::getInstance().PlaySoundEffect("one_up");
			hp += 1;
			changeState(RefreshPowerState::getInstance());
			break;
		case ItemType::STAR:
			if (powerState == PowerState::STAR) {
				starTimer = 5.0f;
				changeState(RefreshPowerState::getInstance());
			}
			else if (powerState == PowerState::SMALL || powerState == PowerState::SUPER || powerState == PowerState::FIRE) {
				changeState(StarTransformState::getInstance());
			}
			break;
		}
	}
}

void Character::enableRainbowEffect() {
	rainbowEffect = true;
	rainbowTimer = 0.0f;
}

void Character::disableRainbowEffect() {
	rainbowEffect = false;
}

bool Character::isRainbowEffectActive() const {
	return rainbowEffect;
}

json Character::toJson() const {
	json data;
	data["saveType"] = getSaveType();

	data["characterType"] = static_cast<int>(characterType);
	data["playerID"] = static_cast<int>(id);

	data["position"] = { position.x, position.y };
	data["gridPosition"] = { gridPosition.x, gridPosition.y };

	data["size"] = { size.x, size.y };
	data["hp"] = hp;
	data["projectilesLeft"] = projectilesLeft;
	data["powerState"] = static_cast<int>(powerState);

	return data;
}

void Character::fromJson(const json& data) {
	characterType = static_cast<CharacterType>(data["characterType"]);
	id = static_cast<PlayerID>(data["playerID"]);
	powerState = static_cast<PowerState>(data["powerState"]);
	position = { data["position"][0], data["position"][1] };
	gridPosition = { data["gridPosition"][0], data["gridPosition"][1] };
	size = { data["size"][0], data["size"][1] };
	hp = data["hp"];
	projectilesLeft = data["projectilesLeft"];

	switch (powerState) {
	case PowerState::SMALL:
		break;
	case PowerState::SUPER:
		changeState(SuperTransformState::getInstance());
		break;
	case PowerState::FIRE:
		changeState(FireTransformState::getInstance());
		break;
	case PowerState::STAR:
		changeState(StarTransformState::getInstance());
		break;
	}
	changeState(IdleState::getInstance());
}

std::string Character::getSaveType() const {
	return "Character";
}