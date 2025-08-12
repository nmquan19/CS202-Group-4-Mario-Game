#include "..\..\include\Characters\IdleState.h"
#include "..\..\include\Characters\MovingState.h"
#include "..\..\include\Characters\JumpingState.h"
#include "..\..\include\Characters\Character.h"
#include "../../include/Characters/AttackState.h"
#include "../../include/Characters/SmallTransformState.h"
#include "../../include/System/Box2DWorldManager.h"

void MovingState::enter(Character* character) {
	character->setAniTime(0);
	character->setAniSpeed(0.2f);
	character->setCurrentStateRow(1);
}

void MovingState::update(Character* character, float deltaTime) {

}

void MovingState::exit(Character* character) {
	Vector2 currentVel = character->getVelocity();
	character->setVelocity({0, currentVel.y});
}

void MovingState::handleInput(Character* character, const InputState& input) {
	b2Vec2 currentVel = character->physicsBody->GetLinearVelocity();
	float speed = character->getSpeed();

	if (input.moveLeft) {
		character->physicsBody->SetLinearVelocity(b2Vec2(-speed, currentVel.y));
		character->setFacingRight(false);
	}

	if (input.moveRight) {
		character->physicsBody->SetLinearVelocity(b2Vec2(speed, currentVel.y));
		character->setFacingRight(true);
	}

	if (character->powerState == PowerState::FIRE) {
		if (input.attack) {
			character->changeState(AttackState::getInstance());
		}
		if (character->projectilesLeft <= 0 && character->attackTimer < 0) {
			character->changeState(SmallTransformState::getInstance());
		}
	}
}

void MovingState::checkTransitions(Character* character, const InputState& input) {
	if (input.jumpPressed && character->isOnGround()) {
		character->jump();
		character->changeState(JumpingState::getInstance());
	}
	else if (!input.moveLeft && !input.moveRight) {
		character->changeState(IdleState::getInstance());
	}
}

MovingState& MovingState::getInstance() {
	static MovingState instance;
	return instance;
}