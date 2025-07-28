#include "../../include/Characters/StunnedState.h"
#include "../../include/Characters/Character.h"
#include "../../include/System/Constant.h"
#include "../../include/Characters/IdleState.h"
#include "../../include/Characters/MovingState.h"
#include "../../include/Characters/JumpingState.h"
#include "../../include/Characters/KnockedState.h"
#include <iostream>

void StunnedState::enter(Character* character) {
	character->aniSpeed = 0.1f;
	character->aniTimer = 0.0f;
	character->setVelocity(Vector2{ 0, 0 });
	character->invincibleTimer = Constants::Character::INVINCIBLE_TIME;
	character->lastBottomY = character->getBottom();
	character->setCurrentStateRow(3);
	character->setActive(false);
}

void StunnedState::update(Character* character, float deltaTime) {
	if (character->invincibleTimer <= 0) {
		if (character->hp <= 0) {
			character->changeState(KnockedState::getInstance());
		}
		else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D)) {
			character->changeState(MovingState::getInstance());
		}
		else if (IsKeyDown(KEY_SPACE)) {
			character->changeState(JumpingState::getInstance());
		}
		else {
			character->changeState(IdleState::getInstance());
		}
	}
}

void StunnedState::exit(Character* character) {

}

StunnedState& StunnedState::getInstance() {
	static StunnedState instance;
	return instance;
}
