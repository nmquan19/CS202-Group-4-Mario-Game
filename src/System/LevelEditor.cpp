#include <memory>
#include <utility>
#include <raylib.h>
#include <type_traits>
#include <variant>
#include <iostream>
#include <string>
#include "../../include/System/LevelEditor.h"
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/Objects/BLock.h"
#include "../../include/System/Grid.h"
#include "../../include/System/Interface.h"
#include "../../include/Enemy/Enemy.h"
#include "../../include/Game/GameContext.h"
#include <exception>
#include <fstream>
#include <stack>
#include <vector>
#include "../../include/System/LightingSystem.h"
#include "../../include/UI/Menu.h"
Background* Background::instance = nullptr;
Background& Background::getInstance() {
    if (!instance) {
        instance = new Background();
    }
    return *instance;
}

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

void LevelEditor::update(float deltaTime) {

    if (editMode) {
        palette.handleSelection();
        palette.updatePalette(deltaTime);
        handleMouseInput();
    }
}

void LevelEditor::draw() {
    
    BeginMode2D(GameContext::getInstance().camera);
    if (mapSelect == 1) {
        Background::getInstance().draw("Forest_1", { 0,0 });
        Background::getInstance().draw("Forest_1", { 0, 512 });
        Background::getInstance().draw("Ghost_house_1", { 0, 1024 });
    }
    if (mapSelect == 2) {
        Background::getInstance().draw("Airship_night_3", { 0,0 });
        Background::getInstance().draw("Airship_night_3", { 0, 512 });
    }
    if (mapSelect == 3) {
        Background::getInstance().draw("Snow_night_1", { 0,0 });
        Background::getInstance().draw("Snow_night_1", { 0,512 });
    }
    if (!clearing) {    
        for (auto& it : gridBlocks) {

            std::stack<std::shared_ptr<Object>> tempStack = it.second;
            std::vector<std::shared_ptr<Object>> objectsInStack;
            
            while (!tempStack.empty()) {
                objectsInStack.push_back(tempStack.top());
                tempStack.pop();
            }
            
            for (auto objIt = objectsInStack.rbegin(); objIt != objectsInStack.rend(); ++objIt) {
                Object* obj = objIt->get();
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
                newBlock->setGridPos(gridCoord);
            }
            GameContext::getInstance().Objects.push_back(newBlock);
        }
        else if constexpr (std::is_same_v<T, EnemyType>) {
            Vector2 size = { 1,1 };
            if (actualType == EnemyType::DRY_BOWSER) size = {1.5,1.5};
            std::shared_ptr<Object> newEnemy = ObjectFactory::createEnemy(actualType, GridSystem::getWorldPosition(gridCoord), size);
            if (newEnemy) {
                gridBlocks[key].push(newEnemy);
                newEnemy->setGridPos(gridCoord);
            }
            GameContext::getInstance().Objects.push_back(newEnemy);

        }
        else if constexpr (std::is_same_v<T, InteractiveType>) {
            std::shared_ptr<Object> newInter = ObjectFactory::createInteractiveObject(actualType, GridSystem::getWorldPosition(gridCoord));
            if (newInter) {
                gridBlocks[key].push(newInter);
                newInter->setGridPos(gridCoord);
            }
            GameContext::getInstance().Objects.push_back(newInter);
        }
        else if constexpr (std::is_same_v<T, ItemType>) {
            std::shared_ptr<Object> newItem = ObjectFactory::createItem(actualType, GridSystem::getWorldPosition(gridCoord), {1, 1});
            if (newItem) {
                gridBlocks[key].push(newItem);
                newItem->setGridPos(gridCoord);
            }
            GameContext::getInstance().Objects.push_back(newItem);
        }
        else if constexpr (std::is_same_v<T, KoopaShellType>) {
            std::shared_ptr<Object> newShell = ObjectFactory::createKoopaShell(actualType, GridSystem::getWorldPosition(gridCoord), Constants::KoopaShell::standardSize);
            if (newShell) {
                gridBlocks[key].push(newShell);
                newShell->setGridPos(gridCoord);
            }
            GameContext::getInstance().Objects.push_back(newShell);
        }
    }, type);
}

// Remove top of stack
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

