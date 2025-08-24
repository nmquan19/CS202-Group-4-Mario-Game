#include "..\..\include\Characters\IdleState.h"
#include "..\..\include\Characters\MovingState.h"
#include "..\..\include\Characters\JumpingState.h"
#include "..\..\include\Characters\Character.h"
#include "../../include/System/Box2DWorldManager.h"
#include "../../include/Characters/AttackState.h"
#include "../../include/Characters/SmallTransformState.h"

void JumpingState::enter(Character* character){
    character->setAniTime(0);
    character->setAniSpeed(0.3f);
    character->setCurrentStateRow(2);
}

void JumpingState::update(Character* character, float deltaTime) {
    if (character->starTimer > 0) {
        character->starTimer -= deltaTime;
    }
}

void JumpingState::exit(Character* character){

}

void JumpingState::handleInput(Character* character, const InputState& input) {
    if (character->powerState == PowerState::FIRE) {
        if (input.attack) {
            character->changeState(AttackState::getInstance());
        }
        if (character->projectilesLeft <= 0 && character->attackTimer < 0) {
            character->changeState(SmallTransformState::getInstance());
        }
    }

    b2Vec2 currentVel = character->physicsBody->GetLinearVelocity();
    float speed = character->getSpeed();


    if (input.moveLeft) {
        character->physicsBody->SetLinearVelocity(b2Vec2(-speed, currentVel.y));
        character->setFacingRight(false);
    }
    else if (input.moveRight) {
        character->physicsBody->SetLinearVelocity(b2Vec2(speed, currentVel.y));
        character->setFacingRight(true);
    }
    else {
        character->physicsBody->SetLinearVelocity(b2Vec2(currentVel.x * 0.95f, currentVel.y));
    }

    if (input.jumpReleased && currentVel.y < 0) {
        b2Vec2 newVel = character->physicsBody->GetLinearVelocity();
        newVel.y *= 0.5f;
        character->physicsBody->SetLinearVelocity(newVel);
    }
}

void JumpingState::checkTransitions(Character* character, const InputState& input) {
    if (character->isOnGround() || character->physicsBody->GetLinearVelocity().y == 0) {
        if (input.moveLeft || input.moveRight) {
            character->changeState(MovingState::getInstance());
        }
        else {
            character->changeState(IdleState::getInstance());
        }
    }
    if (character->powerState == PowerState::STAR && character->starTimer <= 0) {
        character->changeState(SmallTransformState::getInstance());
    }
}

JumpingState& JumpingState::getInstance() {
    static JumpingState instance;
    return instance;
}