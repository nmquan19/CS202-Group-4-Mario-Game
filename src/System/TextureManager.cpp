#include "../../include/System/TextureManager.h"

TextureManager& TextureManager::getInstance() {
    static TextureManager instance;
    return instance;
}

Texture2D TextureManager::getBlockTexture(BlockType type) {
    return blockTextures[type];
}

Texture2D TextureManager::getCharacterTexture(CharacterType type) {
    return characterTextures[type];
}

void TextureManager::loadTextures() {
    if (texturesLoaded) return;

    blockTextures[BlockType::GROUND] = LoadTexture("assets/ground_block.png");
    blockTextures[BlockType::BRICK] = LoadTexture("assets/brick_block.png");

    characterTextures[CharacterType::MARIO] = LoadTexture("assets/mario_sprites.png");
    characterTextures[CharacterType::LUIGI] = LoadTexture("assets/luigi_sprites.png");

    texturesLoaded = true;
}

void TextureManager::unloadTextures() {
    if (!texturesLoaded) return;

    for (auto& pair : blockTextures) {
        UnloadTexture(pair.second);
    } 
    blockTextures.clear();

    for (auto& pair : characterTextures) {
        UnloadTexture(pair.second);
    }
    characterTextures.clear();

    texturesLoaded = false;
}

void BlockPalette::drawPalette() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    DrawRectangleRec(PaletteRect, LIGHTGRAY);
    DrawRectangleLinesEx(PaletteRect, 2, BLACK);

    float startX = screenWidth - 300.0f;
    float y = 60.0f;
    float spacing = 80.0f;

    Rectangle groundRect = { startX, y, 50, 50 };
    DrawRectangle(groundRect.x, groundRect.y, groundRect.width, groundRect.height, BROWN);
    DrawRectangleLinesEx(groundRect, 2, (selectedBlockType == BlockType::GROUND) ? RED : BLACK);
    DrawText("GROUND", groundRect.x - 5, groundRect.y + 55, 10, BLACK);

    Rectangle brickRect = { startX + spacing, y, 50, 50 };
    DrawRectangle(brickRect.x, brickRect.y, brickRect.width, brickRect.height, ORANGE);
    DrawRectangleLinesEx(brickRect, 2, (selectedBlockType == BlockType::BRICK) ? RED : BLACK);
    DrawText("BRICK", brickRect.x + 5, brickRect.y + 55, 10, BLACK);

    DrawText("LClick: Place | RClick: Remove | 1: Ground | 2: Brick", screenWidth - 310, PaletteRect.y + PaletteRect.height + 5, 12, BLACK);
}

void BlockPalette::handleSelection() {
    int screenWidth = GetScreenWidth();

    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Rectangle paletteRect = { screenWidth - 310, 10, 300, 80 };
        if (CheckCollisionPointRec(mousePos, paletteRect)) {
            Rectangle groundBlockRect = { screenWidth - 300, 20, 50, 50 };
            Rectangle brickBlockRect = { screenWidth - 240, 20, 50, 50 };
            if (CheckCollisionPointRec(mousePos, groundBlockRect)) {
                selectedBlockType = BlockType::GROUND;
            }
            else if (CheckCollisionPointRec(mousePos, brickBlockRect)) {
                selectedBlockType = BlockType::BRICK;
            }
        }
    }
}

BlockType BlockPalette::getSelectedType() const {
    return selectedBlockType;
}
