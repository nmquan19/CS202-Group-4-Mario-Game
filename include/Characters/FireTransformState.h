#pragma once
#include "ICharacterState.h"

class Character;
struct InputState;

class FireTransformState : public ICharacterState {
public:
	void enter(Character* character) override;
	void update(Character* chatacter, float deltaTime) override;
	void exit(Character* character) override;
	void handleInput(Character* character, const InputState& input) override;
	void checkTransitions(Character* character, const InputState& input) override;
	static FireTransformState& getInstance();
private:
	FireTransformState() = default;
	FireTransformState(const FireTransformState&) = delete;
	void operator=(const FireTransformState&) = delete;
};