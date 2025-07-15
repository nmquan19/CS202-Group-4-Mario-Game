#include <memory>
#include <utility>
#include <raylib.h>
#include <type_traits>
#include <variant>
#include <iostream>
#include "../../include/System/LevelEditor.h"
#include "../../include/System/PhysicsManager.h"
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/System/Grid.h"
#include "../../include/System/Interface.h"
#include "../../include/Enemy/Enemy.h"

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
            if (it.second.top()) {
                it.second.top()->setActive(false);
                PhysicsManager::getInstance().markForDeletion(it.second.top().get());
            }
            it.second.pop();
        }
    }
    gridBlocks.clear();
}
void LevelEditor::update() {
    if (editMode) {
        palette.handleSelection();

        if (!clearingLevel) handleMouseInput();

        if (IsKeyPressed(KEY_F7)) {
            saveLevel("test_level.json");
        }
        if (IsKeyPressed(KEY_F8)) {
            loadLevel("test_level.json");
        }
        if (IsKeyPressed(KEY_F9)) {
            clearLevel();
        }

        if (clearingLevel && PhysicsManager::getInstance().deletionCompleted()) {
            gridBlocks.clear();
            clearingLevel = false;
            std::cout << "Level cleared successfully!\n";

            if (loadingLevel && !pendingLoadFile.empty()) {
                performLoad(pendingLoadFile);
                pendingLoadFile = "";
                loadingLevel = false;
            }
        }
    }
    else if (!clearingLevel) {
        for (auto& it : gridBlocks) {
            Object* obj = it.second.top().get();
            if (!obj) continue;

            Enemy* enemy = dynamic_cast<Enemy*>(obj);
            bool shouldUpdate = false;

            if (enemy) {
                shouldUpdate = enemy->isAlive();
            }
            else {
                shouldUpdate = obj->isActive();
            }

            if (shouldUpdate) {
                IUpdatable* updatable = dynamic_cast<IUpdatable*>(obj);
                if (updatable){
                    updatable->update(GetFrameTime());
                }
            }
        }
    }
}

void LevelEditor::draw() {
    if (editMode) {
        GridSystem::drawGrid(GetScreenWidth(), GetScreenHeight());

        Vector2 mousePos = GetMousePosition();
        Vector2 gridCoord = GridSystem::getGridCoord(mousePos);
        Rectangle highlightRect = GridSystem::getGridRect(gridCoord);
        DrawRectangleLinesEx(highlightRect, 3, GREEN);
        palette.drawPalette();

        DrawText("F7: Save Level", 20, 100, 16, DARKBLUE);
        DrawText("F8: Load Level", 20, 120, 16, DARKBLUE);
        DrawText("F9: Clear Level", 20, 140, 16, DARKBLUE);
        //DrawText(("Number of objects in level editor: " + std::to_string(gridBlocks.size())).c_str(), 20, 160, 16, DARKBLUE);
    }

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
    
    const char* modeText = editMode ? "EDIT MODE" : "PLAY MODE";
    DrawText(modeText, 20, 70, 20, editMode ? GREEN : BLUE);
}

void LevelEditor::handleMouseInput() {
    Vector2 mousePos = GetMousePosition();
    Vector2 gridCoord = GridSystem::getGridCoord(mousePos);

    bool clickingOnPalette = CheckCollisionPointRec(mousePos, palette.getPaletteRect());
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !clickingOnPalette) {
        placeObject(palette.getSelectedType(), gridCoord);
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        removeObject(gridCoord);
    }
}

void LevelEditor::placeObject(ObjectType type, Vector2 gridCoord) {
    auto key = std::make_pair((int)gridCoord.x, (int)gridCoord.y);
    std::visit([&](auto&& actualType) {
        using T = std::decay_t<decltype(actualType)>;
        if constexpr (std::is_same_v<T, BlockType>) {
            auto newBlock = ObjectFactory::createBlock(actualType, gridCoord);
            if (newBlock) {
                PhysicsManager::getInstance().addObject(newBlock.get());
                gridBlocks[key].push(std::move(newBlock));
            }
        }
        else if constexpr (std::is_same_v<T, EnemyType>) {
            auto newEnemy = ObjectFactory::createEnemy(actualType,GridSystem::getWorldPosition(gridCoord),{1,1});
            if (newEnemy) {
                PhysicsManager::getInstance().addObject(newEnemy.get());
                gridBlocks[key].push(std::move(newEnemy));
            }
        }
    }, type);
}
#include <iostream>
#include <string>


