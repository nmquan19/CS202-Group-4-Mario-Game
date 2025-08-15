#pragma once
#include "../Item.h"

class One_Up : public Item {
public:
	One_Up(Vector2 position);

	~One_Up();

	void update(float deltaTime) override;
};