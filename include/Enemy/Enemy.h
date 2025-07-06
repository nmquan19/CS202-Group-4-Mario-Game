#pragma once
#include "..\System\Interface.h"
#include "raylib.h"
#include "../Objects/ObjectFactory.h"
#include <vector>
#include <utility>
class EnemyState; 
class Enemy : public Object, public IUpdatable, public IMovable {
public:
	Enemy(Vector2 startPos, Texture2D texture, float scale);
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
	Vector2 getCenter() const;
protected:
	float aniTimer, aniSpeed; 
	void handleEnvironmentCollision(Object* other);
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


class LedgeDetector {
public:
    LedgeDetector(float offsetX, float castLength);
    void update(Enemy* enemy, float deltaTime);
    bool isNearLedge() const;
private:
    bool nearLedge;
    float offsetX;
    float castLength;	
};
