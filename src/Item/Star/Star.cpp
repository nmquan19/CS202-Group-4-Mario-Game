#pragma once
#include "../../../include/Item/Star/Star.h"

Star::Star(Vector2 position) : Item(position) {
	animation->SetType(ItemType::STAR);
}

Star::~Star() {
	// Destructor logic if needed
}