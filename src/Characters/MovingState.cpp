#include "..\..\include\Characters\IdleState.h"
#include "..\..\include\Characters\MovingState.h"
#include "..\..\include\Characters\JumpingState.h"
#include "..\..\include\Characters\Character.h"
#include "../../include/System/Box2DWorldManager.h"

void MovingState::enter(Character* character) {
	character->setAniTime(0);
	character->setAniSpeed(0.2f);
	character->setCurrentStateRow(1);
}

void MovingState::update(Character* character, float deltaTime) {
	if (IsKeyPressed(KEY_SPACE) && character->isOnGround()) {
		character->jump();
        character->changeState(JumpingState::getInstance());
        return;
    }

	bool moving = false;
	b2Vec2 currentVel = character->physicsBody->GetLinearVelocity();
	float speed = character->getSpeed();
	
	if (IsKeyDown(KEY_A)) {
		character->physicsBody->SetLinearVelocity(b2Vec2(-speed, currentVel.y));
		character->setFacingRight(false);
		moving = true;
	}
	
	if (IsKeyDown(KEY_D)) {
		character->physicsBody->SetLinearVelocity(b2Vec2(speed, currentVel.y));
		character->setFacingRight(true);
		moving = true;
	}
	
	if (!moving) {
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