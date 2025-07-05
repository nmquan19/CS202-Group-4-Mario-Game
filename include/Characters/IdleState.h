#pragma once
#include "ICharacterState.h"

class Character;

class IdleState : public ICharacterState {
public:
	void enter(Character* character) override;
	void update(Character* chatacter, float deltaTime) override;
	void exit(Character* character) override;
	static IdleState& getInstance();
private:
	IdleState() = default;
	IdleState(const IdleState&) = delete;
	void operator=(const IdleState&) = delete;
};