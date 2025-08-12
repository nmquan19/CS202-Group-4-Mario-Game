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

Texture2D TextureManager::day_groundTexture;
Texture2D TextureManager::day_undergroundTexture;
Texture2D TextureManager::night_airshipTexture;
Texture2D TextureManager::night_snowTexture;

Texture2D TextureManager::fontTexture;
Texture2D TextureManager::interactiveTextures;
Texture2D TextureManager::background_lv1;

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

    interactiveTextures = LoadTexture("assets/interactive_sprites.png");

    fontTexture = LoadTexture("assets/font_sprites.png");

    background_lv1 = LoadTexture("assets/backgrounds/bg_lv1.png");

    day_groundTexture = LoadTexture("assets/day_ground.png");
    day_undergroundTexture = LoadTexture("assets/day_underground.png");
    night_airshipTexture = LoadTexture("assets/night_airship.png");
    night_snowTexture = LoadTexture("assets/night_snow.png");

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

    UnloadTexture(day_groundTexture);
    UnloadTexture(day_undergroundTexture);
    UnloadTexture(night_airshipTexture);
    UnloadTexture(night_snowTexture);

    UnloadTexture(DryBowser_texture);
    UnloadTexture(fontTexture);
    UnloadTexture(interactiveTextures);
    texturesLoaded = false;

	UnloadTexture(itemTextures);
    itemTexturesLoaded = false;
    Enemy_sprite_boxes.clear();
	Item_sprite_boxes.clear();
}

void ObjectPalette::updatePalette(float deltaTime) {
    float dx = target.x - rect.x;
    //float dy = target.y - rect.y;
    float distance = sqrtf(dx * dx /*+ dy * dy*/);

    if (distance > 1.0f) {
        float moveX = (dx / distance) * 300.0f * deltaTime;
        //float moveY = (dy / distance) * 300.0f * deltaTime;

        if (fabsf(moveX) > fabsf(dx)) moveX = dx;
        //if (fabsf(moveY) > fabsf(dy)) moveY = dy;

        rect.x += moveX;
        //rect.y += moveY;
    }
    paletteRect.x = rect.x + 20;
    //paletteRect.y = rect.y - (float)GetScreenHeight() / 4.0f;
    paletteRect.y = target.y - (float)GetScreenHeight() / 4.0f;
    blockRect = { rect.x + 20, 0, 40, (float)GetScreenHeight() / 4.0f };
    enemyRect = { rect.x + 20, (float)GetScreenHeight() / 4.0f, 40, (float)GetScreenHeight() / 4.0f };
    objectRect = { rect.x + 20, (float)GetScreenHeight() * 2.0f / 4.0f, 40, (float)GetScreenHeight() / 4.0f };
    itemRect = { rect.x + 20, (float)GetScreenHeight() * 3.0f / 4.0f, 40, (float)GetScreenHeight() / 4.0f };
    startX = paletteRect.x + 50;
    startY = paletteRect.y + 50;
    spacingX = 120;
    spacingY = 120;
    iconSize = 100;
}

