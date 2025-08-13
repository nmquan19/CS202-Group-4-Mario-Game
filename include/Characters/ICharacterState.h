#pragma once

class Character;
struct InputState;

class ICharacterState {
public:
	virtual ~ICharacterState() = default;
	virtual void enter(Character* character) = 0;
	virtual void update(Character* character, float deltaTime) = 0;
	virtual void exit(Character* character) = 0;
	virtual void handleInput(Character* character, const InputState& input) = 0;
	virtual void checkTransitions(Character* character, const InputState& input) = 0;
};