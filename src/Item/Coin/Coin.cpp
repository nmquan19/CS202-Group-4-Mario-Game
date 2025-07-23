#pragma once
#include "../../../include/Item/Coin/Coin.h"

Coin::Coin(Vector2 position) : Item(position) {
	animation->SetType(ItemType::COIN);
}

Coin::~Coin() {
	// Destructor logic if needed
}
