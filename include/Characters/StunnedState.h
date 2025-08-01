#pragma once
#include "ICharacterState.h"

class Character;

class StunnedState : public ICharacterState {
public:
	void enter(Character* character) override;
	void update(Character* character, float deltaTime) override;
	void exit(Character* character) override;
	static StunnedState& getInstance();

private:
	StunnedState() = default;
	StunnedState(const StunnedState&) = delete;
	void operator=(const StunnedState&) = delete;
};