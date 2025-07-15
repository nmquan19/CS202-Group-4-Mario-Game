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
    AudioManager audioManager;
    UIManager uiManager;
    MenuManager menuManager;
    std::unique_ptr<Character> character;
    std::vector<std::shared_ptr<Object>> Objects;
    std::vector<std::shared_ptr<Object>> ToDeleteObjects;
    std::vector<ObjectInfo> ToSpawnObjects;

    GameState* currentState = nullptr;
    GameState* previousState = nullptr;
    GameState* menuState = nullptr;
    GameState* gamePlayState = nullptr;
    GameState* gameOverState = nullptr;
    GameContext();
    ~GameContext();
	static GameContext& getInstance();
    void setState(GameState* newState);
    void handleInput();
    void update(float deltaTime);
    void draw();
    void setGameStates(GameState* menu, GameState* game, GameState* gameOver);
	void addObject(ObjectType type, Vector2 worldpos, Vector2 size, std::function<void(std::shared_ptr<Object>)> onSpawn = nullptr);
    void mark_for_deletion_Object(std::shared_ptr<Object> object);
    void spawnObject();
	void deleteObjects();
    std::shared_ptr<Object> getSharedPtrFromRaw(Object* rawPtr);

};
