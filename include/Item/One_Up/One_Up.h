#pragma once
#include "../Item.h"

class One_Up : public Item {
public:
	One_Up(Vector2 position);


	~One_Up();

	void update(float deltaTime) override;
	void Move(double dt) override;

private:
	bool isEscaping = false;


	float moveDirX = -1.0f;
	float totalTime = 0.0f;
	float frequency = 1.0f;
	float Amplitude = 100.0f;
};