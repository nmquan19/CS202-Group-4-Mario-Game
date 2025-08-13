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
#include <box2d/box2d.h>
#include "../../include/Enemy/EnemyAI/EnemyNavigator.h"

class EnemyState; 

class Enemy : public Object, public IUpdatable, public IMovable, public IDamageable {
public:
	Enemy(Vector2 startPos, Texture2D texture, Vector2 size);
	Enemy(Vector2 startPos, Vector2 velocity, Vector2 accelleration, Texture2D texture);
	virtual ~Enemy();
	virtual void update(float deltaTime) override;
	virtual void draw();

	std::vector<Rectangle> getHitBox() const override;
	ObjectCategory getObjectCategory() const override;
	virtual void onCollision(std::shared_ptr<Object> other, Direction dir) override = 0;
	std::vector<ObjectCategory> getCollisionTargets() const override;

	bool isActive() const override;
	void changeState(EnemyState* other);
	int curFrame; 	
	
public :  
	void setActive(bool flag) override ;
	void setPosition(Vector2 newPosition) ;
	Vector2 getPosition() const override ;
	void setVelocity(Vector2 newVelocity) override ;
	Vector2 getVelocity() override ;
	bool isCollided() const override ;
	void setCollided(bool flag) override;
	void setSpeed(float newSpeed);
	float getSpeed();
	Vector2 getDirection()const; 
	float getWidth() const;
	float getHeight() const;
	float getBottom() const;
	float getCenterX() const;
	float getCenterY() const;
	bool FacingRight() const;  
	Vector2 getCenter() const;
	void setTarget(Vector2 pos);
	virtual ObjectType getObjectType() const override = 0 ;
	virtual EnemyType getType() const =0;
	Vector2 getSize() const override;
	virtual void takeDamage(int damage) override =0;
	bool isAlive() const override;
	virtual void die() override = 0;
	virtual void handleEnvironmentCollision(std::shared_ptr<Object> other, Direction dir) = 0;
	std::vector<std::pair<int, int>> getSpriteData();
	virtual void setAnimation(const std::string& ani_type) {};
	void flipDirection(); 
	void setDirection(Vector2 newDirection) { direction = newDirection; }
	InterpolatedAnimationController& getAnimController() { return animController; }
	FrameInterpolatedVelocityController& getVelocityController() { return velocityController; }
	bool isOnGround() const { return onGround; }
	virtual float getWalkVelocity() const {
		return 0;
	}
	virtual float getJumpVelocity() const
	{
		return 0;
	}
	float getGravityScale()
	{
		if (!physicsBody) return 0;
		return physicsBody->GetGravityScale();
	}
	virtual NodeType getTraverseType() { return NodeType::Ground; };
public: 
	virtual bool isInChaseRange() { return false; }
	virtual bool checkWallContact()  { return false; } ;
	Vector2 getTargetPos() const { return targetPosition;  }
	virtual void jump(){}
	virtual int isNearWall() const { return false; } 
	virtual bool canWallJump() const { return false; }
	virtual void wallJump() {}; 
	virtual void jumpFromWall() {}; 
	virtual bool isPlayerAtHigherGround() const;
	virtual void walkToTarget() {}; 
	virtual bool moveToTarget();
	virtual void attack() {}; 
	virtual bool isAttacking() { return false; }	
	virtual void patrol() {}; 
	virtual bool isPatrolling(){ return false;  }
	virtual void idle() {};
	virtual bool isTurning() const { return false; }
	virtual void walkTurn(){}
	virtual float jumpTo(Vector2 position, bool apply); 
	std::string getCurAnimation() const { return curAniName; }
	virtual void executeTraversal(const Edge& edge);
	virtual void retreat() {};
	virtual bool isRetreating() {
		return false;
	};
	virtual bool isNearTarget() const { return false; }
	virtual bool isInWallJump() const { return false; }
	virtual bool isJumping() const { return false;  }
	virtual bool isBelowWall() const { return false; } 
	virtual bool canUseBasicAttack() const{ return false; }
protected://pathfinding
	std::shared_ptr<NavGraphNode> currentNode = nullptr;
	std::shared_ptr<NavGraphNode> lastTargetNode = nullptr;
	std::vector<std::shared_ptr<NavGraphNode>> currentPath;
	int pathIndex = 0;
	bool isTraversing = false;
public://pathfinding
	void onTraversalFinished();

protected:
	InterpolatedAnimationController animController;
	FrameInterpolatedVelocityController velocityController;
	std::string curAniName;
	Vector2 targetPosition; 
	Rectangle targetHitboxes;
	bool isalive; 
	float aniTimer, aniSpeed; 
    int HP ;
	bool onGround;
	EnemyState* currentState;
	float speed; 
	float scale;  
	std::vector<Rectangle> hitboxes;
	Rectangle hitbox; 
	bool isFacingRight;
	Vector2 position;
	bool active;
	Vector2 direction; 
	Texture2D texture; 
	Rectangle spritebox;  
	float hitBoxWidth;
	float hitBoxHeight;
	Vector2 velocity; 
	Vector2 accelleration; 
	float groundLevel = INT_MAX;
	bool collided;
	
};



