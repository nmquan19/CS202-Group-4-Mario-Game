#include "../../include/Game/GameContext.h"
#include <raylib.h>
#include <memory>
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/System/Interface.h"
#include "../../include/System/PhysicsManager.h"
#include "../../include/System/TextureManager.h"
#include "../../include/System/LevelEditor.h"
#include "../../include/Game/GameStates.h"
#include "../../include/System/Constant.h"
#include "../../include/Enemy/TriggerZone.h"
#include <type_traits>
#include <variant>
#include <algorithm>
#include "../../include/System/Grid.h"
#include <functional>
#include <cmath>
GameContext::GameContext() {
    TextureManager::getInstance().loadTextures();
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
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
        if ((currentState == gamePlayState || currentState == editorState) && (newState != gamePlayState || currentState != editorState)) {
            LevelEditor::getInstance().cleanup();
            PhysicsManager::getInstance().cleanup();
            clearGame(); // Delete remaining objects in GameContext
            character.reset();
        }

        previousState = currentState;
        currentState = newState;

        if (newState == gamePlayState) {
            PhysicsManager::getInstance().setWorldBounds({ 0, 0, 5000, (float)GetScreenWidth()});
            LevelEditor::getInstance().setEditMode(false);
            LevelEditor::getInstance().loadLevel("testlevel.json");
            character = ObjectFactory::createCharacter(CharacterType::MARIO, Vector2{ 500, 500 });
            PhysicsManager::getInstance().addObject(character);
            // addObject(InteractiveType::SPRING, { 500, 950 }, { 1, 1 });
            // addObject(EnemyType::DRY_BOWSER, {300,300}, {1.5, 1.5});
            camera.offset = {(float)GetScreenWidth()/2.0f, (float)GetScreenHeight()/2.0f};
            camera.target = character->getPosition();
        }
    }
}

void GameContext::handleInput() {
    if (currentState) {
        currentState->handleInput(*this);
    }
}

void GameContext::update(float deltaTime) {
    if (currentState == gamePlayState) {
        audioManager.SetSoundEffectVolume(menuManager.slideBarSound.getValue());
        audioManager.SetBackgroundMusicVolume(menuManager.slideBarMusic.getValue());
        if (!audioManager.isPlaying()) {
            audioManager.PlayBackgroundMusic("theme1");
        }
        audioManager.UpdateBackgroundMusic("theme1");
    }
    else {
        if (audioManager.isPlaying()) {
            audioManager.StopBackgroundMusic("theme1");
        }
    }

    if (currentState) {
        currentState->update(*this, deltaTime);
    }
}

void GameContext::draw() {
    if (currentState) {
        currentState->draw(*this);
    }
}

void GameContext::setGameStates(GameState* menu, GameState* game, GameState* editor, GameState* gameOver) {
    menuState = menu;
    gamePlayState = game;
    editorState = editor;
    gameOverState = gameOver;
    currentState = menuState;
}
void GameContext::addObject(ObjectType type, Vector2 worldPos, Vector2 size, std::function<void(std::shared_ptr<Object>)> onSpawn)
{ 
	//pass the middle point of the object x and the end point(feet) of the object y
    Vector2 topLeft = {
        std::floor(worldPos.x - size.x * Constants::TILE_SIZE / 2),
        std::floor(worldPos.y - size.y * Constants::TILE_SIZE)
    };

    ToSpawnObjects.push_back({ type, topLeft, size, onSpawn});
}

void GameContext::spawnObject() {
    for (const auto& request : ToSpawnObjects) {
        
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
            else if constexpr (std::is_same_v<T, InteractiveType>) {
                object = ObjectFactory::createSpring(GridSystem::getWorldPosition(GridSystem::getGridCoord(request.worldpos)), request.size);
            }
            }, request.type);

        if (object) {
            Objects.push_back(object);
            if (request.onSpawn) {
                request.onSpawn(object);
            }
            PhysicsManager::getInstance().addObject(object);
        }
    }
    ToSpawnObjects.clear();
}


void GameContext::mark_for_deletion_Object(std::shared_ptr<Object> object) {
    if (object) {
        PhysicsManager::getInstance().markForDeletion(object);
        ToDeleteObjects.push_back(object);
    }
}
void GameContext::deleteObjects() {
    for (const auto& obj : ToDeleteObjects)
    {
        if (std::find(Objects.begin(), Objects.end(), obj) != Objects.end()) {
            obj->setActive(false);
            Objects.erase(std::remove_if(Objects.begin(), Objects.end(), [&](const std::shared_ptr<Object>& o) {
                return o == obj;
            }), Objects.end());
        }
        ToDeleteObjects.clear();
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

void GameContext::clearGame() {
    for (auto obj : Objects) {
        mark_for_deletion_Object(obj);
    }
    Objects.clear();
}