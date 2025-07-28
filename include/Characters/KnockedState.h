#pragma once
#include "ICharacterState.h"

class Character;

class KnockedState : public ICharacterState {
public:
	void enter(Character* character) override;
	void update(Character* character, float deltaTime) override;
	void exit(Character* character) override;
	static KnockedState& getInstance();

private:
	KnockedState() = default;
	KnockedState(const KnockedState&) = delete;
	void operator=(const KnockedState&) = delete;
};