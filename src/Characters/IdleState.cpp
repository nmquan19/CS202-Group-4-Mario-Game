#include "..\..\include\Characters\IdleState.h"
#include "..\..\include\Characters\MovingState.h"
#include "..\..\include\Characters\JumpingState.h"
#include "..\..\include\Characters\Character.h"

void IdleState::enter(Character* character) {
	character->setAniTime(0);
	character->setAniSpeed(0.3f);
	character->setCurrentStateRow(0);
	character->physicsBody->SetLinearVelocity(b2Vec2(0, 0));
}

void IdleState::update(Character* character, float deltaTime) {
	if (IsKeyPressed(KEY_SPACE) && character->isOnGround()) {
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