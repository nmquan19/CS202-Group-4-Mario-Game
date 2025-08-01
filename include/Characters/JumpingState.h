#pragma once
#include "ICharacterState.h"

class Character;

class JumpingState : public ICharacterState {
public:
    void enter(Character* character) override;  
    void update(Character* character, float deltaTime) override;
    void exit(Character* character) override;
    static JumpingState& getInstance();
private:
    JumpingState() = default;
    JumpingState(const JumpingState&) = delete;
	void operator-(const JumpingState&) = delete;
};