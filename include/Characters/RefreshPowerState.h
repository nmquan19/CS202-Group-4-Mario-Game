#pragma once
#include "ICharacterState.h"

class Character;
struct InputState;

class RefreshPowerState : public ICharacterState {
public:
	void enter(Character* character) override;
	void update(Character* character, float deltaTime) override;
	void exit(Character* character) override;
	void handleInput(Character* character, const InputState& input) override;
	void checkTransitions(Character* character, const InputState& input) override;
	static RefreshPowerState& getInstance();

private:
	RefreshPowerState() = default;
	RefreshPowerState(const RefreshPowerState&) = delete;
	void operator=(const RefreshPowerState&) = delete;
};