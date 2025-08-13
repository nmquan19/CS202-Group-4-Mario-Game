#pragma once
#include "../Item.h"

class Coin : public Item {
public:
	Coin(Vector2 position); 
	~Coin();

	void update(float deltaTime) override;
private:

};
