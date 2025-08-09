#pragma once
#include "ICharacterState.h"

class Character;

class SuperTransformState : public ICharacterState {
public:
	void enter(Character* character) override;
	void update(Character* chatacter, float deltaTime) override;
	void exit(Character* character) override;
	static SuperTransformState& getInstance();
private:
	SuperTransformState() = default;
	SuperTransformState(const SuperTransformState&) = delete;
	void operator=(const SuperTransformState&) = delete;
};