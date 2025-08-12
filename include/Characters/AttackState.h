#pragma once
#include "ICharacterState.h"

class Character;
struct InputState;

class AttackState : public ICharacterState {
public:
	void enter(Character* character) override;
	void update(Character* character, float deltaTime) override;
	void exit(Character* character) override;
	void handleInput(Character* character, const InputState& input) override;
	void checkTransitions(Character* character, const InputState& input) override;
	static AttackState& getInstance();

private:
	AttackState() = default;
	AttackState(const AttackState&) = delete;
	void operator=(const AttackState&) = delete;
};