#pragma once
#include "raylib.h"

enum class CollissionLayer {
	PLAYER,
	ENEMY,
	ENVIRONMENT,
	ITEM,
	PROJECTILE,
	TRIGGER,
};

class ICollidable {
public:
	virtual ~ICollidable() = default;
	virtual Rectangle getHitBox() const = 0;
	virtual CollissionLayer getCollisionLayer() const = 0;
	virtual std::vector<CollissionLayer> getCollisionTargets() const = 0;
	virtual void checkCollision(const std::vector<ICollidable*>& candidates) = 0;
	virtual void onCollision(ICollidable* other) = 0;
	virtual bool isActive() = 0;
	virtual void setActive(bool flag) = 0;
	virtual Vector2 getPosition() const = 0;
};

