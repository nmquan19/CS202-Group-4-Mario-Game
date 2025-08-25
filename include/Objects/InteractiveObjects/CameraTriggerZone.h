#pragma once
#include "InteractiveObject.h"
#include "../../System/Interface.h"


struct SwitchCameraTriggerZoneData {
	Vector2 position;
	Vector2 size;
	int fromIndex;
	int toIndex;
	Rectangle inWorldBounds;
	Rectangle outWorldBounds;
	Camera2D cameraIn; 
	Camera2D cameraOut;
	bool directSwitch = true;
};
class SwitchCameraTriggerZone : public InteractiveObject, public IOnCollisionExit{
private: 
	SwitchCameraTriggerZoneData data; 
	std::unordered_map<Object*, int> activeContacts;
public:
	virtual void update(float deltaTime) override;
	virtual void draw() override;
	void onCollision(std::shared_ptr<Object> other, Direction direction);
	void onCollisionExit(std::shared_ptr<Object> other, Direction direction) override;
	ObjectType getObjectType() const;
	SwitchCameraTriggerZone(Vector2 position, Vector2 size, SwitchCameraTriggerZoneData data);
	SwitchCameraTriggerZone(Vector2 position, Vector2 size);
	~SwitchCameraTriggerZone();
	Vector2 getSize() const override {
		return size;
	};
	json toJson() const override;
	void fromJson(const json& data) override;

};