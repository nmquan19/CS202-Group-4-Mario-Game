#pragma once
#include "InteractiveObject.h"

class MovingPlatform : public InteractiveObject {
public:
	MovingPlatform(Vector2 position, Vector2 size);
	virtual ~MovingPlatform();

	void update(float deltaTime) override;
	void draw() override;

	void onCollision(std::shared_ptr<Object> other, Direction direction) override;

	std::vector<Rectangle> getHitBox() const override;
	ObjectType getObjectType() const override;
	Vector2 getSize() const override;

private:
	Rectangle spriteRec;
	float time;
};