#pragma once
#include "ICharacterState.h"

class Character;
struct InputState;

class KnockedState : public ICharacterState {
public:
	void enter(Character* character) override;
	void update(Character* character, float deltaTime) override;
	void exit(Character* character) override;
	void handleInput(Character* character, const InputState& input) override;
	void checkTransitions(Character* character, const InputState& input) override;
	static KnockedState& getInstance();

private:
	KnockedState() = default;
	KnockedState(const KnockedState&) = delete;
	void operator=(const KnockedState&) = delete;
};