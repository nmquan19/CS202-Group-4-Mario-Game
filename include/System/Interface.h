#pragma once
#include "raylib.h"

class Object;

enum class ObjectCategory {
	CHARACTER,
	BLOCK,
	ENEMY,
	ITEM,
	PROJECTILE,
	TRIGGER
};

enum class BlockType {
	GROUND,
	BRICK,
	PIPE,
	QUESTION,
	INVISIBLE,
	PLATFORM
};

enum class EnemyType {
	GOOMBA,
	KOOPA,
	PIRANHA_PLANT,
	HAMMER_BRO
};

enum class ItemType {
	COIN,
	MUSHROOM,
	FIRE_FLOWER,
	STAR,
	ONE_UP
};

enum class CharacterType {
	MARIO,
	LUIGI
};

enum class TriggerType {
	CHECKPOINT,
	LEVEL_END,
	WARP_PIPE
};

class IDrawable {
public:
	virtual ~IDrawable() = default;
	virtual void draw() = 0;
};

class ICollidable {
public:
	virtual ~ICollidable() = default;
	virtual Rectangle getHitBox() const = 0;
	virtual ObjectCategory getObjectCategory() const = 0;
	virtual std::vector<ObjectCategory> getCollisionTargets() const = 0;
	virtual void checkCollision(const std::vector<Object*>& candidates) = 0;
	virtual void onCollision(Object* other) = 0;
};