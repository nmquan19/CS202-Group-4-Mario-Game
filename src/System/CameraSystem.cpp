#include "../../include/System/CameraSystem.h" 
#include  "../../include/Game/GameContext.h"
#include "../../include/System/InterpolationController.h"
#include <raymath.h> 
float EaseApproach(float current, float target, float speed, float dt) {
    float diff = target - current;
    float t = Clamp(dt * speed, 0.0f, 1.0f);
    float easedT = Easing::EaseInOutSine(t);
    return current + diff * easedT;
}
PlayerCenteredCamera::PlayerCenteredCamera()
{
	cam.offset = { (float)GetScreenWidth() / 2.0f,(float) GetScreenHeight() / 2.0f };  
	cam.zoom = 1.0f; 
	cam.rotation = 0.0f; 

    groundBounds.push_back(GetScreenHeight());
    groundBounds.push_back(2*GetScreenHeight());

}
void GameCamera::shake(float strength, float decay) {
    shakeIntensity = strength;
    shakeDecay = decay;
    shakeOffset = { 0, 0 };
}
void PlayerCenteredCamera::update(float dt) { 
    
	std::shared_ptr<Character> character =std::dynamic_pointer_cast<Character>(GameContext::getInstance().getCharacter()); 
    Vector2 desired = GameContext::getInstance().getCharacter()->getPosition();
    desired.x += character->isFacingRight() ? horizontalBias : -horizontalBias;
	Vector2 velocity = character->getVelocity(); 
    // Interpolated Y
    float yLerpSpeed = verticalLerpSpeed * Clamp(fabsf(velocity.y) / 10.0f, 0.5f, 2.0f);
    cam.target.x = EaseApproach(cam.target.x, desired.x, 10.0f, dt);
    if (velocity.y < 0)
    {
          cam.target.y = Lerp(cam.target.y, desired.y, dt * yLerpSpeed);

    }
    else if(cam.target.y < desired.y)
    {
        cam.target.y = desired.y; 
    }
    curGroundIndex = -1; 
    for (int i = 0; i < groundBounds.size(); i++)
    {
        if (groundBounds[i] >= character->getPosition().y)
        {
            curGroundIndex = i; 
            break; 
        }
    }
    // Optional: ledge bias
    bool wantLedgeBias = IsKeyDown(KEY_DOWN);
    float targetLedgeBias = wantLedgeBias ? 250.0f : 0.0f;
    currentLedgeBias = Lerp(currentLedgeBias, targetLedgeBias, dt * 5.0f);
   if(curGroundIndex!=-1) std::cout<<groundBounds[curGroundIndex]<<" "<< character->getPosition().y << "\n";

    // Screen shake
    if (shakeIntensity > 0.01f) {
        shakeOffset = {
            GetRandomValue(-100, 100) / 100.0f * shakeIntensity,
			GetRandomValue(-100, 100) / 100.0f * shakeIntensity
        };
        shakeIntensity *= shakeDecay;
    }
    else {
        shakeOffset = { 0 };
    }

    Vector2 shaken = Vector2Add(cam.target, shakeOffset);

    // === CLAMP CAMERA INSIDE WORLD BOUNDS BASED ON SCREEN SIZE ===
    Vector2 screenTopLeft = GetWorldToScreen2D({ 0, 0 }, cam);
    Vector2 screenBottomRight = GetWorldToScreen2D({ (float)GetScreenWidth(), (float)GetScreenHeight() }, cam);

    float cameraWidth = screenBottomRight.x - screenTopLeft.x;
    float cameraHeight = screenBottomRight.y - screenTopLeft.y;

    float minX = bounds.x + cameraWidth * 0.5f;
    float maxX = bounds.x + bounds.width - cameraWidth * 0.5f;
    float minY = bounds.y + cameraHeight * 0.5f;
    float maxY =((curGroundIndex!=-1)? groundBounds[curGroundIndex]:(bounds.y +bounds.height)) +currentLedgeBias - cameraHeight * 0.5f;
    Vector2 topLeft = GetWorldToScreen2D({ 0, maxY }, cam);
    Vector2 bottomRight = GetWorldToScreen2D({ (float)GetScreenWidth(), maxY }, cam);

    DrawLine((int)topLeft.x, (int)topLeft.y, (int)bottomRight.x, (int)bottomRight.y, RED);

    shaken.x = Clamp(shaken.x, minX, maxX);
    shaken.y = Clamp(shaken.y, minY, maxY);

    // Apply to raylib camera
    cam.target = shaken;
} 

void GameCameraSystem::init() {
    cameras.emplace_back(std::make_unique<PlayerCenteredCamera>());
    curIndex = 0;
}
void GameCameraSystem::update(float dt) {
    if (curIndex < 0 || curIndex >= cameras.size()) return;

    if (transition.active) {
        transition.timer += dt;
        float t = transition.timer / transition.duration;
        if (t >= 1.0f) {
            t = 1.0f;
            transition.active = false;
        }
        float easedT = t * t * (3 - 2 * t); 
        Camera2D cam;
        cam.target = Vector2Lerp(transition.from.target, transition.to.target, easedT);
        cam.offset = Vector2Lerp(transition.from.offset, transition.to.offset, easedT);
        cam.zoom = Lerp(transition.from.zoom, transition.to.zoom, easedT);
        cam.rotation = Lerp(transition.from.rotation, transition.to.rotation, easedT);

       cameras[curIndex]->setCamera(cam);
    }
    else
    {
        cameras[curIndex]->update(dt);
    }
}
void GameCameraSystem::addCamera(std::unique_ptr<GameCamera> camera) {
    cameras.emplace_back(std::move(camera));
} 
void GameCameraSystem::switchCamera(int toIndex) {
    if (toIndex < 0 || toIndex >= cameras.size()) return;
    transition.active = true;
    transition.timer = 0.0f;
    transition.duration = 0.5f; 

    transition.from = cameras[curIndex]->GetCamera();
    transition.to = cameras[toIndex]->GetCamera();
    curIndex = toIndex; 
} 
void GameCameraSystem::shakeCurrentCamera(float strength, float decay) {
    if (curIndex < 0 || curIndex >= cameras.size()) return;
    cameras[curIndex]->shake(strength, decay);
}
Camera2D const& GameCameraSystem::getCamera() const {
    if (curIndex < 0 || curIndex >= cameras.size()) return {};
    return cameras[curIndex]->GetCamera();
}

StaticGameCamera::StaticGameCamera():GameCamera() {
    cam.offset = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };
    cam.zoom = 1.0f;
    cam.rotation = 0.0f;
	cam.target = { 0, 0 };   
} 