#pragma once
#include "../ObjectFactory.h"

class Projectile : public Object, public IUpdatable {
public:
	Projectile(Vector2 position, Vector2 size);
	virtual ~Projectile();

	void update(float deltaTime) override;
	void draw() override;

	std::vector<Rectangle> getHitBox() const override;
	virtual ObjectCategory getObjectCategory() const override;
	virtual std::vector<ObjectCategory> getCollisionTargets() const override;
	virtual void onCollision(std::shared_ptr<Object> other, Direction direction) override;

	void setActive(bool flag) override;
	bool isActive() const override;

	void setCollided(bool flag) override;
	bool isCollided() const override;

	Vector2 getPosition() const override;
	void setPosition(Vector2 newPos) override;

	Vector2 getSize() const override;

	virtual ObjectType getObjectType() const = 0;

protected:
	Texture2D texture;
	std::vector<Rectangle> frameData;
	Rectangle spriteRec;
	int currentFrame;
	float aniTimer;
	float aniSpeed;
};