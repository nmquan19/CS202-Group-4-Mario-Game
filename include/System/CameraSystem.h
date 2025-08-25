#pragma once
#include <raylib.h>
#include <vector> 
#include <memory>

#include "../../include/System/Constant.h"
#include "../../include/Characters/Character.h"
class GameCamera
{
protected: 
	Camera2D cam;  
	Vector2 shakeOffset; 
	float shakeIntensity =0.0f; 
	float shakeDecay = 0.9f;	  
	Rectangle bounds = { 0, 0, Constants::WORLDBOUNDS_WIDTH, Constants::WORLDBOUNDS_HEIGHT };
public: 
	virtual void update(float deltaTime) = 0; 
	void shake(float strenght, float decay); 
	const Camera2D& GetCamera() const { return cam; }
	void setCamera(const Camera2D& camera) { cam = camera; } 
	void setBounds(const Rectangle& newBounds) { bounds = newBounds;}
	Rectangle getBounds() const { return bounds; }

};
struct CameraTransition {
	bool active = false;
	float timer = 0.0f;
	float duration = 0.5f;
	Camera2D from;
	Camera2D to;
};

class PlayerCenteredCamera:public GameCamera
{
private: 
	float horizontalBias = 60.0f; 
	float verticalLerpSpeed = 5.0f;
	float fallEaseTimer = 0.0f;
	const float fallEaseDuration = 0.15f;
	std::vector<float> groundBounds;
	int curGroundIndex = 0;
	float currentLedgeBias = 0;
	int playerRequest = 1;
public: 
	PlayerCenteredCamera(); 
	PlayerCenteredCamera(int request);

	void update(float deltaTime) override;  
	void updateCameraTarget(
		float dt,
		Vector2 desired,
		Vector2 velocity,
		bool isMultiPlayer,
		std::shared_ptr<Character> refCharacter
	);
};
class StaticGameCamera : public GameCamera
{
private:
public: 
	StaticGameCamera();
	StaticGameCamera(const Camera2D& camera) {
		cam = camera;
	} 
	void update(float deltaTime) override {};
};  
class GameCameraSystem
{
private: 
	GameCameraSystem() =default; 
	std::vector<std::unique_ptr<GameCamera>> cameras; 
	int curIndex = 0 ;  
	CameraTransition transition;

public: 
	static GameCameraSystem& getInstance() {
		static GameCameraSystem instance;
		return instance;
	}
	void reset()
	{
		cameras.clear();
		init();
	}
	bool isInTransition() const { return transition.active; }
	void init(); 
	void addCamera(std::unique_ptr<GameCamera> camera);
	void switchCamera(int toIndex); 
	void update(float deltaTime);
	void applyBoundsOnSwitch();
	void shakeCurrentCamera(float strength, float decay);  
	void setCamera(const Camera2D& camera) {
		if (curIndex >= 0 && curIndex < cameras.size()) {
			cameras[curIndex]->setCamera(camera);

		}
	}
	void setCameraBounds(const Rectangle& bounds) {
		if (curIndex >= 0 && curIndex < cameras.size()) {
			cameras[curIndex]->setBounds(bounds);
		}
	}
	void setCameraBounds(const Rectangle& bounds,int index) {
		if (index>= 0 && index < cameras.size()) {
			cameras[index]->setBounds(bounds);
		}
	}
	const Camera2D& getCamera() const; 
	GameCameraSystem& operator=(const GameCameraSystem&) = delete; 
}; 
