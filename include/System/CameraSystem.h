#pragma once
#include <raylib.h>
#include <vector> 
#include <memory>
class GameCamera
{
protected: 
	Camera2D cam;  
	Vector2 shakeOffset; 
	float shakeIntensity =0.0f; 
	float shakeDecay = 0.9f;	  
public: 
	virtual void update(float deltaTime) = 0; 
	void shake(float strenght, float decay); 
	const Camera2D& GetCamera() const { return cam; }
	void setCamera(const Camera2D& camera) { cam = camera; } 
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
	Rectangle bounds = { 0, 0, 10000, 10000 };
	float verticalLerpSpeed = 5.0f;
	float fallEaseTimer = 0.0f;
	const float fallEaseDuration = 0.15f;
	std::vector<float> groundBounds;
	int curGroundIndex = 0;
	float currentLedgeBias = 0;
public: 
	PlayerCenteredCamera(); 
	void update(float deltaTime) override;  
	void setBounds(const Rectangle& newBounds) { bounds = newBounds; }    
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
	int curIndex;  
	CameraTransition transition;
public: 
	static GameCameraSystem& getInstance() {
		static GameCameraSystem instance;
		return instance;
	}
	void init(); 
	void addCamera(std::unique_ptr<GameCamera> camera);
	void switchCamera(int toIndex); 
	void update(float deltaTime);
	void shakeCurrentCamera(float strength, float decay);  
	const Camera2D& getCamera() const; 
	GameCameraSystem& operator=(const GameCameraSystem&) = delete; 
}; 


class CameraChangeBoundTrigger
{
private: 
	Vector2  hitbox;  
	Vector2 position;  
public: 
	
};