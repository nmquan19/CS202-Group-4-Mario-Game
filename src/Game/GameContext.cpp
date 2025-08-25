#include <raylib.h>
#include <memory>
#include <type_traits>
#include <variant>
#include <algorithm>
#include <functional>
#include <cmath>
#include "../../include/Game/GameContext.h"
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/Objects/Block.h"
#include "../../include/Objects/InteractiveObjects/InteractiveObject.h"
#include "../../include/Objects/InteractiveObjects/Spring.h"
#include "../../include/Objects/InteractiveObjects/FireBar.h"
#include "../../include/Objects/InteractiveObjects/FireBarBase.h"
#include "../../include/Objects/InteractiveObjects/MovingPlatform.h"
#include "../../include/Enemy/Enemy.h"
#include "../../include/Item/Item.h"
#include "../../include/Enemy/Koopa/KoopaShell.h"
#include "../../include/System/Interface.h"
#include "../../include/System/Box2DWorldManager.h"
#include "../../include/System/TextureManager.h"
#include "../../include/System/LevelEditor.h"
#include "../../include/Game/GameStates.h"
#include "../../include/System/Constant.h"
#include "../../include/Enemy/TriggerZone.h"
#include "../../include/System/Grid.h"
#include "../../include/System/CameraSystem.h"
#include "../../include/System/LightingSystem.h"
#include "../../include/Objects/InteractiveObjects/Endpoint.h"
GameContext::GameContext() {
    TextureManager::getInstance().loadTextures();
    camera.rotation = 0.0f;
	camera.offset = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f }; 
    camera.zoom = 1.0f;
}

