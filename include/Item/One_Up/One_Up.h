#pragma once
#include "../Item.h"

class One_Up : public Item {
public:
	One_Up(Vector2 position);


	~One_Up();

	void update(float deltaTime) override;
	void Move(double dt) override;

private:
	float moveDirX = -1.0f;
	float totalTime = 0.0f;
	float omega = 1.0f;
	float Amplitude = 4.0f;
};