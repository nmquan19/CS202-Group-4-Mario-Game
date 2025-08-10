#include "..\..\include\Characters\IdleState.h"
#include "..\..\include\Characters\MovingState.h"
#include "..\..\include\Characters\JumpingState.h"
#include "..\..\include\Characters\Character.h"
#include "../../include/Characters/SuperTransformState.h"
#include "../../include/Characters/SmallTransformState.h"
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
	if (input.fire) {
		GameContext::getInstance().addObject(ProjectileType::FIRE_BALL, Vector2{ 600, 500 }, { 0.5f, 0.5f });
	}
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

	if (input.superTransform) {
		if (character->powerState == PowerState::SMALL) {
			character->changeState(SuperTransformState::getInstance());
		}
		else if (character->powerState == PowerState::SUPER) {
			character->changeState(SmallTransformState::getInstance());
		}
	}
}

IdleState& IdleState::getInstance() {
	static IdleState instance;
	return instance;
}