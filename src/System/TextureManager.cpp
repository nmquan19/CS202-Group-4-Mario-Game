#include "../../include/System/TextureManager.h"
#include "../../include/System/Interface.h"
#include <raylib.h>
#include <fstream>
#include <vector>
#include <iostream>
std::vector<Rectangle> TextureManager::Enemy_sprite_boxes;
Texture2D TextureManager::enemyTextures;
TextureManager& TextureManager::getInstance() {
    static TextureManager instance;
    return instance;
}

Texture2D TextureManager::getBlockTexture(BlockType type) {
    return blockTextures[type];
}

Texture2D TextureManager::getCharacterTexture() const {
    return characterTextures;
}

void TextureManager::loadTextures() {
    if (texturesLoaded) return;

    blockTextures[BlockType::GROUND] = LoadTexture("assets/ground_block.png");
    blockTextures[BlockType::BRICK] = LoadTexture("assets/brick_block.png");

    characterTextures = LoadTexture("assets/character_spritesheet.png");
    //Enemy textures 
	DrawText("Loading enemy textures...", 10, 10, 20, DARKGRAY);
    std::ifstream  enemy_in;
    enemy_in.open("enemy_output.txt");
    int id, x, y, w, h;
    while (enemy_in >> id >> x >> y >> w >> h)
    {
		std::cout << "Loading enemy sprite: " << id << " at (" << x << ", " << y << ") with size (" << w << ", " << h << ")\n";
        Enemy_sprite_boxes.push_back({ (float)x,(float)y,(float)w, (float)h });
    }
    enemyTextures = Texture2D(LoadTexture("assets/enemy_spritesheet.png"));
    enemy_in.close();
    texturesLoaded = true;
}

void TextureManager::unloadTextures() {
    if (!texturesLoaded) return;

    for (auto& pair : blockTextures) {
        UnloadTexture(pair.second);
    } 
    blockTextures.clear();

    UnloadTexture(characterTextures);
	UnloadTexture(enemyTextures);
    texturesLoaded = false;
}

void ObjectPalette::drawPalette() {
    DrawRectangleRec(paletteRect, LIGHTGRAY);
    DrawRectangleLinesEx(paletteRect, 2, BLACK);

    float startX = paletteRect.x + 10;
    float yBlock = paletteRect.y + 10;
    float yEnemy = yBlock + 80;
    float spacing = 80;

    Rectangle groundRect = { startX, yBlock, 50, 50 };
    DrawRectangleRec(groundRect, BROWN);
    DrawRectangleLinesEx(groundRect, 2, (isBlock() && getBlockType() == BlockType::GROUND) ? RED : BLACK);
    DrawText("GROUND", groundRect.x - 5, groundRect.y + 55, 10, BLACK);

    Rectangle brickRect = { startX + spacing, yBlock, 50, 50 };
    DrawRectangleRec(brickRect, ORANGE);
    DrawRectangleLinesEx(brickRect, 2, (isBlock() && getBlockType() == BlockType::BRICK) ? RED : BLACK);
    DrawText("BRICK", brickRect.x + 5, brickRect.y + 55, 10, BLACK);

    Rectangle goombaRect = { startX, yEnemy, 50, 50 };
    DrawRectangleRec(goombaRect, DARKBROWN);
    DrawRectangleLinesEx(goombaRect, 2, (isEnemy() && getEnemyType() == EnemyType::GOOMBA) ? RED : BLACK);
    DrawText("GOOMBA", goombaRect.x - 5, goombaRect.y + 55, 10, BLACK);

    Rectangle koopaRect = { startX + spacing, yEnemy, 50, 50 };
    DrawRectangleRec(koopaRect, GREEN);
    DrawRectangleLinesEx(koopaRect, 2, (isEnemy() && getEnemyType() == EnemyType::KOOPA) ? RED : BLACK);
    DrawText("KOOPA", koopaRect.x + 5, koopaRect.y + 55, 10, BLACK);
}

void ObjectPalette::handleSelection() {
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return;

    Vector2 mousePos = GetMousePosition();
    if (!CheckCollisionPointRec(mousePos, paletteRect)) return;

    float startX = paletteRect.x + 10;
    float yBlock = paletteRect.y + 10;
    float yEnemy = yBlock + 80;
    float spacing = 80;

    Rectangle groundRect = { startX, yBlock, 50, 50 };
    Rectangle brickRect = { startX + spacing, yBlock, 50, 50 };
    Rectangle goombaRect = { startX, yEnemy, 50, 50 };
    Rectangle koopaRect = { startX + spacing, yEnemy, 50, 50 };

    if (CheckCollisionPointRec(mousePos, groundRect)) {
        selected = BlockType::GROUND;
    }
    else if (CheckCollisionPointRec(mousePos, brickRect)) {
        selected = BlockType::BRICK;
    }
    else if (CheckCollisionPointRec(mousePos, goombaRect)) {
        selected = EnemyType::GOOMBA;
    }
    else if (CheckCollisionPointRec(mousePos, koopaRect)) {
        selected = EnemyType::KOOPA;
    }
}