void LevelEditor::removeObject(Vector2 gridCoord) {
    auto key = std::make_pair((int)gridCoord.x, (int)gridCoord.y);
    auto it = gridBlocks.find(key);
    if (it != gridBlocks.end() && !it->second.empty()) {
        if (it->second.top()) {
            it->second.top()->setActive(false);
            PhysicsManager::getInstance().markForDeletion(it->second.top().get());
            it->second.top().release();
            it->second.pop();
        }
        if (it->second.empty()) {
            gridBlocks.erase(it);
        }
    }
}

void LevelEditor::toggleEditMode() {
    editMode = !editMode;

}

bool LevelEditor::isInEditMode() const {
    return editMode;
}

std::string LevelEditor::objectTypeToString(const ObjectType& type) {
    if (std::holds_alternative<BlockType>(type)) {
        BlockType blockType = std::get<BlockType>(type);
        switch (blockType) {
            case BlockType::GROUND: return "GROUND";
            case BlockType::BRICK: return "BRICK";
            default: return "GROUND";
        }
    }
    else if (std::holds_alternative<EnemyType>(type)) {
        EnemyType enemyType = std::get<EnemyType>(type);
        switch (enemyType) {
            case EnemyType::GOOMBA: return "GOOMBA";
            case EnemyType::GREEN_KOOPA: return "GREEN_KOOPA";
			case EnemyType::RED_KOOPA: return "RED_KOOPA";
            default: return "GOOMBA";
        }
    }
    return "GROUND";
}

ObjectType LevelEditor::stringToObjectType(const std::string& typeStr) {
    if (typeStr == "GROUND") return BlockType::GROUND;
    if (typeStr == "BRICK") return BlockType::BRICK;

    if (typeStr == "GOOMBA") return EnemyType::GOOMBA;
    if (typeStr == "GREEN_KOOPA") return EnemyType::GREEN_KOOPA;
    if (typeStr == "RED_KOOPA") return EnemyType::RED_KOOPA;

    return BlockType::GROUND;
}

void LevelEditor::saveLevel(const std::string& filename) {
    //    std::ofstream file(filename);
    //    if (!file.is_open()) {
    //        std::cout << "Failed to create save file: " << filename << "\n";
    //        return;
    //    }
    //
    //    file << "{\n";
    //    file << " \"level_name\": \"" << filename << "\",\n";
    //    file << " \"objects\": [\n";
    //
    //    bool first = true;
    //    for (const auto& pair : gridBlocks) {
    //        if (!first) {
    //            file << ",\n";
    //        }
    //        first = false;
    //
    //        int x = pair.first.first;
    //        int y = pair.first.second;
    //
    //        std::string objectType = "GROUND";
    //
    //        ObjectCategory category = pair.second->getObjectCategory();
    //
    //        if (category == ObjectCategory::BLOCK) {
    //            Block* blockObj = dynamic_cast<Block*>(pair.second.get());
    //            objectType = objectTypeToString(blockObj->getType());
    //        }
    //        else if (category == ObjectCategory::ENEMY) {
    //            Enemy* enemyObj = dynamic_cast<Enemy*>(pair.second.get());
    //            objectType = objectTypeToString(enemyObj->getType());
    //        }
    //
    //        file << "    {\n";
    //        file << "      \"x\": " << x << ",\n";
    //        file << "      \"y\": " << y << ",\n";
    //        file << "      \"type\": \"" << objectType << "\"\n";
    //        file << "    }";
    //    }
    //
    //    file << "\n  ]\n";
    //    file << "}\n";
    //
    //    file.close();
    //    std::cout << "Level saved: " << filename << std::endl;
}

