#pragma once

#include <map>
#include "raylib.h"
#include "Interface.h"
#include "../UI/UI.h"
#include <variant>
#include <vector>
#include <unordered_map>
class TextureManager {
public:
    static TextureManager& getInstance();

    static std::vector<Rectangle> Enemy_sprite_boxes;
	static Texture2D DryBowser_texture;   
    static std::vector<Rectangle> DryBowser_sprite_boxes;
    static Texture2D blocksTexture;

    static Texture2D day_groundTexture;
    static Texture2D day_undergroundTexture;
    static Texture2D night_airshipTexture;
    static Texture2D night_snowTexture;

    static Texture2D enemyTextures;
    static Texture2D interactiveTextures;

    static std::vector<Rectangle> Item_sprite_boxes;
    static Texture2D itemTextures;
    Texture2D getItemTexture(ItemType type) const;
    static Texture2D fontTexture;
    static std::unordered_map<char, Rectangle> fontSprites;

    static Texture2D background_lv1;
    
    Texture2D getCharacterTexture() const;
    Texture2D getItemTexture() const;
    void loadTextures(); 
    void unloadTextures();

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    //Template for testing
    int getSizeofItemSprite() { return Item_sprite_boxes.size(); }

private:
    TextureManager() = default;
    Texture2D characterTextures;

    bool texturesLoaded = false;
	bool itemTexturesLoaded = false;
};

class ObjectPalette {
public:
    void drawPalette();
    void updatePalette(float deltaTime);
    void handleSelection();

    bool isBlock() const { return std::holds_alternative<BlockType>(selected); }
    bool isEnemy() const { return std::holds_alternative<EnemyType>(selected); }
    bool isInteractive() const { return std::holds_alternative<InteractiveType>(selected); }
    bool isItem() const { return std::holds_alternative<ItemType>(selected); }

    BlockType getBlockType() const { return std::get<BlockType>(selected); }
    EnemyType getEnemyType() const { return std::get<EnemyType>(selected); }
    InteractiveType getInteractiveType() const { return std::get<InteractiveType>(selected); }
    ItemType getItemType() const { return std::get<ItemType>(selected); }

    Rectangle getPaletteRect() const { return paletteRect; }
	ObjectType getSelectedType() const { return selected; }


private:
    Vector2 A = { (float)GetScreenWidth() - 500 - 20, (float)GetScreenHeight() / 4.0f };
    Vector2 B = { (float)GetScreenWidth() - 20, (float)GetScreenHeight() / 4.0f };
    Vector2 target = A;

    bool isAtA = true;
    Rectangle rect = { A.x, (float)GetScreenHeight() / 4.0f, 20, (float)GetScreenHeight() / 2.0f };
    Rectangle blockRect = { rect.x + 20, 0, 40, (float)GetScreenHeight() / 4.0f };
    Rectangle enemyRect = { rect.x + 20, (float)GetScreenHeight() / 4.0f, 40, (float)GetScreenHeight() / 4.0f };
    Rectangle objectRect = { rect.x + 20, (float)GetScreenHeight() * 2.0f / 4.0f, 40, (float)GetScreenHeight() / 4.0f };
    Rectangle itemRect = { rect.x + 20, (float)GetScreenHeight() * 3.0f / 4.0f, 40, (float)GetScreenHeight() / 4.0f };

    Rectangle map1Rect = { rect.x + 60, 0, ((float)GetScreenWidth() - rect.x - 60) / 4.0f, 40 };
    Rectangle map2Rect = { rect.x + 60 + ((float)GetScreenWidth() - rect.x - 60) / 4.0f, 0, ((float)GetScreenWidth() - rect.x - 60) / 4.0f, 40 };
    Rectangle map3Rect = { rect.x + 60 + ((float)GetScreenWidth() - rect.x - 60) * 2.0f / 4.0f, 0, ((float)GetScreenWidth() - rect.x - 60) / 4.0f, 40 };
    Rectangle map4Rect = { rect.x + 60 + ((float)GetScreenWidth() - rect.x - 60) * 3.0f / 4.0f, 0, ((float)GetScreenWidth() - rect.x - 60) / 4.0f, 40 };
    int map = 1;
    int selectRect = 1;
    Rectangle paletteRect = { rect.x + 20, target.y - (float)GetScreenHeight() / 4.0f, (float)GetScreenWidth(), (float)GetScreenHeight() * 10.0f };
    ObjectType selected = BlockType::GROUND;
    float startX = paletteRect.x + 150;
    float startY = paletteRect.y + 50;
    float spacingX = 120;
    float spacingY = 120;
    float iconSize = 100;

    Rectangle ordinalRect(int x) {
        Rectangle result = { startX + ((x - 1) % 4) * spacingX, startY + ((x - 1) / 4) * spacingY, iconSize, iconSize };
        return result;
    }
};
