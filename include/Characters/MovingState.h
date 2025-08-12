#pragma once
#include "ICharacterState.h"

class Character;
struct InputState;

class MovingState : public ICharacterState {
public:
	void enter(Character* character) override;
	void update(Character* chatacter, float deltaTime) override;
	void exit(Character* character) override;
	void handleInput(Character* character, const InputState& input) override;
	void checkTransitions(Character* character, const InputState& input) override;
	static MovingState& getInstance();
private:
	MovingState() = default;
	MovingState(const MovingState&) = delete;
	void operator-(const MovingState&) = delete;
};