void LevelEditor::loadLevel(const std::string & filename) {
    //if (clearingLevel) {
    //    // Already clearing, queue the load
    //    pendingLoadFile = filename;
    //    loadingLevel = true;
    //    std::cout << "Queuing load for: " << filename << " (waiting for clear to complete)\n";
    //    return;
    //}

    //if (!gridBlocks.empty()) {
    //    clearLevel();
    //    pendingLoadFile = filename;
    //    loadingLevel = true;
    //    std::cout << "Clearing level before loading: " << filename << "\n";
    //    return;
    //}

    //performLoad(filename);
}

void LevelEditor::performLoad(const std::string& filename) {
    //std::ifstream file(filename);
    //if (!file.is_open()) {
    //    std::cout << "Failed to open level file: " << filename << "\n";
    //    return;
    //}

    //std::string line;
    //std::string content;
    //while (std::getline(file, line)) {
    //    content += line + "\n";
    //}
    //file.close();

    //std::size_t objectsPos = content.find("\"objects\":");
    //if (objectsPos == std::string::npos) {
    //    std::cout << "Invalid level file format" << "\n";
    //    return;
    //}

    //// Extract objects array content
    //std::size_t arrayStart = content.find("[", objectsPos);
    //std::size_t arrayEnd = content.find("]", arrayStart);

    //if (arrayStart == std::string::npos || arrayEnd == std::string::npos) {
    //    std::cout << "Invalid objects array format" << std::endl;
    //    return;
    //}

    //std::string objectsStr = content.substr(arrayStart + 1, arrayEnd - arrayStart - 1);

    //// Parse individual objects
    //std::size_t pos = 0;
    //while (pos < objectsStr.length()) {
    //    std::size_t objStart = objectsStr.find("{", pos);
    //    std::size_t objEnd = objectsStr.find("}", objStart);

    //    if (objStart == std::string::npos || objEnd == std::string::npos) {
    //        break;
    //    }

    //    std::string objStr = objectsStr.substr(objStart + 1, objEnd - objStart - 1);

    //    // Parse x, y, type
    //    int x = 0, y = 0;
    //    std::string type = "GROUND";

    //    // Extract x
    //    std::size_t xPos = objStr.find("\"x\":");
    //    if (xPos != std::string::npos) {
    //        std::size_t xValStart = objStr.find(":", xPos) + 1;
    //        std::size_t xValEnd = objStr.find(",", xValStart);
    //        std::string xStr = objStr.substr(xValStart, xValEnd - xValStart);
    //        // Remove spaces
    //        xStr.erase(std::remove_if(xStr.begin(), xStr.end(), ::isspace), xStr.end());
    //        x = std::stoi(xStr);
    //    }

    //    // Extract y
    //    std::size_t yPos = objStr.find("\"y\":");
    //    if (yPos != std::string::npos) {
    //        std::size_t yValStart = objStr.find(":", yPos) + 1;
    //        std::size_t yValEnd = objStr.find(",", yValStart);
    //        std::string yStr = objStr.substr(yValStart, yValEnd - yValStart);
    //        // Remove spaces
    //        yStr.erase(std::remove_if(yStr.begin(), yStr.end(), ::isspace), yStr.end());
    //        y = std::stoi(yStr);
    //    }

    //    // Extract type
    //    std::size_t typePos = objStr.find("\"type\":");
    //    if (typePos != std::string::npos) {
    //        std::size_t typeValStart = objStr.find("\"", typePos + 7) + 1;
    //        std::size_t typeValEnd = objStr.find("\"", typeValStart);
    //        type = objStr.substr(typeValStart, typeValEnd - typeValStart);
    //    }

    //    // Place the object
    //    Vector2 gridPos = { (float)x, (float)y };
    //    ObjectType objType = stringToObjectType(type);
    //    placeObject(objType, gridPos);

    //    pos = objEnd + 1;
    //}

    //std::cout << "Level loaded: " << filename << std::endl;
}

void LevelEditor::clearLevel() {
    if (clearingLevel) {
        std::cout << "Already clearing level, please wait...\n";
        return;
    }
    clearingLevel = true;
    for (auto& pair : gridBlocks) {
        while (!pair.second.empty()) {
            if (pair.second.top()) {
                pair.second.top()->setActive(false);
                PhysicsManager::getInstance().markForDeletion(pair.second.top().get());
            }
            pair.second.pop();
        }
    }
}