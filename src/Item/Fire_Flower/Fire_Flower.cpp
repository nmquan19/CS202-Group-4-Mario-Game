#pragma once
#include "../../../include/Item/Fire_Flower/Fire_Flower.h"

Fire_Flower::Fire_Flower(Vector2 position) : Item(position) {
	animation->SetType(ItemType::FIRE_FLOWER);
}

Fire_Flower::~Fire_Flower() {
	// Destructor logic if needed
}





