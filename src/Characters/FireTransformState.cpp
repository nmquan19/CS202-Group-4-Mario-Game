#include "../../include/Characters/FireTransformState.h"
#include "../../include/Characters/IdleState.h"
#include "../../include/System/Box2DWorldManager.h"
#include "../../include/Characters/Character.h"

void FireTransformState::enter(Character* character) {
	character->setAniTime(0.0f);
	character->setAniSpeed(0.2f);
	character->transformTimer = Constants::Character::TRANSFORM_DURATION;
	character->setCurrentStateRow(6);
	character->projectilesLeft = 1;
	for (b2Fixture* fixture = character->physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		b2Filter filter = fixture->GetFilterData();
		filter.maskBits = static_cast<uint16>(ObjectCategory::CHARACTER);
		filter.categoryBits = static_cast<uint16>(ObjectCategory::BLOCK);
		fixture->SetFilterData(filter);
	}
}

void FireTransformState::update(Character* character, float deltaTime) {
	if (character->transformTimer > 0) {
		character->transformTimer -= deltaTime;
	}
	else {
		exit(character);
	}
}

void FireTransformState::exit(Character* character) {
	for (b2Fixture* fixture = character->physicsBody->GetFixtureList(); fixture; ) {
		b2Fixture* temp = fixture;
		fixture = fixture->GetNext();
		character->physicsBody->DestroyFixture(temp);
	}

	Vector2 centerPos = Box2DWorldManager::b2ToRaylib(character->physicsBody->GetPosition());
	Vector2 transformPos = { centerPos.x - character->size.x * Constants::TILE_SIZE * 0.5f, centerPos.y - character->size.y * Constants::TILE_SIZE };
	Box2DWorldManager::getInstance().attachCapsuleFixtures(character->physicsBody,
		transformPos,
		Vector2{ Constants::Character::SUPER_STANDARD_SIZE.x * Constants::TILE_SIZE, Constants::Character::SUPER_STANDARD_SIZE.y * Constants::TILE_SIZE });
	Box2DWorldManager::getInstance().attachSensors(character->physicsBody, Vector2{ Constants::Character::SUPER_STANDARD_SIZE.x * Constants::TILE_SIZE, Constants::Character::SUPER_STANDARD_SIZE.y * Constants::TILE_SIZE });

	for (b2Fixture* fixture = character->physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		b2Filter filter = fixture->GetFilterData();
		filter.maskBits = static_cast<uint16>(ObjectCategory::CHARACTER);
		filter.categoryBits = static_cast<uint16>(ObjectCategory::BLOCK) | static_cast<uint16>(ObjectCategory::CHARACTER) |
			static_cast<uint16>(ObjectCategory::ENEMY) | static_cast<uint16>(ObjectCategory::INTERACTIVE) |
			static_cast<uint16>(ObjectCategory::ITEM) | static_cast<uint16>(ObjectCategory::SHELL) |
			static_cast<uint16>(ObjectCategory::PROJECTILE);
		fixture->SetFilterData(filter);
	}

	character->stateFrameData = Constants::Character::FIRE_MARIO_FRAME_DATA;
	character->size = Constants::Character::SUPER_STANDARD_SIZE;
	character->powerState = PowerState::FIRE;
	character->invincibleTimer = Constants::Character::INVINCIBLE_TIME_AFTER_TRANSFORM;
	character->currentState = &IdleState::getInstance();
	character->changeState(IdleState::getInstance());
}

void FireTransformState::handleInput(Character* character, const InputState& input) {

}

void FireTransformState::checkTransitions(Character* character, const InputState& input) {

}

FireTransformState& FireTransformState::getInstance() {
	static FireTransformState instance;
	return instance;
}