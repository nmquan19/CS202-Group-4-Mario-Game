#pragma once
#include "Projectile.h"

class FireBall : public Projectile {
public:
	FireBall(Vector2 position, int direction, Vector2 size);
	virtual ~FireBall() = default;
	void update(float deltaTime) override;
	ObjectType getObjectType() const override;
private:
	float timer;
};