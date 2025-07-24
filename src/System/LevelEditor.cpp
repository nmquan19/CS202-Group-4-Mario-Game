#include <memory>
#include <utility>
#include <raylib.h>
#include <type_traits>
#include <variant>
#include <iostream>
#include <string>
#include "../../include/System/LevelEditor.h"
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/System/Grid.h"
#include "../../include/System/Interface.h"
#include "../../include/Enemy/Enemy.h"
#include "../../include/Game/GameContext.h"
#include <exception>
#include <fstream>
#include <stack>
#include <vector>
LevelEditor* LevelEditor::instance = nullptr;
LevelEditor& LevelEditor::getInstance() {
    if (!instance) {
        instance = new LevelEditor();
    }
    return *instance;
}

void LevelEditor::cleanup() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

LevelEditor::~LevelEditor() {
    for (auto& it : gridBlocks) {
        while (!it.second.empty()) {
            it.second.pop();
        }
    }
    gridBlocks.clear();
}
void LevelEditor::update() {
    if (editMode) {
        palette.handleSelection();
        handleMouseInput();
    }
}

void LevelEditor::draw() {
    BeginMode2D(GameContext::getInstance().camera);
    if (!clearing) {
        for (auto& it : gridBlocks) {
            Object* obj = it.second.top().get();
            if (!obj) continue;

            Enemy* enemy = dynamic_cast<Enemy*>(obj);
            bool shouldDraw = false;

            if (enemy) {
                shouldDraw = enemy->isAlive();
            }
            else {
                shouldDraw = obj->isActive();
            }

            if (shouldDraw) {
                obj->draw();
            }
        }
    }
    if (editMode) {
        GridSystem::drawGrid(Constants::WORLDBOUNDS_WIDTH, Constants::WORLDBOUNDS_HEIGHT);

        Vector2 sMousePos = GetMousePosition();
        Vector2 wMousePos = GetScreenToWorld2D(sMousePos, GameContext::getInstance().camera);
        Vector2 gridCoord = GridSystem::getGridCoord(wMousePos);
        if (!CheckCollisionPointRec(sMousePos, palette.getPaletteRect())) {
            Rectangle highlightRect = GridSystem::getGridRect(gridCoord);
            DrawRectangleRec(highlightRect, Fade(GREEN, 0.2f));
            DrawRectangleLinesEx(highlightRect, 3, GREEN);
            Vector2 textPos = {highlightRect.x + 5, highlightRect.y + 5};
            std::string coordText = "(" + std::to_string((int)gridCoord.x) + "," + std::to_string((int)gridCoord.y) + ")";
            DrawText(coordText.c_str(), textPos.x, textPos.y, 12, BLACK);
        }
    }
    EndMode2D();
    if (editMode) {
        palette.drawPalette();
        DrawText("F7: Save Level", 20, 100, 16, DARKBLUE);
        DrawText("F8: Load Level", 20, 120, 16, DARKBLUE);
        DrawText("F9: Clear Level", 20, 140, 16, DARKBLUE);
        DrawText(("Number of grids used: " + std::to_string(gridBlocks.size())).c_str(), 20, 160, 16, DARKBLUE);

    }
    const char* modeText = editMode ? "EDIT MODE" : "PLAY MODE";
    DrawText(modeText, 20, 70, 20, editMode ? GREEN : BLUE);
}

void LevelEditor::handleMouseInput() {
    Vector2 screenMousePos = GetMousePosition();
    Vector2 worldMousePos = GetScreenToWorld2D(screenMousePos, GameContext::getInstance().camera);
    Vector2 gridCoord = GridSystem::getGridCoord(worldMousePos);    
    
    // Check screen bounds
    if (screenMousePos.x < 0 || screenMousePos.y < 0 || 
        screenMousePos.x >= GetScreenWidth() || screenMousePos.y >= GetScreenHeight()) {
        return;
    }
    
    // In bounds
    Rectangle gridRect = GridSystem::getGridRect(gridCoord);
    bool mouseInGrid = CheckCollisionPointRec(worldMousePos, gridRect);
    
    // Avoid spam
    static Vector2 lastPlacedGrid = {-1, -1};
    static Vector2 lastRemovedGrid = {-1, -1};
    static bool wasLeftPressed = false;
    static bool wasRightPressed = false;
    
    bool clickingOnPalette = CheckCollisionPointRec(screenMousePos, palette.getPaletteRect());
    bool leftPressed = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    bool rightPressed = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
    
    // Only place/remove when mousePos in grid
    if (leftPressed && !clickingOnPalette && mouseInGrid) {
        // Only place when grid position change or the first time
        if (!wasLeftPressed || 
            (gridCoord.x != lastPlacedGrid.x || gridCoord.y != lastPlacedGrid.y)) {
            placeObject(palette.getSelectedType(), gridCoord);
            lastPlacedGrid = gridCoord;
        }
    }
    
    if (rightPressed && mouseInGrid) {
        if (!wasRightPressed || 
            (gridCoord.x != lastRemovedGrid.x || gridCoord.y != lastRemovedGrid.y)) {
            removeObject(gridCoord);
            lastRemovedGrid = gridCoord;
        }
    }
    
    // Reset when not pressed
    if (!leftPressed) {
        lastPlacedGrid = {-1, -1};
    }
    if (!rightPressed) {
        lastRemovedGrid = {-1, -1};
    }
    
    wasLeftPressed = leftPressed;
    wasRightPressed = rightPressed;
}

