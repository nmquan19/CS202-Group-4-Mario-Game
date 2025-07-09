#pragma once
#include "..\System\Interface.h"
#include "raylib.h"
#include "../Objects/ObjectFactory.h"
#include <vector>
#include <utility>
#include <climits>
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
	virtual void onCollision(Object* other) override;
	std::vector<ObjectCategory> getCollisionTargets() const override;
	virtual void checkCollision(const std::vector<Object*>& candidates) override =0 ;

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

	virtual void takeDamage(int damage) override =0;
	bool isAlive() const override;
	virtual void die() override = 0;

protected:
	bool isalive; 
	float aniTimer, aniSpeed; 
	virtual void handleEnvironmentCollision(Object* other);
    int HP ;
	bool onGround;
	EnemyState* currentState;
	float speed; 
	std::vector<int> numSprites; 	
	std::vector<std::pair<int,int>> num_sprites; 
	float scale;  
	Rectangle hitbox; 
	bool isFacingRight; 
	int max_numSprites; 
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



