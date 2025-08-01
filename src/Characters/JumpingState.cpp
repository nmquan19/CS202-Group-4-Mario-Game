#include "..\..\include\Characters\IdleState.h"
#include "..\..\include\Characters\MovingState.h"
#include "..\..\include\Characters\JumpingState.h"
#include "..\..\include\Characters\Character.h"

void JumpingState::enter(Character* character){
    character->setAniTime(0);
    character->setAniSpeed(0.2f);
    character->setCurrentStateRow(2);
}

void JumpingState::update(Character* character, float deltaTime){
    Vector2 currentVel = character->getVelocity();
    float speed = character->getSpeed();
    float airControlFactor = 0.8f; // Slightly reduced air control for realism

    // Handle horizontal movement during jump
    if(IsKeyDown(KEY_A)){
        character->setVelocity({-speed * airControlFactor, currentVel.y});
        character->setFacingRight(false);
    }
    else if(IsKeyDown(KEY_D)){
        character->setVelocity({speed * airControlFactor, currentVel.y});
        character->setFacingRight(true);
    }
    else{
        // Apply air resistance for smoother feel
        character->setVelocity({currentVel.x * 0.98f, currentVel.y});
    }

    // Variable jump height - cut jump short if space is released
    if (IsKeyReleased(KEY_SPACE) && currentVel.y < 0) {
        Vector2 vel = character->getVelocity();
        vel.y *= 0.5f; // Cut jump short
        character->setVelocity(vel);
    }
    
    // Check if landed using the new ground detection system
    if (character->canJump() && currentVel.y >= -0.1f) { // Small tolerance for landing detection
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