#pragma once
#include "ICharacterState.h"

class Character;
struct InputState;

class StarTransformState : public ICharacterState {
public:
	void enter(Character* character) override;
	void update(Character* chatacter, float deltaTime) override;
	void exit(Character* character) override;
	void handleInput(Character* character, const InputState& input) override;
	void checkTransitions(Character* character, const InputState& input) override;
	static StarTransformState& getInstance();
private:
	StarTransformState() = default;
	StarTransformState(const StarTransformState&) = delete;
	void operator=(const StarTransformState&) = delete;
};