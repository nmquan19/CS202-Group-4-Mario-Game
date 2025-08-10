#pragma once
#include "../Item.h"

class Mushroom : public Item {
public:
	Mushroom(Vector2 position);

	~Mushroom();

	void update(float deltaTime) override;

private:
	float moveDirX = -1.0f;
	float totalTime = 0.0f; 
	float omega = 1.0f; 
	float Amplitude = 4.0f; 

};
