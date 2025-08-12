#pragma once  

#include <vector>  
#include <memory>  
#include "../System/Interface.h"  
#include "../System/Constant.h"
#include <raylib.h>  
#include "box2d/box2d.h"

class Item;  
class Block;  
class Character;  
struct CharacterStats;  
class Enemy;  
class KoopaShell;  

class Object : public ICollidable, public IDrawable {  
public:  
	virtual ~Object();  
	virtual bool isActive() const = 0;  
	virtual void setActive(bool) = 0;  
	virtual bool isCollided() const = 0;  
	virtual void setCollided(bool) = 0;  
	virtual Vector2 getPosition() const = 0;  
	virtual void setPosition(Vector2 newPos) = 0;  
	virtual Vector2 getSize() const = 0;  

	virtual void draw() override = 0;  

	virtual std::vector<Rectangle> getHitBox() const override = 0;  
	virtual ObjectCategory getObjectCategory() const override = 0;  
	virtual std::vector<ObjectCategory> getCollisionTargets() const override = 0;  
	virtual void onCollision(std::shared_ptr<Object> other, Direction direction) override = 0;  
	int getCollidedPart(const Object& other);  
	virtual ObjectType getObjectType() const = 0;  

	b2Body* getPhysicsBody() const {
		return physicsBody;
	}

protected:  
	Vector2 position;  
	Vector2 size;  
	bool active = true;  
	bool collided = false;  
	b2Body* physicsBody;
};  

class ObjectFactory {
public:
	static std::unique_ptr<Object> createBlock(BlockType type, Vector2 gridPos);
	static std::unique_ptr<Object> createCharacter(CharacterType type, Vector2 startPosition, float scale = 4.0f);
	static std::unique_ptr<Object> createEnemy(EnemyType type, Vector2 startPosition, Vector2 size);
	static std::unique_ptr<Object> createKoopaShell(KoopaShellType type, Vector2 position, Vector2 size);
	static std::unique_ptr<Object> createSpring(Vector2 position, Vector2 size = Constants::Spring::STANDARD_SIZE);
	static std::unique_ptr<Object> createItem(ItemType type, Vector2 startPos, Vector2 size);
	static std::unique_ptr<Object> createTorch(Vector2 position, Vector2 size = Constants::Torch::STANDARD_SIZE, float brightness = Constants::Torch::STANDARD_BRIGHTNESS, float radius = Constants::Torch::STANDARD_LIGHT_RADIUS, Color innerColor = { 48,252,244,255 }, Color outerColor = { 0,171,254,255 });

private:
	static std::unique_ptr<Block> createSpecificBlock(BlockType type, Vector2 gridPos);
	static std::unique_ptr<Enemy> createSpecificEnemy(EnemyType type, Vector2 startPosition, Vector2 size);
	static std::unique_ptr<Character> createSpecificCharacter(CharacterType type, Vector2 startPosition, float scale);
	static std::unique_ptr<Item> createSpecificItem(ItemType type, Vector2 startPos, Vector2 size);
	static CharacterStats getStats(CharacterType type);
    static std::vector<std::vector<Rectangle>> getFrameData(CharacterType type);
};