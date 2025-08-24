#include "../../../include/Objects/InteractiveObjects/Spring.h"
#include "../../../include/System/Constant.h"
#include "../../../include/System/TextureManager.h"
#include "../../../include/System/Box2DWorldManager.h"
#include <iostream>

Spring::Spring(Vector2 position, Vector2 size) : InteractiveObject(position, size) {
	this->isBouncing = false;
	this->bounceTimer = 0.0f;
	this->aniTimer = 0.0f;
	this->aniSpeed = 0.2f;
	this->frameData = { {1, 467, 16, 16}, {18, 471, 16, 12}, {35, 476, 16, 7}, {18, 471, 16, 12}, {1, 467, 16, 16} };
	this->currentFrame = 0;
	this->spriteRec = frameData[0];
	this->bottomY = position.y + size.y * Constants::TILE_SIZE;
	this->physicsBody = Box2DWorldManager::getInstance().createBlockBody(position, { size.x * Constants::TILE_SIZE, size.y * Constants::TILE_SIZE });
	if (this->physicsBody) {
		this->physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
		for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
			b2Filter filter = fixture->GetFilterData();
			filter.maskBits = static_cast<uint16>(ObjectCategory::INTERACTIVE);
			filter.categoryBits = static_cast<uint16>(ObjectCategory::CHARACTER) | static_cast<uint16>(ObjectCategory::ENEMY) |
				static_cast<uint16>(ObjectCategory::SHELL) | static_cast<uint16>(ObjectCategory::ITEM) |
				static_cast<uint16>(ObjectCategory::PROJECTILE);
			fixture->SetFilterData(filter);
		}
	}
	interactiveType = InteractiveType::SPRING;
}

Spring::~Spring() {
	if (this->physicsBody) {
		Box2DWorldManager::getInstance().destroyBody(physicsBody);
		physicsBody = nullptr;
	}
}

void Spring::update(float deltaTime) {
	if (isBouncing) {
		bounceTimer += deltaTime;
		if (bounceTimer < Constants::Spring::BOUNCE_DURATION) {
			aniTimer += deltaTime;
			if (aniTimer >= aniSpeed) {
				currentFrame = (currentFrame + 1) % frameData.size();
				aniTimer = 0.0f;
			}
		}
		else {
			isBouncing = false;
			bounceTimer = 0.0f;
			currentFrame = 0;
		}
	}
	else {
		currentFrame = 0;
	}
	updateHitBox();
}

void Spring::draw() {
	Rectangle source = spriteRec; 
	Rectangle dest = hitBox;

	DrawTexturePro(TextureManager::interactiveTextures, source, dest, { 0, 0 }, 0.0f, WHITE);
}

void Spring::onCollision(std::shared_ptr<Object> other, Direction direction) {
	switch (other->getObjectCategory()) {
	case ObjectCategory::CHARACTER:
		if (direction == Direction::UP) {
			setBouncing(true);
			setAniTimer(0.0f);
			setBounceTimer(0.0f);
		}
		break;
	}
}

std::vector<Rectangle> Spring::getHitBox() const {
	return { hitBox };
}

ObjectType Spring::getObjectType() const {
	return InteractiveType::SPRING;
}

Vector2 Spring::getSize() const {
	return size;
}

void Spring::updateHitBox() {
	switch (currentFrame) {
	case 0:
		size.y = 1.0f;
		break;
	case 1:
		size.y = 0.75f;
		break;
	case 2:
		size.y = 0.5f;
		break;
	}
	Vector2 physicsBodyPosition = Box2DWorldManager::b2ToRaylib(this->physicsBody->GetPosition());
	Vector2 drawPosition = { physicsBodyPosition.x - size.x * Constants::TILE_SIZE * 0.5f, physicsBodyPosition.y - size.y * Constants::TILE_SIZE * 0.5f };
	hitBox = { drawPosition.x, bottomY - size.y * Constants::TILE_SIZE, size.x * Constants::TILE_SIZE, size.y * Constants::TILE_SIZE};
	spriteRec = frameData[currentFrame];
}

void Spring::setAniTimer(float val) {
	aniTimer = val;
}

void Spring::setBouncing(bool flag) {
	isBouncing = flag;
}

void Spring::setBounceTimer(float val) {
	bounceTimer = val;
}