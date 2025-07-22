#include "../../include/System/TextureManager.h"
#include "../../include/System/Interface.h"
#include "../../include/System/Constant.h"
#include <raylib.h>
#include <fstream>
#include <vector>
std::vector<Rectangle> TextureManager::Enemy_sprite_boxes;
Texture2D TextureManager::enemyTextures;
std::vector<Rectangle> TextureManager::DryBowser_sprite_boxes;
Texture2D TextureManager::DryBowser_texture;
Texture2D TextureManager::blocksTexture;
TextureManager& TextureManager::getInstance() {
    static TextureManager instance;
    return instance;
}

Texture2D TextureManager::getCharacterTexture() const {
    return characterTextures;
}

Texture2D TextureManager::getItemTexture() const {
    return itemTextures;
}

void TextureManager::loadTextures() {
    if (texturesLoaded) return;

    blocksTexture = LoadTexture("assets/item2_sprites.png");

    characterTextures = LoadTexture("assets/character_spritesheet.png");

    itemTextures = LoadTexture("assets/item_sprites.png");
    //Enemy textures 
    std::ifstream  enemy_in;
    enemy_in.open("assets/enemy/enemy_output.txt");
    int id, x, y, w, h;
    while (enemy_in >> id >> x >> y >> w >> h)
    {
        Enemy_sprite_boxes.push_back({ (float)x,(float)y,(float)w, (float)h });
    }
    enemyTextures = Texture2D(LoadTexture("assets/enemy/enemy_spritesheet.png"));
    enemy_in.close();
    //Bowser Textures
    std::ifstream  dryBowser_in;
    dryBowser_in.open("assets/enemy/DryBowserOutput.txt");
 
    while (dryBowser_in >> id >> x >> y >> w >> h)
    {
        DryBowser_sprite_boxes.push_back({ (float)x,(float)y,(float)w, (float)h });
    }
    DryBowser_texture= Texture2D(LoadTexture("assets/enemy/DryBowser.png"));
    dryBowser_in.close();
    texturesLoaded = true;
}

void TextureManager::unloadTextures() {
    if (!texturesLoaded) return;

    UnloadTexture(characterTextures);
	UnloadTexture(enemyTextures);
    UnloadTexture(itemTextures);
    texturesLoaded = false;
}

void ObjectPalette::drawPalette() {
    DrawRectangleRec(paletteRect, LIGHTGRAY);
    DrawRectangleLinesEx(paletteRect, 2, BLACK);

    TextureManager& tm = TextureManager::getInstance();
    Texture2D blocksTexture = TextureManager::blocksTexture;

    // Ground Block
    Rectangle groundRect = { startX, yBlock, iconSize, iconSize };
    if (blocksTexture.id != 0) {
        DrawTexturePro(blocksTexture, Constants::PaletteResources::GROUND, groundRect, { 0, 0 }, 0.0f, WHITE);
    }
    DrawRectangleLinesEx(groundRect, 2, (isBlock() && getBlockType() == BlockType::GROUND) ? RED : BLACK);

    // Brick Block
    Rectangle brickRect = { startX + spacing, yBlock, iconSize, iconSize };
    if (blocksTexture.id != 0) {
        DrawTexturePro(blocksTexture, Constants::PaletteResources::BRICK, brickRect, { 0, 0 }, 0.0f, WHITE);
    } 
    DrawRectangleLinesEx(brickRect, 2, (isBlock() && getBlockType() == BlockType::BRICK) ? RED : BLACK);
    
    // Goomba
    Rectangle goombaRect = { startX, yEnemy, iconSize, iconSize };
    if (tm.enemyTextures.id != 0 && !tm.Enemy_sprite_boxes.empty()) {
        Rectangle goombaSource = tm.Enemy_sprite_boxes[0]; 
        DrawTexturePro(tm.enemyTextures, goombaSource, goombaRect, { 0, 0 }, 0.0f, WHITE);
    }
    DrawRectangleLinesEx(goombaRect, 2, (isEnemy() && getEnemyType() == EnemyType::GOOMBA) ? RED : BLACK);

    // Green Koopa
    Rectangle greenKoopaRect = { startX + spacing, yEnemy, iconSize, iconSize };
    if (tm.enemyTextures.id != 0 && !tm.Enemy_sprite_boxes.empty()) {
        Rectangle greenKoopaSource = tm.Enemy_sprite_boxes[45];
        DrawTexturePro(tm.enemyTextures, greenKoopaSource, greenKoopaRect, {0, 0}, 0.0f, WHITE);
    }
    DrawRectangleLinesEx(greenKoopaRect, 2, (isEnemy() && getEnemyType() == EnemyType::GREEN_KOOPA) ? RED : BLACK);

    // Spring
    Rectangle springRect = { startX, yInteractive, iconSize, iconSize };
    Texture2D springTexture = tm.getItemTexture();
    if (springTexture.id != 0) {
        Rectangle springSource = {1, 467, 16, 16};
        DrawTexturePro(springTexture, springSource, springRect, { 0, 0 }, 0.0f, WHITE);
    }
    DrawText("SPRING", springRect.x + 10, springRect.y + iconSize + 5, 10, BLACK);
}

void ObjectPalette::handleSelection() {
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return;

    Vector2 mousePos = GetMousePosition();
    if (!CheckCollisionPointRec(mousePos, paletteRect)) return;

    // Block selection rectangles
    Rectangle groundRect = { startX, yBlock, iconSize, iconSize };
    Rectangle brickRect = { startX + spacing, yBlock, iconSize, iconSize };
    
    // Enemy selection rectangles
    Rectangle goombaRect = { startX, yEnemy, iconSize, iconSize };
    Rectangle gkoopaRect = { startX + spacing, yEnemy, iconSize, iconSize };
    Rectangle rkoopaRect = { startX + 2*spacing, yEnemy, iconSize, iconSize };

    Rectangle springRect = { startX, yInteractive, iconSize, iconSize };

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
    else if (CheckCollisionPointRec(mousePos, springRect)) {
        selected = InteractiveType::SPRING;
    }
}
