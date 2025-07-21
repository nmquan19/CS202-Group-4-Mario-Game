#include "..\..\include\Characters\IdleState.h"
#include "..\..\include\Characters\MovingState.h"
#include "..\..\include\Characters\JumpingState.h"
#include "..\..\include\Characters\Character.h"

#include <iostream>

void MovingState::enter(Character* character) {
	character->setAniTime(0);
	character->setAniSpeed(0.1f);
	character->setCurrentStateRow(1);
}

void MovingState::update(Character* character, float deltaTime) {
	std::cout << "Moving state\n";
	if (IsKeyPressed(KEY_SPACE) && character->isOnGround()) {
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
		std::cout << "Move right\n";
	}
	if (!moving) {
		character->changeState(IdleState::getInstance());
		return;
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