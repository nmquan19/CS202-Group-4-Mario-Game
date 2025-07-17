#pragma once  

#include <vector>  
#include <memory>  
#include "../System/Interface.h"  
#include <raylib.h>  
// Forward declaration for Item class  
class Item;  
class Block;  
class Character;  
struct CharacterStats;  
class Enemy;  
class KoopaShell;  

class Object : public ICollidable, public IDrawable {  
public:  
	virtual ~Object() = default;  
	virtual bool isActive() const = 0;  
	virtual void setActive(bool) = 0;  
	virtual bool isCollided() const = 0;  
	virtual void setCollided(bool) = 0;  
	virtual Vector2 getPosition() const = 0;  
	virtual void setPosition(Vector2 newPos) = 0;  
	virtual Vector2 getSize() const = 0;  

	virtual void draw() override = 0;  

	virtual Rectangle getHitBox() const override = 0;  
	virtual ObjectCategory getObjectCategory() const override = 0;  
	virtual std::vector<ObjectCategory> getCollisionTargets() const override = 0;  
	virtual void checkCollision(const std::vector<Object*>& candidates) override = 0;
	virtual void onCollision(Object* other) override = 0;  
	int getCollidedPart(const Object& other);  
	virtual ObjectType getObjectType() const = 0;  

protected:  
	Vector2 position;  
	Vector2 size;  
	bool active = true;  
	bool collided = false;  
};  

class ObjectFactory {  
public:  
	static std::unique_ptr<Object> createBlock(BlockType type, Vector2 gridPos);  
	static std::unique_ptr<Object> createCharacter(CharacterType type, Vector2 startPosition, float scale = 2.0f);  
	static std::unique_ptr<Object> createEnemy(EnemyType type, Vector2 startPosition, Vector2 size);  
	static std::unique_ptr<Object> createKoopaShell(KoopaShellType type, Vector2 position, Vector2 size);  
	static std::unique_ptr<Object> createItem(ItemType type, Vector2 startPos, Vector2 size);  
	static std::unique_ptr<Item> createCoin(Vector2 startPos, Vector2 size);  
	static std::unique_ptr<Item> createMushroom(Vector2 startPos, Vector2 size);  

private:  
	static std::unique_ptr<Block> createSpecificBlock(BlockType type, Vector2 gridPos);  
	static std::unique_ptr<Enemy> createSpecificEnemy(EnemyType type, Vector2 startPosition, Vector2 size);  
	static std::unique_ptr<Item> createSpecificItem(ItemType type, Vector2 startPos, Vector2 size);  

	static std::unique_ptr<Character> createSpecificCharacter(CharacterType type, Vector2 startPosition, float scale);  
	static std::unique_ptr<KoopaShell> createSpecificKoopaShell(KoopaShellType type, Vector2 position, Vector2 size);  
	static CharacterStats getStats(CharacterType type);  
	static std::vector<std::vector<Rectangle>> getFrameData(CharacterType type);  
};