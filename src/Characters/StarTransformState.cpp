#include "../../include/Characters/StarTransformState.h" 
#include "../../include/Characters/Character.h"
#include "../../include/System/Box2DWorldManager.h"
#include "../../include/Characters/IdleState.h"
#include "../../include/UI/SoundEffect.h"

void StarTransformState::enter(Character* character) {
	AudioManager::getInstance().PlaySoundEffect("level_up");
	character->setAniTime(0.0f);
	character->setAniSpeed(0.2f);
	character->transformTimer = Constants::Character::TRANSFORM_DURATION;
	character->setCurrentStateRow(5);
	for (b2Fixture* fixture = character->physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		b2Filter filter = fixture->GetFilterData();
		filter.maskBits = static_cast<uint16>(ObjectCategory::CHARACTER);
		filter.categoryBits = static_cast<uint16>(ObjectCategory::BLOCK);
		fixture->SetFilterData(filter);
	}
}

void StarTransformState::update(Character* character, float deltaTime) {
	if (character->transformTimer > 0) {
		character->transformTimer -= deltaTime;
	}
	else {
		exit(character);
	}
}

void StarTransformState::exit(Character* character) {
	for (b2Fixture* fixture = character->physicsBody->GetFixtureList(); fixture; ) {
		b2Fixture* temp = fixture;
		fixture = fixture->GetNext();
		character->physicsBody->DestroyFixture(temp);
	}

	Vector2 centerPos = Box2DWorldManager::b2ToRaylib(character->physicsBody->GetPosition());

	Vector2 transformPos = { centerPos.x - character->size.x * Constants::TILE_SIZE * 0.5f, centerPos.y - character->size.y * Constants::TILE_SIZE };

	Vector2 ssize;
	switch (character->characterType) {
	case CharacterType::MARIO:
		ssize = Constants::Character::Mario::SUPER_SIZE;
		break;
	case CharacterType::LUIGI:
		ssize = Constants::Character::Luigi::SUPER_SIZE;
		break;
	case CharacterType::TOAD:
		ssize = Constants::Character::Toad::SUPER_SIZE;
		break;
	case CharacterType::TOADETTE:
		ssize = Constants::Character::Toadette::SUPER_SIZE;
		break;
	}

	Box2DWorldManager::getInstance().attachCapsuleFixtures(character->physicsBody, transformPos, Vector2{ ssize.x * Constants::TILE_SIZE, ssize.y * Constants::TILE_SIZE });

	Box2DWorldManager::getInstance().attachSensors(character->physicsBody, Vector2{ ssize.x * Constants::TILE_SIZE, ssize.y * Constants::TILE_SIZE });

	for (b2Fixture* fixture = character->physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		b2Filter filter = fixture->GetFilterData();
		filter.maskBits = static_cast<uint16>(ObjectCategory::CHARACTER);
		filter.categoryBits = static_cast<uint16>(ObjectCategory::BLOCK) | static_cast<uint16>(ObjectCategory::CHARACTER) |
			static_cast<uint16>(ObjectCategory::ENEMY) | static_cast<uint16>(ObjectCategory::INTERACTIVE) |
			static_cast<uint16>(ObjectCategory::ITEM) | static_cast<uint16>(ObjectCategory::SHELL) |
			static_cast<uint16>(ObjectCategory::PROJECTILE);
		fixture->SetFilterData(filter);
	}

	switch (character->characterType) {
	case CharacterType::MARIO:
		character->stateFrameData = Constants::Character::Mario::SUPER_FRAME_DATA;
		character->size = Constants::Character::Mario::SUPER_SIZE;
		break;
	case CharacterType::LUIGI:
		character->stateFrameData = Constants::Character::Luigi::SUPER_FRAME_DATA;
		character->size = Constants::Character::Luigi::SUPER_SIZE;
		break;
	case CharacterType::TOAD:
		character->stateFrameData = Constants::Character::Toad::SUPER_FRAME_DATA;
		character->size = Constants::Character::Toad::SUPER_SIZE;
		break;
	case CharacterType::TOADETTE:
		character->stateFrameData = Constants::Character::Toadette::SUPER_FRAME_DATA;
		character->size = Constants::Character::Toad::SUPER_SIZE;
		break;
	}

	character->powerState = PowerState::STAR;
	character->enableRainbowEffect();
	character->invincibleTimer = Constants::Character::INVINCIBLE_TIME_AFTER_TRANSFORM;
	character->starTimer = 5.0f;
	character->currentState = &IdleState::getInstance();
	character->changeState(IdleState::getInstance());
}

void StarTransformState::handleInput(Character* character, const InputState& input) {

}

void StarTransformState::checkTransitions(Character* character, const InputState& input) {

}

StarTransformState& StarTransformState::getInstance() {
	static StarTransformState instance;
	return instance;
}