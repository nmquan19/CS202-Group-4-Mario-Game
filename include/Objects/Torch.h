#pragma once
#include "ObjectFactory.h"
#include "../System/LightingSystem.h"
#include "../System/InterpolationController.h"
#include "../System/Interface.h"
class Torch : public Object, public IUpdatable, public ISavable
{
private:
	Rectangle spriteBox;  
	Texture2D  texture;
	std::shared_ptr<LightSource> source; 
	int curFrame;
	InterpolatedAnimationController animController;
public:
	Torch(Vector2 position, Vector2 size, const LightSource& light);
	Torch(Vector2 position, Vector2 size, float radius,float brightness, Color innerColor, Color outerColor);

	bool isActive() const;
	void setActive(bool);
	bool isCollided() const;
	void setCollided(bool);
	Vector2 getPosition() const;
	void setPosition(Vector2 newPos);
	Vector2 getSize() const;
	void draw();
	void update(float deltaTime);

	std::vector<Rectangle> getHitBox() const override { return {}; };
	ObjectCategory getObjectCategory() const override { return ObjectCategory::BACKGROUND; }
	std::vector<ObjectCategory> getCollisionTargets() const {
		return {};
	}
	void onCollision(std::shared_ptr<Object> other, Direction direction) override {};
	ObjectType getObjectType() const { return BackGroundObjectType::TORCH; }

	json toJson() const override;
	void fromJson(const json& data) override;
	std::string getSaveType() const;
};