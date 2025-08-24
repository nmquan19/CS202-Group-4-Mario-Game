#pragma once
#include "InteractiveObject.h"

class Spring : public InteractiveObject {
public:
	Spring(Vector2 position, Vector2 size);
	virtual ~Spring();

	void update(float deltaTime) override;
	void draw() override;

	void onCollision(std::shared_ptr<Object> other, Direction direction) override;

	std::vector<Rectangle> getHitBox() const override;
	ObjectType getObjectType() const override;
	Vector2 getSize() const override;

	void setAniTimer(float val);
	void setBouncing(bool flag);
	void setBounceTimer(float val);

	void updateHitBox();

private:
	bool isBouncing;
	float bounceTimer;
	float aniTimer;
	float aniSpeed;
	int currentFrame;
	float bottomY;
	Rectangle spriteRec;
	std::vector<Rectangle> frameData;
};