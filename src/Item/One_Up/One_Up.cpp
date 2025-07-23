#pragma once
#include "../../../include/Item/One_Up/One_Up.h"

One_Up::One_Up(Vector2 position) : Item(position) {
	animation->SetType(ItemType::ONE_UP);
}

One_Up::~One_Up() {
	// Destructor logic if needed
}