void LevelEditor::placeObject(ObjectType type, Vector2 gridCoord) {
    auto key = std::make_pair((int)gridCoord.x, (int)gridCoord.y);
    std::visit([&](auto&& actualType) {
        using T = std::decay_t<decltype(actualType)>;
        if constexpr (std::is_same_v<T, BlockType>) {
            std::shared_ptr<Object> newBlock = ObjectFactory::createBlock(actualType, gridCoord);
            if (newBlock) {
                gridBlocks[key].push(newBlock);
            }
        }
        else if constexpr (std::is_same_v<T, EnemyType>) {
            std::shared_ptr<Object> newEnemy = ObjectFactory::createEnemy(actualType, GridSystem::getWorldPosition(gridCoord),{1,1});
            if (newEnemy) {
                gridBlocks[key].push(newEnemy);
            }
        }
        else if constexpr (std::is_same_v<T, CharacterType>) {
            std::shared_ptr<Object> newChar = ObjectFactory::createCharacter(actualType, GridSystem::getWorldPosition(gridCoord));
            if (newChar) {
                gridBlocks[key].push(newChar);
            }
        }
        else if constexpr (std::is_same_v<T, InteractiveType>) {
            std::shared_ptr<Object> newInter = ObjectFactory::createSpring(GridSystem::getWorldPosition(gridCoord));
            if (newInter) {
                gridBlocks[key].push(newInter);
            }
        }
    }, type);
}

void LevelEditor::removeObject(Vector2 gridCoord) {
    auto key = std::make_pair((int)gridCoord.x, (int)gridCoord.y);
    auto it = gridBlocks.find(key);
    if (it != gridBlocks.end() && !it->second.empty()) {
        if (it->second.top()) {
            it->second.pop();
        }

        if (it->second.empty()) {
            gridBlocks.erase(it);
        }
    }
}

void LevelEditor::setEditMode(bool flag) {
    editMode = flag;
}

bool LevelEditor::isInEditMode() const {
    return editMode;
}

void LevelEditor::saveLevel(const std::string& filename) {
    try {
        json levelData;
        levelData["version"] = "1.0";
        levelData["gridSize"] = GridSystem::GRID_SIZE;
        levelData["objects"] = json::array();

        for (const auto& it : gridBlocks) {
            const auto& gridPos = it.first;
            const auto& objectStack = it.second;

            if (objectStack.empty()) continue;

            std::stack<std::shared_ptr<Object>> tempStack = objectStack;
            std::vector<json> stackJson;
            while (!tempStack.empty()) {
                auto obj = tempStack.top();
                tempStack.pop();

                if (obj && obj->isActive()) {
                    json objectData;
                    objectData["gridX"] = gridPos.first;
                    objectData["gridY"] = gridPos.second;
                    objectData["type"] = objectTypeToString(obj->getObjectType());
                    objectData["sizeX"] = obj->getSize().x;
                    objectData["sizeY"] = obj->getSize().y;
                    stackJson.push_back(objectData);
                } 
            }
            for (const auto& objData : stackJson) {
                levelData["objects"].push_back(objData);
            }
        }
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error when opening file to save level!\n";
            return;
        }

        file << levelData.dump(5);
        file.close();

        std::cout << "Level saved successfully to: " << filename << "\n";
        std::cout << "Saved " << levelData["objects"].size() << "objects\n";
    } catch (const std::exception& e) {
        std::cerr << "Error saving level: " << e.what() << "\n";
    }
}

