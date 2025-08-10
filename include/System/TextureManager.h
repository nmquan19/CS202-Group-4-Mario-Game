#pragma once

#include <map>
#include "raylib.h"
#include "Interface.h"
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
    Rectangle rect = { A.x, A.y, 20, (float)GetScreenHeight() / 2.0f };
    Rectangle paletteRect = { rect.x + 20, rect.y - (float)GetScreenHeight() / 4.0f, (float)GetScreenWidth(), (float)GetScreenHeight() };    ObjectType selected = BlockType::GROUND;
    float startX = paletteRect.x + 50;
    float yBlock = paletteRect.y + 50;
    float yEnemy = yBlock + 150;
    float yInteractive = yEnemy + 150;
    float yItem = yInteractive + 150;
    float spacing = 120;    
    float iconSize = 100;
};
