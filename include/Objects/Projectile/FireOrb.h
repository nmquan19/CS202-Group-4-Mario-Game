#pragma once
#include "Projectile.h"

class FireOrb : public Projectile {
public:
	FireOrb(Vector2 position, int direction, Vector2 size);
	virtual ~FireOrb() = default;
	void draw() override;
	void onCollision(std::shared_ptr<Object> other, Direction direction) override;
	ObjectType getObjectType() const override;
private:
	int direction;
	float timer;
};