#pragma once
#include "raylib.h"
#include <vector>
#include <variant>
class Object;
enum class ObjectCategory {
	CHARACTER,
	BLOCK,
	ENEMY,
	ITEM,
	PROJECTILE,
	TRIGGER,
	SHELL,
	BACKGROUND,
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
	GREEN_KOOPA,
	RED_KOOPA,
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
enum class KoopaShellType {
	GREEN_KOOPA_SHELL,
	RED_KOOPA_SHELL
};
enum class TriggerType {
	CHECKPOINT,
	LEVEL_END,
	WARP_PIPE,
	DETECTOR,
	ZONE,
};
enum class Direction {
	UP = 1,
	DOWN,
	LEFT,
	RIGHT
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
class IDamageable {
public:
	virtual ~IDamageable() = default;
	virtual void takeDamage(int amount) = 0;
	virtual bool isAlive() const = 0;
	virtual void die() = 0;
};
class IUpdatable {
public:
	virtual ~IUpdatable() = default;
	virtual void update(float deltaTime) = 0;
};
class IMovable {
public:
	virtual ~IMovable() = default;
	virtual void setVelocity(Vector2 newVelocity) = 0;
	virtual Vector2 getVelocity() = 0;
};
using ObjectType = std::variant<CharacterType, BlockType, EnemyType, KoopaShellType, TriggerType, ItemType>;