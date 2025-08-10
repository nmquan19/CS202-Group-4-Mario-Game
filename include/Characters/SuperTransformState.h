#pragma once
#include "ICharacterState.h"

class Character;
struct InputState;

class SuperTransformState : public ICharacterState {
public:
	void enter(Character* character) override;
	void update(Character* chatacter, float deltaTime) override;
	void exit(Character* character) override;
	void handleInput(Character* character, const InputState& input) override;
	void checkTransitions(Character* character, const InputState& input) override;
	static SuperTransformState& getInstance();
private:
	SuperTransformState() = default;
	SuperTransformState(const SuperTransformState&) = delete;
	void operator=(const SuperTransformState&) = delete;
};