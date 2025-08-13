#pragma once
#include "../Item.h"

class Star : public Item {
public:
	Star(Vector2 position);

	~Star();

	void Move(double dt) override;
	void update(float deltaTime) override;

private:
	float Amplitude; 
	float frequency;
	float radius;
};