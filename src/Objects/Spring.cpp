#include "../../include/Objects/Spring.h"
#include "../../include/System/Constant.h"
#include "../../include/System/TextureManager.h"
#include <iostream>

Spring::Spring(Vector2 position, Vector2 size) : InteractiveObject(position, size) {
	isBouncing = false;
	bounceTimer = 0.0f;
	aniTimer = 0.0f;
	aniSpeed = 0.2f;
	frameData = { {1, 467, 16, 16}, {18, 471, 16, 12}, {35, 476, 16, 7} };
	currentFrame = 0;
	spriteRec = frameData[0];
	hitBox = { position.x, position.y, size.x * Constants::TILE_SIZE, size.y * Constants::TILE_SIZE };
	bottomY = position.y + size.y * Constants::TILE_SIZE;
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

void Spring::checkCollision(const std::vector<std::shared_ptr<Object>>& candidates) {
	
}

void Spring::onCollision(std::shared_ptr<Object> other) {
	switch (other->getObjectCategory()) {
	case ObjectCategory::CHARACTER:
		isBouncing = true;
		currentFrame = 0;
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
	hitBox = { position.x, bottomY - size.y * Constants::TILE_SIZE, size.x * Constants::TILE_SIZE, size.y * Constants::TILE_SIZE};
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