void ObjectPalette::drawPalette() {
    DrawRectangleRec(rect, DARKGRAY);
    DrawRectangleRec(paletteRect, ORANGE);
    Vector2 blockText = MeasureTextEx(UIManager::getInstance().menuFont, "BLOCK", 30, 3);
    Vector2 enemyText = MeasureTextEx(UIManager::getInstance().menuFont, "ENEMY", 30, 3);
    Vector2 objectText = MeasureTextEx(UIManager::getInstance().menuFont, "OBJECT", 30, 3);
    Vector2 itemText = MeasureTextEx(UIManager::getInstance().menuFont, "ITEM", 30, 3);

    Vector2 map1Text = MeasureTextEx(UIManager::getInstance().menuFont, "1", 30, 3);
    Vector2 map2Text = MeasureTextEx(UIManager::getInstance().menuFont, "2", 30, 3);
    Vector2 map3Text = MeasureTextEx(UIManager::getInstance().menuFont, "3", 30, 3);
    Vector2 map4Text = MeasureTextEx(UIManager::getInstance().menuFont, "4", 30, 3);
    if (selectRect == 1) {
        DrawRectangleRec(blockRect, { 75, 75, 75, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "BLOCK", { blockRect.x + (blockRect.width - blockText.y) / 2.0f, blockRect.y + (blockRect.height + blockText.x) / 2.0f }, { 0,0 }, -90, 30, 3, WHITE);
        DrawRectangleRec(enemyRect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "ENEMY", { enemyRect.x + (enemyRect.width - enemyText.y) / 2.0f, enemyRect.y + (enemyRect.height + enemyText.x) / 2.0f }, { 0,0 }, -90, 30, 3, GRAY);
        DrawRectangleRec(objectRect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "OBJECT", { objectRect.x + (objectRect.width - objectText.y) / 2.0f, objectRect.y + (objectRect.height + objectText.x) / 2.0f }, { 0,0 }, -90, 30, 3, GRAY);
        DrawRectangleRec(itemRect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "ITEM", { itemRect.x + (itemRect.width - itemText.y) / 2.0f, itemRect.y + (itemRect.height + itemText.x) / 2.0f }, { 0,0 }, -90, 30, 3, GRAY);
    }
    if (selectRect == 2) {
        DrawRectangleRec(blockRect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "BLOCK", { blockRect.x + (blockRect.width - blockText.y) / 2.0f, blockRect.y + (blockRect.height + blockText.x) / 2.0f }, { 0,0 }, -90, 30, 3, GRAY);
        DrawRectangleRec(enemyRect, { 75, 75, 75, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "ENEMY", { enemyRect.x + (enemyRect.width - enemyText.y) / 2.0f, enemyRect.y + (enemyRect.height + enemyText.x) / 2.0f }, { 0,0 }, -90, 30, 3, WHITE);
        DrawRectangleRec(objectRect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "OBJECT", { objectRect.x + (objectRect.width - objectText.y) / 2.0f, objectRect.y + (objectRect.height + objectText.x) / 2.0f }, { 0,0 }, -90, 30, 3, GRAY);
        DrawRectangleRec(itemRect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "ITEM", { itemRect.x + (itemRect.width - itemText.y) / 2.0f, itemRect.y + (itemRect.height + itemText.x) / 2.0f }, { 0,0 }, -90, 30, 3, GRAY);
    }
    if (selectRect == 3) {
        DrawRectangleRec(blockRect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "BLOCK", { blockRect.x + (blockRect.width - blockText.y) / 2.0f, blockRect.y + (blockRect.height + blockText.x) / 2.0f }, { 0,0 }, -90, 30, 3, GRAY);
        DrawRectangleRec(enemyRect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "ENEMY", { enemyRect.x + (enemyRect.width - enemyText.y) / 2.0f, enemyRect.y + (enemyRect.height + enemyText.x) / 2.0f }, { 0,0 }, -90, 30, 3, GRAY);
        DrawRectangleRec(objectRect, { 75, 75, 75, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "OBJECT", { objectRect.x + (objectRect.width - objectText.y) / 2.0f, objectRect.y + (objectRect.height + objectText.x) / 2.0f }, { 0,0 }, -90, 30, 3, WHITE);
        DrawRectangleRec(itemRect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "ITEM", { itemRect.x + (itemRect.width - itemText.y) / 2.0f, itemRect.y + (itemRect.height + itemText.x) / 2.0f }, { 0,0 }, -90, 30, 3, GRAY);
    }
    if (selectRect == 4) {
        DrawRectangleRec(blockRect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "BLOCK", { blockRect.x + (blockRect.width - blockText.y) / 2.0f, blockRect.y + (blockRect.height + blockText.x) / 2.0f }, { 0,0 }, -90, 30, 3, GRAY);
        DrawRectangleRec(enemyRect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "ENEMY", { enemyRect.x + (enemyRect.width - enemyText.y) / 2.0f, enemyRect.y + (enemyRect.height + enemyText.x) / 2.0f }, { 0,0 }, -90, 30, 3, GRAY);
        DrawRectangleRec(objectRect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "OBJECT", { objectRect.x + (objectRect.width - objectText.y) / 2.0f, objectRect.y + (objectRect.height + objectText.x) / 2.0f }, { 0,0 }, -90, 30, 3, GRAY);
        DrawRectangleRec(itemRect, { 75, 75, 75, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "ITEM", { itemRect.x + (itemRect.width - itemText.y) / 2.0f, itemRect.y + (itemRect.height + itemText.x) / 2.0f }, { 0,0 }, -90, 30, 3, WHITE);
    }

    if (map == 1) {
        DrawRectangleRec(map1Rect, { 75, 75, 75, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "1", { map1Rect.x + (map1Rect.width - map1Text.x) / 2.0f, map1Rect.y + (map1Rect.height - map1Text.y) / 2.0f }, { 0,0 }, 0, 30, 3, WHITE);
        DrawRectangleRec(map2Rect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "2", { map2Rect.x + (map2Rect.width - map2Text.x) / 2.0f, map2Rect.y + (map2Rect.height - map2Text.y) / 2.0f }, { 0,0 }, 0, 30, 3, GRAY);
        DrawRectangleRec(map3Rect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "3", { map3Rect.x + (map3Rect.width - map3Text.x) / 2.0f, map3Rect.y + (map3Rect.height - map3Text.y) / 2.0f }, { 0,0 }, 0, 30, 3, GRAY);
        DrawRectangleRec(map4Rect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "4", { map4Rect.x + (map4Rect.width - map4Text.x) / 2.0f, map4Rect.y + (map4Rect.height - map4Text.y) / 2.0f }, { 0,0 }, 0, 30, 3, GRAY);
    }
    if (map == 2) {
        DrawRectangleRec(map1Rect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "1", { map1Rect.x + (map1Rect.width - map1Text.x) / 2.0f, map1Rect.y + (map1Rect.height - map1Text.y) / 2.0f }, { 0,0 }, 0, 30, 3, GRAY);
        DrawRectangleRec(map2Rect, { 75, 75, 75, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "2", { map2Rect.x + (map2Rect.width - map2Text.x) / 2.0f, map2Rect.y + (map2Rect.height - map2Text.y) / 2.0f }, { 0,0 }, 0, 30, 3, WHITE);
        DrawRectangleRec(map3Rect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "3", { map3Rect.x + (map3Rect.width - map3Text.x) / 2.0f, map3Rect.y + (map3Rect.height - map3Text.y) / 2.0f }, { 0,0 }, 0, 30, 3, GRAY);
        DrawRectangleRec(map4Rect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "4", { map4Rect.x + (map4Rect.width - map4Text.x) / 2.0f, map4Rect.y + (map4Rect.height - map4Text.y) / 2.0f }, { 0,0 }, 0, 30, 3, GRAY);
    }
    if (map == 3) {
        DrawRectangleRec(map1Rect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "1", { map1Rect.x + (map1Rect.width - map1Text.x) / 2.0f, map1Rect.y + (map1Rect.height - map1Text.y) / 2.0f }, { 0,0 }, 0, 30, 3, GRAY);
        DrawRectangleRec(map2Rect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "2", { map2Rect.x + (map2Rect.width - map2Text.x) / 2.0f, map2Rect.y + (map2Rect.height - map2Text.y) / 2.0f }, { 0,0 }, 0, 30, 3, GRAY);
        DrawRectangleRec(map3Rect, { 75, 75, 75, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "3", { map3Rect.x + (map3Rect.width - map3Text.x) / 2.0f, map3Rect.y + (map3Rect.height - map3Text.y) / 2.0f }, { 0,0 }, 0, 30, 3, WHITE);
        DrawRectangleRec(map4Rect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "4", { map4Rect.x + (map4Rect.width - map4Text.x) / 2.0f, map4Rect.y + (map4Rect.height - map4Text.y) / 2.0f }, { 0,0 }, 0, 30, 3, GRAY);
    }
    if (map == 4) {
        DrawRectangleRec(map1Rect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "1", { map1Rect.x + (map1Rect.width - map1Text.x) / 2.0f, map1Rect.y + (map1Rect.height - map1Text.y) / 2.0f }, { 0,0 }, 0, 30, 3, GRAY);
        DrawRectangleRec(map2Rect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "2", { map2Rect.x + (map2Rect.width - map2Text.x) / 2.0f, map2Rect.y + (map2Rect.height - map2Text.y) / 2.0f }, { 0,0 }, 0, 30, 3, GRAY);
        DrawRectangleRec(map3Rect, { 159, 159, 159, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "3", { map3Rect.x + (map3Rect.width - map3Text.x) / 2.0f, map3Rect.y + (map3Rect.height - map3Text.y) / 2.0f }, { 0,0 }, 0, 30, 3, GRAY);
        DrawRectangleRec(map4Rect, { 75, 75, 75, 100 }); DrawTextPro(UIManager::getInstance().menuFont, "4", { map4Rect.x + (map4Rect.width - map4Text.x) / 2.0f, map4Rect.y + (map4Rect.height - map4Text.y) / 2.0f }, { 0,0 }, 0, 30, 3, WHITE);
    }

    DrawRectangleLinesEx(paletteRect, 2, BLACK);

    TextureManager& tm = TextureManager::getInstance();
    Texture2D blocksTexture = TextureManager::blocksTexture;

    Texture2D day_groundTexture;
    if (map == 1) day_groundTexture = TextureManager::day_groundTexture;
    else if (map == 2) day_groundTexture = TextureManager::day_undergroundTexture;
    else if (map == 3) day_groundTexture = TextureManager::night_airshipTexture;
    else day_groundTexture = TextureManager::night_snowTexture;
    //Texture2D day_groundTexture = TextureManager::day_groundTexture;
    //Texture2D day_undergroundTexture = TextureManager::day_undergroundTexture;
    //Texture2D night_airshipTexture = TextureManager::night_airshipTexture;
    //Texture2D night_snowTexture = TextureManager::night_snowTexture;
    if (selectRect == 1) {
        // Ground Block
        Rectangle groundRect = { startX, startY, iconSize, iconSize };
        if (blocksTexture.id != 0) {
            DrawTexturePro(blocksTexture, Constants::PaletteResources::GROUND, groundRect, { 0, 0 }, 0.0f, WHITE);
        }
        DrawRectangleLinesEx(groundRect, 2, (isBlock() && getBlockType() == BlockType::GROUND) ? RED : BLACK);

        // Brick Block
        Rectangle brickRect = { startX + spacingX, startY, iconSize, iconSize };
        if (blocksTexture.id != 0) {
            DrawTexturePro(blocksTexture, Constants::PaletteResources::BRICK, brickRect, { 0, 0 }, 0.0f, WHITE);
        }
        DrawRectangleLinesEx(brickRect, 2, (isBlock() && getBlockType() == BlockType::BRICK) ? RED : BLACK);
        int traverse;
        if (map == 1 || map == 2 || map == 3 || map == 4) {
            traverse = 3;
            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_1_2, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_1_2) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_1_3, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_1_3) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_1_12, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_1_12) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_1_13, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_1_13) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_1_14, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_1_14) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_1_15, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_1_15) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_1_16, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_1_16) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_2_7, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_2_7) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_2_8, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_2_8) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_2_12, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_2_12) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_2_13, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_2_13) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_2_14, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_2_14) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_2_15, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_2_15) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_2_16, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_2_16) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_3_4, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_3_4) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_3_5, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_3_5) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_3_6, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_3_6) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_3_7, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_3_7) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_3_8, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_3_8) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_3_15, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_3_15) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_3_16, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_3_16) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_4_4, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_4_4) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_4_5, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_4_5) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_4_6, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_4_6) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_4_7, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_4_7) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_4_11, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_4_11) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_4_12, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_4_12) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_4_13, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_4_13) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_4_14, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_4_14) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_4_15, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_4_15) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_4_16, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_4_16) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_5_4, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_5_4) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_5_5, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_5_5) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_5_6, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_5_6) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_5_8, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_5_8) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_5_9, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_5_9) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_5_10, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_5_10) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_5_11, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_5_11) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_5_12, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_5_12) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_5_13, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_5_13) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_5_14, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_5_14) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_5_15, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_5_15) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_5_16, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_5_16) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_6_6, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_6_6) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_7_1, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_7_1) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_7_2, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_7_2) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_7_3, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_7_3) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_7_4, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_7_4) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_7_5, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_7_5) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_7_6, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_7_6) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_7_7, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_7_7) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_7_11, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_7_11) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_7_12, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_7_12) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_7_13, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_7_13) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_7_14, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_7_14) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_7_15, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_7_15) ? RED : BLACK); traverse++;

            if (day_groundTexture.id != 0) {
                DrawTexturePro(day_groundTexture, Constants::PaletteResources::BLOCK_1_7_16, ordinalRect(traverse), { 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangleLinesEx(ordinalRect(traverse), 2, (isBlock() && getBlockType() == BlockType::BLOCK_1_7_16) ? RED : BLACK); traverse++;

        }
    }
    if (selectRect == 2) {
        // Goomba
        Rectangle goombaRect = { startX, startY, iconSize, iconSize };
        if (tm.enemyTextures.id != 0 && !tm.Enemy_sprite_boxes.empty()) {
            Rectangle goombaSource = tm.Enemy_sprite_boxes[0];
            DrawTexturePro(tm.enemyTextures, goombaSource, goombaRect, { 0, 0 }, 0.0f, WHITE);
        }
        DrawRectangleLinesEx(goombaRect, 2, (isEnemy() && getEnemyType() == EnemyType::GOOMBA) ? RED : BLACK);

        // Green Koopa
        Rectangle greenKoopaRect = { startX + spacingX, startY, iconSize, iconSize };
        if (tm.enemyTextures.id != 0 && !tm.Enemy_sprite_boxes.empty()) {
            Rectangle greenKoopaSource = tm.Enemy_sprite_boxes[45];
            DrawTexturePro(tm.enemyTextures, greenKoopaSource, greenKoopaRect, { 0,0 }, 0.0f, WHITE);
        }
        DrawRectangleLinesEx(greenKoopaRect, 2, (isEnemy() && getEnemyType() == EnemyType::GREEN_KOOPA) ? RED : BLACK);

        // Red Koopa
        Rectangle redKoopaRect = { startX + 2 * spacingX, startY, iconSize, iconSize };
        if (tm.enemyTextures.id != 0 && !tm.Enemy_sprite_boxes.empty()) {
            Rectangle redKoopaSource = tm.Enemy_sprite_boxes[53];
            DrawTexturePro(tm.enemyTextures, redKoopaSource, redKoopaRect, { 0,0 }, 0.0f, WHITE);
        }
        DrawRectangleLinesEx(redKoopaRect, 2, (isEnemy() && getEnemyType() == EnemyType::RED_KOOPA) ? RED : BLACK);

        // DryBowser
        Rectangle dryBRect = { startX + 3 * spacingX, startY, iconSize, iconSize };
        if (tm.DryBowser_texture.id != 0 && !tm.DryBowser_sprite_boxes.empty()) {
            Rectangle dryBSource = tm.DryBowser_sprite_boxes[2];
            DrawTexturePro(tm.DryBowser_texture, dryBSource, dryBRect, { 0,0 }, 0.0f, WHITE);
        }
        DrawRectangleLinesEx(dryBRect, 2, (isEnemy() && getEnemyType() == EnemyType::DRY_BOWSER) ? RED : BLACK);

    }
    if (selectRect == 3) {
        // Spring
        Rectangle springRect = { startX, startY, iconSize, iconSize };
        Rectangle springSource = { 1, 467, 16, 16 };
        DrawTexturePro(TextureManager::interactiveTextures, springSource, springRect, { 0, 0 }, 0.0f, WHITE);
        DrawRectangleLinesEx(springRect, 2, (isInteractive() && getInteractiveType() == InteractiveType::SPRING) ? RED : BLACK);

    }
    if (selectRect == 4) {
        // Coin
        Rectangle coinRect = { startX, startY, iconSize, iconSize };
        Rectangle coinSource = { 0, 0, 100, 100 };
        DrawTexturePro(tm.getItemTexture(), coinSource, coinRect, { 0, 0 }, 0.0f, WHITE);
        DrawRectangleLinesEx(coinRect, 2, (isItem() && getItemType() == ItemType::COIN) ? RED : BLACK);

        // Fire_flower
        Rectangle ffRect = { startX + spacingX, startY, iconSize, iconSize };
        Rectangle ffSource = { 0, 200, 100, 100 };
        DrawTexturePro(tm.getItemTexture(), ffSource, ffRect, { 0, 0 }, 0.0f, WHITE);
        DrawRectangleLinesEx(ffRect, 2, (isItem() && getItemType() == ItemType::FIRE_FLOWER) ? RED : BLACK);
    }

}

