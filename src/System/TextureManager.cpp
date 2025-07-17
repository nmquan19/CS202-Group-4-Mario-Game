#include "../../include/System/TextureManager.h"
#include "../../include/System/Interface.h"
#include <raylib.h>
#include <fstream>
#include <vector>
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
    enemy_in.open("assets/enemy/enemy_output.txt");
    int id, x, y, w, h;
    while (enemy_in >> id >> x >> y >> w >> h)
    {
        Enemy_sprite_boxes.push_back({ (float)x,(float)y,(float)w, (float)h });
    }
    enemyTextures = Texture2D(LoadTexture("assets/enemy/enemy_spritesheet.png"));
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
    float yCharacter = yEnemy + 80;
    float spacing = 80;
    float iconSize = 50;

    TextureManager& tm = TextureManager::getInstance();

    // Draw Block Section
    DrawText("BLOCKS", startX, yBlock - 20, 12, BLACK);
    
    // Ground Block
    Rectangle groundRect = { startX, yBlock, iconSize, iconSize };
    Texture2D groundTexture = tm.getBlockTexture(BlockType::GROUND);
    if (groundTexture.id != 0) {
        DrawTexturePro(groundTexture, 
            { 0, 0, (float)groundTexture.width, (float)groundTexture.height },
            groundRect, { 0, 0 }, 0.0f, WHITE);
    } else {
        DrawRectangleRec(groundRect, BROWN);
    }
    DrawRectangleLinesEx(groundRect, 2, (isBlock() && getBlockType() == BlockType::GROUND) ? RED : BLACK);
    DrawText("GROUND", groundRect.x - 5, groundRect.y + iconSize + 5, 10, BLACK);

    // Brick Block
    Rectangle brickRect = { startX + spacing, yBlock, iconSize, iconSize };
    Texture2D brickTexture = tm.getBlockTexture(BlockType::BRICK);
    if (brickTexture.id != 0) {
        DrawTexturePro(brickTexture,
            { 0, 0, (float)brickTexture.width, (float)brickTexture.height },
            brickRect, { 0, 0 }, 0.0f, WHITE);
    } else {
        DrawRectangleRec(brickRect, ORANGE);
    }
    DrawRectangleLinesEx(brickRect, 2, (isBlock() && getBlockType() == BlockType::BRICK) ? RED : BLACK);
    DrawText("BRICK", brickRect.x + 5, brickRect.y + iconSize + 5, 10, BLACK);

    // Draw Enemy Section
    DrawText("ENEMIES", startX, yEnemy - 20, 12, BLACK);
    
    // Goomba
    Rectangle goombaRect = { startX, yEnemy, iconSize, iconSize };
    if (tm.enemyTextures.id != 0 && !tm.Enemy_sprite_boxes.empty()) {
        Rectangle goombaSource = tm.Enemy_sprite_boxes[0]; // Assuming first sprite is Goomba
        DrawTexturePro(tm.enemyTextures, goombaSource, goombaRect, { 0, 0 }, 0.0f, WHITE);
    } else {
        DrawRectangleRec(goombaRect, DARKBROWN);
    }
    DrawRectangleLinesEx(goombaRect, 2, (isEnemy() && getEnemyType() == EnemyType::GOOMBA) ? RED : BLACK);
    DrawText("GOOMBA", goombaRect.x - 5, goombaRect.y + iconSize + 5, 10, BLACK);

    Rectangle GreenkoopaRect = { startX + spacing, yEnemy, 50, 50 };
    DrawRectangleRec(GreenkoopaRect, GREEN);
    DrawRectangleLinesEx(GreenkoopaRect, 2, (isEnemy() && getEnemyType() == EnemyType::GREEN_KOOPA) ? RED : BLACK);
    DrawText("GreenKOOPA", GreenkoopaRect.x + 5, GreenkoopaRect.y + 55, 10, BLACK);

    Rectangle RedkoopaRect = { startX + 2*spacing, yEnemy, 50, 50 };
    DrawRectangleRec(RedkoopaRect, GREEN);
    DrawRectangleLinesEx(RedkoopaRect, 2, (isEnemy() && getEnemyType() == EnemyType::RED_KOOPA) ? RED : BLACK);
    DrawText("RedKOOPA", RedkoopaRect.x + 15, RedkoopaRect.y + 55, 10, BLACK);
   
    // Draw Character Section
    DrawText("CHARACTERS", startX, yCharacter - 20, 12, BLACK);
    
    // Mario
    Rectangle marioRect = { startX, yCharacter, iconSize, iconSize };
    Texture2D marioTexture = tm.getCharacterTexture();
    if (marioTexture.id != 0) {
        // Use a portion of Mario sprite sheet (idle frame)
        Rectangle marioSource = { 11, 23, 12, 15 }; // Adjust based on your sprite sheet
        DrawTexturePro(marioTexture, marioSource, marioRect, { 0, 0 }, 0.0f, WHITE);
    } else {
        DrawRectangleRec(marioRect, RED);
    }
    DrawRectangleLinesEx(marioRect, 2, (isCharacter() && getCharacterType() == CharacterType::MARIO) ? RED : BLACK);
    DrawText("MARIO", marioRect.x + 5, marioRect.y + iconSize + 5, 10, BLACK);

    // Luigi (optional)
    Rectangle luigiRect = { startX + spacing, yCharacter, iconSize, iconSize };
    Texture2D luigiTexture = tm.getCharacterTexture();
    if (luigiTexture.id != 0) {
        Rectangle luigiSource = { 11, 893, 12, 15 }; // Adjust based on your sprite sheet
        DrawTexturePro(luigiTexture, luigiSource, luigiRect, { 0, 0 }, 0.0f, WHITE);
    } else {
        DrawRectangleRec(luigiRect, DARKGREEN);
    }
    DrawRectangleLinesEx(luigiRect, 2, (isCharacter() && getCharacterType() == CharacterType::LUIGI) ? RED : BLACK);
    DrawText("LUIGI", luigiRect.x + 10, luigiRect.y + iconSize + 5, 10, BLACK);
}

