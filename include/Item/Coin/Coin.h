#pragma once
#include "../Item.h"

class Coin : public Item {
public:
	Coin(Vector2 position) : Item(position) {
		animation->SetType(ItemType::COIN);
	}


	~Coin() {
		// Destructor logic if needed
	}

private:

};
