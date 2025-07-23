#pragma once
#include "../Characters/Character.h"
#include "../System/Interface.h"
#include <string>

class HUD : public IDrawable {
public:
	HUD(std::shared_ptr<Character> character);
	~HUD() = default;
	void draw() override;
	void setCharacterTracking(std::shared_ptr<Character> character);
	friend class Character;

private:
	void drawString(std::string str, Vector2 position);

private:
	std::shared_ptr<Character> character;
};