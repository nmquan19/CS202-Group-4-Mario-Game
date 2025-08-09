#pragma once
#include "ICharacterState.h"

class Character;

class SmallTransformState : public ICharacterState {
public:
	void enter(Character* character) override;
	void update(Character* chatacter, float deltaTime) override;
	void exit(Character* character) override;
	static SmallTransformState& getInstance();
private:
	SmallTransformState() = default;
	SmallTransformState(const SmallTransformState&) = delete;
	void operator=(const SmallTransformState&) = delete;
};