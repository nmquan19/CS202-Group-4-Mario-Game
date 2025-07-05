#pragma once
#include "ICharacterState.h"

class Character;

class MovingState : public ICharacterState {
public:
	void enter(Character* character) override;
	void update(Character* chatacter, float deltaTime) override;
	void exit(Character* character) override;
	static MovingState& getInstance();
private:
	MovingState() = default;
	MovingState(const MovingState&) = delete;
	void operator-(const MovingState&) = delete;
};