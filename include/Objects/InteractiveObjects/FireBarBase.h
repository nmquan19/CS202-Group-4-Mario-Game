#pragma once
#include "InteractiveObject.h"
#include "FireBar.h"

class FireBarBase : public InteractiveObject {
public:
	FireBarBase(Vector2 position, Vector2 size);
	virtual ~FireBarBase();

	void update(float deltaTime) override;
	void draw() override;

	void onCollision(std::shared_ptr<Object> other, Direction direction) override;

	std::vector<Rectangle> getHitBox() const override;
	ObjectType getObjectType() const override;
	Vector2 getSize() const override;

private:
	std::shared_ptr<Object> fireBar;
};