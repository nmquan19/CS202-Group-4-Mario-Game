#include "../../include/System/LevelEditor.h"
#include "../../include/System/PhysicsManager.h"
#include "../../include/Objects/Block.h"
#include "../../include/Objects/ObjectFactory.h"

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
        placeBlock(palette.getSelectedType(), gridCoord);
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        removeBlock(gridCoord);
    }
}

void LevelEditor::placeBlock(BlockType type, Vector2 gridCoord) {
    removeBlock(gridCoord);

    std::unique_ptr<Object> newBlock = ObjectFactory::createBlock(type, gridCoord);

    if (newBlock) {
        PhysicsManager::getInstance().addObject(newBlock.get());
        gridBlocks[{(int)gridCoord.x, (int)gridCoord.y}] = std::move(newBlock);
    }
}

void LevelEditor::removeBlock(Vector2 gridCoord) {
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

