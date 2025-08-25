#include "../../include/Characters/StunnedState.h"
#include "../../include/Characters/Character.h"
#include "../../include/System/Constant.h"
#include "../../include/Characters/IdleState.h"
#include "../../include/Characters/MovingState.h"
#include "../../include/Characters/JumpingState.h"
#include "../../include/Characters/KnockedState.h"
#include "../../include/Characters/SmallTransformState.h"
#include "../../include/UI/SoundEffect.h"
#include "../../include/UI/UI.h"
#include <iostream>

void StunnedState::enter(Character* character) {
	character->aniSpeed = 0.1f;
	character->aniTimer = 0.0f;
	character->setCurrentStateRow(3);
	character->invincibleTimer = 0.75f;
	AudioManager::getInstance().PlaySoundEffect("hit");
	for (b2Fixture* fixture = character->physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		b2Filter filter = fixture->GetFilterData();
		filter.maskBits = static_cast<uint16>(ObjectCategory::CHARACTER);
		filter.categoryBits = static_cast<uint16>(ObjectCategory::BLOCK) | static_cast<uint16>(ObjectCategory::INTERACTIVE);
		fixture->SetFilterData(filter);
	}
}

void StunnedState::update(Character* character, float deltaTime) {
	
}

void StunnedState::exit(Character* character) {

}

void StunnedState::handleInput(Character* character, const InputState& input) {

}

void StunnedState::checkTransitions(Character* character, const InputState& input) {
	if (character->invincibleTimer > 0) {
		return;
	}

	if (character->hp <= 0) {
		if (character->powerState == PowerState::SMALL) {
			character->changeState(KnockedState::getInstance());
		}
		return;
	}

	if (character->powerState == PowerState::SUPER || character->powerState == PowerState::FIRE) {
		character->changeState(SmallTransformState::getInstance());
	}

	if (input.moveLeft || input.moveRight) {
		character->changeState(MovingState::getInstance());
	}
	else if (input.jumpPressed) {
		character->changeState(JumpingState::getInstance());
	}
	else {
		character->changeState(IdleState::getInstance());
	}
}

StunnedState& StunnedState::getInstance() {
	static StunnedState instance;
	return instance;
}
