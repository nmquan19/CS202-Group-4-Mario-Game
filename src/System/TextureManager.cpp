#include "../../include/System/TextureManager.h"
#include "../../include/System/Interface.h"
#include "../../include/System/Constant.h"
#include <raylib.h>
#include <fstream>
#include <vector>
#include <iostream>
std::vector<Rectangle> TextureManager::Enemy_sprite_boxes;
Texture2D TextureManager::enemyTextures;

std::vector<Rectangle> TextureManager::Item_sprite_boxes;
Texture2D TextureManager::itemTextures;

std::vector<Rectangle> TextureManager::DryBowser_sprite_boxes;
Texture2D TextureManager::DryBowser_texture;
Texture2D TextureManager::blocksTexture;
Texture2D TextureManager::fontTexture;
std::unordered_map<char, Rectangle> TextureManager::fontSprites = {
    {'0', {90, 3, 6, 7}}, {'1', {97, 3, 4, 7}}, {'2', {102, 3, 6, 7}}, {'3', {109, 3, 6, 7}}, {'4', {116, 3, 6, 7}},
    {'5', {1, 16, 6, 7}}, {'6', {8, 16, 6, 7}}, {'7', {15, 16, 6, 7}}, {'8', {22, 16, 6, 7}}, {'9', {29, 16, 6, 7}}, 
    {':', {36, 17, 2, 6}}, {'A', {78, 16, 6, 7}}, {'B', {85, 16, 7, 7}}, {'C', {93, 16, 7, 7}}, {'D', {101, 16, 7, 7}},
    {'E', {109, 16, 7, 7}}, {'F', {117, 16, 7, 7}}, {'G', {1, 29, 7, 7}}, {'H', {9, 29, 8, 7}}, {'I', {18, 29, 4, 7}},
    {'J', {23, 29, 6, 7}}, {'K', {30, 29, 8, 7}}, {'L', {39, 29, 6, 7}}, {'M', {46, 29, 10, 7}}, {'N', {57, 29, 9, 7}},
    {'O', {67, 29, 7, 7}}, {'P', {75, 29, 7, 7}}, {'Q', {83, 29, 7, 9}}, {'R', {91, 29, 8, 7}}, {'S', {100, 29, 6, 7}},
    {'T', {107, 29, 6, 7}}, {'U', {114, 29, 9, 7}}, {'V', {1, 42, 9, 7}}, {'W', {11, 42, 10, 7}}, {'X', {22, 42, 8, 7}},
    {'Y', {31, 42, 8, 7}}, {'Z', {40, 42, 6, 7}}, {'a', {83, 44, 7, 5}}, {'b', {91, 42, 7, 7}}, {'c', {99, 44, 5, 5}},
    {'d', {105, 42, 7, 7}}, {'e', {113, 44, 6, 5}}, {'f', {120, 42, 5, 7}}, {'g', {1, 57, 7, 7}}, {'h', {9, 55, 7, 7}},
    {'i', {17, 55, 4, 7}}, {'j', {22, 55, 3, 9}}, {'k', {26, 55, 7, 7}}, {'l', {34, 55, 4, 7}}, {'m', {39, 57, 9, 5}},
    {'n', {49, 57, 8, 5}}, {'o', {58, 57, 6, 5}}, {'p', {65, 57, 7, 7}}, {'q', {73, 57, 7, 7}}, {'r', {81, 57, 6, 5}},
    {'s', {88, 57, 5, 5}}, {'t', {94, 56, 4, 6}}, {'u', {99, 57, 7, 5}}, {'v', {107, 57, 7, 5}}, {'w', {115, 57, 9, 5}},
    {'x', {1, 70, 7, 5}}, {'y', {9, 70, 7, 7}}, {'z', {17, 70, 6, 5}}
};

TextureManager& TextureManager::getInstance() {
    static TextureManager instance;
    return instance;
}

Texture2D TextureManager::getItemTexture(ItemType type) const {
    return itemTextures;
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

    fontTexture = LoadTexture("assets/font_sprites.png");
    //Enemy textures 
    std::ifstream  enemy_in;
    enemy_in.open("assets/enemy/enemy_output.txt");
    int id, x, y, w, h;
    while (enemy_in >> id >> x >> y >> w >> h){
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

	//Item textures
    DrawText("Loading item textures...", 10, 30, 20, DARKGRAY);
    std::ifstream item_in;
    item_in.open("assets/item/item_output.txt");
    int item_id, item_x, item_y, item_w, item_h;
    while (item_in >> item_id >> item_x >> item_y >> item_w >> item_h){
        Item_sprite_boxes.push_back({ (float)item_x,(float)item_y,(float)item_w, (float)item_h });
        std::cout << "Loaded ID " << item_id << item_x << item_y << item_w << item_h << std::endl;
    }
    itemTextures = LoadTexture("assets/item/item_spritesheet.png");
    item_in.close();

    itemTexturesLoaded = true;
}

void TextureManager::unloadTextures() {
    if (!texturesLoaded) return;

    UnloadTexture(characterTextures);
	UnloadTexture(enemyTextures);
    UnloadTexture(itemTextures);
    UnloadTexture(blocksTexture);
    UnloadTexture(DryBowser_texture);
    UnloadTexture(fontTexture);
    texturesLoaded = false;

	UnloadTexture(itemTextures);
    itemTexturesLoaded = false;
    Enemy_sprite_boxes.clear();
	Item_sprite_boxes.clear();
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
        DrawTexturePro(tm.enemyTextures, greenKoopaSource, greenKoopaRect, { 0,0 }, 0.0f, WHITE);
    }
    DrawRectangleLinesEx(greenKoopaRect, 2, (isEnemy() && getEnemyType() == EnemyType::GREEN_KOOPA) ? RED : BLACK);

    // Red Koopa
    Rectangle redKoopaRect = { startX + 2 * spacing, yEnemy, iconSize, iconSize };
    if (tm.enemyTextures.id != 0 && !tm.Enemy_sprite_boxes.empty()) {
        Rectangle redKoopaSource = tm.Enemy_sprite_boxes[53];
        DrawTexturePro(tm.enemyTextures, redKoopaSource, redKoopaRect, { 0,0 }, 0.0f, WHITE);
    }
    DrawRectangleLinesEx(redKoopaRect, 2, (isEnemy() && getEnemyType() == EnemyType::RED_KOOPA) ? RED : BLACK);

    // Spring
    Rectangle springRect = { startX, yInteractive, iconSize, iconSize };
    Texture2D springTexture = tm.getItemTexture();
    if (springTexture.id != 0) {
        Rectangle springSource = {1, 467, 16, 16};
        DrawTexturePro(springTexture, springSource, springRect, { 0, 0 }, 0.0f, WHITE);
    }
    DrawRectangleLinesEx(springRect, 2, (isInteractive() && getInteractiveType() == InteractiveType::SPRING) ? RED : BLACK);
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
