#pragma once
#include "../../../include/Item/Mushroom/Mushroom.h"

Mushroom::Mushroom(Vector2 position) : Item(position) {
	animation->SetType(ItemType::MUSHROOM);
}

Mushroom::~Mushroom() {
	// Destructor logic if needed
}

