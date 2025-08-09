#include "..\..\include\Characters\IdleState.h"
#include "..\..\include\Characters\MovingState.h"
#include "..\..\include\Characters\JumpingState.h"
#include "..\..\include\Characters\Character.h"
#include "../../include/System/Box2DWorldManager.h"

void JumpingState::enter(Character* character){
    character->setAniTime(0);
    character->setAniSpeed(0.3f);
    character->setCurrentStateRow(2);
}

void JumpingState::update(Character* character, float deltaTime){
    b2Vec2 currentVel = character->physicsBody->GetLinearVelocity();
    float speed = character->getSpeed();
    float airControlFactor = 0.8f;

    if (IsKeyDown(KEY_A)) {
        character->physicsBody->SetLinearVelocity(b2Vec2(-speed * airControlFactor, currentVel.y));
        character->setFacingRight(false);
    }
    else if (IsKeyDown(KEY_D)) {
        character->physicsBody->SetLinearVelocity(b2Vec2(speed * airControlFactor, currentVel.y));
        character->setFacingRight(true);
    }
    else {
        character->physicsBody->SetLinearVelocity(b2Vec2(currentVel.x * 0.95f, currentVel.y));
    }

    if (IsKeyReleased(KEY_SPACE) && currentVel.y < 0) {
        b2Vec2 newVel = character->physicsBody->GetLinearVelocity();
        newVel.y *= 0.5f;
        character->physicsBody->SetLinearVelocity(newVel);
    }

    if (character->isOnGround()) {
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D)) {
            character->changeState(MovingState::getInstance());
        } else {
            character->changeState(IdleState::getInstance());
        }
        return;
    }
}

void JumpingState::exit(Character* character){}

JumpingState& JumpingState::getInstance() {
    static JumpingState instance;
    return instance;
}