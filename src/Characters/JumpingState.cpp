#include "..\..\include\Characters\IdleState.h"
#include "..\..\include\Characters\MovingState.h"
#include "..\..\include\Characters\JumpingState.h"
#include "..\..\include\Characters\Character.h"

void JumpingState::enter(Character* character){
    character->setAniTime(0);
    character->setAniSpeed(0.2f);
    character->setCurrentStateRow(2);

    character->jump();
}

void JumpingState::update(Character* character, float deltaTime){
    Vector2 currentVel = character->getVelocity();
    float airSpeed = character->getSpeed() * 0.7f; // reduce air control

    if(IsKeyDown(KEY_A)){
        character->setVelocity({-airSpeed, currentVel.y});
        character->setFacingRight(false);
    }
    else if(IsKeyDown(KEY_D)){
        character->setVelocity({airSpeed, currentVel.y});
        character->setFacingRight(true);
    }
    else{
        character->setVelocity({currentVel.x * 0.95f, currentVel.y});
    }

    if (IsKeyReleased(KEY_SPACE) && currentVel.y < 0) {
        Vector2 vel = character->getVelocity();
        vel.y *= 0.5f; // Cut jump short
        character->setVelocity(vel);
    }
    
    if (character->isOnGround() && currentVel.y >= 0) {
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