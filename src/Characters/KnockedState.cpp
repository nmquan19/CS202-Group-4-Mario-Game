#include "../../include/Characters/Character.h"
#include "../../include/Characters/KnockedState.h"
#include "../../include/Characters/IdleState.h"
#include "../../include/System/Constant.h"

void KnockedState::enter(Character* character) {
	character->setActive(false);
	character->setVelocity(Vector2{ 0, 200.0f });
	character->reviveTimer = Constants::Character::REVIVE_TIME;
}

void KnockedState::update(Character* character, float deltaTime) {
	if (character->reviveTimer <= 0) {
		character->setActive(true);
		character->setPosition(Vector2{ 500, 500 });
		character->setVelocity(Vector2{ 0, 0 });
		character->setOnGround(false);
		character->changeState(IdleState::getInstance());
		character->invincibleTimer = 2.0f;
		character->hp = 1;
	}
}

void KnockedState::exit(Character* character) {

}

KnockedState& KnockedState::getInstance() {
	static KnockedState instance;
	return instance;
}