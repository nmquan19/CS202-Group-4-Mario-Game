#pragma once
#include "raylib.h"
#include <vector>
#include <variant>
#include <memory>
#include <box2d/box2d.h>

class Object;
enum class ObjectCategory : uint16 {
	CHARACTER = 0x0001,
	BLOCK = 0x0002,
	ENEMY = 0x0004,
	ITEM = 0x0008,
	PROJECTILE = 0x0010,
	TRIGGER = 0x0020,
	SHELL = 0x0040,
	INTERACTIVE = 0x0080,
	BACKGROUND = 0x0100
};

enum class PlayerID {
	PLAYER_01,
	PLAYER_02
};

enum class PowerState {
	SMALL,
	SUPER,
	FIRE
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
	HAMMER_BRO,
	DRY_BOWSER
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

enum class InteractiveType {
	MOVING_PLATFORM,
	SPRING
};

enum class ProjectileType {
	FIRE_BALL,
	BLAST
};

enum class Direction {
	UP = 1,
	DOWN,
	LEFT,
	RIGHT,
	NONE
};

class IDrawable {
public:
	virtual ~IDrawable() = default;
	virtual void draw() = 0;
};

class ICollidable {
public:
	virtual ~ICollidable() = default;
	virtual std::vector<Rectangle> getHitBox() const = 0;
	virtual ObjectCategory getObjectCategory() const = 0;
	virtual std::vector<ObjectCategory> getCollisionTargets() const = 0;
	virtual void onCollision(std::shared_ptr<Object> other, Direction direction = Direction::NONE) = 0;
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

using ObjectType = std::variant<CharacterType, BlockType, EnemyType, KoopaShellType, TriggerType, ItemType, InteractiveType, ProjectileType>;