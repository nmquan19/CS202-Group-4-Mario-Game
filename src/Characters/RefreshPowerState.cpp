#include "../../include/Characters/RefreshPowerState.h"
#include "../../include/Characters/IdleState.h"
#include "../../include/Characters/Character.h"

void RefreshPowerState::enter(Character* character) {
	character->aniTimer = 0.0f;
	character->aniSpeed = 0.3f;
	character->invincibleTimer = 0.5f;
	character->transformTimer = 0.5f;
	character->setCurrentStateRow(8);
	b2Vec2 currentVel = character->physicsBody->GetLinearVelocity();
	float mass = character->physicsBody->GetMass();
	character->physicsBody->ApplyLinearImpulseToCenter(b2Vec2(0, mass * (-5.0f - currentVel.y)), true);
}

void RefreshPowerState::update(Character* character, float deltaTime) {
	if (character->transformTimer > 0) {
		character->transformTimer -= deltaTime;
	}
}

void RefreshPowerState::exit(Character* character) {

}

void RefreshPowerState::handleInput(Character* character, const InputState& input) {

}

void RefreshPowerState::checkTransitions(Character* character, const InputState& input) {
	if (character->transformTimer <= 0) {
		character->changeState(IdleState::getInstance());
	}
}

RefreshPowerState& RefreshPowerState::getInstance() {
	static RefreshPowerState instance;
	return instance;
}