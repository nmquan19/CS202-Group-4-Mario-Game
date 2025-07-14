#include "../../include/Game/GameContext.h"
#include <raylib.h>
#include <memory>
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/System/Interface.h"
#include "../../include/System/PhysicsManager.h"
#include "../../include/System/TextureManager.h"
#include "../../include/System/LevelEditor.h"
#include "../../include/Game/GameStates.h"
#include "../../include/System/Grid.h"
#include <type_traits>
#include <variant>
#include <algorithm>
#include <iostream>
GameContext::GameContext() {
    TextureManager::getInstance().loadTextures();
}

GameContext::~GameContext() {
    if (previousState) {
        LevelEditor::getInstance().cleanup();
        PhysicsManager::getInstance().cleanup();
    }
    TextureManager::getInstance().unloadTextures();
}
GameContext& GameContext::getInstance() {
    static GameContext instance;
    return instance;
}
void GameContext::setState(GameState* newState) {
    if (currentState != newState) {
        if (currentState == gamePlayState && newState != gamePlayState) {
            LevelEditor::getInstance().cleanup();
            PhysicsManager::getInstance().cleanup();
            character.reset();
        }

        if (newState == gamePlayState) {
            PhysicsManager::getInstance().setWorldBounds({ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() });
            character = ObjectFactory::createCharacter(CharacterType::MARIO, Vector2{ 500, 500 });
            PhysicsManager::getInstance().addObject(character);
            addObject(BlockType::GROUND, {300, 1500}, {GridSystem::GRID_SIZE, GridSystem::GRID_SIZE});
            addObject(BlockType::GROUND, {400, 1500}, {GridSystem::GRID_SIZE, GridSystem::GRID_SIZE});
            addObject(BlockType::GROUND, {500, 1500}, {GridSystem::GRID_SIZE, GridSystem::GRID_SIZE});
            addObject(BlockType::GROUND, {600, 1500}, {GridSystem::GRID_SIZE, GridSystem::GRID_SIZE});
            addObject(BlockType::GROUND, {700, 1500}, {GridSystem::GRID_SIZE, GridSystem::GRID_SIZE});
            addObject(BlockType::GROUND, {700, 1400}, {GridSystem::GRID_SIZE, GridSystem::GRID_SIZE});
        }

        previousState = currentState;
        currentState = newState;
    }
}

void GameContext::handleInput() {
    if (currentState) {
        currentState->handleInput(*this);
    }
}

void GameContext::update(float deltaTime) {
    if (currentState) {
        currentState->update(*this, deltaTime);
    }
}

void GameContext::draw() {
    if (currentState) {
        currentState->draw(*this);
    }
}

void GameContext::setGameStates(GameState* menu, GameState* game, GameState* gameOver) {
    menuState = menu;
    gamePlayState = game;
    gameOverState = gameOver;
    currentState = menuState;
}
void GameContext::addObject(ObjectType type, Vector2 worldPos, Vector2 size)
{
    ToSpawnObjects.push_back({ type, worldPos, size });
}

void GameContext::spawnObject() {
    for (const auto& request : ToSpawnObjects) {
        std::cout << "SPAWNING" << "\n"; 
        
        std::shared_ptr<Object> object = nullptr;

        std::visit([&](auto&& actualType) {
            using T = std::decay_t<decltype(actualType)>;

            if constexpr (std::is_same_v<T, BlockType>) {
                object = ObjectFactory::createBlock(actualType, GridSystem::getGridCoord(request.worldpos));
            }
            else if constexpr (std::is_same_v<T, EnemyType>) {
                object = ObjectFactory::createEnemy(actualType, request.worldpos, request.size);
            }
            else if constexpr (std::is_same_v<T, KoopaShellType>) {
                object = ObjectFactory::createKoopaShell(actualType, request.worldpos, request.size);
            }
            }, request.type);

        if (object) {
            Objects.push_back(object);
            PhysicsManager::getInstance().addObject(object);
        }
    }
    ToSpawnObjects.clear();
}


void GameContext::mark_for_deletion_Object(std::shared_ptr<Object> object) {
    if (object) {
        ToDeleteObjects.push_back(object);
    }
}
void GameContext::deleteObjects(){
    for (const auto& obj : ToDeleteObjects)
    {
       if(std::find(Objects.begin(), Objects.end(), obj) != Objects.end()) {
            Objects.erase(std::remove(Objects.begin(), Objects.end(), obj), Objects.end());
	   }
    }
}

std::shared_ptr<Object> GameContext::getSharedPtrFromRaw(Object* rawPtr) {
    for (const auto& obj : Objects) {
        if (obj.get() == rawPtr) {
            return obj;
        }
    }
    return nullptr;
}
