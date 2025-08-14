#pragma once
#include "../Item.h"

class Mushroom : public Item {
public:
	Mushroom(Vector2 position);

	~Mushroom();

	void update(float deltaTime) override;
};
