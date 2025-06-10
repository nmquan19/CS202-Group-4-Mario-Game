#include "..\..\include\Characters\Character.h"
#include "..\..\include\Characters\IdleState.h"
#include "..\..\include\Characters\MovingState.h"
#include "..\..\include\Characters\JumpingState.h"

Character::Character(CharacterType type, Vector2 startPosition, float scale) 
    : characterType(type), velocity({0, 0}), scale(scale),
	facingRight(true), currentFrame(0), currentStateRow(0), aniTimer(0), aniSpeed(0.2f) {

	setPosition(startPosition);
    loadCharacterData(type);

    if (!stateFrameData.empty() && !stateFrameData[0].empty()) {
        setCurrentStateRow(0);
    }

    currentState = &IdleState::getInstance();
    currentState->enter(this);

    PhysicsManager::getInstance().addObject(this);
}
 
Character::~Character() {
	UnloadTexture(spriteSheet);
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
		spriteRec = getCurrentStateFrame().toRectangle();
		updateHitBox();
		aniTimer = 0;
	}
	
	applyGravity(deltaTime);
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

	if (getBottom() >= groundLevel) {
        position.y = groundLevel - (spriteRec.height * scale);
        setOnGround(true);
        if (velocity.y > 0) {
            velocity.y = 0;
        }
    } else {
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
}

StateFrame Character::getCurrentStateFrame() const{
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
		spriteRec = getCurrentStateFrame().toRectangle();
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

void Character::setGroundLevel(float newGroundLevel) {
	groundLevel = newGroundLevel;
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

bool Character::isActive() {
	return active;
}

void Character::setActive(bool flag) {
	active = flag;
}

ObjectCategory Character::getObjectCategory() const {
	return ObjectCategory::CHARACTER;
}

std::vector<ObjectCategory> Character::getCollisionTargets() const {
	return { ObjectCategory::BLOCK, ObjectCategory::PROJECTILE, ObjectCategory::ITEM};
}

void Character::checkCollision(const std::vector<Object*>& candidates) {
	for (auto* candidate : candidates) {
		switch(candidate->getObjectCategory()) {
			case ObjectCategory::ENEMY:
				//handleEnemyCollision(candidate);
				break;
			case ObjectCategory::BLOCK:
				handleEnvironmentCollision(candidate);
				break;
			case ObjectCategory::PROJECTILE:
				// implement
				break;
			case ObjectCategory::ITEM:
				// implement
				break;
		}
	}
}

void Character::handleEnvironmentCollision(Object* other) {
    Rectangle playerHitBox = getHitBox();
    Rectangle otherHitBox = other->getHitBox();
    
    // Calculate overlap amounts for each direction
    float overlapLeft = (playerHitBox.x + playerHitBox.width) - otherHitBox.x;
    float overlapRight = (otherHitBox.x + otherHitBox.width) - playerHitBox.x;
    float overlapTop = (playerHitBox.y + playerHitBox.height) - otherHitBox.y;
    float overlapBottom = (otherHitBox.y + otherHitBox.height) - playerHitBox.y;
    
    if (overlapTop < overlapBottom && overlapTop < overlapLeft && overlapTop < overlapRight) {
        // Collision from top - player is on top of block
        position.y = otherHitBox.y - (spriteRec.height * scale);  // ← Use sprite dimensions, not hitbox
        setOnGround(true);
    }
    else if (overlapBottom < overlapTop && overlapBottom < overlapLeft && overlapBottom < overlapRight) {
        // Collision from bottom
        position.y = otherHitBox.y + otherHitBox.height;
        if (velocity.y < 0) {
            velocity.y = 0;
        }
    }
    else if (overlapLeft < overlapRight) {
        // Collision from left side
        position.x = otherHitBox.x - (spriteRec.width * scale);  // ← Use sprite dimensions
        velocity.x = 0;
    }
    else {
        // Collision from right side
        position.x = otherHitBox.x + otherHitBox.width;
        velocity.x = 0;
    }
}

void Character::loadCharacterData(CharacterType type){
	CharacterStats stats = getCharacterStats(type);
	speed = stats.baseSpeed;
	jumpForce = stats.jumpForce;
	gravity = stats.gravity;

	stateFrameData = getCharacterFrameData(type);

	const char* spritePath = getCharacterSpritePath(type);
	spriteSheet = LoadTexture(spritePath);
}

CharacterStats Character::getCharacterStats(CharacterType type){
	switch(type){
		case CharacterType::MARIO:
			return {220.0f, 550.0f, 980.0f};

		case CharacterType::LUIGI:
			return {175.0f, 730.0f, 980.0f};

		default:
			return  {250.0f, 550.0f, 980.0f};
	}
}

std::vector<std::vector<StateFrame>> Character::getCharacterFrameData(CharacterType type){
	switch(type){
		case CharacterType::MARIO:
			return {
				{{31, 21, 17, 43}, {111, 21, 17, 43}, {191, 20, 17, 44}, {271, 20, 17, 44}, {351, 20, 17, 44}}, // idle state
				{{31, 85, 19, 43}, {112, 85, 16, 43}, {193, 84, 15, 44}, {272, 84, 16, 44}, {351, 85, 18, 43}, {432, 85, 16, 43}, {512, 84, 16, 44}, {592, 84, 16, 44}}, // moving state
				{{30, 209, 19, 46}, {110, 209, 19, 46}, {190, 209, 19, 46}, {270, 209, 19, 46}} // jumping state
			};

		case CharacterType::LUIGI:
			return {
				{{31, 21, 17, 43}, {111, 21, 17, 43}, {191, 20, 17, 44}, {271, 20, 17, 44}, {351, 20, 17, 44}}, // idle state
				{{31, 85, 19, 43}, {112, 85, 16, 43}, {193, 84, 15, 44}, {272, 84, 16, 44}, {351, 85, 18, 43}, {432, 85, 16, 43}, {512, 84, 16, 44}, {592, 84, 16, 44}}, // moving state
				{{30, 209, 19, 46}, {110, 209, 19, 46}, {190, 209, 19, 46}, {270, 209, 19, 46}} // jumping state
			};

		default:
			return {
				{{31, 21, 17, 43}, {111, 21, 17, 43}, {191, 20, 17, 44}, {271, 20, 17, 44}, {351, 20, 17, 44}}, // idle state
				{{31, 85, 19, 43}, {112, 85, 16, 43}, {193, 84, 15, 44}, {272, 84, 16, 44}, {351, 85, 18, 43}, {432, 85, 16, 43}, {512, 84, 16, 44}, {592, 84, 16, 44}}, // moving state
				{{30, 209, 19, 46}, {110, 209, 19, 46}, {190, 209, 19, 46}, {270, 209, 19, 46}} // jumping state
			};
	}
}

const char* Character::getCharacterSpritePath(CharacterType type){
	switch(type){
		case CharacterType::MARIO:
			return "assets/mario_sprites.png";

		case CharacterType::LUIGI:
			return "assets/luigi_sprites.png";

		default:
			return "assets/mario_sprites.png";
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