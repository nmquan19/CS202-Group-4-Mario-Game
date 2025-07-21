#pragma once

#include <map>
#include "raylib.h"
#include "Interface.h"
#include <variant>
#include <vector>
class TextureManager {
public:
    static TextureManager& getInstance();

    static std::vector<Rectangle> Enemy_sprite_boxes;
	static Texture2D DryBowser_texture;    
    static std::vector<Rectangle> DryBowser_sprite_boxes;

    static Texture2D enemyTextures;

    static std::vector<Rectangle> Item_sprite_boxes;
    static Texture2D itemTextures;
    Texture2D getItemTexture(ItemType type) const;

    Texture2D getBlockTexture(BlockType type);
    Texture2D getCharacterTexture() const;
    void loadTextures();
    void unloadTextures();

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    //Template for testing
    int getSizeofItemSprite() { return Item_sprite_boxes.size(); }

private:
    TextureManager() = default;
   
    std::map<BlockType, Texture2D> blockTextures;
    Texture2D characterTextures;

    bool texturesLoaded = false;
	bool itemTexturesLoaded = false;
};

class ObjectPalette {
public:
    void drawPalette();
    void handleSelection();

    bool isBlock() const { return std::holds_alternative<BlockType>(selected); }
    bool isEnemy() const { return std::holds_alternative<EnemyType>(selected); }
    bool isCharacter() const {return std::holds_alternative<CharacterType>(selected); }
    bool isItem() const { return std::holds_alternative<ItemType>(selected); }

    BlockType getBlockType() const { return std::get<BlockType>(selected); }
    EnemyType getEnemyType() const { return std::get<EnemyType>(selected); }
    CharacterType getCharacterType() const {return std::get<CharacterType>(selected); }
    ItemType getItemType() const { return std::get<ItemType>(selected); }

    Rectangle getPaletteRect() const { return paletteRect; }
	ObjectType getSelectedType() const { return selected; }


private:
    Rectangle paletteRect = { (float)GetScreenWidth() - 320, 50, 300, 200 };
    ObjectType selected = BlockType::GROUND;
};
