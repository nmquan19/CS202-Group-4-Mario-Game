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
    Texture2D getBlockTexture(BlockType type);
    Texture2D getCharacterTexture(CharacterType type);
    void loadTextures();
    void unloadTextures();

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

private:
    TextureManager() = default;
   
    std::map<BlockType, Texture2D> blockTextures;
    std::map<CharacterType, Texture2D> characterTextures;
 
    bool texturesLoaded = false;
};

class ObjectPalette {
public:
    void drawPalette();
    void handleSelection();

    bool isBlock() const { return std::holds_alternative<BlockType>(selected); }
    bool isEnemy() const { return std::holds_alternative<EnemyType>(selected); }
    bool isCharacter() const {return std::holds_alternative<CharacterType>(selected); }

    BlockType getBlockType() const { return std::get<BlockType>(selected); }
    EnemyType getEnemyType() const { return std::get<EnemyType>(selected); }
    CharacterType getCharacterType() const {return std::get<CharacterType>(selected); }

    Rectangle getPaletteRect() const { return paletteRect; }
	ObjectType getSelectedType() const { return selected; }
private:
    Rectangle paletteRect = { (float)GetScreenWidth() - 320, 50, 300, 200 };
    ObjectType selected = BlockType::GROUND;
};
