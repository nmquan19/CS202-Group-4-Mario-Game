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
	PIPE,
	QUESTION,
	INVISIBLE,
	PLATFORM,

	BLOCK_1_1_2,
	BLOCK_1_1_3,
	BLOCK_1_1_12,
	BLOCK_1_1_13,
	BLOCK_1_1_14,
	BLOCK_1_1_15,
	BLOCK_1_1_16,
	BLOCK_1_2_7,
	BLOCK_1_2_8,
	BLOCK_1_2_12,
	BLOCK_1_2_13,
	BLOCK_1_2_14,
	BLOCK_1_2_15,
	BLOCK_1_2_16,
	BLOCK_1_3_4,
	BLOCK_1_3_5,
	BLOCK_1_3_6,
	BLOCK_1_3_7,
	BLOCK_1_3_8,
	BLOCK_1_3_15,
	BLOCK_1_3_16,
	BLOCK_1_4_4,
	BLOCK_1_4_5,
	BLOCK_1_4_6,
	BLOCK_1_4_7,
	BLOCK_1_4_11,
	BLOCK_1_4_12,
	BLOCK_1_4_13,
	BLOCK_1_4_14,
	BLOCK_1_4_15,
	BLOCK_1_4_16,
	BLOCK_1_5_4,
	BLOCK_1_5_5,
	BLOCK_1_5_6,
	BLOCK_1_5_8,
	BLOCK_1_5_9,
	BLOCK_1_5_10,
	BLOCK_1_5_11,
	BLOCK_1_5_12,
	BLOCK_1_5_13,
	BLOCK_1_5_14,
	BLOCK_1_5_15,
	BLOCK_1_5_16,
	BLOCK_1_6_5,
	BLOCK_1_6_6,
	BLOCK_1_7_1,
	BLOCK_1_7_2,
	BLOCK_1_7_3,
	BLOCK_1_7_4,
	BLOCK_1_7_5,
	BLOCK_1_7_6,
	BLOCK_1_7_7,
	BLOCK_1_7_11,
	BLOCK_1_7_12,
	BLOCK_1_7_13,
	BLOCK_1_7_14,
	BLOCK_1_7_15,
	BLOCK_1_7_16,

	BLOCK_2_1_2,
	BLOCK_2_1_3,
	BLOCK_2_1_12,
	BLOCK_2_1_13,
	BLOCK_2_1_14,
	BLOCK_2_1_15,
	BLOCK_2_1_16,
	BLOCK_2_2_7,
	BLOCK_2_2_8,
	BLOCK_2_2_12,
	BLOCK_2_2_13,
	BLOCK_2_2_14,
	BLOCK_2_2_15,
	BLOCK_2_2_16,
	BLOCK_2_3_4,
	BLOCK_2_3_5,
	BLOCK_2_3_6,
	BLOCK_2_3_7,
	BLOCK_2_3_8,
	BLOCK_2_3_15,
	BLOCK_2_3_16,
	BLOCK_2_4_4,
	BLOCK_2_4_5,
	BLOCK_2_4_6,
	BLOCK_2_4_7,
	BLOCK_2_4_11,
	BLOCK_2_4_12,
	BLOCK_2_4_13,
	BLOCK_2_4_14,
	BLOCK_2_4_15,
	BLOCK_2_4_16,
	BLOCK_2_5_4,
	BLOCK_2_5_5,
	BLOCK_2_5_6,
	BLOCK_2_5_8,
	BLOCK_2_5_9,
	BLOCK_2_5_10,
	BLOCK_2_5_11,
	BLOCK_2_5_12,
	BLOCK_2_5_13,
	BLOCK_2_5_14,
	BLOCK_2_5_15,
	BLOCK_2_5_16,
	BLOCK_2_6_5,
	BLOCK_2_6_6,
	BLOCK_2_7_1,
	BLOCK_2_7_2,
	BLOCK_2_7_3,
	BLOCK_2_7_4,
	BLOCK_2_7_5,
	BLOCK_2_7_6,
	BLOCK_2_7_7,
	BLOCK_2_7_11,
	BLOCK_2_7_12,
	BLOCK_2_7_13,
	BLOCK_2_7_14,
	BLOCK_2_7_15,
	BLOCK_2_7_16,

	BLOCK_3_1_2,
	BLOCK_3_1_3,
	BLOCK_3_1_12,
	BLOCK_3_1_13,
	BLOCK_3_1_14,
	BLOCK_3_1_15,
	BLOCK_3_1_16,
	BLOCK_3_2_7,
	BLOCK_3_2_8,
	BLOCK_3_2_12,
	BLOCK_3_2_13,
	BLOCK_3_2_14,
	BLOCK_3_2_15,
	BLOCK_3_2_16,
	BLOCK_3_3_4,
	BLOCK_3_3_5,
	BLOCK_3_3_6,
	BLOCK_3_3_7,
	BLOCK_3_3_8,
	BLOCK_3_3_15,
	BLOCK_3_3_16,
	BLOCK_3_4_4,
	BLOCK_3_4_5,
	BLOCK_3_4_6,
	BLOCK_3_4_7,
	BLOCK_3_4_11,
	BLOCK_3_4_12,
	BLOCK_3_4_13,
	BLOCK_3_4_14,
	BLOCK_3_4_15,
	BLOCK_3_4_16,
	BLOCK_3_5_4,
	BLOCK_3_5_5,
	BLOCK_3_5_6,
	BLOCK_3_5_8,
	BLOCK_3_5_9,
	BLOCK_3_5_10,
	BLOCK_3_5_11,
	BLOCK_3_5_12,
	BLOCK_3_5_13,
	BLOCK_3_5_14,
	BLOCK_3_5_15,
	BLOCK_3_5_16,
	BLOCK_3_6_5,
	BLOCK_3_6_6,
	BLOCK_3_7_1,
	BLOCK_3_7_2,
	BLOCK_3_7_3,
	BLOCK_3_7_4,
	BLOCK_3_7_5,
	BLOCK_3_7_6,
	BLOCK_3_7_7,
	BLOCK_3_7_11,
	BLOCK_3_7_12,
	BLOCK_3_7_13,
	BLOCK_3_7_14,
	BLOCK_3_7_15,
	BLOCK_3_7_16,

	BLOCK_4_1_2,
	BLOCK_4_1_3,
	BLOCK_4_1_12,
	BLOCK_4_1_13,
	BLOCK_4_1_14,
	BLOCK_4_1_15,
	BLOCK_4_1_16,
	BLOCK_4_2_7,
	BLOCK_4_2_8,
	BLOCK_4_2_12,
	BLOCK_4_2_13,
	BLOCK_4_2_14,
	BLOCK_4_2_15,
	BLOCK_4_2_16,
	BLOCK_4_3_4,
	BLOCK_4_3_5,
	BLOCK_4_3_6,
	BLOCK_4_3_7,
	BLOCK_4_3_8,
	BLOCK_4_3_15,
	BLOCK_4_3_16,
	BLOCK_4_4_4,
	BLOCK_4_4_5,
	BLOCK_4_4_6,
	BLOCK_4_4_7,
	BLOCK_4_4_11,
	BLOCK_4_4_12,
	BLOCK_4_4_13,
	BLOCK_4_4_14,
	BLOCK_4_4_15,
	BLOCK_4_4_16,
	BLOCK_4_5_4,
	BLOCK_4_5_5,
	BLOCK_4_5_6,
	BLOCK_4_5_8,
	BLOCK_4_5_9,
	BLOCK_4_5_10,
	BLOCK_4_5_11,
	BLOCK_4_5_12,
	BLOCK_4_5_13,
	BLOCK_4_5_14,
	BLOCK_4_5_15,
	BLOCK_4_5_16,
	BLOCK_4_6_5,
	BLOCK_4_6_6,
	BLOCK_4_7_1,
	BLOCK_4_7_2,
	BLOCK_4_7_3,
	BLOCK_4_7_4,
	BLOCK_4_7_5,
	BLOCK_4_7_6,
	BLOCK_4_7_7,
	BLOCK_4_7_11,
	BLOCK_4_7_12,
	BLOCK_4_7_13,
	BLOCK_4_7_14,
	BLOCK_4_7_15,
	BLOCK_4_7_16
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