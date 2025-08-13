#pragma once
#include "../Item.h"

class Mushroom : public Item {
public:
	Mushroom(Vector2 position);

	~Mushroom();

	void update(float deltaTime) override;

private:
	bool isEscaping = false;

	float moveDirX = -1.0f;
	float totalTime = 0.0f; 
	float omega = 1.0f; 
	float Amplitude = 100.0f; 

};
