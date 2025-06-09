#pragma once

#include <memory>
#include "../Objects/Block.h"
#include "../System/Grid.h"
#include "../System/TextureManager.h"

class LevelEditor {
public:
    LevelEditor(const LevelEditor&) = delete;
    LevelEditor& operator=(const LevelEditor&) = delete;

    static LevelEditor& getInstance();
    static void cleanup();

    ~LevelEditor();

    void update();
    void draw();
    void handleMouseInput();
    void placeBlock(BlockType type, Vector2 gridCoord);
    void removeBlock(Vector2 gridCoord);
    void toggleEditMode();
    bool isInEditMode() const;
private:
    static LevelEditor* instance;
    std::map<std::pair<int, int>, std::unique_ptr<Object>> gridBlocks;
    BlockPalette palette;
    bool editMode = true;
    LevelEditor() = default;
};