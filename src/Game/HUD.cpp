#include "../../include/Game/HUD.h"
#include "../../include/System/TextureManager.h"
#include "../../include/System/Constant.h"
#include <iostream>

HUD::HUD(std::shared_ptr<Character> character) : character(character) {

}

void HUD::draw() {
	std::string hp = "HP x " + std::to_string(character->hp);
	drawString(hp, Vector2{ 200, 100 });
}

void HUD::setCharacterTracking(std::shared_ptr<Character> character) {
	this->character = character;
}      

void HUD::drawString(std::string str, Vector2 position) {	float topLeftX = position.x;
	float bottomY = position.y + TextureManager::fontSprites[str[0]].height * Constants::HUD::SCALE;
	Rectangle dest;
	for (int i = 0; i < str.length(); ++i) {
		dest = { 
			topLeftX, 
			bottomY - TextureManager::fontSprites[str[i]].height * Constants::HUD::SCALE, 
			TextureManager::fontSprites[str[i]].width * Constants::HUD::SCALE, 
			TextureManager::fontSprites[str[i]].height * Constants::HUD::SCALE 
		};
		DrawTexturePro(TextureManager::fontTexture, TextureManager::fontSprites[str[i]], dest, {0, 0}, 0.0f, RED);
		topLeftX += TextureManager::fontSprites[str[i]].width * Constants::HUD::SCALE + Constants::HUD::SPACING;
	}
}