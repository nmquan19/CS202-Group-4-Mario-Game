#pragma once
#include "../Item.h"

class Star : public Item {
public:
	Star(Vector2 position);

	~Star();

	void update(float deltaTime) override;
};