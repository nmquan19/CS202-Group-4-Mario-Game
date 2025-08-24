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
#include <fstream>
#include "../System/json.hpp"
#include "../../include/Objects/InteractiveObjects/CameraTriggerZone.h"
using json = nlohmann::json;

class GameState;

struct ObjectInfo
{
    ObjectType type;
    Vector2 worldpos;
	Vector2 size;
    std::function<void(std::shared_ptr<Object>)> onSpawn = nullptr;

};
struct LeveLInfo
{
	Color ambientColor; 
	Rectangle initialWorldBounds;
	std::vector<SwitchCameraTriggerZoneData> cameraTriggersData; 
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
	std::vector<LeveLInfo> levelInfo;
    GameState* currentState = nullptr;
    GameState* previousState = nullptr;
    GameState* menuState = nullptr;
    GameState* redirectState = nullptr;
    GameState* playerSelectingState = nullptr;
    GameState* characterSelectingState = nullptr;
    GameState* levelSelectingState = nullptr;
    GameState* informationState = nullptr;
    GameState* gamePlayState = nullptr;
    GameState* editorState = nullptr;
    GameState* editorSelectingState = nullptr;
    GameState* gameOverState = nullptr;
    int playerCallsRequest =1 ;
    int level = 1;
    GameContext();
    ~GameContext();
	static GameContext& getInstance();
    void setState(GameState* newState);
    void handleInput();
    void update(float deltaTime);
    void draw();
	void addObject(ObjectType type, Vector2 worldpos, Vector2 size, std::function<void(std::shared_ptr<Object>)> onSpawn = nullptr);
    void setGameStates(GameState* menu, GameState* redirect, GameState* player, GameState* character, GameState* level, GameState* information, GameState* game, GameState* editor, GameState* editorSelecting, GameState* gameOver);
    void mark_for_deletion_Object(std::shared_ptr<Object> object);
    void spawnObject();
	void deleteObjects();
    std::shared_ptr<Object> getSharedPtrFromRaw(Object* rawPtr);
    void clearGame();

    void setPlayerCallsRequest(int id) { playerCallsRequest = id; }
    std::shared_ptr<Object> getCharacter() { return character01; }
    
    void saveGameState(const std::string& filename);
    void loadGameState(const std::string& filename);
    
    Camera2D camera;
};
