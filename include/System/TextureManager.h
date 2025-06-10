#pragma once

#include <map>
#include "../Objects/Block.h"
#include "raylib.h"

class TextureManager {
public:
    static TextureManager& getInstance();
    Texture2D getBlockTexture(BlockType type);
    void loadTextures();
    void unloadTextures();

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
private:
    TextureManager() = default;
    std::map<BlockType, Texture2D> blockTextures;
    bool texturesLoaded = false;
};

class BlockPalette {
public:
    void drawPalette();
    void handleSelection();
    BlockType getSelectedType() const;
    Rectangle getPaletteRect() { return PaletteRect; }
private:
    Rectangle PaletteRect = { (int)GetScreenWidth() - 320.0f, 50.0f, 300.0f, 100.0f};
    BlockType selectedBlockType = BlockType::GROUND;
};