#pragma once
#include <vector>
#include "raylib.h"
#include "../Objects/ObjectFactory.h"
#include "../System/Interface.h"
#include "../System/PhysicsManager.h"

struct StateFrame{
	Vector2 startPosition;
	float width;
	float height;

	StateFrame(float x, float y, float w, float h) : startPosition(Vector2{x, y}), width(w), height(h){}

	Rectangle toRectangle() const{
		return {startPosition.x, startPosition.y, width, height};
	}
};

struct CharacterStats{
	float baseSpeed;
	float jumpForce;
	float gravity;
};

class ICharacterState;

class Character : public Object, public IUpdatable, public IMovable {
public:
	Character(CharacterType type, Vector2 startPosition, float scale = 2.0f);
	~Character();
	void changeState(ICharacterState& newState);
  
	void update(float deltaTime) override;
	void draw() override;

	StateFrame getCurrentStateFrame() const;
	int getCurrentStateRow() const;
	void setSpriteRec(Rectangle newRec);
	void setCurrentStateRow(int newRow);
	void setFrame(int newFrame);
	void setAniTime(float newTime);
	void setAniSpeed(float newSpeed);

	bool isOnGround() const;
  	void setOnGround(bool flag);
	void jump();
	void applyGravity(float deltaTime);

	void setGroundLevel(float newGroundLevel);

	void setVelocity(Vector2 newVelocity);
	Vector2 getVelocity();

	void setSpeed(float newSpeed);
	float getSpeed();

	void setPosition(Vector2 newPosition);
	Vector2 getPosition() const;

	bool isFacingRight() const;
	void setFacingRight(bool flag);

	void updateHitBox();
	Rectangle getHitBox() const override;
	ObjectCategory getObjectCategory() const override;
	std::vector<ObjectCategory> getCollisionTargets() const override;
	void checkCollision(const std::vector<Object*>& candidates) override;
	void onCollision(Object* other) override {}
	bool isActive() const override;
	void setActive(bool flag) override;

	bool isCollided() const override;
	void setCollided(bool flag) override;

	float getWidth() const;
	float getHeight() const;
	float getBottom() const;
	float getCenterX() const;
	float getCenterY() const;
	Vector2 getCenter() const;

private:
	void loadCharacterData(CharacterType type);
	CharacterStats getCharacterStats(CharacterType type);
	std::vector<std::vector<StateFrame>> getCharacterFrameData(CharacterType type);
	const char* getCharacterSpritePath(CharacterType type);
	
private:
	ICharacterState* currentState;
	CharacterType characterType;

	float scale;
	Vector2 velocity;

	bool onGround = true;
	float speed;
	float jumpForce;
	float gravity;
	float groundLevel = 500.0f;

	bool facingRight;

	Texture2D spriteSheet;
	std::vector<std::vector<StateFrame>> stateFrameData;
	Rectangle spriteRec;
	int currentFrame;
	int currentStateRow;
	float aniTimer;
	float aniSpeed;

	float hitBoxWidth;
	float hitBoxHeight;
	bool active = true;

	void handleEnvironmentCollision(Object* other);
	//void handleItemCollsion(ICollidable* other);
};
