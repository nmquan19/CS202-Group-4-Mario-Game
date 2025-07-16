#pragma once

class Character;

class ICharacterState {
public:
	virtual ~ICharacterState() = default;
	virtual void enter(Character* character) = 0;
	virtual void update(Character* character, float deltaTime) = 0;
	virtual void exit(Character* character) = 0;
};