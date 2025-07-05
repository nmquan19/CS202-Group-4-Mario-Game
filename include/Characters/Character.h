#pragma once
#include <vector>
#include "raylib.h"
#include "../Objects/ObjectFactory.h"
#include "../System/Interface.h"
#include "../System/PhysicsManager.h"

struct CharacterStats{
	float baseSpeed;
	float jumpForce;
	float gravity;
};

class ICharacterState;

class Character : public Object, public IUpdatable, public IMovable {
public:
	Character(Vector2 startPosition,  const CharacterStats& stats, const std::vector<std::vector<Rectangle>>& stateFrameData, CharacterType type, float scale = 2.0f);
	~Character();
	void changeState(ICharacterState& newState);
  
	void update(float deltaTime) override;
	void draw() override;

	Rectangle getCurrentStateFrame() const;
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
	void onCollision(Object* other) override;
	bool isActive() const override;
	void setActive(bool) override;
	bool isCollided() const override;
	void setCollided(bool flag) override;

	float getWidth() const;
	float getHeight() const;
	float getBottom() const;
	float getCenterX() const;
	float getCenterY() const;
	Vector2 getCenter() const;
	
private:
	ICharacterState* currentState;
	CharacterType characterType;

	float scale;
	Vector2 velocity;

	bool onGround = true;
	float speed;
	float jumpForce;
	float gravity;

	bool facingRight;

	Texture2D spriteSheet;
	std::vector<std::vector<Rectangle>> stateFrameData;
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
