#include "../../include/Characters/StunnedState.h"
#include "../../include/Characters/Character.h"
#include "../../include/System/Constant.h"
#include "../../include/Characters/IdleState.h"
#include "../../include/Characters/MovingState.h"
#include "../../include/Characters/JumpingState.h"
#include "../../include/Characters/KnockedState.h"
#include "../../include/Characters/SmallTransformState.h"
#include <iostream>

void StunnedState::enter(Character* character) {
	character->aniSpeed = 0.1f;
	character->aniTimer = 0.0f;
	character->setCurrentStateRow(3);
	character->invincibleTimer = 0.75f;
}

void StunnedState::update(Character* character, float deltaTime) {
	if (character->invincibleTimer > 0) {
		return;
	}
	if (character->hp <= 0) {
		if (character->powerState == PowerState::SMALL) {
			character->changeState(KnockedState::getInstance());
		}
		else if (character->powerState == PowerState::SUPER) {
			character->changeState(SmallTransformState::getInstance());
		}
		return;
	}

	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D)) {
		character->changeState(MovingState::getInstance());
	}
	else if (IsKeyDown(KEY_SPACE)) {
		character->changeState(JumpingState::getInstance());
	}
	else {
		character->changeState(IdleState::getInstance());
	}
}

void StunnedState::exit(Character* character) {

}

StunnedState& StunnedState::getInstance() {
	static StunnedState instance;
	return instance;
}
