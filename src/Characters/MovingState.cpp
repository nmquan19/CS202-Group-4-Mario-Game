#include "..\..\include\Characters\IdleState.h"
#include "..\..\include\Characters\MovingState.h"
#include "..\..\include\Characters\JumpingState.h"
#include "..\..\include\Characters\Character.h"

void MovingState::enter(Character* character) {
	character->setAniTime(0);
	character->setAniSpeed(0.3f);
	character->setCurrentStateRow(1);
}

void MovingState::update(Character* character, float deltaTime) {
	if (IsKeyPressed(KEY_SPACE) && character->canJump()) {
		character->jump();
        character->changeState(JumpingState::getInstance());
        return;
    }

	bool moving = false;
	Vector2 currentVel = character->getVelocity();
	float speed = character->getSpeed();
	
	if (IsKeyDown(KEY_A)) {
		character->setVelocity({-speed, currentVel.y});
		character->setFacingRight(false);
		moving = true;
	}
	if (IsKeyDown(KEY_D)) {
		character->setVelocity({speed, currentVel.y});
		character->setFacingRight(true);
		moving = true;
	}
	
	if (!moving) {
		// // Apply ground friction when stopping
		// if (character->isOnGround()) {
		// 	character->setVelocity({currentVel.x * 0.8f, currentVel.y}); // Ground friction
		// 	if (abs(currentVel.x) < 0.1f) { // If almost stopped
		// 		character->setVelocity({0, currentVel.y});
		// 		character->changeState(IdleState::getInstance());
		// 	}
		// } else {
		// 	character->changeState(IdleState::getInstance());
		// }
		character->changeState(IdleState::getInstance());
	}
}

void MovingState::exit(Character* character) {
	Vector2 currentVel = character->getVelocity();
	character->setVelocity({0, currentVel.y});
}

MovingState& MovingState::getInstance() {
	static MovingState instance;
	return instance;
}