// Remove correct object in stack
void LevelEditor::removeObject(Vector2 gridCoord, std::shared_ptr<Object>& target) { 
    auto key = std::make_pair((int)gridCoord.x, (int)gridCoord.y);
    auto it = gridBlocks.find(key);
    if (it != gridBlocks.end() && !it->second.empty()) {
        std::stack<std::shared_ptr<Object>> tempStack;
        while (!it->second.empty()) {
            auto obj = it->second.top();
            it->second.pop();

            if (obj == target) {
                break;
            }
            tempStack.push(obj);
        }

        while (!tempStack.empty()) {
            it->second.push(tempStack.top());
            tempStack.pop();
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
    json levelData;
    levelData["version"] = "1.0";
    levelData["mapSelect"] = mapSelect;
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

        // save from bottom to top of stack!!!
        for (auto objIt = stackJson.rbegin(); objIt != stackJson.rend(); ++objIt) {
            levelData["objects"].push_back(*objIt);
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

        if (levelData.contains("mapSelect")) mapSelect = levelData["mapSelect"];
        
        if (levelData.contains("objects") && levelData["objects"].is_array()) {
            int loadedCount = 0;
            for (const auto& objData : levelData["objects"]) {
                float gridX = objData["gridX"];
                float gridY = objData["gridY"];
                std::string typeStr = objData["type"];
                if ((typeStr == "FIRE_FLOWER" || typeStr == "MUSHROOM" || typeStr == "ONE_UP") && !editMode) {
                    std::shared_ptr<Object> obj = findQuestionBlock({gridX, gridY});
                    std::shared_ptr<Block> quesBlock = dynamic_pointer_cast<Block>(obj);
                    if (quesBlock) {
                        quesBlock->setItemType(typeStr);
                    }
                }
                else { 
                    ObjectType objType = stringToObjectType(typeStr);
                    placeObject(objType, {gridX, gridY});
                    float sizeX = objData["sizeX"];
                    float sizeY = objData["sizeY"];
                    loadedCount++;
                }
            }
            std::cout << "Level loaded successfully from: " << filename << "\n";
            std::cout << "Loaded " << loadedCount << " objects\n";
        }
		NavGraph::getInstance().buildNodes({ 0,0,Constants::WORLDBOUNDS_WIDTH, Constants::WORLDBOUNDS_HEIGHT });
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
        case BlockType::BLOCK_1_1_2: return "BLOCK_1_1_2";
            break;
        case BlockType::BLOCK_1_1_3: return "BLOCK_1_1_3";
            break;
        case BlockType::BLOCK_1_1_12: return "BLOCK_1_1_12";
            break;
        case BlockType::BLOCK_1_1_13: return "BLOCK_1_1_13";
            break;
        case BlockType::BLOCK_1_1_14: return "BLOCK_1_1_14";
            break;
        case BlockType::BLOCK_1_1_15: return "BLOCK_1_1_15";
            break;
        case BlockType::BLOCK_1_1_16: return "BLOCK_1_1_16";
            break;
        case BlockType::BLOCK_1_2_7: return "BLOCK_1_2_7";
            break;
        case BlockType::BLOCK_1_2_8: return "BLOCK_1_2_8";
            break;
        case BlockType::BLOCK_1_2_12: return "BLOCK_1_2_12";
            break;
        case BlockType::BLOCK_1_2_13: return "BLOCK_1_2_13";
            break;
        case BlockType::BLOCK_1_2_14: return "BLOCK_1_2_14";
            break;
        case BlockType::BLOCK_1_2_15: return "BLOCK_1_2_15";
            break;
        case BlockType::BLOCK_1_2_16: return "BLOCK_1_2_16";
            break;
        case BlockType::BLOCK_1_3_4: return "BLOCK_1_3_4";
            break;
        case BlockType::BLOCK_1_3_5: return "BLOCK_1_3_5";
            break;
        case BlockType::BLOCK_1_3_6: return "BLOCK_1_3_6";
            break;
        case BlockType::BLOCK_1_3_7: return "BLOCK_1_3_7";
            break;
        case BlockType::BLOCK_1_3_8: return "BLOCK_1_3_8";
            break;
        case BlockType::BLOCK_1_3_15: return "BLOCK_1_3_15";
            break;
        case BlockType::BLOCK_1_3_16: return "BLOCK_1_3_16";
            break;
        case BlockType::BLOCK_1_4_4: return "BLOCK_1_4_4";
            break;
        case BlockType::BLOCK_1_4_5: return "BLOCK_1_4_5";
            break;
        case BlockType::BLOCK_1_4_6: return "BLOCK_1_4_6";
            break;
        case BlockType::BLOCK_1_4_7: return "BLOCK_1_4_7";
            break;
        case BlockType::BLOCK_1_4_11: return "BLOCK_1_4_11";
            break;
        case BlockType::BLOCK_1_4_12: return "BLOCK_1_4_12";
            break;
        case BlockType::BLOCK_1_4_13: return "BLOCK_1_4_13";
            break;
        case BlockType::BLOCK_1_4_14: return "BLOCK_1_4_14";
            break;
        case BlockType::BLOCK_1_4_15: return "BLOCK_1_4_15";
            break;
        case BlockType::BLOCK_1_4_16: return "BLOCK_1_4_16";
            break;
        case BlockType::BLOCK_1_5_4: return "BLOCK_1_5_4";
            break;
        case BlockType::BLOCK_1_5_5: return "BLOCK_1_5_5";
            break;
        case BlockType::BLOCK_1_5_6: return "BLOCK_1_5_6";
            break;
        case BlockType::BLOCK_1_5_8: return "BLOCK_1_5_8";
            break;
        case BlockType::BLOCK_1_5_9: return "BLOCK_1_5_9";
            break;
        case BlockType::BLOCK_1_5_10: return "BLOCK_1_5_10";
            break;
        case BlockType::BLOCK_1_5_11: return "BLOCK_1_5_11";
            break;
        case BlockType::BLOCK_1_5_12: return "BLOCK_1_5_12";
            break;
        case BlockType::BLOCK_1_5_13: return "BLOCK_1_5_13";
            break;
        case BlockType::BLOCK_1_5_14: return "BLOCK_1_5_14";
            break;
        case BlockType::BLOCK_1_5_15: return "BLOCK_1_5_15";
            break;
        case BlockType::BLOCK_1_5_16: return "BLOCK_1_5_16";
            break;
        case BlockType::BLOCK_1_6_5: return "BLOCK_1_6_5";
            break;
        case BlockType::BLOCK_1_6_6: return "BLOCK_1_6_6";
            break;
        case BlockType::BLOCK_1_7_1: return "BLOCK_1_7_1";
            break;
        case BlockType::BLOCK_1_7_2: return "BLOCK_1_7_2";
            break;
        case BlockType::BLOCK_1_7_3: return "BLOCK_1_7_3";
            break;
        case BlockType::BLOCK_1_7_4: return "BLOCK_1_7_4";
            break;
        case BlockType::BLOCK_1_7_5: return "BLOCK_1_7_5";
            break;
        case BlockType::BLOCK_1_7_6: return "BLOCK_1_7_6";
            break;
        case BlockType::BLOCK_1_7_7: return "BLOCK_1_7_7";
            break;
        case BlockType::BLOCK_1_7_11: return "BLOCK_1_7_11";
            break;
        case BlockType::BLOCK_1_7_12: return "BLOCK_1_7_12";
            break;
        case BlockType::BLOCK_1_7_13: return "BLOCK_1_7_13";
            break;
        case BlockType::BLOCK_1_7_14: return "BLOCK_1_7_14";
            break;
        case BlockType::BLOCK_1_7_15: return "BLOCK_1_7_15";
            break;
        case BlockType::BLOCK_1_7_16: return "BLOCK_1_7_16";
            break;
        case BlockType::BLOCK_1_13_1: return "BLOCK_1_13_1";
            break;
        case BlockType::BLOCK_1_13_2: return "BLOCK_1_13_2";
            break;
        case BlockType::BLOCK_1_13_3: return "BLOCK_1_13_3";
            break;
        case BlockType::BLOCK_1_13_4: return "BLOCK_1_13_4";
            break;
        case BlockType::BLOCK_1_13_5: return "BLOCK_1_13_5";
            break;
        case BlockType::BLOCK_1_13_6: return "BLOCK_1_13_6";
            break;
        case BlockType::BLOCK_1_13_7: return "BLOCK_1_13_7";
            break;
        case BlockType::BLOCK_1_13_8: return "BLOCK_1_13_8";
            break;
        case BlockType::BLOCK_1_13_9: return "BLOCK_1_13_9";
            break;
        case BlockType::BLOCK_1_13_10: return "BLOCK_1_13_10";
            break;
        case BlockType::BLOCK_1_13_11: return "BLOCK_1_13_11";
            break;
        case BlockType::BLOCK_1_13_12: return "BLOCK_1_13_12";
            break;
        case BlockType::BLOCK_1_13_13: return "BLOCK_1_13_13";
            break;
        case BlockType::BLOCK_1_13_14: return "BLOCK_1_13_14";
            break;
        case BlockType::BLOCK_1_13_15: return "BLOCK_1_13_15";
            break;
        case BlockType::BLOCK_1_13_16: return "BLOCK_1_13_16";
            break;
        case BlockType::BLOCK_1_14_1: return "BLOCK_1_14_1";
            break;
        case BlockType::BLOCK_1_14_2: return "BLOCK_1_14_2";
            break;
        case BlockType::BLOCK_1_14_3: return "BLOCK_1_14_3";
            break;
        case BlockType::BLOCK_1_14_4: return "BLOCK_1_14_4";
            break;
        case BlockType::BLOCK_1_14_5: return "BLOCK_1_14_5";
            break;
        case BlockType::BLOCK_1_14_6: return "BLOCK_1_14_6";
            break;
        case BlockType::BLOCK_1_14_7: return "BLOCK_1_14_7";
            break;
        case BlockType::BLOCK_1_14_8: return "BLOCK_1_14_8";
            break;
        case BlockType::BLOCK_1_14_9: return "BLOCK_1_14_9";
            break;
        case BlockType::BLOCK_1_14_10: return "BLOCK_1_14_10";
            break;
        case BlockType::BLOCK_1_14_11: return "BLOCK_1_14_11";
            break;
        case BlockType::BLOCK_1_14_12: return "BLOCK_1_14_12";
            break;
        case BlockType::BLOCK_1_14_13: return "BLOCK_1_14_13";
            break;
        case BlockType::BLOCK_1_14_14: return "BLOCK_1_14_14";
            break;
        case BlockType::BLOCK_1_14_15: return "BLOCK_1_14_15";
            break;
        case BlockType::BLOCK_1_14_16: return "BLOCK_1_14_16";
            break;
        case BlockType::BLOCK_1_15_1: return "BLOCK_1_15_1";
            break;
        case BlockType::BLOCK_1_15_2: return "BLOCK_1_15_2";
            break;
        case BlockType::BLOCK_1_15_3: return "BLOCK_1_15_3";
            break;
        case BlockType::BLOCK_1_15_4: return "BLOCK_1_15_4";
            break;
        case BlockType::BLOCK_1_15_5: return "BLOCK_1_15_5";
            break;
        case BlockType::BLOCK_1_15_6: return "BLOCK_1_15_6";
            break;
        case BlockType::BLOCK_1_15_7: return "BLOCK_1_15_7";
            break;
        case BlockType::BLOCK_1_15_8: return "BLOCK_1_15_8";
            break;
        case BlockType::BLOCK_1_15_9: return "BLOCK_1_15_9";
            break;
        case BlockType::BLOCK_1_15_10: return "BLOCK_1_15_10";
            break;
        case BlockType::BLOCK_1_15_11: return "BLOCK_1_15_11";
            break;
        case BlockType::BLOCK_1_15_12: return "BLOCK_1_15_12";
            break;
        case BlockType::BLOCK_1_15_13: return "BLOCK_1_15_13";
            break;
        case BlockType::BLOCK_1_15_14: return "BLOCK_1_15_14";
            break;
        case BlockType::BLOCK_1_15_15: return "BLOCK_1_15_15";
            break;
        case BlockType::BLOCK_1_15_16: return "BLOCK_1_15_16";
            break;
        case BlockType::BLOCK_1_16_1: return "BLOCK_1_16_1";
            break;
        case BlockType::BLOCK_1_16_2: return "BLOCK_1_16_2";
            break;
        case BlockType::BLOCK_1_16_3: return "BLOCK_1_16_3";
            break;
        case BlockType::BLOCK_1_16_4: return "BLOCK_1_16_4";
            break;
        case BlockType::BLOCK_1_16_5: return "BLOCK_1_16_5";
            break;
        case BlockType::BLOCK_1_16_6: return "BLOCK_1_16_6";
            break;
        case BlockType::BLOCK_1_16_7: return "BLOCK_1_16_7";
            break;
        case BlockType::BLOCK_1_16_8: return "BLOCK_1_16_8";
            break;
        case BlockType::BLOCK_1_16_9: return "BLOCK_1_16_9";
            break;
        case BlockType::BLOCK_1_16_10: return "BLOCK_1_16_10";
            break;
        case BlockType::BLOCK_1_16_11: return "BLOCK_1_16_11";
            break;
        case BlockType::BLOCK_1_16_12: return "BLOCK_1_16_12";
            break;
        case BlockType::BLOCK_1_16_13: return "BLOCK_1_16_13";
            break;
        case BlockType::BLOCK_1_16_14: return "BLOCK_1_16_14";
            break;
        case BlockType::BLOCK_1_16_15: return "BLOCK_1_16_15";
            break;
        case BlockType::BLOCK_1_16_16: return "BLOCK_1_16_16";
            break;





        case BlockType::BLOCK_2_1_2: return "BLOCK_2_1_2";
            break;
        case BlockType::BLOCK_2_1_3: return "BLOCK_2_1_3";
            break;
        case BlockType::BLOCK_2_1_12: return "BLOCK_2_1_12";
            break;
        case BlockType::BLOCK_2_1_13: return "BLOCK_2_1_13";
            break;
        case BlockType::BLOCK_2_1_14: return "BLOCK_2_1_14";
            break;
        case BlockType::BLOCK_2_1_15: return "BLOCK_2_1_15";
            break;
        case BlockType::BLOCK_2_1_16: return "BLOCK_2_1_16";
            break;
        case BlockType::BLOCK_2_2_7: return "BLOCK_2_2_7";
            break;
        case BlockType::BLOCK_2_2_8: return "BLOCK_2_2_8";
            break;
        case BlockType::BLOCK_2_2_12: return "BLOCK_2_2_12";
            break;
        case BlockType::BLOCK_2_2_13: return "BLOCK_2_2_13";
            break;
        case BlockType::BLOCK_2_2_14: return "BLOCK_2_2_14";
            break;
        case BlockType::BLOCK_2_2_15: return "BLOCK_2_2_15";
            break;
        case BlockType::BLOCK_2_2_16: return "BLOCK_2_2_16";
            break;
        case BlockType::BLOCK_2_3_4: return "BLOCK_2_3_4";
            break;
        case BlockType::BLOCK_2_3_5: return "BLOCK_2_3_5";
            break;
        case BlockType::BLOCK_2_3_6: return "BLOCK_2_3_6";
            break;
        case BlockType::BLOCK_2_3_7: return "BLOCK_2_3_7";
            break;
        case BlockType::BLOCK_2_3_8: return "BLOCK_2_3_8";
            break;
        case BlockType::BLOCK_2_3_15: return "BLOCK_2_3_15";
            break;
        case BlockType::BLOCK_2_3_16: return "BLOCK_2_3_16";
            break;
        case BlockType::BLOCK_2_4_4: return "BLOCK_2_4_4";
            break;
        case BlockType::BLOCK_2_4_5: return "BLOCK_2_4_5";
            break;
        case BlockType::BLOCK_2_4_6: return "BLOCK_2_4_6";
            break;
        case BlockType::BLOCK_2_4_7: return "BLOCK_2_4_7";
            break;
        case BlockType::BLOCK_2_4_11: return "BLOCK_2_4_11";
            break;
        case BlockType::BLOCK_2_4_12: return "BLOCK_2_4_12";
            break;
        case BlockType::BLOCK_2_4_13: return "BLOCK_2_4_13";
            break;
        case BlockType::BLOCK_2_4_14: return "BLOCK_2_4_14";
            break;
        case BlockType::BLOCK_2_4_15: return "BLOCK_2_4_15";
            break;
        case BlockType::BLOCK_2_4_16: return "BLOCK_2_4_16";
            break;
        case BlockType::BLOCK_2_5_4: return "BLOCK_2_5_4";
            break;
        case BlockType::BLOCK_2_5_5: return "BLOCK_2_5_5";
            break;
        case BlockType::BLOCK_2_5_6: return "BLOCK_2_5_6";
            break;
        case BlockType::BLOCK_2_5_8: return "BLOCK_2_5_8";
            break;
        case BlockType::BLOCK_2_5_9: return "BLOCK_2_5_9";
            break;
        case BlockType::BLOCK_2_5_10: return "BLOCK_2_5_10";
            break;
        case BlockType::BLOCK_2_5_11: return "BLOCK_2_5_11";
            break;
        case BlockType::BLOCK_2_5_12: return "BLOCK_2_5_12";
            break;
        case BlockType::BLOCK_2_5_13: return "BLOCK_2_5_13";
            break;
        case BlockType::BLOCK_2_5_14: return "BLOCK_2_5_14";
            break;
        case BlockType::BLOCK_2_5_15: return "BLOCK_2_5_15";
            break;
        case BlockType::BLOCK_2_5_16: return "BLOCK_2_5_16";
            break;
        case BlockType::BLOCK_2_6_5: return "BLOCK_2_6_5";
            break;
        case BlockType::BLOCK_2_6_6: return "BLOCK_2_6_6";
            break;
        case BlockType::BLOCK_2_7_1: return "BLOCK_2_7_1";
            break;
        case BlockType::BLOCK_2_7_2: return "BLOCK_2_7_2";
            break;
        case BlockType::BLOCK_2_7_3: return "BLOCK_2_7_3";
            break;
        case BlockType::BLOCK_2_7_4: return "BLOCK_2_7_4";
            break;
        case BlockType::BLOCK_2_7_5: return "BLOCK_2_7_5";
            break;
        case BlockType::BLOCK_2_7_6: return "BLOCK_2_7_6";
            break;
        case BlockType::BLOCK_2_7_7: return "BLOCK_2_7_7";
            break;
        case BlockType::BLOCK_2_7_11: return "BLOCK_2_7_11";
            break;
        case BlockType::BLOCK_2_7_12: return "BLOCK_2_7_12";
            break;
        case BlockType::BLOCK_2_7_13: return "BLOCK_2_7_13";
            break;
        case BlockType::BLOCK_2_7_14: return "BLOCK_2_7_14";
            break;
        case BlockType::BLOCK_2_7_15: return "BLOCK_2_7_15";
            break;
        case BlockType::BLOCK_2_7_16: return "BLOCK_2_7_16";
            break;
        case BlockType::BLOCK_2_13_1: return "BLOCK_2_13_1";
            break;
        case BlockType::BLOCK_2_13_2: return "BLOCK_2_13_2";
            break;
        case BlockType::BLOCK_2_13_3: return "BLOCK_2_13_3";
            break;
        case BlockType::BLOCK_2_13_4: return "BLOCK_2_13_4";
            break;
        case BlockType::BLOCK_2_13_5: return "BLOCK_2_13_5";
            break;
        case BlockType::BLOCK_2_13_6: return "BLOCK_2_13_6";
            break;
        case BlockType::BLOCK_2_13_7: return "BLOCK_2_13_7";
            break;
        case BlockType::BLOCK_2_13_8: return "BLOCK_2_13_8";
            break;
        case BlockType::BLOCK_2_13_9: return "BLOCK_2_13_9";
            break;
        case BlockType::BLOCK_2_13_10: return "BLOCK_2_13_10";
            break;
        case BlockType::BLOCK_2_13_11: return "BLOCK_2_13_11";
            break;
        case BlockType::BLOCK_2_13_12: return "BLOCK_2_13_12";
            break;
        case BlockType::BLOCK_2_13_13: return "BLOCK_2_13_13";
            break;
        case BlockType::BLOCK_2_13_14: return "BLOCK_2_13_14";
            break;
        case BlockType::BLOCK_2_13_15: return "BLOCK_2_13_15";
            break;
        case BlockType::BLOCK_2_13_16: return "BLOCK_2_13_16";
            break;
        case BlockType::BLOCK_2_14_1: return "BLOCK_2_14_1";
            break;
        case BlockType::BLOCK_2_14_2: return "BLOCK_2_14_2";
            break;
        case BlockType::BLOCK_2_14_3: return "BLOCK_2_14_3";
            break;
        case BlockType::BLOCK_2_14_4: return "BLOCK_2_14_4";
            break;
        case BlockType::BLOCK_2_14_5: return "BLOCK_2_14_5";
            break;
        case BlockType::BLOCK_2_14_6: return "BLOCK_2_14_6";
            break;
        case BlockType::BLOCK_2_14_7: return "BLOCK_2_14_7";
            break;
        case BlockType::BLOCK_2_14_8: return "BLOCK_2_14_8";
            break;
        case BlockType::BLOCK_2_14_9: return "BLOCK_2_14_9";
            break;
        case BlockType::BLOCK_2_14_10: return "BLOCK_2_14_10";
            break;
        case BlockType::BLOCK_2_14_11: return "BLOCK_2_14_11";
            break;
        case BlockType::BLOCK_2_14_12: return "BLOCK_2_14_12";
            break;
        case BlockType::BLOCK_2_14_13: return "BLOCK_2_14_13";
            break;
        case BlockType::BLOCK_2_14_14: return "BLOCK_2_14_14";
            break;
        case BlockType::BLOCK_2_14_15: return "BLOCK_2_14_15";
            break;
        case BlockType::BLOCK_2_14_16: return "BLOCK_2_14_16";
            break;
        case BlockType::BLOCK_2_15_1: return "BLOCK_2_15_1";
            break;
        case BlockType::BLOCK_2_15_2: return "BLOCK_2_15_2";
            break;
        case BlockType::BLOCK_2_15_3: return "BLOCK_2_15_3";
            break;
        case BlockType::BLOCK_2_15_4: return "BLOCK_2_15_4";
            break;
        case BlockType::BLOCK_2_15_5: return "BLOCK_2_15_5";
            break;
        case BlockType::BLOCK_2_15_6: return "BLOCK_2_15_6";
            break;
        case BlockType::BLOCK_2_15_7: return "BLOCK_2_15_7";
            break;
        case BlockType::BLOCK_2_15_8: return "BLOCK_2_15_8";
            break;
        case BlockType::BLOCK_2_15_9: return "BLOCK_2_15_9";
            break;
        case BlockType::BLOCK_2_15_10: return "BLOCK_2_15_10";
            break;
        case BlockType::BLOCK_2_15_11: return "BLOCK_2_15_11";
            break;
        case BlockType::BLOCK_2_15_12: return "BLOCK_2_15_12";
            break;
        case BlockType::BLOCK_2_15_13: return "BLOCK_2_15_13";
            break;
        case BlockType::BLOCK_2_15_14: return "BLOCK_2_15_14";
            break;
        case BlockType::BLOCK_2_15_15: return "BLOCK_2_15_15";
            break;
        case BlockType::BLOCK_2_15_16: return "BLOCK_2_15_16";
            break;
        case BlockType::BLOCK_2_16_1: return "BLOCK_2_16_1";
            break;
        case BlockType::BLOCK_2_16_2: return "BLOCK_2_16_2";
            break;
        case BlockType::BLOCK_2_16_3: return "BLOCK_2_16_3";
            break;
        case BlockType::BLOCK_2_16_4: return "BLOCK_2_16_4";
            break;
        case BlockType::BLOCK_2_16_5: return "BLOCK_2_16_5";
            break;
        case BlockType::BLOCK_2_16_6: return "BLOCK_2_16_6";
            break;
        case BlockType::BLOCK_2_16_7: return "BLOCK_2_16_7";
            break;
        case BlockType::BLOCK_2_16_8: return "BLOCK_2_16_8";
            break;
        case BlockType::BLOCK_2_16_9: return "BLOCK_2_16_9";
            break;
        case BlockType::BLOCK_2_16_10: return "BLOCK_2_16_10";
            break;
        case BlockType::BLOCK_2_16_11: return "BLOCK_2_16_11";
            break;
        case BlockType::BLOCK_2_16_12: return "BLOCK_2_16_12";
            break;
        case BlockType::BLOCK_2_16_13: return "BLOCK_2_16_13";
            break;
        case BlockType::BLOCK_2_16_14: return "BLOCK_2_16_14";
            break;
        case BlockType::BLOCK_2_16_15: return "BLOCK_2_16_15";
            break;
        case BlockType::BLOCK_2_16_16: return "BLOCK_2_16_16";
            break;





        case BlockType::BLOCK_3_1_2: return "BLOCK_3_1_2";
            break;
        case BlockType::BLOCK_3_1_3: return "BLOCK_3_1_3";
            break;
        case BlockType::BLOCK_3_1_12: return "BLOCK_3_1_12";
            break;
        case BlockType::BLOCK_3_1_13: return "BLOCK_3_1_13";
            break;
        case BlockType::BLOCK_3_1_14: return "BLOCK_3_1_14";
            break;
        case BlockType::BLOCK_3_1_15: return "BLOCK_3_1_15";
            break;
        case BlockType::BLOCK_3_1_16: return "BLOCK_3_1_16";
            break;
        case BlockType::BLOCK_3_2_7: return "BLOCK_3_2_7";
            break;
        case BlockType::BLOCK_3_2_8: return "BLOCK_3_2_8";
            break;
        case BlockType::BLOCK_3_2_12: return "BLOCK_3_2_12";
            break;
        case BlockType::BLOCK_3_2_13: return "BLOCK_3_2_13";
            break;
        case BlockType::BLOCK_3_2_14: return "BLOCK_3_2_14";
            break;
        case BlockType::BLOCK_3_2_15: return "BLOCK_3_2_15";
            break;
        case BlockType::BLOCK_3_2_16: return "BLOCK_3_2_16";
            break;
        case BlockType::BLOCK_3_3_4: return "BLOCK_3_3_4";
            break;
        case BlockType::BLOCK_3_3_5: return "BLOCK_3_3_5";
            break;
        case BlockType::BLOCK_3_3_6: return "BLOCK_3_3_6";
            break;
        case BlockType::BLOCK_3_3_7: return "BLOCK_3_3_7";
            break;
        case BlockType::BLOCK_3_3_8: return "BLOCK_3_3_8";
            break;
        case BlockType::BLOCK_3_3_15: return "BLOCK_3_3_15";
            break;
        case BlockType::BLOCK_3_3_16: return "BLOCK_3_3_16";
            break;
        case BlockType::BLOCK_3_4_4: return "BLOCK_3_4_4";
            break;
        case BlockType::BLOCK_3_4_5: return "BLOCK_3_4_5";
            break;
        case BlockType::BLOCK_3_4_6: return "BLOCK_3_4_6";
            break;
        case BlockType::BLOCK_3_4_7: return "BLOCK_3_4_7";
            break;
        case BlockType::BLOCK_3_4_11: return "BLOCK_3_4_11";
            break;
        case BlockType::BLOCK_3_4_12: return "BLOCK_3_4_12";
            break;
        case BlockType::BLOCK_3_4_13: return "BLOCK_3_4_13";
            break;
        case BlockType::BLOCK_3_4_14: return "BLOCK_3_4_14";
            break;
        case BlockType::BLOCK_3_4_15: return "BLOCK_3_4_15";
            break;
        case BlockType::BLOCK_3_4_16: return "BLOCK_3_4_16";
            break;
        case BlockType::BLOCK_3_5_4: return "BLOCK_3_5_4";
            break;
        case BlockType::BLOCK_3_5_5: return "BLOCK_3_5_5";
            break;
        case BlockType::BLOCK_3_5_6: return "BLOCK_3_5_6";
            break;
        case BlockType::BLOCK_3_5_8: return "BLOCK_3_5_8";
            break;
        case BlockType::BLOCK_3_5_9: return "BLOCK_3_5_9";
            break;
        case BlockType::BLOCK_3_5_10: return "BLOCK_3_5_10";
            break;
        case BlockType::BLOCK_3_5_11: return "BLOCK_3_5_11";
            break;
        case BlockType::BLOCK_3_5_12: return "BLOCK_3_5_12";
            break;
        case BlockType::BLOCK_3_5_13: return "BLOCK_3_5_13";
            break;
        case BlockType::BLOCK_3_5_14: return "BLOCK_3_5_14";
            break;
        case BlockType::BLOCK_3_5_15: return "BLOCK_3_5_15";
            break;
        case BlockType::BLOCK_3_5_16: return "BLOCK_3_5_16";
            break;
        case BlockType::BLOCK_3_6_5: return "BLOCK_3_6_5";
            break;
        case BlockType::BLOCK_3_6_6: return "BLOCK_3_6_6";
            break;
        case BlockType::BLOCK_3_7_1: return "BLOCK_3_7_1";
            break;
        case BlockType::BLOCK_3_7_2: return "BLOCK_3_7_2";
            break;
        case BlockType::BLOCK_3_7_3: return "BLOCK_3_7_3";
            break;
        case BlockType::BLOCK_3_7_4: return "BLOCK_3_7_4";
            break;
        case BlockType::BLOCK_3_7_5: return "BLOCK_3_7_5";
            break;
        case BlockType::BLOCK_3_7_6: return "BLOCK_3_7_6";
            break;
        case BlockType::BLOCK_3_7_7: return "BLOCK_3_7_7";
            break;
        case BlockType::BLOCK_3_7_11: return "BLOCK_3_7_11";
            break;
        case BlockType::BLOCK_3_7_12: return "BLOCK_3_7_12";
            break;
        case BlockType::BLOCK_3_7_13: return "BLOCK_3_7_13";
            break;
        case BlockType::BLOCK_3_7_14: return "BLOCK_3_7_14";
            break;
        case BlockType::BLOCK_3_7_15: return "BLOCK_3_7_15";
            break;
        case BlockType::BLOCK_3_7_16: return "BLOCK_3_7_16";
            break;
        case BlockType::BLOCK_3_13_1: return "BLOCK_3_13_1";
            break;
        case BlockType::BLOCK_3_13_2: return "BLOCK_3_13_2";
            break;
        case BlockType::BLOCK_3_13_3: return "BLOCK_3_13_3";
            break;
        case BlockType::BLOCK_3_13_4: return "BLOCK_3_13_4";
            break;
        case BlockType::BLOCK_3_13_5: return "BLOCK_3_13_5";
            break;
        case BlockType::BLOCK_3_13_6: return "BLOCK_3_13_6";
            break;
        case BlockType::BLOCK_3_13_7: return "BLOCK_3_13_7";
            break;
        case BlockType::BLOCK_3_13_8: return "BLOCK_3_13_8";
            break;
        case BlockType::BLOCK_3_13_9: return "BLOCK_3_13_9";
            break;
        case BlockType::BLOCK_3_13_10: return "BLOCK_3_13_10";
            break;
        case BlockType::BLOCK_3_13_11: return "BLOCK_3_13_11";
            break;
        case BlockType::BLOCK_3_13_12: return "BLOCK_3_13_12";
            break;
        case BlockType::BLOCK_3_13_13: return "BLOCK_3_13_13";
            break;
        case BlockType::BLOCK_3_13_14: return "BLOCK_3_13_14";
            break;
        case BlockType::BLOCK_3_13_15: return "BLOCK_3_13_15";
            break;
        case BlockType::BLOCK_3_13_16: return "BLOCK_3_13_16";
            break;
        case BlockType::BLOCK_3_14_1: return "BLOCK_3_14_1";
            break;
        case BlockType::BLOCK_3_14_2: return "BLOCK_3_14_2";
            break;
        case BlockType::BLOCK_3_14_3: return "BLOCK_3_14_3";
            break;
        case BlockType::BLOCK_3_14_4: return "BLOCK_3_14_4";
            break;
        case BlockType::BLOCK_3_14_5: return "BLOCK_3_14_5";
            break;
        case BlockType::BLOCK_3_14_6: return "BLOCK_3_14_6";
            break;
        case BlockType::BLOCK_3_14_7: return "BLOCK_3_14_7";
            break;
        case BlockType::BLOCK_3_14_8: return "BLOCK_3_14_8";
            break;
        case BlockType::BLOCK_3_14_9: return "BLOCK_3_14_9";
            break;
        case BlockType::BLOCK_3_14_10: return "BLOCK_3_14_10";
            break;
        case BlockType::BLOCK_3_14_11: return "BLOCK_3_14_11";
            break;
        case BlockType::BLOCK_3_14_12: return "BLOCK_3_14_12";
            break;
        case BlockType::BLOCK_3_14_13: return "BLOCK_3_14_13";
            break;
        case BlockType::BLOCK_3_14_14: return "BLOCK_3_14_14";
            break;
        case BlockType::BLOCK_3_14_15: return "BLOCK_3_14_15";
            break;
        case BlockType::BLOCK_3_14_16: return "BLOCK_3_14_16";
            break;
        case BlockType::BLOCK_3_15_1: return "BLOCK_3_15_1";
            break;
        case BlockType::BLOCK_3_15_2: return "BLOCK_3_15_2";
            break;
        case BlockType::BLOCK_3_15_3: return "BLOCK_3_15_3";
            break;
        case BlockType::BLOCK_3_15_4: return "BLOCK_3_15_4";
            break;
        case BlockType::BLOCK_3_15_5: return "BLOCK_3_15_5";
            break;
        case BlockType::BLOCK_3_15_6: return "BLOCK_3_15_6";
            break;
        case BlockType::BLOCK_3_15_7: return "BLOCK_3_15_7";
            break;
        case BlockType::BLOCK_3_15_8: return "BLOCK_3_15_8";
            break;
        case BlockType::BLOCK_3_15_9: return "BLOCK_3_15_9";
            break;
        case BlockType::BLOCK_3_15_10: return "BLOCK_3_15_10";
            break;
        case BlockType::BLOCK_3_15_11: return "BLOCK_3_15_11";
            break;
        case BlockType::BLOCK_3_15_12: return "BLOCK_3_15_12";
            break;
        case BlockType::BLOCK_3_15_13: return "BLOCK_3_15_13";
            break;
        case BlockType::BLOCK_3_15_14: return "BLOCK_3_15_14";
            break;
        case BlockType::BLOCK_3_15_15: return "BLOCK_3_15_15";
            break;
        case BlockType::BLOCK_3_15_16: return "BLOCK_3_15_16";
            break;
        case BlockType::BLOCK_3_16_1: return "BLOCK_3_16_1";
            break;
        case BlockType::BLOCK_3_16_2: return "BLOCK_3_16_2";
            break;
        case BlockType::BLOCK_3_16_3: return "BLOCK_3_16_3";
            break;
        case BlockType::BLOCK_3_16_4: return "BLOCK_3_16_4";
            break;
        case BlockType::BLOCK_3_16_5: return "BLOCK_3_16_5";
            break;
        case BlockType::BLOCK_3_16_6: return "BLOCK_3_16_6";
            break;
        case BlockType::BLOCK_3_16_7: return "BLOCK_3_16_7";
            break;
        case BlockType::BLOCK_3_16_8: return "BLOCK_3_16_8";
            break;
        case BlockType::BLOCK_3_16_9: return "BLOCK_3_16_9";
            break;
        case BlockType::BLOCK_3_16_10: return "BLOCK_3_16_10";
            break;
        case BlockType::BLOCK_3_16_11: return "BLOCK_3_16_11";
            break;
        case BlockType::BLOCK_3_16_12: return "BLOCK_3_16_12";
            break;
        case BlockType::BLOCK_3_16_13: return "BLOCK_3_16_13";
            break;
        case BlockType::BLOCK_3_16_14: return "BLOCK_3_16_14";
            break;
        case BlockType::BLOCK_3_16_15: return "BLOCK_3_16_15";
            break;
        case BlockType::BLOCK_3_16_16: return "BLOCK_3_16_16";
            break;





        case BlockType::BLOCK_4_1_2: return "BLOCK_4_1_2";
            break;
        case BlockType::BLOCK_4_1_3: return "BLOCK_4_1_3";
            break;
        case BlockType::BLOCK_4_1_12: return "BLOCK_4_1_12";
            break;
        case BlockType::BLOCK_4_1_13: return "BLOCK_4_1_13";
            break;
        case BlockType::BLOCK_4_1_14: return "BLOCK_4_1_14";
            break;
        case BlockType::BLOCK_4_1_15: return "BLOCK_4_1_15";
            break;
        case BlockType::BLOCK_4_1_16: return "BLOCK_4_1_16";
            break;
        case BlockType::BLOCK_4_2_7: return "BLOCK_4_2_7";
            break;
        case BlockType::BLOCK_4_2_8: return "BLOCK_4_2_8";
            break;
        case BlockType::BLOCK_4_2_12: return "BLOCK_4_2_12";
            break;
        case BlockType::BLOCK_4_2_13: return "BLOCK_4_2_13";
            break;
        case BlockType::BLOCK_4_2_14: return "BLOCK_4_2_14";
            break;
        case BlockType::BLOCK_4_2_15: return "BLOCK_4_2_15";
            break;
        case BlockType::BLOCK_4_2_16: return "BLOCK_4_2_16";
            break;
        case BlockType::BLOCK_4_3_4: return "BLOCK_4_3_4";
            break;
        case BlockType::BLOCK_4_3_5: return "BLOCK_4_3_5";
            break;
        case BlockType::BLOCK_4_3_6: return "BLOCK_4_3_6";
            break;
        case BlockType::BLOCK_4_3_7: return "BLOCK_4_3_7";
            break;
        case BlockType::BLOCK_4_3_8: return "BLOCK_4_3_8";
            break;
        case BlockType::BLOCK_4_3_15: return "BLOCK_4_3_15";
            break;
        case BlockType::BLOCK_4_3_16: return "BLOCK_4_3_16";
            break;
        case BlockType::BLOCK_4_4_4: return "BLOCK_4_4_4";
            break;
        case BlockType::BLOCK_4_4_5: return "BLOCK_4_4_5";
            break;
        case BlockType::BLOCK_4_4_6: return "BLOCK_4_4_6";
            break;
        case BlockType::BLOCK_4_4_7: return "BLOCK_4_4_7";
            break;
        case BlockType::BLOCK_4_4_11: return "BLOCK_4_4_11";
            break;
        case BlockType::BLOCK_4_4_12: return "BLOCK_4_4_12";
            break;
        case BlockType::BLOCK_4_4_13: return "BLOCK_4_4_13";
            break;
        case BlockType::BLOCK_4_4_14: return "BLOCK_4_4_14";
            break;
        case BlockType::BLOCK_4_4_15: return "BLOCK_4_4_15";
            break;
        case BlockType::BLOCK_4_4_16: return "BLOCK_4_4_16";
            break;
        case BlockType::BLOCK_4_5_4: return "BLOCK_4_5_4";
            break;
        case BlockType::BLOCK_4_5_5: return "BLOCK_4_5_5";
            break;
        case BlockType::BLOCK_4_5_6: return "BLOCK_4_5_6";
            break;
        case BlockType::BLOCK_4_5_8: return "BLOCK_4_5_8";
            break;
        case BlockType::BLOCK_4_5_9: return "BLOCK_4_5_9";
            break;
        case BlockType::BLOCK_4_5_10: return "BLOCK_4_5_10";
            break;
        case BlockType::BLOCK_4_5_11: return "BLOCK_4_5_11";
            break;
        case BlockType::BLOCK_4_5_12: return "BLOCK_4_5_12";
            break;
        case BlockType::BLOCK_4_5_13: return "BLOCK_4_5_13";
            break;
        case BlockType::BLOCK_4_5_14: return "BLOCK_4_5_14";
            break;
        case BlockType::BLOCK_4_5_15: return "BLOCK_4_5_15";
            break;
        case BlockType::BLOCK_4_5_16: return "BLOCK_4_5_16";
            break;
        case BlockType::BLOCK_4_6_5: return "BLOCK_4_6_5";
            break;
        case BlockType::BLOCK_4_6_6: return "BLOCK_4_6_6";
            break;
        case BlockType::BLOCK_4_7_1: return "BLOCK_4_7_1";
            break;
        case BlockType::BLOCK_4_7_2: return "BLOCK_4_7_2";
            break;
        case BlockType::BLOCK_4_7_3: return "BLOCK_4_7_3";
            break;
        case BlockType::BLOCK_4_7_4: return "BLOCK_4_7_4";
            break;
        case BlockType::BLOCK_4_7_5: return "BLOCK_4_7_5";
            break;
        case BlockType::BLOCK_4_7_6: return "BLOCK_4_7_6";
            break;
        case BlockType::BLOCK_4_7_7: return "BLOCK_4_7_7";
            break;
        case BlockType::BLOCK_4_7_11: return "BLOCK_4_7_11";
            break;
        case BlockType::BLOCK_4_7_12: return "BLOCK_4_7_12";
            break;
        case BlockType::BLOCK_4_7_13: return "BLOCK_4_7_13";
            break;
        case BlockType::BLOCK_4_7_14: return "BLOCK_4_7_14";
            break;
        case BlockType::BLOCK_4_7_15: return "BLOCK_4_7_15";
            break;
        case BlockType::BLOCK_4_7_16: return "BLOCK_4_7_16";
            break;
        case BlockType::BLOCK_4_13_1: return "BLOCK_4_13_1";
            break;
        case BlockType::BLOCK_4_13_2: return "BLOCK_4_13_2";
            break;
        case BlockType::BLOCK_4_13_3: return "BLOCK_4_13_3";
            break;
        case BlockType::BLOCK_4_13_4: return "BLOCK_4_13_4";
            break;
        case BlockType::BLOCK_4_13_5: return "BLOCK_4_13_5";
            break;
        case BlockType::BLOCK_4_13_6: return "BLOCK_4_13_6";
            break;
        case BlockType::BLOCK_4_13_7: return "BLOCK_4_13_7";
            break;
        case BlockType::BLOCK_4_13_8: return "BLOCK_4_13_8";
            break;
        case BlockType::BLOCK_4_13_9: return "BLOCK_4_13_9";
            break;
        case BlockType::BLOCK_4_13_10: return "BLOCK_4_13_10";
            break;
        case BlockType::BLOCK_4_13_11: return "BLOCK_4_13_11";
            break;
        case BlockType::BLOCK_4_13_12: return "BLOCK_4_13_12";
            break;
        case BlockType::BLOCK_4_13_13: return "BLOCK_4_13_13";
            break;
        case BlockType::BLOCK_4_13_14: return "BLOCK_4_13_14";
            break;
        case BlockType::BLOCK_4_13_15: return "BLOCK_4_13_15";
            break;
        case BlockType::BLOCK_4_13_16: return "BLOCK_4_13_16";
            break;
        case BlockType::BLOCK_4_14_1: return "BLOCK_4_14_1";
            break;
        case BlockType::BLOCK_4_14_2: return "BLOCK_4_14_2";
            break;
        case BlockType::BLOCK_4_14_3: return "BLOCK_4_14_3";
            break;
        case BlockType::BLOCK_4_14_4: return "BLOCK_4_14_4";
            break;
        case BlockType::BLOCK_4_14_5: return "BLOCK_4_14_5";
            break;
        case BlockType::BLOCK_4_14_6: return "BLOCK_4_14_6";
            break;
        case BlockType::BLOCK_4_14_7: return "BLOCK_4_14_7";
            break;
        case BlockType::BLOCK_4_14_8: return "BLOCK_4_14_8";
            break;
        case BlockType::BLOCK_4_14_9: return "BLOCK_4_14_9";
            break;
        case BlockType::BLOCK_4_14_10: return "BLOCK_4_14_10";
            break;
        case BlockType::BLOCK_4_14_11: return "BLOCK_4_14_11";
            break;
        case BlockType::BLOCK_4_14_12: return "BLOCK_4_14_12";
            break;
        case BlockType::BLOCK_4_14_13: return "BLOCK_4_14_13";
            break;
        case BlockType::BLOCK_4_14_14: return "BLOCK_4_14_14";
            break;
        case BlockType::BLOCK_4_14_15: return "BLOCK_4_14_15";
            break;
        case BlockType::BLOCK_4_14_16: return "BLOCK_4_14_16";
            break;
        case BlockType::BLOCK_4_15_1: return "BLOCK_4_15_1";
            break;
        case BlockType::BLOCK_4_15_2: return "BLOCK_4_15_2";
            break;
        case BlockType::BLOCK_4_15_3: return "BLOCK_4_15_3";
            break;
        case BlockType::BLOCK_4_15_4: return "BLOCK_4_15_4";
            break;
        case BlockType::BLOCK_4_15_5: return "BLOCK_4_15_5";
            break;
        case BlockType::BLOCK_4_15_6: return "BLOCK_4_15_6";
            break;
        case BlockType::BLOCK_4_15_7: return "BLOCK_4_15_7";
            break;
        case BlockType::BLOCK_4_15_8: return "BLOCK_4_15_8";
            break;
        case BlockType::BLOCK_4_15_9: return "BLOCK_4_15_9";
            break;
        case BlockType::BLOCK_4_15_10: return "BLOCK_4_15_10";
            break;
        case BlockType::BLOCK_4_15_11: return "BLOCK_4_15_11";
            break;
        case BlockType::BLOCK_4_15_12: return "BLOCK_4_15_12";
            break;
        case BlockType::BLOCK_4_15_13: return "BLOCK_4_15_13";
            break;
        case BlockType::BLOCK_4_15_14: return "BLOCK_4_15_14";
            break;
        case BlockType::BLOCK_4_15_15: return "BLOCK_4_15_15";
            break;
        case BlockType::BLOCK_4_15_16: return "BLOCK_4_15_16";
            break;
        case BlockType::BLOCK_4_16_1: return "BLOCK_4_16_1";
            break;
        case BlockType::BLOCK_4_16_2: return "BLOCK_4_16_2";
            break;
        case BlockType::BLOCK_4_16_3: return "BLOCK_4_16_3";
            break;
        case BlockType::BLOCK_4_16_4: return "BLOCK_4_16_4";
            break;
        case BlockType::BLOCK_4_16_5: return "BLOCK_4_16_5";
            break;
        case BlockType::BLOCK_4_16_6: return "BLOCK_4_16_6";
            break;
        case BlockType::BLOCK_4_16_7: return "BLOCK_4_16_7";
            break;
        case BlockType::BLOCK_4_16_8: return "BLOCK_4_16_8";
            break;
        case BlockType::BLOCK_4_16_9: return "BLOCK_4_16_9";
            break;
        case BlockType::BLOCK_4_16_10: return "BLOCK_4_16_10";
            break;
        case BlockType::BLOCK_4_16_11: return "BLOCK_4_16_11";
            break;
        case BlockType::BLOCK_4_16_12: return "BLOCK_4_16_12";
            break;
        case BlockType::BLOCK_4_16_13: return "BLOCK_4_16_13";
            break;
        case BlockType::BLOCK_4_16_14: return "BLOCK_4_16_14";
            break;
        case BlockType::BLOCK_4_16_15: return "BLOCK_4_16_15";
            break;
        case BlockType::BLOCK_4_16_16: return "BLOCK_4_16_16";
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
            case EnemyType::BOO: return "BOO";
                break;
            case EnemyType::BOB_OMBS: return "BOB_OMBS";
                break;
            case EnemyType::LASER_MECHA_KOOPA: return "LASER_MECHA_KOOPA";
                break;
            default: return "GOOMBA";
                break;
        }
    }
    else if (std::holds_alternative<InteractiveType>(type)) {
        InteractiveType inter = std::get<InteractiveType>(type);
        switch (inter) {
        case InteractiveType::SPRING: 
            return "SPRING";
        case InteractiveType::MOVING_PLATFORM:
            return "MOVING_PLATFORM";
        case InteractiveType::FIRE_BAR_BASE:
            return "FIRE_BAR_BASE";
        }
    }
    else if (std::holds_alternative<BackGroundObjectType>(type)) {
        BackGroundObjectType bg = std::get<BackGroundObjectType>(type);
        switch (bg) {
        case BackGroundObjectType::TORCH:
            return "TORCH";
        }
    }
    else if (std::holds_alternative<ItemType>(type)) {
        ItemType item = std::get<ItemType>(type);
        switch (item) {
            case ItemType::COIN: return "COIN";
                break;
            case ItemType::FIRE_FLOWER: return "FIRE_FLOWER";
                break;
            case ItemType::MUSHROOM: return "MUSHROOM";
                break;
            case ItemType::ONE_UP: return "ONE_UP";
                break;
            case ItemType::STAR: return "STAR";
                break;
            default: return "COIN";
                break;
        }
    }
    return "UNKNOWN";
}

ObjectType LevelEditor::stringToObjectType(const std::string& typeStr) {

    if (typeStr == "BLOCK_1_1_2") return BlockType::BLOCK_1_1_2;
    if (typeStr == "BLOCK_1_1_3") return BlockType::BLOCK_1_1_3;
    if (typeStr == "BLOCK_1_1_12") return BlockType::BLOCK_1_1_12;
    if (typeStr == "BLOCK_1_1_13") return BlockType::BLOCK_1_1_13;
    if (typeStr == "BLOCK_1_1_14") return BlockType::BLOCK_1_1_14;
    if (typeStr == "BLOCK_1_1_15") return BlockType::BLOCK_1_1_15;
    if (typeStr == "BLOCK_1_1_16") return BlockType::BLOCK_1_1_16;
    if (typeStr == "BLOCK_1_2_7") return BlockType::BLOCK_1_2_7;
    if (typeStr == "BLOCK_1_2_8") return BlockType::BLOCK_1_2_8;
    if (typeStr == "BLOCK_1_2_12") return BlockType::BLOCK_1_2_12;
    if (typeStr == "BLOCK_1_2_13") return BlockType::BLOCK_1_2_13;
    if (typeStr == "BLOCK_1_2_14") return BlockType::BLOCK_1_2_14;
    if (typeStr == "BLOCK_1_2_15") return BlockType::BLOCK_1_2_15;
    if (typeStr == "BLOCK_1_2_16") return BlockType::BLOCK_1_2_16;
    if (typeStr == "BLOCK_1_3_4") return BlockType::BLOCK_1_3_4;
    if (typeStr == "BLOCK_1_3_5") return BlockType::BLOCK_1_3_5;
    if (typeStr == "BLOCK_1_3_6") return BlockType::BLOCK_1_3_6;
    if (typeStr == "BLOCK_1_3_7") return BlockType::BLOCK_1_3_7;
    if (typeStr == "BLOCK_1_3_8") return BlockType::BLOCK_1_3_8;
    if (typeStr == "BLOCK_1_3_15") return BlockType::BLOCK_1_3_15;
    if (typeStr == "BLOCK_1_3_16") return BlockType::BLOCK_1_3_16;
    if (typeStr == "BLOCK_1_4_4") return BlockType::BLOCK_1_4_4;
    if (typeStr == "BLOCK_1_4_5") return BlockType::BLOCK_1_4_5;
    if (typeStr == "BLOCK_1_4_6") return BlockType::BLOCK_1_4_6;
    if (typeStr == "BLOCK_1_4_7") return BlockType::BLOCK_1_4_7;
    if (typeStr == "BLOCK_1_4_11") return BlockType::BLOCK_1_4_11;
    if (typeStr == "BLOCK_1_4_12") return BlockType::BLOCK_1_4_12;
    if (typeStr == "BLOCK_1_4_13") return BlockType::BLOCK_1_4_13;
    if (typeStr == "BLOCK_1_4_14") return BlockType::BLOCK_1_4_14;
    if (typeStr == "BLOCK_1_4_15") return BlockType::BLOCK_1_4_15;
    if (typeStr == "BLOCK_1_4_16") return BlockType::BLOCK_1_4_16;
    if (typeStr == "BLOCK_1_5_4") return BlockType::BLOCK_1_5_4;
    if (typeStr == "BLOCK_1_5_5") return BlockType::BLOCK_1_5_5;
    if (typeStr == "BLOCK_1_5_6") return BlockType::BLOCK_1_5_6;
    if (typeStr == "BLOCK_1_5_8") return BlockType::BLOCK_1_5_8;
    if (typeStr == "BLOCK_1_5_9") return BlockType::BLOCK_1_5_9;
    if (typeStr == "BLOCK_1_5_10") return BlockType::BLOCK_1_5_10;
    if (typeStr == "BLOCK_1_5_11") return BlockType::BLOCK_1_5_11;
    if (typeStr == "BLOCK_1_5_12") return BlockType::BLOCK_1_5_12;
    if (typeStr == "BLOCK_1_5_13") return BlockType::BLOCK_1_5_13;
    if (typeStr == "BLOCK_1_5_14") return BlockType::BLOCK_1_5_14;
    if (typeStr == "BLOCK_1_5_15") return BlockType::BLOCK_1_5_15;
    if (typeStr == "BLOCK_1_5_16") return BlockType::BLOCK_1_5_16;
    if (typeStr == "BLOCK_1_6_5") return BlockType::BLOCK_1_6_5;
    if (typeStr == "BLOCK_1_6_6") return BlockType::BLOCK_1_6_6;
    if (typeStr == "BLOCK_1_7_1") return BlockType::BLOCK_1_7_1;
    if (typeStr == "BLOCK_1_7_2") return BlockType::BLOCK_1_7_2;
    if (typeStr == "BLOCK_1_7_3") return BlockType::BLOCK_1_7_3;
    if (typeStr == "BLOCK_1_7_4") return BlockType::BLOCK_1_7_4;
    if (typeStr == "BLOCK_1_7_5") return BlockType::BLOCK_1_7_5;
    if (typeStr == "BLOCK_1_7_6") return BlockType::BLOCK_1_7_6;
    if (typeStr == "BLOCK_1_7_7") return BlockType::BLOCK_1_7_7;
    if (typeStr == "BLOCK_1_7_11") return BlockType::BLOCK_1_7_11;
    if (typeStr == "BLOCK_1_7_12") return BlockType::BLOCK_1_7_12;
    if (typeStr == "BLOCK_1_7_13") return BlockType::BLOCK_1_7_13;
    if (typeStr == "BLOCK_1_7_14") return BlockType::BLOCK_1_7_14;
    if (typeStr == "BLOCK_1_7_15") return BlockType::BLOCK_1_7_15;
    if (typeStr == "BLOCK_1_7_16") return BlockType::BLOCK_1_7_16;
    if (typeStr == "BLOCK_1_13_1") return BlockType::BLOCK_1_13_1;
    if (typeStr == "BLOCK_1_13_2") return BlockType::BLOCK_1_13_2;
    if (typeStr == "BLOCK_1_13_3") return BlockType::BLOCK_1_13_3;
    if (typeStr == "BLOCK_1_13_4") return BlockType::BLOCK_1_13_4;
    if (typeStr == "BLOCK_1_13_5") return BlockType::BLOCK_1_13_5;
    if (typeStr == "BLOCK_1_13_6") return BlockType::BLOCK_1_13_6;
    if (typeStr == "BLOCK_1_13_7") return BlockType::BLOCK_1_13_7;
    if (typeStr == "BLOCK_1_13_8") return BlockType::BLOCK_1_13_8;
    if (typeStr == "BLOCK_1_13_9") return BlockType::BLOCK_1_13_9;
    if (typeStr == "BLOCK_1_13_10") return BlockType::BLOCK_1_13_10;
    if (typeStr == "BLOCK_1_13_11") return BlockType::BLOCK_1_13_11;
    if (typeStr == "BLOCK_1_13_12") return BlockType::BLOCK_1_13_12;
    if (typeStr == "BLOCK_1_13_13") return BlockType::BLOCK_1_13_13;
    if (typeStr == "BLOCK_1_13_14") return BlockType::BLOCK_1_13_14;
    if (typeStr == "BLOCK_1_13_15") return BlockType::BLOCK_1_13_15;
    if (typeStr == "BLOCK_1_13_16") return BlockType::BLOCK_1_13_16;
    if (typeStr == "BLOCK_1_14_1") return BlockType::BLOCK_1_14_1;
    if (typeStr == "BLOCK_1_14_2") return BlockType::BLOCK_1_14_2;
    if (typeStr == "BLOCK_1_14_3") return BlockType::BLOCK_1_14_3;
    if (typeStr == "BLOCK_1_14_4") return BlockType::BLOCK_1_14_4;
    if (typeStr == "BLOCK_1_14_5") return BlockType::BLOCK_1_14_5;
    if (typeStr == "BLOCK_1_14_6") return BlockType::BLOCK_1_14_6;
    if (typeStr == "BLOCK_1_14_7") return BlockType::BLOCK_1_14_7;
    if (typeStr == "BLOCK_1_14_8") return BlockType::BLOCK_1_14_8;
    if (typeStr == "BLOCK_1_14_9") return BlockType::BLOCK_1_14_9;
    if (typeStr == "BLOCK_1_14_10") return BlockType::BLOCK_1_14_10;
    if (typeStr == "BLOCK_1_14_11") return BlockType::BLOCK_1_14_11;
    if (typeStr == "BLOCK_1_14_12") return BlockType::BLOCK_1_14_12;
    if (typeStr == "BLOCK_1_14_13") return BlockType::BLOCK_1_14_13;
    if (typeStr == "BLOCK_1_14_14") return BlockType::BLOCK_1_14_14;
    if (typeStr == "BLOCK_1_14_15") return BlockType::BLOCK_1_14_15;
    if (typeStr == "BLOCK_1_14_16") return BlockType::BLOCK_1_14_16;
    if (typeStr == "BLOCK_1_15_1") return BlockType::BLOCK_1_15_1;
    if (typeStr == "BLOCK_1_15_2") return BlockType::BLOCK_1_15_2;
    if (typeStr == "BLOCK_1_15_3") return BlockType::BLOCK_1_15_3;
    if (typeStr == "BLOCK_1_15_4") return BlockType::BLOCK_1_15_4;
    if (typeStr == "BLOCK_1_15_5") return BlockType::BLOCK_1_15_5;
    if (typeStr == "BLOCK_1_15_6") return BlockType::BLOCK_1_15_6;
    if (typeStr == "BLOCK_1_15_7") return BlockType::BLOCK_1_15_7;
    if (typeStr == "BLOCK_1_15_8") return BlockType::BLOCK_1_15_8;
    if (typeStr == "BLOCK_1_15_9") return BlockType::BLOCK_1_15_9;
    if (typeStr == "BLOCK_1_15_10") return BlockType::BLOCK_1_15_10;
    if (typeStr == "BLOCK_1_15_11") return BlockType::BLOCK_1_15_11;
    if (typeStr == "BLOCK_1_15_12") return BlockType::BLOCK_1_15_12;
    if (typeStr == "BLOCK_1_15_13") return BlockType::BLOCK_1_15_13;
    if (typeStr == "BLOCK_1_15_14") return BlockType::BLOCK_1_15_14;
    if (typeStr == "BLOCK_1_15_15") return BlockType::BLOCK_1_15_15;
    if (typeStr == "BLOCK_1_15_16") return BlockType::BLOCK_1_15_16;
    if (typeStr == "BLOCK_1_16_1") return BlockType::BLOCK_1_16_1;
    if (typeStr == "BLOCK_1_16_2") return BlockType::BLOCK_1_16_2;
    if (typeStr == "BLOCK_1_16_3") return BlockType::BLOCK_1_16_3;
    if (typeStr == "BLOCK_1_16_4") return BlockType::BLOCK_1_16_4;
    if (typeStr == "BLOCK_1_16_5") return BlockType::BLOCK_1_16_5;
    if (typeStr == "BLOCK_1_16_6") return BlockType::BLOCK_1_16_6;
    if (typeStr == "BLOCK_1_16_7") return BlockType::BLOCK_1_16_7;
    if (typeStr == "BLOCK_1_16_8") return BlockType::BLOCK_1_16_8;
    if (typeStr == "BLOCK_1_16_9") return BlockType::BLOCK_1_16_9;
    if (typeStr == "BLOCK_1_16_10") return BlockType::BLOCK_1_16_10;
    if (typeStr == "BLOCK_1_16_11") return BlockType::BLOCK_1_16_11;
    if (typeStr == "BLOCK_1_16_12") return BlockType::BLOCK_1_16_12;
    if (typeStr == "BLOCK_1_16_13") return BlockType::BLOCK_1_16_13;
    if (typeStr == "BLOCK_1_16_14") return BlockType::BLOCK_1_16_14;
    if (typeStr == "BLOCK_1_16_15") return BlockType::BLOCK_1_16_15;
    if (typeStr == "BLOCK_1_16_16") return BlockType::BLOCK_1_16_16;





    if (typeStr == "BLOCK_2_1_2") return BlockType::BLOCK_2_1_2;
    if (typeStr == "BLOCK_2_1_3") return BlockType::BLOCK_2_1_3;
    if (typeStr == "BLOCK_2_1_12") return BlockType::BLOCK_2_1_12;
    if (typeStr == "BLOCK_2_1_13") return BlockType::BLOCK_2_1_13;
    if (typeStr == "BLOCK_2_1_14") return BlockType::BLOCK_2_1_14;
    if (typeStr == "BLOCK_2_1_15") return BlockType::BLOCK_2_1_15;
    if (typeStr == "BLOCK_2_1_16") return BlockType::BLOCK_2_1_16;
    if (typeStr == "BLOCK_2_2_7") return BlockType::BLOCK_2_2_7;
    if (typeStr == "BLOCK_2_2_8") return BlockType::BLOCK_2_2_8;
    if (typeStr == "BLOCK_2_2_12") return BlockType::BLOCK_2_2_12;
    if (typeStr == "BLOCK_2_2_13") return BlockType::BLOCK_2_2_13;
    if (typeStr == "BLOCK_2_2_14") return BlockType::BLOCK_2_2_14;
    if (typeStr == "BLOCK_2_2_15") return BlockType::BLOCK_2_2_15;
    if (typeStr == "BLOCK_2_2_16") return BlockType::BLOCK_2_2_16;
    if (typeStr == "BLOCK_2_3_4") return BlockType::BLOCK_2_3_4;
    if (typeStr == "BLOCK_2_3_5") return BlockType::BLOCK_2_3_5;
    if (typeStr == "BLOCK_2_3_6") return BlockType::BLOCK_2_3_6;
    if (typeStr == "BLOCK_2_3_7") return BlockType::BLOCK_2_3_7;
    if (typeStr == "BLOCK_2_3_8") return BlockType::BLOCK_2_3_8;
    if (typeStr == "BLOCK_2_3_15") return BlockType::BLOCK_2_3_15;
    if (typeStr == "BLOCK_2_3_16") return BlockType::BLOCK_2_3_16;
    if (typeStr == "BLOCK_2_4_4") return BlockType::BLOCK_2_4_4;
    if (typeStr == "BLOCK_2_4_5") return BlockType::BLOCK_2_4_5;
    if (typeStr == "BLOCK_2_4_6") return BlockType::BLOCK_2_4_6;
    if (typeStr == "BLOCK_2_4_7") return BlockType::BLOCK_2_4_7;
    if (typeStr == "BLOCK_2_4_11") return BlockType::BLOCK_2_4_11;
    if (typeStr == "BLOCK_2_4_12") return BlockType::BLOCK_2_4_12;
    if (typeStr == "BLOCK_2_4_13") return BlockType::BLOCK_2_4_13;
    if (typeStr == "BLOCK_2_4_14") return BlockType::BLOCK_2_4_14;
    if (typeStr == "BLOCK_2_4_15") return BlockType::BLOCK_2_4_15;
    if (typeStr == "BLOCK_2_4_16") return BlockType::BLOCK_2_4_16;
    if (typeStr == "BLOCK_2_5_4") return BlockType::BLOCK_2_5_4;
    if (typeStr == "BLOCK_2_5_5") return BlockType::BLOCK_2_5_5;
    if (typeStr == "BLOCK_2_5_6") return BlockType::BLOCK_2_5_6;
    if (typeStr == "BLOCK_2_5_8") return BlockType::BLOCK_2_5_8;
    if (typeStr == "BLOCK_2_5_9") return BlockType::BLOCK_2_5_9;
    if (typeStr == "BLOCK_2_5_10") return BlockType::BLOCK_2_5_10;
    if (typeStr == "BLOCK_2_5_11") return BlockType::BLOCK_2_5_11;
    if (typeStr == "BLOCK_2_5_12") return BlockType::BLOCK_2_5_12;
    if (typeStr == "BLOCK_2_5_13") return BlockType::BLOCK_2_5_13;
    if (typeStr == "BLOCK_2_5_14") return BlockType::BLOCK_2_5_14;
    if (typeStr == "BLOCK_2_5_15") return BlockType::BLOCK_2_5_15;
    if (typeStr == "BLOCK_2_5_16") return BlockType::BLOCK_2_5_16;
    if (typeStr == "BLOCK_2_6_5") return BlockType::BLOCK_2_6_5;
    if (typeStr == "BLOCK_2_6_6") return BlockType::BLOCK_2_6_6;
    if (typeStr == "BLOCK_2_7_1") return BlockType::BLOCK_2_7_1;
    if (typeStr == "BLOCK_2_7_2") return BlockType::BLOCK_2_7_2;
    if (typeStr == "BLOCK_2_7_3") return BlockType::BLOCK_2_7_3;
    if (typeStr == "BLOCK_2_7_4") return BlockType::BLOCK_2_7_4;
    if (typeStr == "BLOCK_2_7_5") return BlockType::BLOCK_2_7_5;
    if (typeStr == "BLOCK_2_7_6") return BlockType::BLOCK_2_7_6;
    if (typeStr == "BLOCK_2_7_7") return BlockType::BLOCK_2_7_7;
    if (typeStr == "BLOCK_2_7_11") return BlockType::BLOCK_2_7_11;
    if (typeStr == "BLOCK_2_7_12") return BlockType::BLOCK_2_7_12;
    if (typeStr == "BLOCK_2_7_13") return BlockType::BLOCK_2_7_13;
    if (typeStr == "BLOCK_2_7_14") return BlockType::BLOCK_2_7_14;
    if (typeStr == "BLOCK_2_7_15") return BlockType::BLOCK_2_7_15;
    if (typeStr == "BLOCK_2_7_16") return BlockType::BLOCK_2_7_16;
    if (typeStr == "BLOCK_2_13_1") return BlockType::BLOCK_2_13_1;
    if (typeStr == "BLOCK_2_13_2") return BlockType::BLOCK_2_13_2;
    if (typeStr == "BLOCK_2_13_3") return BlockType::BLOCK_2_13_3;
    if (typeStr == "BLOCK_2_13_4") return BlockType::BLOCK_2_13_4;
    if (typeStr == "BLOCK_2_13_5") return BlockType::BLOCK_2_13_5;
    if (typeStr == "BLOCK_2_13_6") return BlockType::BLOCK_2_13_6;
    if (typeStr == "BLOCK_2_13_7") return BlockType::BLOCK_2_13_7;
    if (typeStr == "BLOCK_2_13_8") return BlockType::BLOCK_2_13_8;
    if (typeStr == "BLOCK_2_13_9") return BlockType::BLOCK_2_13_9;
    if (typeStr == "BLOCK_2_13_10") return BlockType::BLOCK_2_13_10;
    if (typeStr == "BLOCK_2_13_11") return BlockType::BLOCK_2_13_11;
    if (typeStr == "BLOCK_2_13_12") return BlockType::BLOCK_2_13_12;
    if (typeStr == "BLOCK_2_13_13") return BlockType::BLOCK_2_13_13;
    if (typeStr == "BLOCK_2_13_14") return BlockType::BLOCK_2_13_14;
    if (typeStr == "BLOCK_2_13_15") return BlockType::BLOCK_2_13_15;
    if (typeStr == "BLOCK_2_13_16") return BlockType::BLOCK_2_13_16;
    if (typeStr == "BLOCK_2_14_1") return BlockType::BLOCK_2_14_1;
    if (typeStr == "BLOCK_2_14_2") return BlockType::BLOCK_2_14_2;
    if (typeStr == "BLOCK_2_14_3") return BlockType::BLOCK_2_14_3;
    if (typeStr == "BLOCK_2_14_4") return BlockType::BLOCK_2_14_4;
    if (typeStr == "BLOCK_2_14_5") return BlockType::BLOCK_2_14_5;
    if (typeStr == "BLOCK_2_14_6") return BlockType::BLOCK_2_14_6;
    if (typeStr == "BLOCK_2_14_7") return BlockType::BLOCK_2_14_7;
    if (typeStr == "BLOCK_2_14_8") return BlockType::BLOCK_2_14_8;
    if (typeStr == "BLOCK_2_14_9") return BlockType::BLOCK_2_14_9;
    if (typeStr == "BLOCK_2_14_10") return BlockType::BLOCK_2_14_10;
    if (typeStr == "BLOCK_2_14_11") return BlockType::BLOCK_2_14_11;
    if (typeStr == "BLOCK_2_14_12") return BlockType::BLOCK_2_14_12;
    if (typeStr == "BLOCK_2_14_13") return BlockType::BLOCK_2_14_13;
    if (typeStr == "BLOCK_2_14_14") return BlockType::BLOCK_2_14_14;
    if (typeStr == "BLOCK_2_14_15") return BlockType::BLOCK_2_14_15;
    if (typeStr == "BLOCK_2_14_16") return BlockType::BLOCK_2_14_16;
    if (typeStr == "BLOCK_2_15_1") return BlockType::BLOCK_2_15_1;
    if (typeStr == "BLOCK_2_15_2") return BlockType::BLOCK_2_15_2;
    if (typeStr == "BLOCK_2_15_3") return BlockType::BLOCK_2_15_3;
    if (typeStr == "BLOCK_2_15_4") return BlockType::BLOCK_2_15_4;
    if (typeStr == "BLOCK_2_15_5") return BlockType::BLOCK_2_15_5;
    if (typeStr == "BLOCK_2_15_6") return BlockType::BLOCK_2_15_6;
    if (typeStr == "BLOCK_2_15_7") return BlockType::BLOCK_2_15_7;
    if (typeStr == "BLOCK_2_15_8") return BlockType::BLOCK_2_15_8;
    if (typeStr == "BLOCK_2_15_9") return BlockType::BLOCK_2_15_9;
    if (typeStr == "BLOCK_2_15_10") return BlockType::BLOCK_2_15_10;
    if (typeStr == "BLOCK_2_15_11") return BlockType::BLOCK_2_15_11;
    if (typeStr == "BLOCK_2_15_12") return BlockType::BLOCK_2_15_12;
    if (typeStr == "BLOCK_2_15_13") return BlockType::BLOCK_2_15_13;
    if (typeStr == "BLOCK_2_15_14") return BlockType::BLOCK_2_15_14;
    if (typeStr == "BLOCK_2_15_15") return BlockType::BLOCK_2_15_15;
    if (typeStr == "BLOCK_2_15_16") return BlockType::BLOCK_2_15_16;
    if (typeStr == "BLOCK_2_16_1") return BlockType::BLOCK_2_16_1;
    if (typeStr == "BLOCK_2_16_2") return BlockType::BLOCK_2_16_2;
    if (typeStr == "BLOCK_2_16_3") return BlockType::BLOCK_2_16_3;
    if (typeStr == "BLOCK_2_16_4") return BlockType::BLOCK_2_16_4;
    if (typeStr == "BLOCK_2_16_5") return BlockType::BLOCK_2_16_5;
    if (typeStr == "BLOCK_2_16_6") return BlockType::BLOCK_2_16_6;
    if (typeStr == "BLOCK_2_16_7") return BlockType::BLOCK_2_16_7;
    if (typeStr == "BLOCK_2_16_8") return BlockType::BLOCK_2_16_8;
    if (typeStr == "BLOCK_2_16_9") return BlockType::BLOCK_2_16_9;
    if (typeStr == "BLOCK_2_16_10") return BlockType::BLOCK_2_16_10;
    if (typeStr == "BLOCK_2_16_11") return BlockType::BLOCK_2_16_11;
    if (typeStr == "BLOCK_2_16_12") return BlockType::BLOCK_2_16_12;
    if (typeStr == "BLOCK_2_16_13") return BlockType::BLOCK_2_16_13;
    if (typeStr == "BLOCK_2_16_14") return BlockType::BLOCK_2_16_14;
    if (typeStr == "BLOCK_2_16_15") return BlockType::BLOCK_2_16_15;
    if (typeStr == "BLOCK_2_16_16") return BlockType::BLOCK_2_16_16;





    if (typeStr == "BLOCK_3_1_2") return BlockType::BLOCK_3_1_2;
    if (typeStr == "BLOCK_3_1_3") return BlockType::BLOCK_3_1_3;
    if (typeStr == "BLOCK_3_1_12") return BlockType::BLOCK_3_1_12;
    if (typeStr == "BLOCK_3_1_13") return BlockType::BLOCK_3_1_13;
    if (typeStr == "BLOCK_3_1_14") return BlockType::BLOCK_3_1_14;
    if (typeStr == "BLOCK_3_1_15") return BlockType::BLOCK_3_1_15;
    if (typeStr == "BLOCK_3_1_16") return BlockType::BLOCK_3_1_16;
    if (typeStr == "BLOCK_3_2_7") return BlockType::BLOCK_3_2_7;
    if (typeStr == "BLOCK_3_2_8") return BlockType::BLOCK_3_2_8;
    if (typeStr == "BLOCK_3_2_12") return BlockType::BLOCK_3_2_12;
    if (typeStr == "BLOCK_3_2_13") return BlockType::BLOCK_3_2_13;
    if (typeStr == "BLOCK_3_2_14") return BlockType::BLOCK_3_2_14;
    if (typeStr == "BLOCK_3_2_15") return BlockType::BLOCK_3_2_15;
    if (typeStr == "BLOCK_3_2_16") return BlockType::BLOCK_3_2_16;
    if (typeStr == "BLOCK_3_3_4") return BlockType::BLOCK_3_3_4;
    if (typeStr == "BLOCK_3_3_5") return BlockType::BLOCK_3_3_5;
    if (typeStr == "BLOCK_3_3_6") return BlockType::BLOCK_3_3_6;
    if (typeStr == "BLOCK_3_3_7") return BlockType::BLOCK_3_3_7;
    if (typeStr == "BLOCK_3_3_8") return BlockType::BLOCK_3_3_8;
    if (typeStr == "BLOCK_3_3_15") return BlockType::BLOCK_3_3_15;
    if (typeStr == "BLOCK_3_3_16") return BlockType::BLOCK_3_3_16;
    if (typeStr == "BLOCK_3_4_4") return BlockType::BLOCK_3_4_4;
    if (typeStr == "BLOCK_3_4_5") return BlockType::BLOCK_3_4_5;
    if (typeStr == "BLOCK_3_4_6") return BlockType::BLOCK_3_4_6;
    if (typeStr == "BLOCK_3_4_7") return BlockType::BLOCK_3_4_7;
    if (typeStr == "BLOCK_3_4_11") return BlockType::BLOCK_3_4_11;
    if (typeStr == "BLOCK_3_4_12") return BlockType::BLOCK_3_4_12;
    if (typeStr == "BLOCK_3_4_13") return BlockType::BLOCK_3_4_13;
    if (typeStr == "BLOCK_3_4_14") return BlockType::BLOCK_3_4_14;
    if (typeStr == "BLOCK_3_4_15") return BlockType::BLOCK_3_4_15;
    if (typeStr == "BLOCK_3_4_16") return BlockType::BLOCK_3_4_16;
    if (typeStr == "BLOCK_3_5_4") return BlockType::BLOCK_3_5_4;
    if (typeStr == "BLOCK_3_5_5") return BlockType::BLOCK_3_5_5;
    if (typeStr == "BLOCK_3_5_6") return BlockType::BLOCK_3_5_6;
    if (typeStr == "BLOCK_3_5_8") return BlockType::BLOCK_3_5_8;
    if (typeStr == "BLOCK_3_5_9") return BlockType::BLOCK_3_5_9;
    if (typeStr == "BLOCK_3_5_10") return BlockType::BLOCK_3_5_10;
    if (typeStr == "BLOCK_3_5_11") return BlockType::BLOCK_3_5_11;
    if (typeStr == "BLOCK_3_5_12") return BlockType::BLOCK_3_5_12;
    if (typeStr == "BLOCK_3_5_13") return BlockType::BLOCK_3_5_13;
    if (typeStr == "BLOCK_3_5_14") return BlockType::BLOCK_3_5_14;
    if (typeStr == "BLOCK_3_5_15") return BlockType::BLOCK_3_5_15;
    if (typeStr == "BLOCK_3_5_16") return BlockType::BLOCK_3_5_16;
    if (typeStr == "BLOCK_3_6_5") return BlockType::BLOCK_3_6_5;
    if (typeStr == "BLOCK_3_6_6") return BlockType::BLOCK_3_6_6;
    if (typeStr == "BLOCK_3_7_1") return BlockType::BLOCK_3_7_1;
    if (typeStr == "BLOCK_3_7_2") return BlockType::BLOCK_3_7_2;
    if (typeStr == "BLOCK_3_7_3") return BlockType::BLOCK_3_7_3;
    if (typeStr == "BLOCK_3_7_4") return BlockType::BLOCK_3_7_4;
    if (typeStr == "BLOCK_3_7_5") return BlockType::BLOCK_3_7_5;
    if (typeStr == "BLOCK_3_7_6") return BlockType::BLOCK_3_7_6;
    if (typeStr == "BLOCK_3_7_7") return BlockType::BLOCK_3_7_7;
    if (typeStr == "BLOCK_3_7_11") return BlockType::BLOCK_3_7_11;
    if (typeStr == "BLOCK_3_7_12") return BlockType::BLOCK_3_7_12;
    if (typeStr == "BLOCK_3_7_13") return BlockType::BLOCK_3_7_13;
    if (typeStr == "BLOCK_3_7_14") return BlockType::BLOCK_3_7_14;
    if (typeStr == "BLOCK_3_7_15") return BlockType::BLOCK_3_7_15;
    if (typeStr == "BLOCK_3_7_16") return BlockType::BLOCK_3_7_16;
    if (typeStr == "BLOCK_3_13_1") return BlockType::BLOCK_3_13_1;
    if (typeStr == "BLOCK_3_13_2") return BlockType::BLOCK_3_13_2;
    if (typeStr == "BLOCK_3_13_3") return BlockType::BLOCK_3_13_3;
    if (typeStr == "BLOCK_3_13_4") return BlockType::BLOCK_3_13_4;
    if (typeStr == "BLOCK_3_13_5") return BlockType::BLOCK_3_13_5;
    if (typeStr == "BLOCK_3_13_6") return BlockType::BLOCK_3_13_6;
    if (typeStr == "BLOCK_3_13_7") return BlockType::BLOCK_3_13_7;
    if (typeStr == "BLOCK_3_13_8") return BlockType::BLOCK_3_13_8;
    if (typeStr == "BLOCK_3_13_9") return BlockType::BLOCK_3_13_9;
    if (typeStr == "BLOCK_3_13_10") return BlockType::BLOCK_3_13_10;
    if (typeStr == "BLOCK_3_13_11") return BlockType::BLOCK_3_13_11;
    if (typeStr == "BLOCK_3_13_12") return BlockType::BLOCK_3_13_12;
    if (typeStr == "BLOCK_3_13_13") return BlockType::BLOCK_3_13_13;
    if (typeStr == "BLOCK_3_13_14") return BlockType::BLOCK_3_13_14;
    if (typeStr == "BLOCK_3_13_15") return BlockType::BLOCK_3_13_15;
    if (typeStr == "BLOCK_3_13_16") return BlockType::BLOCK_3_13_16;
    if (typeStr == "BLOCK_3_14_1") return BlockType::BLOCK_3_14_1;
    if (typeStr == "BLOCK_3_14_2") return BlockType::BLOCK_3_14_2;
    if (typeStr == "BLOCK_3_14_3") return BlockType::BLOCK_3_14_3;
    if (typeStr == "BLOCK_3_14_4") return BlockType::BLOCK_3_14_4;
    if (typeStr == "BLOCK_3_14_5") return BlockType::BLOCK_3_14_5;
    if (typeStr == "BLOCK_3_14_6") return BlockType::BLOCK_3_14_6;
    if (typeStr == "BLOCK_3_14_7") return BlockType::BLOCK_3_14_7;
    if (typeStr == "BLOCK_3_14_8") return BlockType::BLOCK_3_14_8;
    if (typeStr == "BLOCK_3_14_9") return BlockType::BLOCK_3_14_9;
    if (typeStr == "BLOCK_3_14_10") return BlockType::BLOCK_3_14_10;
    if (typeStr == "BLOCK_3_14_11") return BlockType::BLOCK_3_14_11;
    if (typeStr == "BLOCK_3_14_12") return BlockType::BLOCK_3_14_12;
    if (typeStr == "BLOCK_3_14_13") return BlockType::BLOCK_3_14_13;
    if (typeStr == "BLOCK_3_14_14") return BlockType::BLOCK_3_14_14;
    if (typeStr == "BLOCK_3_14_15") return BlockType::BLOCK_3_14_15;
    if (typeStr == "BLOCK_3_14_16") return BlockType::BLOCK_3_14_16;
    if (typeStr == "BLOCK_3_15_1") return BlockType::BLOCK_3_15_1;
    if (typeStr == "BLOCK_3_15_2") return BlockType::BLOCK_3_15_2;
    if (typeStr == "BLOCK_3_15_3") return BlockType::BLOCK_3_15_3;
    if (typeStr == "BLOCK_3_15_4") return BlockType::BLOCK_3_15_4;
    if (typeStr == "BLOCK_3_15_5") return BlockType::BLOCK_3_15_5;
    if (typeStr == "BLOCK_3_15_6") return BlockType::BLOCK_3_15_6;
    if (typeStr == "BLOCK_3_15_7") return BlockType::BLOCK_3_15_7;
    if (typeStr == "BLOCK_3_15_8") return BlockType::BLOCK_3_15_8;
    if (typeStr == "BLOCK_3_15_9") return BlockType::BLOCK_3_15_9;
    if (typeStr == "BLOCK_3_15_10") return BlockType::BLOCK_3_15_10;
    if (typeStr == "BLOCK_3_15_11") return BlockType::BLOCK_3_15_11;
    if (typeStr == "BLOCK_3_15_12") return BlockType::BLOCK_3_15_12;
    if (typeStr == "BLOCK_3_15_13") return BlockType::BLOCK_3_15_13;
    if (typeStr == "BLOCK_3_15_14") return BlockType::BLOCK_3_15_14;
    if (typeStr == "BLOCK_3_15_15") return BlockType::BLOCK_3_15_15;
    if (typeStr == "BLOCK_3_15_16") return BlockType::BLOCK_3_15_16;
    if (typeStr == "BLOCK_3_16_1") return BlockType::BLOCK_3_16_1;
    if (typeStr == "BLOCK_3_16_2") return BlockType::BLOCK_3_16_2;
    if (typeStr == "BLOCK_3_16_3") return BlockType::BLOCK_3_16_3;
    if (typeStr == "BLOCK_3_16_4") return BlockType::BLOCK_3_16_4;
    if (typeStr == "BLOCK_3_16_5") return BlockType::BLOCK_3_16_5;
    if (typeStr == "BLOCK_3_16_6") return BlockType::BLOCK_3_16_6;
    if (typeStr == "BLOCK_3_16_7") return BlockType::BLOCK_3_16_7;
    if (typeStr == "BLOCK_3_16_8") return BlockType::BLOCK_3_16_8;
    if (typeStr == "BLOCK_3_16_9") return BlockType::BLOCK_3_16_9;
    if (typeStr == "BLOCK_3_16_10") return BlockType::BLOCK_3_16_10;
    if (typeStr == "BLOCK_3_16_11") return BlockType::BLOCK_3_16_11;
    if (typeStr == "BLOCK_3_16_12") return BlockType::BLOCK_3_16_12;
    if (typeStr == "BLOCK_3_16_13") return BlockType::BLOCK_3_16_13;
    if (typeStr == "BLOCK_3_16_14") return BlockType::BLOCK_3_16_14;
    if (typeStr == "BLOCK_3_16_15") return BlockType::BLOCK_3_16_15;
    if (typeStr == "BLOCK_3_16_16") return BlockType::BLOCK_3_16_16;





    if (typeStr == "BLOCK_4_1_2") return BlockType::BLOCK_4_1_2;
    if (typeStr == "BLOCK_4_1_3") return BlockType::BLOCK_4_1_3;
    if (typeStr == "BLOCK_4_1_12") return BlockType::BLOCK_4_1_12;
    if (typeStr == "BLOCK_4_1_13") return BlockType::BLOCK_4_1_13;
    if (typeStr == "BLOCK_4_1_14") return BlockType::BLOCK_4_1_14;
    if (typeStr == "BLOCK_4_1_15") return BlockType::BLOCK_4_1_15;
    if (typeStr == "BLOCK_4_1_16") return BlockType::BLOCK_4_1_16;
    if (typeStr == "BLOCK_4_2_7") return BlockType::BLOCK_4_2_7;
    if (typeStr == "BLOCK_4_2_8") return BlockType::BLOCK_4_2_8;
    if (typeStr == "BLOCK_4_2_12") return BlockType::BLOCK_4_2_12;
    if (typeStr == "BLOCK_4_2_13") return BlockType::BLOCK_4_2_13;
    if (typeStr == "BLOCK_4_2_14") return BlockType::BLOCK_4_2_14;
    if (typeStr == "BLOCK_4_2_15") return BlockType::BLOCK_4_2_15;
    if (typeStr == "BLOCK_4_2_16") return BlockType::BLOCK_4_2_16;
    if (typeStr == "BLOCK_4_3_4") return BlockType::BLOCK_4_3_4;
    if (typeStr == "BLOCK_4_3_5") return BlockType::BLOCK_4_3_5;
    if (typeStr == "BLOCK_4_3_6") return BlockType::BLOCK_4_3_6;
    if (typeStr == "BLOCK_4_3_7") return BlockType::BLOCK_4_3_7;
    if (typeStr == "BLOCK_4_3_8") return BlockType::BLOCK_4_3_8;
    if (typeStr == "BLOCK_4_3_15") return BlockType::BLOCK_4_3_15;
    if (typeStr == "BLOCK_4_3_16") return BlockType::BLOCK_4_3_16;
    if (typeStr == "BLOCK_4_4_4") return BlockType::BLOCK_4_4_4;
    if (typeStr == "BLOCK_4_4_5") return BlockType::BLOCK_4_4_5;
    if (typeStr == "BLOCK_4_4_6") return BlockType::BLOCK_4_4_6;
    if (typeStr == "BLOCK_4_4_7") return BlockType::BLOCK_4_4_7;
    if (typeStr == "BLOCK_4_4_11") return BlockType::BLOCK_4_4_11;
    if (typeStr == "BLOCK_4_4_12") return BlockType::BLOCK_4_4_12;
    if (typeStr == "BLOCK_4_4_13") return BlockType::BLOCK_4_4_13;
    if (typeStr == "BLOCK_4_4_14") return BlockType::BLOCK_4_4_14;
    if (typeStr == "BLOCK_4_4_15") return BlockType::BLOCK_4_4_15;
    if (typeStr == "BLOCK_4_4_16") return BlockType::BLOCK_4_4_16;
    if (typeStr == "BLOCK_4_5_4") return BlockType::BLOCK_4_5_4;
    if (typeStr == "BLOCK_4_5_5") return BlockType::BLOCK_4_5_5;
    if (typeStr == "BLOCK_4_5_6") return BlockType::BLOCK_4_5_6;
    if (typeStr == "BLOCK_4_5_8") return BlockType::BLOCK_4_5_8;
    if (typeStr == "BLOCK_4_5_9") return BlockType::BLOCK_4_5_9;
    if (typeStr == "BLOCK_4_5_10") return BlockType::BLOCK_4_5_10;
    if (typeStr == "BLOCK_4_5_11") return BlockType::BLOCK_4_5_11;
    if (typeStr == "BLOCK_4_5_12") return BlockType::BLOCK_4_5_12;
    if (typeStr == "BLOCK_4_5_13") return BlockType::BLOCK_4_5_13;
    if (typeStr == "BLOCK_4_5_14") return BlockType::BLOCK_4_5_14;
    if (typeStr == "BLOCK_4_5_15") return BlockType::BLOCK_4_5_15;
    if (typeStr == "BLOCK_4_5_16") return BlockType::BLOCK_4_5_16;
    if (typeStr == "BLOCK_4_6_5") return BlockType::BLOCK_4_6_5;
    if (typeStr == "BLOCK_4_6_6") return BlockType::BLOCK_4_6_6;
    if (typeStr == "BLOCK_4_7_1") return BlockType::BLOCK_4_7_1;
    if (typeStr == "BLOCK_4_7_2") return BlockType::BLOCK_4_7_2;
    if (typeStr == "BLOCK_4_7_3") return BlockType::BLOCK_4_7_3;
    if (typeStr == "BLOCK_4_7_4") return BlockType::BLOCK_4_7_4;
    if (typeStr == "BLOCK_4_7_5") return BlockType::BLOCK_4_7_5;
    if (typeStr == "BLOCK_4_7_6") return BlockType::BLOCK_4_7_6;
    if (typeStr == "BLOCK_4_7_7") return BlockType::BLOCK_4_7_7;
    if (typeStr == "BLOCK_4_7_11") return BlockType::BLOCK_4_7_11;
    if (typeStr == "BLOCK_4_7_12") return BlockType::BLOCK_4_7_12;
    if (typeStr == "BLOCK_4_7_13") return BlockType::BLOCK_4_7_13;
    if (typeStr == "BLOCK_4_7_14") return BlockType::BLOCK_4_7_14;
    if (typeStr == "BLOCK_4_7_15") return BlockType::BLOCK_4_7_15;
    if (typeStr == "BLOCK_4_7_16") return BlockType::BLOCK_4_7_16;
    if (typeStr == "BLOCK_4_13_1") return BlockType::BLOCK_4_13_1;
    if (typeStr == "BLOCK_4_13_2") return BlockType::BLOCK_4_13_2;
    if (typeStr == "BLOCK_4_13_3") return BlockType::BLOCK_4_13_3;
    if (typeStr == "BLOCK_4_13_4") return BlockType::BLOCK_4_13_4;
    if (typeStr == "BLOCK_4_13_5") return BlockType::BLOCK_4_13_5;
    if (typeStr == "BLOCK_4_13_6") return BlockType::BLOCK_4_13_6;
    if (typeStr == "BLOCK_4_13_7") return BlockType::BLOCK_4_13_7;
    if (typeStr == "BLOCK_4_13_8") return BlockType::BLOCK_4_13_8;
    if (typeStr == "BLOCK_4_13_9") return BlockType::BLOCK_4_13_9;
    if (typeStr == "BLOCK_4_13_10") return BlockType::BLOCK_4_13_10;
    if (typeStr == "BLOCK_4_13_11") return BlockType::BLOCK_4_13_11;
    if (typeStr == "BLOCK_4_13_12") return BlockType::BLOCK_4_13_12;
    if (typeStr == "BLOCK_4_13_13") return BlockType::BLOCK_4_13_13;
    if (typeStr == "BLOCK_4_13_14") return BlockType::BLOCK_4_13_14;
    if (typeStr == "BLOCK_4_13_15") return BlockType::BLOCK_4_13_15;
    if (typeStr == "BLOCK_4_13_16") return BlockType::BLOCK_4_13_16;
    if (typeStr == "BLOCK_4_14_1") return BlockType::BLOCK_4_14_1;
    if (typeStr == "BLOCK_4_14_2") return BlockType::BLOCK_4_14_2;
    if (typeStr == "BLOCK_4_14_3") return BlockType::BLOCK_4_14_3;
    if (typeStr == "BLOCK_4_14_4") return BlockType::BLOCK_4_14_4;
    if (typeStr == "BLOCK_4_14_5") return BlockType::BLOCK_4_14_5;
    if (typeStr == "BLOCK_4_14_6") return BlockType::BLOCK_4_14_6;
    if (typeStr == "BLOCK_4_14_7") return BlockType::BLOCK_4_14_7;
    if (typeStr == "BLOCK_4_14_8") return BlockType::BLOCK_4_14_8;
    if (typeStr == "BLOCK_4_14_9") return BlockType::BLOCK_4_14_9;
    if (typeStr == "BLOCK_4_14_10") return BlockType::BLOCK_4_14_10;
    if (typeStr == "BLOCK_4_14_11") return BlockType::BLOCK_4_14_11;
    if (typeStr == "BLOCK_4_14_12") return BlockType::BLOCK_4_14_12;
    if (typeStr == "BLOCK_4_14_13") return BlockType::BLOCK_4_14_13;
    if (typeStr == "BLOCK_4_14_14") return BlockType::BLOCK_4_14_14;
    if (typeStr == "BLOCK_4_14_15") return BlockType::BLOCK_4_14_15;
    if (typeStr == "BLOCK_4_14_16") return BlockType::BLOCK_4_14_16;
    if (typeStr == "BLOCK_4_15_1") return BlockType::BLOCK_4_15_1;
    if (typeStr == "BLOCK_4_15_2") return BlockType::BLOCK_4_15_2;
    if (typeStr == "BLOCK_4_15_3") return BlockType::BLOCK_4_15_3;
    if (typeStr == "BLOCK_4_15_4") return BlockType::BLOCK_4_15_4;
    if (typeStr == "BLOCK_4_15_5") return BlockType::BLOCK_4_15_5;
    if (typeStr == "BLOCK_4_15_6") return BlockType::BLOCK_4_15_6;
    if (typeStr == "BLOCK_4_15_7") return BlockType::BLOCK_4_15_7;
    if (typeStr == "BLOCK_4_15_8") return BlockType::BLOCK_4_15_8;
    if (typeStr == "BLOCK_4_15_9") return BlockType::BLOCK_4_15_9;
    if (typeStr == "BLOCK_4_15_10") return BlockType::BLOCK_4_15_10;
    if (typeStr == "BLOCK_4_15_11") return BlockType::BLOCK_4_15_11;
    if (typeStr == "BLOCK_4_15_12") return BlockType::BLOCK_4_15_12;
    if (typeStr == "BLOCK_4_15_13") return BlockType::BLOCK_4_15_13;
    if (typeStr == "BLOCK_4_15_14") return BlockType::BLOCK_4_15_14;
    if (typeStr == "BLOCK_4_15_15") return BlockType::BLOCK_4_15_15;
    if (typeStr == "BLOCK_4_15_16") return BlockType::BLOCK_4_15_16;
    if (typeStr == "BLOCK_4_16_1") return BlockType::BLOCK_4_16_1;
    if (typeStr == "BLOCK_4_16_2") return BlockType::BLOCK_4_16_2;
    if (typeStr == "BLOCK_4_16_3") return BlockType::BLOCK_4_16_3;
    if (typeStr == "BLOCK_4_16_4") return BlockType::BLOCK_4_16_4;
    if (typeStr == "BLOCK_4_16_5") return BlockType::BLOCK_4_16_5;
    if (typeStr == "BLOCK_4_16_6") return BlockType::BLOCK_4_16_6;
    if (typeStr == "BLOCK_4_16_7") return BlockType::BLOCK_4_16_7;
    if (typeStr == "BLOCK_4_16_8") return BlockType::BLOCK_4_16_8;
    if (typeStr == "BLOCK_4_16_9") return BlockType::BLOCK_4_16_9;
    if (typeStr == "BLOCK_4_16_10") return BlockType::BLOCK_4_16_10;
    if (typeStr == "BLOCK_4_16_11") return BlockType::BLOCK_4_16_11;
    if (typeStr == "BLOCK_4_16_12") return BlockType::BLOCK_4_16_12;
    if (typeStr == "BLOCK_4_16_13") return BlockType::BLOCK_4_16_13;
    if (typeStr == "BLOCK_4_16_14") return BlockType::BLOCK_4_16_14;
    if (typeStr == "BLOCK_4_16_15") return BlockType::BLOCK_4_16_15;
    if (typeStr == "BLOCK_4_16_16") return BlockType::BLOCK_4_16_16;

    if (typeStr == "INVISIBLE") return BlockType::INVISIBLE;
    if (typeStr == "PIPE") return BlockType::PIPE;
    if (typeStr == "PLATFORM") return BlockType::PLATFORM;
    if (typeStr == "QUESTION") return BlockType::QUESTION;

	if (typeStr == "GREEN_KOOPA") return EnemyType::GREEN_KOOPA;
	if (typeStr == "RED_KOOPA") return EnemyType::RED_KOOPA;
    if (typeStr == "GOOMBA") return EnemyType::GOOMBA;
    if (typeStr == "DRY_BOWSER") return EnemyType::DRY_BOWSER;
    if (typeStr == "BOO") return EnemyType::BOO;
    if (typeStr == "BOB_OMBS") return EnemyType::BOB_OMBS;
    if (typeStr == "LASER_MECHA_KOOPA") return EnemyType::LASER_MECHA_KOOPA;

    if (typeStr == "SPRING") return InteractiveType::SPRING;
    if (typeStr == "MOVING_PLATFORM") return InteractiveType::MOVING_PLATFORM;
    if (typeStr == "FIRE_BAR_BASE") return InteractiveType::FIRE_BAR_BASE;
    if (typeStr == "TORCH") return BackGroundObjectType::TORCH;
    if (typeStr == "COIN") return ItemType::COIN;
    if (typeStr == "FIRE_FLOWER") return ItemType::FIRE_FLOWER;
    if (typeStr == "MUSHROOM") return ItemType::MUSHROOM;
    if (typeStr == "ONE_UP") return ItemType::ONE_UP;
    if (typeStr == "STAR") return ItemType::STAR;

    return BlockType::BLOCK_1_1_2;
}
bool LevelEditor::isBlock(std::pair<int, int> coord)
{
    auto it = gridBlocks.find(coord);
    if (it == gridBlocks.end()) return false;

    std::stack<std::shared_ptr<Object>> copy = it->second;
    while (!copy.empty()) {
        std::shared_ptr<Object> obj = copy.top();
        copy.pop();
        if (obj && (obj->getObjectCategory()==ObjectCategory::BLOCK)) {
            return true;
        }
    }
    return false;
}

std::shared_ptr<Object> LevelEditor::findQuestionBlock(Vector2 gridPos) {
    bool found = false;
    std::shared_ptr<Object> res = nullptr;
    auto key = std::make_pair((int)gridPos.x, (int)gridPos.y);
    auto it = gridBlocks.find(key);
    if (it != gridBlocks.end() && !it->second.empty()) {
        std::stack<std::shared_ptr<Object>> tempStack;
        while (!it->second.empty()) {
            std::shared_ptr<Object> topObj = it->second.top();
            std::shared_ptr<Block> obj = std::dynamic_pointer_cast<Block>(topObj);
            it->second.pop();

            if (obj && (obj->getType() == BlockType::BLOCK_1_1_3 || obj->getType() == BlockType::BLOCK_2_1_3 
            || obj->getType() == BlockType::BLOCK_3_1_3 || obj->getType() == BlockType::BLOCK_4_1_3)) {
                res = topObj;
                found = true;
            }
            
            tempStack.push(topObj);
            if (found) break;
        }

        while (!tempStack.empty()) {
            it->second.push(tempStack.top());
            tempStack.pop();
        }
    }
    return res;
}
