#include "../../include/System/LevelEditor.h"
#include "../../include/System/PhysicsManager.h"
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/System/Grid.h"
#include <memory>
#include <utility>
#include "../../include/System/Interface.h"
#include <raylib.h>
#include <type_traits>
#include <variant>
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
        if (it.second) {
            PhysicsManager::getInstance().markForDeletion(it.second.get());
        }
    }
    gridBlocks.clear();
}

void LevelEditor::update() {
    if (editMode) {
        palette.handleSelection();
        handleMouseInput();
    }
    else {
        for (auto& it : gridBlocks) {
            if (it.second && it.second->isActive()) {
                IUpdatable* updatable = dynamic_cast<IUpdatable*>(it.second.get());
                if (updatable) {
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
    }
    for (auto& it : gridBlocks) {
        if (it.second && it.second->isActive()) {
            it.second->draw();
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
    removeObject(gridCoord);
    std::visit([&](auto&& actualType) {
        using T = std::decay_t<decltype(actualType)>;
        if constexpr (std::is_same_v<T, BlockType>) {
            auto newBlock = ObjectFactory::createBlock(actualType, gridCoord);
            if (newBlock) {
                PhysicsManager::getInstance().addObject(newBlock.get());
                gridBlocks[{(int)gridCoord.x, (int)gridCoord.y}] = std::move(newBlock);
            }
        }
        else if constexpr (std::is_same_v<T, EnemyType>) {
            auto newEnemy = ObjectFactory::createEnemy(actualType,GridSystem::getWorldPosition(gridCoord), 5.0f);
            if (newEnemy) {
                PhysicsManager::getInstance().addObject(newEnemy.get());
                gridBlocks[{(int)gridCoord.x, (int)gridCoord.y}] = std::move(newEnemy);
            }
        }

        }, type);
}


void LevelEditor::removeObject(Vector2 gridCoord) {
    auto key = std::make_pair((int)gridCoord.x, (int)gridCoord.y);
    auto it = gridBlocks.find(key);
    if (it != gridBlocks.end()) {
        it->second.get()->setActive(false);
        it->second.release();
        gridBlocks.erase(it);
    }
}

void LevelEditor::toggleEditMode() {
    editMode = !editMode;

}

bool LevelEditor::isInEditMode() const {
    return editMode;
}