GameContext::~GameContext() {
    if (previousState) {
        LevelEditor::getInstance().cleanup();
        Box2DWorldManager::getInstance().cleanup();
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
            clearGame(); // Delete remaining objects in GameContext
            Box2DWorldManager::getInstance().cleanup();
            ParticleSystem::getInstance().cleanup();
            character01.reset();
            character02.reset();
        }

        previousState = currentState;
        currentState = newState;

        if (newState == gamePlayState) {
            Box2DWorldManager::getInstance().initialize(Vector2{ 0, Constants::GRAVITY });
//LevelEditor::getInstance().setEditMode(false);
            UIManager::getInstance().resetCoin();
            UIManager::getInstance().resetTimer();
            UIManager::getInstance().resetScore();

            LevelEditor::getInstance().setEditMode(false);
            if (level == 1) LevelEditor::getInstance().loadLevel("Level3.json");
            if (level == 2) LevelEditor::getInstance().loadLevel("snowmap.json");
            if (level == 3) LevelEditor::getInstance().loadLevel("level1_new.json");
            if (level == 4) LevelEditor::getInstance().loadLevel("testlevel.json");

            // Calculate ambient color
            Color base = { 85, 57, 204, 255 };
            float brightness = 2.5f;
            Color brighter = {
                (unsigned char)std::min(255.0f, base.r * brightness),
                (unsigned char)std::min(255.0f, base.g * brightness),
                (unsigned char)std::min(255.0f, base.b * brightness),
                base.a
            };
            switch (menuManager.character01Select) {
            case 0:
                character01 = ObjectFactory::createCharacter(CharacterType::MARIO, PlayerID::PLAYER_01, Vector2{400, 400 });
                break;
            case 1:
                character01 = ObjectFactory::createCharacter(CharacterType::MARIO, PlayerID::PLAYER_01, Vector2{ 300, 400 });
                break;
            case 2:
                character01 = ObjectFactory::createCharacter(CharacterType::TOAD, PlayerID::PLAYER_01, Vector2{ 400, 400 });
                break;
            case 3:
                character01 = ObjectFactory::createCharacter(CharacterType::TOADETTE, PlayerID::PLAYER_01, Vector2{ 400, 400 });
                break;
            default:
                character01 = ObjectFactory::createCharacter(CharacterType::MARIO, PlayerID::PLAYER_01, Vector2{ 400, 400 });
            }

            switch (menuManager.character02Select) {
            case 0:
                character02 = ObjectFactory::createCharacter(CharacterType::MARIO, PlayerID::PLAYER_02, Vector2{ 500, 400 });
                break;
            case 1:
                character02 = ObjectFactory::createCharacter(CharacterType::LUIGI, PlayerID::PLAYER_02, Vector2{ 500, 400 });
                break;
            case 2:
                character02 = ObjectFactory::createCharacter(CharacterType::TOAD, PlayerID::PLAYER_02, Vector2{ 500, 400 });
                break;
            case 3:
                character02 = ObjectFactory::createCharacter(CharacterType::TOADETTE, PlayerID::PLAYER_02, Vector2{ 500, 400 });
                break;
            }
            EndpointData data1;
            data1.position = { 500, 3000 };
            data1.size = { 2, 3 };
            data1.targetLevel = 2; // Goes to level selector


            EndpointData data2;
            data1.position = { 500, 1800 };
            data1.size = { 2, 3 };
            data1.targetLevel = 3; // Goes to level selector

            GameCameraSystem::getInstance().init();
            Camera2D initialCam = {
                .offset = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f },
                .target = character01->getPosition(),
                .rotation = 0.0f,
                .zoom = 1.0f
            };
            GameCameraSystem::getInstance().setCamera(initialCam);
            levelInfo.clear();
            Camera2D ncam;
            ncam.target = { 11850.0f, 1450.0f };
            ncam.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
            ncam.zoom = 1.1f;
            ncam.rotation = 0.0f;
            LeveLInfo info3 = {
                .ambientColor = WHITE,
                .initialWorldBounds = { 0, 0, Constants::WORLDBOUNDS_WIDTH, 1500 },
                .cameraTriggersData = {
                    {
                        Vector2{4160,1300},
                        Vector2{2.5,1.5},
                        0,
                        0,
                        Rectangle{ 0, 0, 10000, 2750 },   // inWorldBounds
                        Rectangle{ 0, 0, 10000, 2750},   // outWorldBounds
                        GameCameraSystem::getInstance().getCamera(),
                        GameCameraSystem::getInstance().getCamera()
                    },
                    {
                        Vector2{4160,700},
                        Vector2{2.5,1.5},
                        0,
                        0,
                        Rectangle{ 0, 0, Constants::WORLDBOUNDS_WIDTH, 2500 },   // inWorldBounds
                        Rectangle{ 0,0, Constants::WORLDBOUNDS_WIDTH, 2500},   // outWorldBounds
                        GameCameraSystem::getInstance().getCamera(),
                        GameCameraSystem::getInstance().getCamera()
                    },
                    {
                        Vector2{9816.5,700},
                        Vector2{2.5,1.5},
                        0,
                        0,
                        Rectangle{ 0, 0, Constants::WORLDBOUNDS_WIDTH, 2500 },   // inWorldBounds
                        Rectangle{ 0,0, Constants::WORLDBOUNDS_WIDTH, 1500},   // outWorldBounds
                        GameCameraSystem::getInstance().getCamera(),
                        GameCameraSystem::getInstance().getCamera(),
                        false
                    },
                  {
                        Vector2{13500,700},
                        Vector2{2.5,1.5},
                        0,
                        0,
                        Rectangle{ 0, 0,  14000, 2500},   // inWorldBounds
                        Rectangle{ 0,0,  14000, 2500},   // outWorldBounds
                        GameCameraSystem::getInstance().getCamera(),
                        GameCameraSystem::getInstance().getCamera(),
                        false
                    },
                    {
                        Vector2{13500,1200},
                        Vector2{2.5,1.5},
                        0,
                        0,
                        Rectangle{ 12500, 0, 14500, 3000 },   // inWorldBounds
                        Rectangle{ 12500, 0, 14500, 3000 },   // outWorldBounds
                        GameCameraSystem::getInstance().getCamera(),
                        GameCameraSystem::getInstance().getCamera()
                    },
                    {
                        Vector2{12620,1800},
                        Vector2{1,2.5},
                        0,
                        0,
                        Rectangle{ 10000, 700, 11000, 2000 },   // outWorldBounds
                        Rectangle{ 10000, 700, 11000, 2000 },   // outWorldBounds
                        GameCameraSystem::getInstance().getCamera(),
                        GameCameraSystem::getInstance().getCamera(),
                        false
                    },
                },
                //.endpointData= data1
            };
            LeveLInfo info2 = {
                .ambientColor = brighter,
                .initialWorldBounds = { 0, 0, Constants::WORLDBOUNDS_WIDTH, Constants::WORLDBOUNDS_HEIGHT},
                .cameraTriggersData = {

                },.endpointData = data2
            };
            LeveLInfo info1 = {
              .ambientColor = brighter,
              .initialWorldBounds = { 0, 0, Constants::WORLDBOUNDS_WIDTH, Constants::WORLDBOUNDS_HEIGHT},
              .cameraTriggersData = {

              },
              .endpointData = data1
            };
            LeveLInfo info4 = {
           .ambientColor = brighter,
           .initialWorldBounds = { 0, 0, Constants::WORLDBOUNDS_WIDTH, Constants::WORLDBOUNDS_HEIGHT},
           .cameraTriggersData = {

           }
            };
            levelInfo.push_back(info1);
            levelInfo.push_back(info2);
            levelInfo.push_back(info3);
            levelInfo.push_back(info4);

            LightingManager::getInstance().setAmbientColor(levelInfo[level-1].ambientColor);
            if (GameContext::getInstance().character02 == nullptr) {
                for (const auto& triggerData : levelInfo[level - 1].cameraTriggersData) {
                    std::shared_ptr<SwitchCameraTriggerZone> cameraTrigger = std::make_shared<SwitchCameraTriggerZone>(triggerData.position, triggerData.size, triggerData);

                    Objects.push_back(cameraTrigger);
                }

            }
            auto endpoint = std::make_shared<Endpoint>(levelInfo[level - 1].endpointData.position, levelInfo[level - 1].endpointData.size, levelInfo[level - 1].endpointData);
			Objects.push_back(endpoint);
            if (character01) {
                camera.offset = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };
                camera.target = character01->getPosition();
            }
            if (GameContext::getInstance().character02 == nullptr)
            {
                GameCameraSystem::getInstance().setCameraBounds(levelInfo[level - 1].initialWorldBounds, 0);
            }
            else
            {
                GameCameraSystem::getInstance().setCameraBounds({0,0,Constants::WORLDBOUNDS_WIDTH,Constants::WORLDBOUNDS_HEIGHT}, 0);
                GameCameraSystem::getInstance().setCameraBounds({ 0,0,Constants::WORLDBOUNDS_WIDTH,Constants::WORLDBOUNDS_HEIGHT },2);
                GameCameraSystem::getInstance().setCameraBounds({ 0,0,Constants::WORLDBOUNDS_WIDTH,Constants::WORLDBOUNDS_HEIGHT }, 3);
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
    if (currentState == gamePlayState && !menuManager.settingDialog) {
        AudioManager::getInstance().SetSoundEffectVolume(menuManager.slideBarSound.getValue() * menuManager.slideBarMaster.getValue());
        AudioManager::getInstance().SetBackgroundMusicVolume(menuManager.slideBarMusic.getValue() * menuManager.slideBarMaster.getValue());
        if (!AudioManager::getInstance().isPlaying()) {
            AudioManager::getInstance().PlayBackgroundMusic("theme1");
        }
        AudioManager::getInstance().UpdateBackgroundMusic("theme1");
    }
    else {
        if (AudioManager::getInstance().isPlaying()) {
            AudioManager::getInstance().StopBackgroundMusic("theme1");
        }
    }

    if (character01) {
        auto character = std::dynamic_pointer_cast<Character>(character01);
        if (character) {
            std::cout << character->getCenterPos().x << " "
                << character->getCenterPos().y << "\n";
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

void GameContext::setGameStates(GameState* menu, GameState* redirect, GameState* player, GameState* character, GameState* levelRedirect, GameState* level, GameState* information, GameState* game, GameState* editor, GameState* editorSelecting, GameState* gameOver) {
    menuState = menu;
    redirectState = redirect;
    playerSelectingState = player;
    characterSelectingState = character;
    levelRedirectState = levelRedirect;
    levelSelectingState = level;
    informationState = information;
    gamePlayState = game;
    editorState = editor;
    editorSelectingState = editorSelecting;
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
                object = ObjectFactory::createInteractiveObject(actualType, GridSystem::getWorldPosition(GridSystem::getGridCoord(request.worldpos)));
            }
            else if constexpr (std::is_same_v<T, ProjectileType>) {
                object = ObjectFactory::createProjectile(actualType, request.worldpos, std::dynamic_pointer_cast<Character>((playerCallsRequest == 1 ? character01 : character02))->isFacingRight() ? 1 : -1,  request.size);
            }
            else if constexpr (std::is_same_v<T, ItemType>) {
				object = ObjectFactory::createItem(actualType, request.worldpos, request.size);
            }
            else if constexpr (std::is_same_v<T, BackGroundObjectType>) {
                object = ObjectFactory::createTorch(GridSystem::getWorldPosition(GridSystem::getGridCoord(request.worldpos)), request.size);
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
        LevelEditor::getInstance().removeObject(object->getGridPos(), object);
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

void GameContext::saveGameState(const std::string& filename) {
    json gameData;
    json objectsArray = json::array();
    
    // Save character01
    if (character01) {
        auto savableChar = std::dynamic_pointer_cast<ISavable>(character01);
        if (savableChar) {
            objectsArray.push_back(savableChar->toJson());
        }
    }
    
    // Save character02
    if (character02) {
        auto savableChar = std::dynamic_pointer_cast<ISavable>(character02);
        if (savableChar) {
            objectsArray.push_back(savableChar->toJson());
        }
    }
    
    // Save all other objects that implement ISavable
    for (const auto& obj : Objects) {
        auto savableObj = std::dynamic_pointer_cast<ISavable>(obj);
        if (savableObj) {
            objectsArray.push_back(savableObj->toJson());
        }
    }
    
    gameData["objects"] = objectsArray;
    gameData["playerCallsRequest"] = playerCallsRequest;
    
    // Save camera state
    gameData["camera"] = {
        {"target", {camera.target.x, camera.target.y}},
        {"offset", {camera.offset.x, camera.offset.y}},
        {"rotation", camera.rotation},
        {"zoom", camera.zoom}
    };
    
    // Write to file
    std::ofstream file(filename);
    if (file.is_open()) {
        file << gameData.dump(4);
        file.close();
        std::cout << "Game state saved to " << filename << std::endl;
    } else {
        std::cerr << "Failed to save game state to " << filename << std::endl;
    }
}

void GameContext::loadGameState(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to load game state from " << filename << std::endl;
        return;
    }
    
    json gameData;
    file >> gameData;
    file.close();
    
    // Clear existing objects
    clearGame();
    character01.reset();
    character02.reset();
    
    // Load player calls request
    if (gameData.contains("playerCallsRequest")) {
        playerCallsRequest = gameData["playerCallsRequest"];
    }
    
    // Load camera state
    if (gameData.contains("camera")) {
        const auto& cameraData = gameData["camera"];
        camera.target = {cameraData["target"][0], cameraData["target"][1]};
        camera.offset = {cameraData["offset"][0], cameraData["offset"][1]};
        camera.rotation = cameraData["rotation"];
        camera.zoom = cameraData["zoom"];
    }
    
    // Load objects
    if (gameData.contains("objects")) {
        for (const auto& objData : gameData["objects"]) {
            if (!objData.contains("saveType")) continue;
            
            std::string saveType = objData["saveType"];
            
            if (saveType == "Character") {
                CharacterType charType = static_cast<CharacterType>(objData["characterType"]);
                PlayerID playerID = static_cast<PlayerID>(objData["playerID"]);
                Vector2 startPos = GridSystem::getWorldPosition({objData["gridPosition"][0], objData["gridPosition"][1]});
                
                auto characterUniquePtr = ObjectFactory::createCharacter(charType, playerID, startPos);
                auto character = std::shared_ptr<Character>(
                    static_cast<Character*>(characterUniquePtr.release())
                );
                
                if (character) {
                    character->fromJson(objData);
                
                    if (playerID == PlayerID::PLAYER_01) {
                        character01 = character;
                    } else if (playerID == PlayerID::PLAYER_02) {
                        character02 = character;
                    }
                }
            } else if (saveType == "Block") {
                Vector2 gridPos = {objData["gridPosition"][0], objData["gridPosition"][1]};
                BlockType blockType = static_cast<BlockType>(objData["blockType"]);
                
                LevelEditor::getInstance().placeObject(blockType, gridPos);
                
                if (!Objects.empty()) {
                    auto lastObject = Objects.back();
                    auto block = std::dynamic_pointer_cast<Block>(lastObject);
                    if (block && block->getSaveType() == "Block") {
                        block->fromJson(objData);
                    }
                }   
            } else if (saveType == "Interactive") {
                Vector2 gridPos = {objData["gridPosition"][0], objData["gridPosition"][1]};
                InteractiveType interactiveType = static_cast<InteractiveType>(objData["interactiveType"]);
                LevelEditor::getInstance().placeObject(interactiveType, gridPos);
                if (!Objects.empty()) {
                    auto lastObject = Objects.back();
                    auto interactive = std::dynamic_pointer_cast<InteractiveObject>(lastObject);
                    if (interactive) {
                        interactive->fromJson(objData);
                    }
                }
            } else if (saveType == "Enemy") {
                b2Vec2 pos = {objData["position"][0], objData["position"][1]};
                EnemyType enemyType = static_cast<EnemyType>(objData["enemyType"]);

                LevelEditor::getInstance().placeObject(enemyType, GridSystem::getGridCoord(Box2DWorldManager::b2ToRaylib(pos)));
                
                if (!Objects.empty()) {
                    auto lastObject = Objects.back();
                    auto enemy = std::dynamic_pointer_cast<Enemy>(lastObject);
                    if (enemy) {
                        enemy->fromJson(objData);
                    }
                }
            } else if (saveType == "Item") {
                b2Vec2 pos = {objData["position"][0], objData["position"][1]};
                ItemType itemType = static_cast<ItemType>(objData["itemType"]);

                LevelEditor::getInstance().placeObject(itemType, GridSystem::getGridCoord(Box2DWorldManager::b2ToRaylib(pos)));
                
                if (!Objects.empty()) {
                    auto lastObject = Objects.back();
                    auto item = std::dynamic_pointer_cast<Item>(lastObject);
                    if (item) {
                        item->fromJson(objData);
                    }
                }
            } else if (saveType == "KoopaShell") {
                b2Vec2 pos = {objData["position"][0], objData["position"][1]};
                KoopaShellType shellType = static_cast<KoopaShellType>(objData["shellType"]);

                LevelEditor::getInstance().placeObject(shellType, GridSystem::getGridCoord(Box2DWorldManager::b2ToRaylib(pos)));
                
                if (!Objects.empty()) {
                    auto lastObject = Objects.back();
                    auto koopaShell = std::dynamic_pointer_cast<KoopaShell>(lastObject);
                    if (koopaShell) {
                        koopaShell->fromJson(objData);
                    }
                }
            }
        }
    }
    
    std::cout << "Game state loaded from " << filename << std::endl;
}