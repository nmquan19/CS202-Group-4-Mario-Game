#pragma once
#include "../Objects/ObjectFactory.h"
#include "../System/Interface.h"
#include "../Enemy/Enemy.h"
#include <vector>
#include <raylib.h>
class LedgeDetector : public Object
{
public:
    LedgeDetector(float castLength);
    ~LedgeDetector();
    void update(Enemy* enemy, float deltaTime);

    std::vector<Rectangle> getHitBox() const ;
    ObjectCategory getObjectCategory() const ;
    std::vector<ObjectCategory> getCollisionTargets() const;
    void onCollision(std::shared_ptr<Object> other, Direction direction);
    bool isNearLedge() const;
	LedgeDetector& getInstance();

	bool isActive() const;
	void setActive(bool);
	bool isCollided() const ;
	void setCollided(bool);
    Vector2 getPosition() const ;
	void setPosition(Vector2 newPos);
    void draw() override ;

    ObjectType getObjectType() const override;
    Vector2 getSize() const override;
private:
    bool nearLedge;
    float castLength;
	Rectangle hitBox;  
}; 