void LevelEditor::loadLevel(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error when opening file to load level\n";
            return;
        }

        json levelData;
        file >> levelData;
        file.close();

        clearLevel();
        
        if (levelData.contains("objects") && levelData["objects"].is_array()) {
            int loadedCount = 0;
            for (const auto& objData : levelData["objects"]) {
                try {
                    float gridX = objData["gridX"];
                    float gridY = objData["gridY"];
                    std::string typeStr = objData["type"];
                    ObjectType objType = stringToObjectType(typeStr);
                    placeObject(objType, {gridX, gridY});
                    float sizeX = objData["sizeX"];
                    float sizeY = objData["sizeY"];
                    if (GameContext::getInstance().currentState && GameContext::getInstance().currentState == GameContext::getInstance().gamePlayState) {
                        Vector2 worldPos = GridSystem::getWorldPosition({gridX, gridY});
                        GameContext::getInstance().addObject(objType, worldPos, {sizeX, sizeY});
                    }
                    loadedCount++;
                } catch (const std::exception& e) {
                    std::cerr << "Error loading object: " << e.what() << "\n";
                }
            }
            std::cout << "Level loaded successfully from: " << filename << "\n";
            std::cout << "Loaded " << loadedCount << " objects\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading level: " << e.what() << "\n";
    }
}

void LevelEditor::clearLevel() {
    clearing = true;
    for (auto& pair : gridBlocks) {
        while (!pair.second.empty()) {
            pair.second.pop();
        }
    }
    gridBlocks.clear();
    clearing = false;
}

std::string LevelEditor::objectTypeToString(const ObjectType& type) {
    if (std::holds_alternative<BlockType>(type)) {
        BlockType blockType = std::get<BlockType>(type); 
        switch (blockType)
        {
        case BlockType::GROUND: return "GROUND";
            
        case BlockType::BRICK: return "BRICK";
            break;
        case BlockType::INVISIBLE: return "INVISIBLE";
            break;        
        case BlockType::PIPE: return "PIPE";
            break;
        case BlockType::PLATFORM: return "PLATFORM";
            break;
        case BlockType::QUESTION: return "QUESTION";
            break;
        default: return "GROUND";
            break;
        }
    }
    else if (std::holds_alternative<EnemyType>(type)) {
        EnemyType enemyType = std::get<EnemyType>(type);
        switch (enemyType) 
        {
            case EnemyType::GOOMBA: return "GOOMBA";
            case EnemyType::GREEN_KOOPA: return "GREEN_KOOPA";
            case EnemyType::RED_KOOPA: return "RED_KOOPA";
                break;
            case EnemyType::HAMMER_BRO: return "HAMMER_BRO";
                break;
            case EnemyType::PIRANHA_PLANT: return "PIRANHA_PLANT";
                break;
            case EnemyType::DRY_BOWSER: return "DRY_BOWSER";
				break;
            default: return "GOOMBA";
                break;
        }
    }
    else if (std::holds_alternative<CharacterType>(type)) {
        CharacterType charType = std::get<CharacterType>(type);
        switch (charType) {
            case CharacterType::LUIGI: return "LUIGI";
                break;
            default: return "MARIO";
                break;
        }
    }
    else if (std::holds_alternative<InteractiveType>(type)) {
        InteractiveType inter = std::get<InteractiveType>(type);
        switch (inter) {
            case InteractiveType::SPRING: return "SPRING";
                break;
            default: return "SPRING";
                break;
        }
    }
    return "UNKNOWN";
}

ObjectType LevelEditor::stringToObjectType(const std::string& typeStr) {
    if (typeStr == "GROUND") return BlockType::GROUND;
    if (typeStr == "BRICK") return BlockType::BRICK;
    if (typeStr == "INVISIBLE") return BlockType::INVISIBLE;
    if (typeStr == "PIPE") return BlockType::PIPE;
    if (typeStr == "PLATFORM") return BlockType::PLATFORM;
    if (typeStr == "QUESTION") return BlockType::QUESTION;
	if (typeStr == "GREEN_KOOPA") return EnemyType::GREEN_KOOPA;
	if (typeStr == "RED_KOOPA") return EnemyType::RED_KOOPA;
    if (typeStr == "GOOMBA") return EnemyType::GOOMBA;
    if (typeStr == "SPRING") return InteractiveType::SPRING;
    return BlockType::GROUND;
}

