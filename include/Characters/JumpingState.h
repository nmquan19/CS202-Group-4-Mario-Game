#pragma once
#include "ICharacterState.h"

class Character;
struct InputState;

class JumpingState : public ICharacterState {
public:
    void enter(Character* character) override;  
    void update(Character* character, float deltaTime) override;
    void exit(Character* character) override;
    void handleInput(Character* character, const InputState& input) override;
    void checkTransitions(Character* character, const InputState& input) override;
    static JumpingState& getInstance();
private:
    JumpingState() = default;
    JumpingState(const JumpingState&) = delete;
	void operator-(const JumpingState&) = delete;
};