void ObjectPalette::handleSelection() {
    //if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return;

    Vector2 mousePos = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, rect)) {
        if (isAtA) {
            target = B;
        }
        else {
            target = A;
        }
        isAtA = !isAtA;
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, blockRect)) selectRect = 1;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, enemyRect)) selectRect = 2;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, objectRect)) selectRect = 3;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, itemRect)) selectRect = 4;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, map1Rect)) map = 1;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, map2Rect)) map = 2;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, map3Rect)) map = 3;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, map4Rect)) map = 4;
    if (GetMouseWheelMove() != 0.0f && CheckCollisionPointRec(mousePos, paletteRect)) {
        if (paletteRect.y + GetMouseWheelMove() <= -20 && GetMouseWheelMove() >= 0) target.y += GetMouseWheelMove() * 10;
        if (paletteRect.y + paletteRect.height + GetMouseWheelMove() >= GetScreenHeight() + 20 && GetMouseWheelMove() <= 0) target.y += GetMouseWheelMove() * 10;
    }

    if (!CheckCollisionPointRec(mousePos, paletteRect)) return;

    // Block selection rectangles
    Rectangle groundRect = { startX, startY, iconSize, iconSize };
    Rectangle brickRect = { startX + spacingX, startY, iconSize, iconSize };
    // Enemy selection rectangles
    Rectangle goombaRect = { startX, startY, iconSize, iconSize };
    Rectangle gkoopaRect = { startX + spacingX, startY, iconSize, iconSize };
    Rectangle rkoopaRect = { startX + 2 * spacingX, startY, iconSize, iconSize };
    Rectangle dryBRect = { startX + 3 * spacingX, startY, iconSize, iconSize };

    Rectangle springRect = { startX, startY, iconSize, iconSize };

    Rectangle coinRect = { startX, startY, iconSize, iconSize };
    Rectangle ffRect = { startX + spacingX, startY, iconSize, iconSize };
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        int traverse = 3;
        if (CheckCollisionPointRec(mousePos, groundRect) && selectRect == 1) selected = BlockType::GROUND;
        if (CheckCollisionPointRec(mousePos, brickRect) && selectRect == 1) selected = BlockType::BRICK;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_1_2; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_1_3; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_1_12; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_1_13; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_1_14; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_1_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_1_16; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_2_7; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_2_8; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_2_12; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_2_13; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_2_14; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_2_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_2_16; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_3_4; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_3_5; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_3_6; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_3_7; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_3_8; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_3_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_3_16; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_4_4; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_4_5; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_4_6; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_4_7; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_4_11; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_4_12; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_4_13; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_4_14; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_4_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_4_16; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_5_4; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_5_5; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_5_6; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_5_8; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_5_9; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_5_10; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_5_11; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_5_12; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_5_13; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_5_14; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_5_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_5_16; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_6_5; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_6_6; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_7_1; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_7_2; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_7_3; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_7_4; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_7_5; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_7_6; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_7_7; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_7_11; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_7_12; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_7_13; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_7_14; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_7_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 1) selected = BlockType::BLOCK_1_7_16; traverse++;
        traverse = 3;
        if (CheckCollisionPointRec(mousePos, groundRect) && selectRect == 1) selected = BlockType::GROUND;
        if (CheckCollisionPointRec(mousePos, brickRect) && selectRect == 1) selected = BlockType::BRICK;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_1_2; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_1_3; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_1_12; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_1_13; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_1_14; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_1_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_1_16; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_2_7; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_2_8; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_2_12; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_2_13; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_2_14; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_2_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_2_16; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_3_4; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_3_5; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_3_6; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_3_7; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_3_8; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_3_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_3_16; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_4_4; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_4_5; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_4_6; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_4_7; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_4_11; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_4_12; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_4_13; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_4_14; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_4_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_4_16; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_5_4; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_5_5; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_5_6; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_5_8; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_5_9; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_5_10; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_5_11; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_5_12; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_5_13; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_5_14; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_5_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_5_16; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_6_5; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_6_6; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_7_1; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_7_2; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_7_3; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_7_4; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_7_5; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_7_6; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_7_7; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_7_11; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_7_12; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_7_13; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_7_14; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_7_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 2) selected = BlockType::BLOCK_2_7_16; traverse++;
        traverse = 3;
        if (CheckCollisionPointRec(mousePos, groundRect) && selectRect == 1) selected = BlockType::GROUND;
        if (CheckCollisionPointRec(mousePos, brickRect) && selectRect == 1) selected = BlockType::BRICK;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_1_2; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_1_3; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_1_12; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_1_13; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_1_14; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_1_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_1_16; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_2_7; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_2_8; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_2_12; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_2_13; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_2_14; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_2_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_2_16; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_3_4; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_3_5; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_3_6; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_3_7; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_3_8; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_3_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_3_16; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_4_4; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_4_5; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_4_6; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_4_7; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_4_11; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_4_12; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_4_13; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_4_14; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_4_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_4_16; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_5_4; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_5_5; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_5_6; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_5_8; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_5_9; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_5_10; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_5_11; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_5_12; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_5_13; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_5_14; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_5_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_5_16; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_6_5; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_6_6; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_7_1; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_7_2; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_7_3; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_7_4; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_7_5; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_7_6; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_7_7; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_7_11; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_7_12; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_7_13; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_7_14; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_7_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 3) selected = BlockType::BLOCK_3_7_16; traverse++;

        traverse = 3;
        if (CheckCollisionPointRec(mousePos, groundRect) && selectRect == 1) selected = BlockType::GROUND;
        if (CheckCollisionPointRec(mousePos, brickRect) && selectRect == 1) selected = BlockType::BRICK;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_1_2; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_1_3; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_1_12; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_1_13; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_1_14; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_1_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_1_16; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_2_7; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_2_8; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_2_12; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_2_13; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_2_14; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_2_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_2_16; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_3_4; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_3_5; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_3_6; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_3_7; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_3_8; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_3_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_3_16; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_4_4; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_4_5; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_4_6; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_4_7; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_4_11; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_4_12; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_4_13; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_4_14; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_4_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_4_16; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_5_4; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_5_5; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_5_6; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_5_8; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_5_9; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_5_10; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_5_11; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_5_12; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_5_13; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_5_14; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_5_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_5_16; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_6_5; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_6_6; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_7_1; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_7_2; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_7_3; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_7_4; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_7_5; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_7_6; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_7_7; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_7_11; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_7_12; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_7_13; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_7_14; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_7_15; traverse++;
        if (CheckCollisionPointRec(mousePos, ordinalRect(traverse)) && selectRect == 1 && map == 4) selected = BlockType::BLOCK_4_7_16; traverse++;

        if (CheckCollisionPointRec(mousePos, goombaRect) && selectRect == 2) selected = EnemyType::GOOMBA;
        if (CheckCollisionPointRec(mousePos, gkoopaRect) && selectRect == 2) selected = EnemyType::GREEN_KOOPA;
        if (CheckCollisionPointRec(mousePos, rkoopaRect) && selectRect == 2) selected = EnemyType::RED_KOOPA;
        if (CheckCollisionPointRec(mousePos, dryBRect) && selectRect == 2) selected = EnemyType::DRY_BOWSER;
        if (CheckCollisionPointRec(mousePos, springRect) && selectRect == 3) selected = InteractiveType::SPRING;
        if (CheckCollisionPointRec(mousePos, coinRect) && selectRect == 4) selected = ItemType::COIN;
        if (CheckCollisionPointRec(mousePos, ffRect) && selectRect == 4) selected = ItemType::FIRE_FLOWER;
    }

}
