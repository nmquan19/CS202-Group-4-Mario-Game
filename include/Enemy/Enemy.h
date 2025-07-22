#pragma once
#include "..\System\Interface.h"
#include "raylib.h"
#include "../Objects/ObjectFactory.h"
#include <vector>
#include <utility>
#include <string>
#include <climits>
#include "../System/InterpolationController.h"
#include <memory>
class EnemyState; 

class Enemy : public Object, public IUpdatable, public IMovable, public IDamageable {
public:
	Enemy(Vector2 startPos, Texture2D texture, Vector2 size);
	Enemy(Vector2 startPos, Vector2 velocity, Vector2 accelleration, Texture2D texture);
	~Enemy();
	virtual void update(float deltaTime) override;
	virtual void draw();

	Rectangle getHitBox() const override;
	ObjectCategory getObjectCategory() const override;
	virtual void onCollision(std::shared_ptr<Object> other) override;
	std::vector<ObjectCategory> getCollisionTargets() const override;
	virtual void checkCollision(const std::vector<std::shared_ptr<Object>>& candidates) override = 0 ;

	bool isActive() const override;
	void changeState(EnemyState* other);
	int curFrame; 	
public :  
	void applyGravity(float deltaTime);
	void setActive(bool flag) override ;
	void setPosition(Vector2 newPosition) ;
	Vector2 getPosition() const override ;
	void setVelocity(Vector2 newVelocity) override ;
	Vector2 getVelocity() override ;
	bool isCollided() const override ;
	void setCollided(bool flag) override;
	void setSpeed(float newSpeed);
	float getSpeed();

	float getWidth() const;
	float getHeight() const;
	float getBottom() const;
	float getCenterX() const;
	float getCenterY() const;
	bool FacingRight() const;  
	Vector2 getCenter() const;
	virtual ObjectType getObjectType() const override = 0 ;
	virtual EnemyType getType() const =0;
	Vector2 getSize() const override;
	virtual void takeDamage(int damage) override =0;
	bool isAlive() const override;
	virtual void die() override = 0;
	virtual void handleEnvironmentCollision(std::shared_ptr<Object> other);
	std::vector<std::pair<int, int>> getSpriteData();
	virtual void setAnimation(const std::string& ani_type) {};
	void flipDirection(); 
	InterpolatedAnimationController& getAnimController() { return animController; }
	FrameInterpolatedVelocityController& getVelocityController() { return velocityController; }
public: 
	Vector2 getTargetPos() const { return targetPosition;  }
	virtual void walkToTarget() {}; 
	virtual void attack() {}; 
	virtual bool isAttacking() { return false; }	
	virtual void patrol() {}; 
	virtual void idle() {};
	virtual void avoidDanger() {};
	virtual bool isNearTarget() const { return false; }
 protected:
	InterpolatedAnimationController animController;
	FrameInterpolatedVelocityController velocityController;
	std::string curAniName;
	Vector2 targetPosition; 
	bool isalive; 
	float aniTimer, aniSpeed; 
    int HP ;
	bool onGround;
	EnemyState* currentState;
	float speed; 
	float scale;  
	Rectangle hitbox; 
	bool isFacingRight;
	Vector2 position;
	bool active;
	Texture2D texture; 
	Rectangle spritebox;  
	float hitBoxWidth;
	float hitBoxHeight;
	Vector2 velocity; 
	Vector2 accelleration; 
	float groundLevel = INT_MAX;
};



