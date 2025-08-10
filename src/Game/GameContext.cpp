#include <raylib.h>
#include <memory>
#include <type_traits>
#include <variant>
#include <algorithm>
#include <functional>
#include <cmath>
#include "../../include/Game/GameContext.h"
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/System/Interface.h"
#include "../../include/System/Box2DWorldManager.h"
#include "../../include/System/TextureManager.h"
#include "../../include/System/LevelEditor.h"
#include "../../include/Game/GameStates.h"
#include "../../include/System/Constant.h"
#include "../../include/Enemy/TriggerZone.h"
#include "../../include/System/Grid.h"

GameContext::GameContext() {
    TextureManager::getInstance().loadTextures();
    camera.rotation = 0.0f;
	camera.offset = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f }; 
    camera.zoom = 1.0f;
    testParticle = new ParticleSystem({ 500, 500 }, { 100, 100 }, { 0, 100 }, 5.0f, 0.1f, 0.0f, "assets/item_coin.png");
}

GameContext::~GameContext() {
    if (previousState) {
        LevelEditor::getInstance().cleanup();
        Box2DWorldManager::getInstance().cleanup();
    }
    TextureManager::getInstance().unloadTextures();
    delete testParticle;
}

GameContext& GameContext::getInstance() {
    static GameContext instance;
    return instance;
}

void GameContext::setState(GameState* newState) {
    if (currentState != newState) {
        if ((currentState == gamePlayState || currentState == editorState) && (newState != gamePlayState || currentState != editorState)) {
            LevelEditor::getInstance().cleanup();
            clearGame(); // Delete remaining objects in GameContext
            Box2DWorldManager::getInstance().cleanup();
            character01.reset();
            character02.reset();
        }

        previousState = currentState;
        currentState = newState;

        if (newState == gamePlayState) {
            Box2DWorldManager::getInstance().initialize(Vector2{0, Constants::GRAVITY});
            LevelEditor::getInstance().setEditMode(false);
            
            // Create test blocks for Box2D physics testing
            createTestBlocks();
            
            ////LevelEditor::getInstance().loadLevel("testlevel.json");
            character01 = ObjectFactory::createCharacter(CharacterType::MARIO, PlayerID::PLAYER_01, Vector2{ 500, 400 });
            character02 = ObjectFactory::createCharacter(CharacterType::MARIO, PlayerID::PLAYER_02, Vector2{ 400, 400 });
            // CAMERA NEEDS CHANGING
            if (character01) {
                camera.offset = {(float)GetScreenWidth()/2.0f, (float)GetScreenHeight()/2.0f};
                camera.target = character01->getPosition();
            }
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
        AudioManager::getInstance().SetSoundEffectVolume(menuManager.slideBarSound.getValue());
        AudioManager::getInstance().SetBackgroundMusicVolume(menuManager.slideBarMusic.getValue());
        if (!AudioManager::getInstance().isPlaying()) {
            AudioManager::getInstance().PlayBackgroundMusic("theme1");
        }
        AudioManager::getInstance().UpdateBackgroundMusic("theme1");
        testParticle->update(deltaTime);

    }
    else {
        if (AudioManager::getInstance().isPlaying()) {
            AudioManager::getInstance().StopBackgroundMusic("theme1");
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

void GameContext::setGameStates(GameState* menu, GameState* redirect, GameState* character, GameState* information, GameState* game, GameState* editor, GameState* gameOver) {
    menuState = menu;
    redirectState = redirect;
    characterSelectingState = character;
    informationState = information;
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
                if (actualType == EnemyType::DRY_BOWSER) object = ObjectFactory::createEnemy(actualType, request.worldpos, {2, 2});
                else object = ObjectFactory::createEnemy(actualType, request.worldpos, request.size);
            }
            else if constexpr (std::is_same_v<T, KoopaShellType>) {
                object = ObjectFactory::createKoopaShell(actualType, request.worldpos, request.size);
            }
            else if constexpr (std::is_same_v<T, InteractiveType>) {
                object = ObjectFactory::createSpring(GridSystem::getWorldPosition(GridSystem::getGridCoord(request.worldpos)), request.size);
            }
            else if constexpr (std::is_same_v<T, ItemType>) {
				object = ObjectFactory::createItem(actualType, request.worldpos, request.size);
            }
            }, request.type);

        if (object) {
            Objects.push_back(object);
            if (request.onSpawn) {
                request.onSpawn(object);
            }
        }
    }
    ToSpawnObjects.clear();
}

void GameContext::mark_for_deletion_Object(std::shared_ptr<Object> object) {
    if (object) {
        ToDeleteObjects.push_back(object);
    }
}

void GameContext::deleteObjects() {
    for (const auto& obj : ToDeleteObjects)
    {
        if (std::find(Objects.begin(), Objects.end(), obj) != Objects.end()) {
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
    deleteObjects();
    Objects.clear();
}

void GameContext::createTestBlocks() {
    // Create a ground platform for testing using world coordinates
    // Bottom ground blocks (y = 15 grid units = 15 * TILE_SIZE pixels, spanning from x = 5 to x = 25)
    for (int x = 5; x <= 25; x++) {
        Vector2 worldPos = GridSystem::getWorldPosition({static_cast<float>(x), 15.0f});
        addObject(BlockType::GROUND, worldPos, {1, 1});
    }
    
    // Create some brick blocks for variety
    for (int x = 8; x <= 12; x++) {
        Vector2 worldPos = GridSystem::getWorldPosition({static_cast<float>(x), 14.0f});
        addObject(BlockType::BRICK, worldPos, {1, 1});
    }
    
    // Add some single blocks for platforming
    addObject(BlockType::BRICK, GridSystem::getWorldPosition({10.0f, 11.0f}), {1, 1});
    addObject(BlockType::BRICK, GridSystem::getWorldPosition({25.0f, 13.0f}), {1, 1});
    addObject(BlockType::BRICK, GridSystem::getWorldPosition({25.0f, 14.0f}), {1, 1});
	//addObject(ItemType::COIN, GridSystem::getWorldPosition({ 19.0f, 9.0f }), { 1, 1 });
	addObject(ItemType::MUSHROOM, GridSystem::getWorldPosition({ 20.0f, 9.0f }), { 1, 1 });
	//addObject(ItemType::FIRE_FLOWER, GridSystem::getWorldPosition({ 21.0f, 9.0f }), { 1, 1 });
	//addObject(ItemType::STAR, GridSystem::getWorldPosition({ 22.0f, 9.0f }), { 1, 1 });
	//addObject(ItemType::ONE_UP, GridSystem::getWorldPosition({ 23.0f, 9.0f }), { 1, 1 });
    //addObject(EnemyType::GOOMBA, GridSystem::getWorldPosition({ 20.0f, 10.0f }), { 0.75f, 0.75f });
    //addObject(InteractiveType::SPRING, GridSystem::getWorldPosition({ 13.0f, 14.0f }), {1, 1});
    //addObject(EnemyType::RED_KOOPA, GridSystem::getWorldPosition({15.0f, 10.0f}), {0.75f, 0.75f});
    //addObject(EnemyType::DRY_BOWSER, GridSystem::getWorldPosition({20.0f, 10.0f}), {1,1});
    
    spawnObject();
}