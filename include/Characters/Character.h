#pragma once
#include <vector>
#include "raylib.h"
#include "../Objects/ObjectFactory.h"
#include "../System/Interface.h"
#include "../System/PhysicsManager.h"

class ICharacterState;

class Character : public Object, public IUpdatable, public IMovable, public IDamageable {
public:
	Character(Vector2 startPosition,  const CharacterStats& stats, const std::vector<std::vector<Rectangle>>& stateFrameData, CharacterType type, float scale);
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
	void checkCollision(const std::vector<std::shared_ptr<Object>>& candidates) override;
	void onCollision(std::shared_ptr<Object> other) override;
	bool isActive() const override;
	void setActive(bool) override;
	bool isCollided() const override;
	void setCollided(bool flag) override;

	ObjectType getObjectType() const override;
	Vector2 getSize() const override;

	void takeDamage(int amount) override;
	bool isAlive() const override;
	void die() override;

	void setHoldingProjectile(bool flag);
	bool isHoldingProjectile() const;
	void holdProjectile(KoopaShell& p);

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

	int hp;
	float invincibleTime;
	float invincibleTimer;
	
	KoopaShell* projectile;
	bool holdingProjectile;

	void handleEnvironmentCollision(std::shared_ptr<Object> other);
	void handleEnemyCollision(std::shared_ptr<Object> other);
	//void handleItemCollsion(ICollidable* other);
};
