#pragma once
#include "InteractiveObject.h"

class Spring : public InteractiveObject {
public:
	Spring(Vector2 position, Vector2 size);

	void update(float deltaTime) override;
	void draw() override;

	void checkCollision(const std::vector<std::shared_ptr<Object>>& candidates) override;
	void onCollision(std::shared_ptr<Object> other) override;

	std::vector<Rectangle> getHitBox() const override;
	ObjectType getObjectType() const override;
	Vector2 getSize() const override;

	void updateHitBox();

private:
	bool isBouncing;
	float bouncingTimer;
	float aniTimer;
	float aniSpeed;
	int currentFrame;
	Rectangle spriteRec;
	std::vector<Rectangle> frameData;
};