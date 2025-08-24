#pragma once
#include "InteractiveObject.h"

class FireBar : public InteractiveObject {
public:
	FireBar(Vector2 position, Vector2 size);
	virtual ~FireBar();

	void update(float deltaTime) override;
	void draw() override;
	void draw(int);

	void onCollision(std::shared_ptr<Object> other, Direction dir) override;

	std::vector<Rectangle> getHitBox() const override;
	ObjectType getObjectType() const override;
	Vector2 getSize() const override;

private:
	float timer;
	bool triggering;
	int currentFrame;
	float aniTimer;
	float aniSpeed;
	std::vector<Rectangle> frameData;
};