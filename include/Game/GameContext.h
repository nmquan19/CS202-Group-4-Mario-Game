#pragma once
#pragma once
#include <memory>
#include <vector>
#include "../UI/SoundEffect.h"
#include "../UI/Menu.h"
#include "../UI/UI.h"
#include "../Characters/Character.h"
#include "../Objects/ObjectFactory.h"
#include "../System/Interface.h"
#include "../System/ParticleSystem.h"
#include <functional>
#include <raylib.h>
class GameState;
struct ObjectInfo
{
    ObjectType type;
    Vector2 worldpos;
	Vector2 size;
    std::function<void(std::shared_ptr<Object>)> onSpawn = nullptr;

};
class GameContext {
public:
    //AudioManager audioManager;
    //UIManager uiManager;
    MenuManager menuManager;
    std::shared_ptr<Object> character01;
    std::shared_ptr<Object> character02;
    std::vector<std::shared_ptr<Object>> Objects;
    std::vector<std::shared_ptr<Object>> ToDeleteObjects;
    std::vector<ObjectInfo> ToSpawnObjects;

    GameState* currentState = nullptr;
    GameState* previousState = nullptr;
    GameState* menuState = nullptr;
    GameState* redirectState = nullptr;
    GameState* characterSelectingState = nullptr;
    GameState* informationState = nullptr;
    GameState* gamePlayState = nullptr;
    GameState* editorState = nullptr;
    GameState* editorSelectingState = nullptr;
    GameState* gameOverState = nullptr;
    int playerCallsRequest;
    GameContext();
    ~GameContext();
	static GameContext& getInstance();
    void setState(GameState* newState);
    void handleInput();
    void update(float deltaTime);
    void draw();
	void addObject(ObjectType type, Vector2 worldpos, Vector2 size, std::function<void(std::shared_ptr<Object>)> onSpawn = nullptr);
    void setGameStates(GameState* menu, GameState* redirect, GameState* character, GameState* information, GameState* game, GameState* editor, GameState* editorSelecting, GameState* gameOver);
    void mark_for_deletion_Object(std::shared_ptr<Object> object);
    void spawnObject();
	void deleteObjects();
    std::shared_ptr<Object> getSharedPtrFromRaw(Object* rawPtr);
    void clearGame();
    void createTestBlocks(); // Add test blocks for Box2D testing
    void setPlayerCallsRequest(int id) { playerCallsRequest = id; }
    std::shared_ptr<Object> getCharacter() { return character01; }
    Camera2D camera;

    ParticleSystem* testParticle;
};
