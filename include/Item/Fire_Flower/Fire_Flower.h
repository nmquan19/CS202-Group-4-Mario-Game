#pragma once
#include "../Item.h"

class Fire_Flower : public Item {
public:
	Fire_Flower(Vector2 position);

	~Fire_Flower();

	void update(float deltaTime) override;
};


