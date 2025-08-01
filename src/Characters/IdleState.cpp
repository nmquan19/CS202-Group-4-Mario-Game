#include "..\..\include\Characters\IdleState.h"
#include "..\..\include\Characters\MovingState.h"
#include "..\..\include\Characters\JumpingState.h"
#include "..\..\include\Characters\Character.h"

void IdleState::enter(Character* character) {
	character->setAniTime(0);
	character->setAniSpeed(0.3f);
	character->setCurrentStateRow(0);

	// Stop horizontal movement, keep vertical velocity for Box2D physics
	Vector2 currentVel = character->getVelocity();
	character->setVelocity({0, currentVel.y});
}

void IdleState::update(Character* character, float deltaTime) {
	if (IsKeyPressed(KEY_SPACE) && character->canJump()) {
		character->jump();
        character->changeState(JumpingState::getInstance());
        return;
    }

	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D)) {
		character->changeState(MovingState::getInstance());
	}
}

void IdleState::exit(Character* character) {}

IdleState& IdleState::getInstance() {
	static IdleState instance;
	return instance;
}