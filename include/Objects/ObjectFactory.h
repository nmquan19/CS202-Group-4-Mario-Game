#pragma once

#include <vector>
#include <memory>
#include "../System/Interface.h"
#include <raylib.h>

class Block;
class Character;
struct CharacterStats;

class Object : public ICollidable, public IDrawable {
public:
	virtual ~Object() = default;
	virtual bool isActive() const = 0;
	virtual void setActive(bool) = 0;
	virtual bool isCollided() const = 0;
	virtual void setCollided(bool) = 0;
	virtual Vector2 getPosition() const = 0;
	virtual void setPosition(Vector2 newPos) = 0;

	virtual void draw() override = 0;

	virtual Rectangle getHitBox() const override = 0;
	virtual ObjectCategory getObjectCategory() const override = 0;
	virtual std::vector<ObjectCategory> getCollisionTargets() const override = 0;
	virtual void checkCollision(const std::vector<Object*>& candidates) override = 0;
	void onCollision(Object* other) override = 0;
protected:
	Vector2 position;
	Vector2 size;
	bool active = true;
	bool collided = false;
};

class ObjectFactory {
public:
	static std::unique_ptr<Object> createBlock(BlockType type, Vector2 gridPos);
	static std::unique_ptr<Character> createCharacter(CharacterType type, Vector2 startPosition, float scale = 2.0f);

private:
	static std::unique_ptr<Block> createSpecificBlock(BlockType type, Vector2 gridPos);

	static std::unique_ptr<Character> createSpecificCharacter(CharacterType type, Vector2 startPosition, float scale);

    static CharacterStats getStats(CharacterType type);
    static std::vector<std::vector<Rectangle>> getFrameData(CharacterType type);
};