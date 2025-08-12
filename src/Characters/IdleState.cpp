#include "..\..\include\Characters\IdleState.h"
#include "..\..\include\Characters\MovingState.h"
#include "..\..\include\Characters\JumpingState.h"
#include "..\..\include\Characters\Character.h"
#include "../../include/Characters/SuperTransformState.h"
#include "../../include/Characters/SmallTransformState.h"
#include "../../include/Characters/FireTransformState.h"
#include "../../include/Characters/AttackState.h"
#include "../../include/Game/GameContext.h"

void IdleState::enter(Character* character) {
	character->setAniTime(0);
	character->setAniSpeed(0.3f);
	character->setCurrentStateRow(0);
	character->physicsBody->SetLinearVelocity(b2Vec2(0, 0));
}

void IdleState::update(Character* character, float deltaTime) {
	
}

void IdleState::exit(Character* character) {

}

void IdleState::handleInput(Character* character, const InputState& input) {
	
}

void IdleState::checkTransitions(Character* character, const InputState& input) {
	if (input.jumpPressed && character->isOnGround()) {
		character->jump();
		character->changeState(JumpingState::getInstance());
		return;
	}

	if (input.moveLeft || input.moveRight) {
		character->changeState(MovingState::getInstance());
	}

	if (character->powerState == PowerState::FIRE) {
		if (input.attack) {
			character->changeState(AttackState::getInstance());
		}
		if (character->projectilesLeft <= 0 && character->attackTimer < 0) {
			character->changeState(SmallTransformState::getInstance());
		}
	}

	if (input.superTransform) {
		if (character->powerState == PowerState::SMALL) {
			character->changeState(SuperTransformState::getInstance());
		}
		else if (character->powerState == PowerState::SUPER) {
			character->changeState(SmallTransformState::getInstance());
		}
	}

	if (input.fireTransform) {
		if (character->powerState == PowerState::SMALL || character->powerState == PowerState::SUPER) {
			character->changeState(FireTransformState::getInstance());
		}
		else if (character->powerState == PowerState::FIRE) {
			character->changeState(SmallTransformState::getInstance());
		}
	}
}

IdleState& IdleState::getInstance() {
	static IdleState instance;
	return instance;
}