void ObjectPalette::handleSelection() {
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return;

    Vector2 mousePos = GetMousePosition();
    if (!CheckCollisionPointRec(mousePos, paletteRect)) return;

    float startX = paletteRect.x + 10;
    float yBlock = paletteRect.y + 10;
    float yEnemy = yBlock + 80;
    float yCharacter = yEnemy + 80;
    float spacing = 80;
    float iconSize = 50;

    // Block selection rectangles
    Rectangle groundRect = { startX, yBlock, iconSize, iconSize };
    Rectangle brickRect = { startX + spacing, yBlock, iconSize, iconSize };
    
    // Enemy selection rectangles
    Rectangle goombaRect = { startX, yEnemy, iconSize, iconSize };
    
    // Character selection rectangles
    Rectangle marioRect = { startX, yCharacter, iconSize, iconSize };
    Rectangle luigiRect = { startX + spacing, yCharacter, iconSize, iconSize };

    Rectangle gkoopaRect = { startX + spacing, yEnemy, 50, 50 };
    Rectangle rkoopaRect = { startX + 2*spacing, yEnemy, 50, 50 };

    if (CheckCollisionPointRec(mousePos, groundRect)) {
        selected = BlockType::GROUND;
    }
    else if (CheckCollisionPointRec(mousePos, brickRect)) {
        selected = BlockType::BRICK;
    }
    else if (CheckCollisionPointRec(mousePos, goombaRect)) {
        selected = EnemyType::GOOMBA;
    }
    else if (CheckCollisionPointRec(mousePos, gkoopaRect)) {
        selected = EnemyType::GREEN_KOOPA;
    }
    else if (CheckCollisionPointRec(mousePos, rkoopaRect)) {
        selected = EnemyType::RED_KOOPA;
    }
    else if (CheckCollisionPointRec(mousePos, marioRect)) {
        selected = CharacterType::MARIO;
    }
    else if (CheckCollisionPointRec(mousePos, luigiRect)) {
        selected = CharacterType::LUIGI;
    }
}
