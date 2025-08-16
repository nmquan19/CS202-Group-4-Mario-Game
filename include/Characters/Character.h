#pragma once
#include <vector>
#include "raylib.h"
#include "../Objects/ObjectFactory.h"
#include "../System/Interface.h"
#include "../Objects/InteractiveObjects/Spring.h"
#include "../Objects/InteractiveObjects/MovingPlatform.h"

struct PlayerInputMapping {
	int moveLeft;
	int moveRight;
	int jump;
	int superTransform;
	int fireTransform;
	int attack;

	static PlayerInputMapping getMapping(PlayerID id) {
		switch (id) {
		case PlayerID::PLAYER_01:
			return { KEY_A, KEY_D, KEY_W, KEY_F1, KEY_F2, KEY_F };
		case PlayerID::PLAYER_02:
			return { KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_F3, KEY_F4, KEY_J };
		}
	}
};

struct InputState {
	bool moveLeft = false;
	bool moveRight = false;
	bool jump = false;
	bool jumpPressed = false;
	bool jumpReleased = false;
	bool superTransform = false;
	bool fireTransform = false;
	bool starTransform = false;
	bool attack = false;

	static InputState fromPlayer(PlayerID playerID) {
		PlayerInputMapping mapping = PlayerInputMapping::getMapping(playerID);

		InputState input;
		input.moveLeft = IsKeyDown(mapping.moveLeft);
		input.moveRight = IsKeyDown(mapping.moveRight);
		input.jump = IsKeyDown(mapping.jump);
		input.jumpPressed = IsKeyPressed(mapping.jump);
		input.jumpReleased = IsKeyReleased(mapping.jump);
		input.superTransform = IsKeyPressed(mapping.superTransform);
		input.fireTransform = IsKeyPressed(mapping.fireTransform);
		input.starTransform = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && IsKeyPressed(mapping.fireTransform);
		input.attack = IsKeyPressed(mapping.attack);
		return input;
	}
};

class ICharacterState;

class Character : public Object, public IUpdatable, public IMovable, public IDamageable, public ISavable {
public:
	Character(Vector2 startPosition,  const CharacterStats& stats, const std::vector<std::vector<Rectangle>>& stateFrameData, CharacterType type, PlayerID id, Vector2 size);
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

	bool isOnPlatform() const;
	bool isOnGround() const;
	void jump();

	void setVelocity(Vector2 newVelocity);
	Vector2 getVelocity();
	
	void setSpeed(float newSpeed);
	float getSpeed();

	void setPosition(Vector2 newPosition);
	Vector2 getPosition() const;

	bool isFacingRight() const;
	void setFacingRight(bool flag);
	Vector2 getCenterPos() const ;
	std::vector<Rectangle> getHitBox() const override;
	ObjectCategory getObjectCategory() const override;
	std::vector<ObjectCategory> getCollisionTargets() const override;
	void onCollision(std::shared_ptr<Object> other, Direction direction) override;
	PowerState getPowerState() const;

	bool isActive() const override;
	void setActive(bool) override;
	bool isCollided() const override;
	void setCollided(bool flag) override;

	ObjectType getObjectType() const override;
	Vector2 getSize() const override;

	void takeDamage(int amount) override;
	bool isAlive() const override;
	void die() override;

	json toJson() const override;
	void fromJson(const json& data) override;
	std::string getSaveType() const override;

	PowerState getPowerState() const;

	friend class IdleState;
	friend class MovingState;
	friend class JumpingState;
	friend class StunnedState;
	friend class KnockedState;
	friend class AttackState;
	friend class SuperTransformState;
	friend class SmallTransformState;
	friend class FireTransformState;
	friend class StarTransformState;
	friend class RefreshPowerState;

	void addGroundContact();
	void removeGroundContact();
	void setPlatform(std::shared_ptr<MovingPlatform> platform);

	void enableRainbowEffect();
	void disableRainbowEffect();
	bool isRainbowEffectActive() const;

private:
	void handleEnvironmentCollision(std::shared_ptr<Object> other, Direction direction);
	void handleEnemyCollision(std::shared_ptr<Object> other, Direction direction);
	void handleInteractiveCollision(std::shared_ptr<Object> other, Direction direction);
	void handleSpringCollision(std::shared_ptr<Spring> other, Direction direction);
	void handleMovingPlatformCollision(std::shared_ptr<MovingPlatform> other, Direction direction);
	void handleItemCollision(std::shared_ptr<Object> other, Direction direction);

private:
	ICharacterState* currentState;
	CharacterType characterType;
	PlayerID id;
	PowerState powerState;

	Vector2 velocity;

	float speed;
	float jumpVel;

	bool facingRight;

	Texture2D spriteSheet;
	std::vector<std::vector<Rectangle>> stateFrameData;
	Rectangle spriteRec;
	int currentFrame;
	int currentStateRow;
	float aniTimer;
	float aniSpeed;

	bool active = true;

	int hp;
	float invincibleTimer;
	float reviveTimer;
	float transformTimer;
	float attackTimer;
	float starTimer;

	int projectilesLeft = 0;

	MovingPlatform* platform;
	int groundContactCount = 0;

	bool rainbowEffect = false;
	float rainbowTimer = 0.0f;
};