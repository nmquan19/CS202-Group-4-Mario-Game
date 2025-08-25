#include "../../include/Objects/Block.h"
#include "../../include/System/Grid.h"
#include "../../include/System/TextureManager.h"
#include "../../include/System/Constant.h"
#include <vector>
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/System/Interface.h"
#include "../../include/System/Box2DWorldManager.h"
#include "../../include/System/ParticleSystem.h"
#include "../../include/Game/GameContext.h"
#include <raylib.h>
#include <iostream>

Block::Block(Vector2 gridPos, BlockType type, Vector2 s) : blockType(type), isMoving(false) {
    gridPosition = gridPos;
    position = { gridPos.x * GridSystem::GRID_SIZE, gridPos.y * GridSystem::GRID_SIZE };
    originalPos = position;
    size = s;
    hitbox = {position.x, position.y, size.x*GridSystem::GRID_SIZE, size.y*GridSystem::GRID_SIZE};

    physicsBody = Box2DWorldManager::getInstance().createBlockBody(position, { hitbox.width, hitbox.height });
    if (physicsBody) {
        physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
        for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            b2Filter filter = fixture->GetFilterData();
            filter.maskBits = static_cast<uint16>(ObjectCategory::BLOCK);
            filter.categoryBits = static_cast<uint16>(ObjectCategory::CHARACTER) | static_cast<uint16>(ObjectCategory::ENEMY) |
                                static_cast<uint16>(ObjectCategory::PROJECTILE) | static_cast<uint16>(ObjectCategory::SHELL) |
                                static_cast<uint16>(ObjectCategory::BLOCK) | static_cast<uint16>(ObjectCategory::ITEM);
            fixture->SetFilterData(filter);
        }
    }
}

Block::~Block() {
    if (physicsBody) {
        Box2DWorldManager::getInstance().destroyBody(physicsBody);
        physicsBody = nullptr;
    }
}

void Block::draw() {
    if (!active) return;

    Rectangle destRec = { position.x, position.y, hitbox.width, hitbox.height };
    Rectangle srcRec = getSrcRect();
    
    Texture2D textureToUse = getTextureSprite();

    DrawTexturePro(textureToUse, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
}

Texture2D Block::getTextureSprite() const {
    if (static_cast<int>(blockType) >= static_cast<int>(BlockType::BLOCK_1_1_2) &&
        static_cast<int>(blockType) <= static_cast<int>(BlockType::BLOCK_1_16_16)) {
        return TextureManager::day_groundTexture;
    }
    else if (static_cast<int>(blockType) >= static_cast<int>(BlockType::BLOCK_2_1_2) &&
        static_cast<int>(blockType) <= static_cast<int>(BlockType::BLOCK_2_16_16)) {
        return TextureManager::day_undergroundTexture;
    }
    else if (static_cast<int>(blockType) >= static_cast<int>(BlockType::BLOCK_3_1_2) &&
        static_cast<int>(blockType) <= static_cast<int>(BlockType::BLOCK_3_16_16)) {
        return TextureManager::night_airshipTexture;
    }
    return TextureManager::night_snowTexture;
}

Rectangle Block::getSrcRect() const {
    switch (blockType) {
        case BlockType::BLOCK_1_1_2: return Constants::PaletteResources::BLOCK_1_1_2;
        case BlockType::BLOCK_1_1_3: {
            if (!itemSpawned) return Constants::PaletteResources::BLOCK_1_1_3;
            return Constants::PaletteResources::BLOCK_1_1_4;
        }
        case BlockType::BLOCK_1_1_12: return Constants::PaletteResources::BLOCK_1_1_12;
        case BlockType::BLOCK_1_1_13: return Constants::PaletteResources::BLOCK_1_1_13;
        case BlockType::BLOCK_1_1_14: return Constants::PaletteResources::BLOCK_1_1_14;
        case BlockType::BLOCK_1_1_15: return Constants::PaletteResources::BLOCK_1_1_15;
        case BlockType::BLOCK_1_1_16: return Constants::PaletteResources::BLOCK_1_1_16;
        case BlockType::BLOCK_1_2_7: return Constants::PaletteResources::BLOCK_1_2_7;
        case BlockType::BLOCK_1_2_8: return Constants::PaletteResources::BLOCK_1_2_8;
        case BlockType::BLOCK_1_2_12: return Constants::PaletteResources::BLOCK_1_2_12;
        case BlockType::BLOCK_1_2_13: return Constants::PaletteResources::BLOCK_1_2_13;
        case BlockType::BLOCK_1_2_14: return Constants::PaletteResources::BLOCK_1_2_14;
        case BlockType::BLOCK_1_2_15: return Constants::PaletteResources::BLOCK_1_2_15;
        case BlockType::BLOCK_1_2_16: return Constants::PaletteResources::BLOCK_1_2_16;
        case BlockType::BLOCK_1_3_4: return Constants::PaletteResources::BLOCK_1_3_4;
        case BlockType::BLOCK_1_3_5: return Constants::PaletteResources::BLOCK_1_3_5;
        case BlockType::BLOCK_1_3_6: return Constants::PaletteResources::BLOCK_1_3_6;
        case BlockType::BLOCK_1_3_7: return Constants::PaletteResources::BLOCK_1_3_7;
        case BlockType::BLOCK_1_3_8: return Constants::PaletteResources::BLOCK_1_3_8;
        case BlockType::BLOCK_1_3_15: return Constants::PaletteResources::BLOCK_1_3_15;
        case BlockType::BLOCK_1_3_16: return Constants::PaletteResources::BLOCK_1_3_16;
        case BlockType::BLOCK_1_4_4: return Constants::PaletteResources::BLOCK_1_4_4;
        case BlockType::BLOCK_1_4_5: return Constants::PaletteResources::BLOCK_1_4_5;
        case BlockType::BLOCK_1_4_6: return Constants::PaletteResources::BLOCK_1_4_6;
        case BlockType::BLOCK_1_4_7: return Constants::PaletteResources::BLOCK_1_4_7;
        case BlockType::BLOCK_1_4_11: return Constants::PaletteResources::BLOCK_1_4_11;
        case BlockType::BLOCK_1_4_12: return Constants::PaletteResources::BLOCK_1_4_12;
        case BlockType::BLOCK_1_4_13: return Constants::PaletteResources::BLOCK_1_4_13;
        case BlockType::BLOCK_1_4_14: return Constants::PaletteResources::BLOCK_1_4_14;
        case BlockType::BLOCK_1_4_15: return Constants::PaletteResources::BLOCK_1_4_15;
        case BlockType::BLOCK_1_4_16: return Constants::PaletteResources::BLOCK_1_4_16;
        case BlockType::BLOCK_1_5_4: return Constants::PaletteResources::BLOCK_1_5_4;
        case BlockType::BLOCK_1_5_5: return Constants::PaletteResources::BLOCK_1_5_5;
        case BlockType::BLOCK_1_5_6: return Constants::PaletteResources::BLOCK_1_5_6;
        case BlockType::BLOCK_1_5_8: return Constants::PaletteResources::BLOCK_1_5_8;
        case BlockType::BLOCK_1_5_9: return Constants::PaletteResources::BLOCK_1_5_9;
        case BlockType::BLOCK_1_5_10: return Constants::PaletteResources::BLOCK_1_5_10;
        case BlockType::BLOCK_1_5_11: return Constants::PaletteResources::BLOCK_1_5_11;
        case BlockType::BLOCK_1_5_12: return Constants::PaletteResources::BLOCK_1_5_12;
        case BlockType::BLOCK_1_5_13: return Constants::PaletteResources::BLOCK_1_5_13;
        case BlockType::BLOCK_1_5_14: return Constants::PaletteResources::BLOCK_1_5_14;
        case BlockType::BLOCK_1_5_15: return Constants::PaletteResources::BLOCK_1_5_15;
        case BlockType::BLOCK_1_5_16: return Constants::PaletteResources::BLOCK_1_5_16;
        case BlockType::BLOCK_1_6_5: return Constants::PaletteResources::BLOCK_1_6_5;
        case BlockType::BLOCK_1_6_6: return Constants::PaletteResources::BLOCK_1_6_6;
        case BlockType::BLOCK_1_7_1: return Constants::PaletteResources::BLOCK_1_7_1;
        case BlockType::BLOCK_1_7_2: return Constants::PaletteResources::BLOCK_1_7_2;
        case BlockType::BLOCK_1_7_3: return Constants::PaletteResources::BLOCK_1_7_3;
        case BlockType::BLOCK_1_7_4: return Constants::PaletteResources::BLOCK_1_7_4;
        case BlockType::BLOCK_1_7_5: return Constants::PaletteResources::BLOCK_1_7_5;
        case BlockType::BLOCK_1_7_6: return Constants::PaletteResources::BLOCK_1_7_6;
        case BlockType::BLOCK_1_7_7: return Constants::PaletteResources::BLOCK_1_7_7;
        case BlockType::BLOCK_1_7_11: return Constants::PaletteResources::BLOCK_1_7_11;
        case BlockType::BLOCK_1_7_12: return Constants::PaletteResources::BLOCK_1_7_12;
        case BlockType::BLOCK_1_7_13: return Constants::PaletteResources::BLOCK_1_7_13;
        case BlockType::BLOCK_1_7_14: return Constants::PaletteResources::BLOCK_1_7_14;
        case BlockType::BLOCK_1_7_15: return Constants::PaletteResources::BLOCK_1_7_15;
        case BlockType::BLOCK_1_7_16: return Constants::PaletteResources::BLOCK_1_7_16;
        case BlockType::BLOCK_1_13_1: return Constants::PaletteResources::BLOCK_1_13_1;
        case BlockType::BLOCK_1_13_2: return Constants::PaletteResources::BLOCK_1_13_2;
        case BlockType::BLOCK_1_13_3: return Constants::PaletteResources::BLOCK_1_13_3;
        case BlockType::BLOCK_1_13_4: return Constants::PaletteResources::BLOCK_1_13_4;
        case BlockType::BLOCK_1_13_5: return Constants::PaletteResources::BLOCK_1_13_5;
        case BlockType::BLOCK_1_13_6: return Constants::PaletteResources::BLOCK_1_13_6;
        case BlockType::BLOCK_1_13_7: return Constants::PaletteResources::BLOCK_1_13_7;
        case BlockType::BLOCK_1_13_8: return Constants::PaletteResources::BLOCK_1_13_8;
        case BlockType::BLOCK_1_13_9: return Constants::PaletteResources::BLOCK_1_13_9;
        case BlockType::BLOCK_1_13_10: return Constants::PaletteResources::BLOCK_1_13_10;
        case BlockType::BLOCK_1_13_11: return Constants::PaletteResources::BLOCK_1_13_11;
        case BlockType::BLOCK_1_13_12: return Constants::PaletteResources::BLOCK_1_13_12;
        case BlockType::BLOCK_1_13_13: return Constants::PaletteResources::BLOCK_1_13_13;
        case BlockType::BLOCK_1_13_14: return Constants::PaletteResources::BLOCK_1_13_14;
        case BlockType::BLOCK_1_13_15: return Constants::PaletteResources::BLOCK_1_13_15;
        case BlockType::BLOCK_1_13_16: return Constants::PaletteResources::BLOCK_1_13_16;
        case BlockType::BLOCK_1_14_1: return Constants::PaletteResources::BLOCK_1_14_1;
        case BlockType::BLOCK_1_14_2: return Constants::PaletteResources::BLOCK_1_14_2;
        case BlockType::BLOCK_1_14_3: return Constants::PaletteResources::BLOCK_1_14_3;
        case BlockType::BLOCK_1_14_4: return Constants::PaletteResources::BLOCK_1_14_4;
        case BlockType::BLOCK_1_14_5: return Constants::PaletteResources::BLOCK_1_14_5;
        case BlockType::BLOCK_1_14_6: return Constants::PaletteResources::BLOCK_1_14_6;
        case BlockType::BLOCK_1_14_7: return Constants::PaletteResources::BLOCK_1_14_7;
        case BlockType::BLOCK_1_14_8: return Constants::PaletteResources::BLOCK_1_14_8;
        case BlockType::BLOCK_1_14_9: return Constants::PaletteResources::BLOCK_1_14_9;
        case BlockType::BLOCK_1_14_10: return Constants::PaletteResources::BLOCK_1_14_10;
        case BlockType::BLOCK_1_14_11: return Constants::PaletteResources::BLOCK_1_14_11;
        case BlockType::BLOCK_1_14_12: return Constants::PaletteResources::BLOCK_1_14_12;
        case BlockType::BLOCK_1_14_13: return Constants::PaletteResources::BLOCK_1_14_13;
        case BlockType::BLOCK_1_14_14: return Constants::PaletteResources::BLOCK_1_14_14;
        case BlockType::BLOCK_1_14_15: return Constants::PaletteResources::BLOCK_1_14_15;
        case BlockType::BLOCK_1_14_16: return Constants::PaletteResources::BLOCK_1_14_16;
        case BlockType::BLOCK_1_15_1: return Constants::PaletteResources::BLOCK_1_15_1;
        case BlockType::BLOCK_1_15_2: return Constants::PaletteResources::BLOCK_1_15_2;
        case BlockType::BLOCK_1_15_3: return Constants::PaletteResources::BLOCK_1_15_3;
        case BlockType::BLOCK_1_15_4: return Constants::PaletteResources::BLOCK_1_15_4;
        case BlockType::BLOCK_1_15_5: return Constants::PaletteResources::BLOCK_1_15_5;
        case BlockType::BLOCK_1_15_6: return Constants::PaletteResources::BLOCK_1_15_6;
        case BlockType::BLOCK_1_15_7: return Constants::PaletteResources::BLOCK_1_15_7;
        case BlockType::BLOCK_1_15_8: return Constants::PaletteResources::BLOCK_1_15_8;
        case BlockType::BLOCK_1_15_9: return Constants::PaletteResources::BLOCK_1_15_9;
        case BlockType::BLOCK_1_15_10: return Constants::PaletteResources::BLOCK_1_15_10;
        case BlockType::BLOCK_1_15_11: return Constants::PaletteResources::BLOCK_1_15_11;
        case BlockType::BLOCK_1_15_12: return Constants::PaletteResources::BLOCK_1_15_12;
        case BlockType::BLOCK_1_15_13: return Constants::PaletteResources::BLOCK_1_15_13;
        case BlockType::BLOCK_1_15_14: return Constants::PaletteResources::BLOCK_1_15_14;
        case BlockType::BLOCK_1_15_15: return Constants::PaletteResources::BLOCK_1_15_15;
        case BlockType::BLOCK_1_15_16: return Constants::PaletteResources::BLOCK_1_15_16;
        case BlockType::BLOCK_1_16_1: return Constants::PaletteResources::BLOCK_1_16_1;
        case BlockType::BLOCK_1_16_2: return Constants::PaletteResources::BLOCK_1_16_2;
        case BlockType::BLOCK_1_16_3: return Constants::PaletteResources::BLOCK_1_16_3;
        case BlockType::BLOCK_1_16_4: return Constants::PaletteResources::BLOCK_1_16_4;
        case BlockType::BLOCK_1_16_5: return Constants::PaletteResources::BLOCK_1_16_5;
        case BlockType::BLOCK_1_16_6: return Constants::PaletteResources::BLOCK_1_16_6;
        case BlockType::BLOCK_1_16_7: return Constants::PaletteResources::BLOCK_1_16_7;
        case BlockType::BLOCK_1_16_8: return Constants::PaletteResources::BLOCK_1_16_8;
        case BlockType::BLOCK_1_16_9: return Constants::PaletteResources::BLOCK_1_16_9;
        case BlockType::BLOCK_1_16_10: return Constants::PaletteResources::BLOCK_1_16_10;
        case BlockType::BLOCK_1_16_11: return Constants::PaletteResources::BLOCK_1_16_11;
        case BlockType::BLOCK_1_16_12: return Constants::PaletteResources::BLOCK_1_16_12;
        case BlockType::BLOCK_1_16_13: return Constants::PaletteResources::BLOCK_1_16_13;
        case BlockType::BLOCK_1_16_14: return Constants::PaletteResources::BLOCK_1_16_14;
        case BlockType::BLOCK_1_16_15: return Constants::PaletteResources::BLOCK_1_16_15;
        case BlockType::BLOCK_1_16_16: return Constants::PaletteResources::BLOCK_1_16_16;
        
        case BlockType::BLOCK_2_1_2: return Constants::PaletteResources::BLOCK_1_1_2;
        case BlockType::BLOCK_2_1_3: {
            if (!itemSpawned) return Constants::PaletteResources::BLOCK_1_1_3;
            return Constants::PaletteResources::BLOCK_1_1_4;
        }
        case BlockType::BLOCK_2_1_12: return Constants::PaletteResources::BLOCK_1_1_12;
        case BlockType::BLOCK_2_1_13: return Constants::PaletteResources::BLOCK_1_1_13;
        case BlockType::BLOCK_2_1_14: return Constants::PaletteResources::BLOCK_1_1_14;
        case BlockType::BLOCK_2_1_15: return Constants::PaletteResources::BLOCK_1_1_15;
        case BlockType::BLOCK_2_1_16: return Constants::PaletteResources::BLOCK_1_1_16;
        case BlockType::BLOCK_2_2_7: return Constants::PaletteResources::BLOCK_1_2_7;
        case BlockType::BLOCK_2_2_8: return Constants::PaletteResources::BLOCK_1_2_8;
        case BlockType::BLOCK_2_2_12: return Constants::PaletteResources::BLOCK_1_2_12;
        case BlockType::BLOCK_2_2_13: return Constants::PaletteResources::BLOCK_1_2_13;
        case BlockType::BLOCK_2_2_14: return Constants::PaletteResources::BLOCK_1_2_14;
        case BlockType::BLOCK_2_2_15: return Constants::PaletteResources::BLOCK_1_2_15;
        case BlockType::BLOCK_2_2_16: return Constants::PaletteResources::BLOCK_1_2_16;
        case BlockType::BLOCK_2_3_4: return Constants::PaletteResources::BLOCK_1_3_4;
        case BlockType::BLOCK_2_3_5: return Constants::PaletteResources::BLOCK_1_3_5;
        case BlockType::BLOCK_2_3_6: return Constants::PaletteResources::BLOCK_1_3_6;
        case BlockType::BLOCK_2_3_7: return Constants::PaletteResources::BLOCK_1_3_7;
        case BlockType::BLOCK_2_3_8: return Constants::PaletteResources::BLOCK_1_3_8;
        case BlockType::BLOCK_2_3_15: return Constants::PaletteResources::BLOCK_1_3_15;
        case BlockType::BLOCK_2_3_16: return Constants::PaletteResources::BLOCK_1_3_16;
        case BlockType::BLOCK_2_4_4: return Constants::PaletteResources::BLOCK_1_4_4;
        case BlockType::BLOCK_2_4_5: return Constants::PaletteResources::BLOCK_1_4_5;
        case BlockType::BLOCK_2_4_6: return Constants::PaletteResources::BLOCK_1_4_6;
        case BlockType::BLOCK_2_4_7: return Constants::PaletteResources::BLOCK_1_4_7;
        case BlockType::BLOCK_2_4_11: return Constants::PaletteResources::BLOCK_1_4_11;
        case BlockType::BLOCK_2_4_12: return Constants::PaletteResources::BLOCK_1_4_12;
        case BlockType::BLOCK_2_4_13: return Constants::PaletteResources::BLOCK_1_4_13;
        case BlockType::BLOCK_2_4_14: return Constants::PaletteResources::BLOCK_1_4_14;
        case BlockType::BLOCK_2_4_15: return Constants::PaletteResources::BLOCK_1_4_15;
        case BlockType::BLOCK_2_4_16: return Constants::PaletteResources::BLOCK_1_4_16;
        case BlockType::BLOCK_2_5_4: return Constants::PaletteResources::BLOCK_1_5_4;
        case BlockType::BLOCK_2_5_5: return Constants::PaletteResources::BLOCK_1_5_5;
        case BlockType::BLOCK_2_5_6: return Constants::PaletteResources::BLOCK_1_5_6;
        case BlockType::BLOCK_2_5_8: return Constants::PaletteResources::BLOCK_1_5_8;
        case BlockType::BLOCK_2_5_9: return Constants::PaletteResources::BLOCK_1_5_9;
        case BlockType::BLOCK_2_5_10: return Constants::PaletteResources::BLOCK_1_5_10;
        case BlockType::BLOCK_2_5_11: return Constants::PaletteResources::BLOCK_1_5_11;
        case BlockType::BLOCK_2_5_12: return Constants::PaletteResources::BLOCK_1_5_12;
        case BlockType::BLOCK_2_5_13: return Constants::PaletteResources::BLOCK_1_5_13;
        case BlockType::BLOCK_2_5_14: return Constants::PaletteResources::BLOCK_1_5_14;
        case BlockType::BLOCK_2_5_15: return Constants::PaletteResources::BLOCK_1_5_15;
        case BlockType::BLOCK_2_5_16: return Constants::PaletteResources::BLOCK_1_5_16;
        case BlockType::BLOCK_2_6_5: return Constants::PaletteResources::BLOCK_1_6_5;
        case BlockType::BLOCK_2_6_6: return Constants::PaletteResources::BLOCK_1_6_6;
        case BlockType::BLOCK_2_7_1: return Constants::PaletteResources::BLOCK_1_7_1;
        case BlockType::BLOCK_2_7_2: return Constants::PaletteResources::BLOCK_1_7_2;
        case BlockType::BLOCK_2_7_3: return Constants::PaletteResources::BLOCK_1_7_3;
        case BlockType::BLOCK_2_7_4: return Constants::PaletteResources::BLOCK_1_7_4;
        case BlockType::BLOCK_2_7_5: return Constants::PaletteResources::BLOCK_1_7_5;
        case BlockType::BLOCK_2_7_6: return Constants::PaletteResources::BLOCK_1_7_6;
        case BlockType::BLOCK_2_7_7: return Constants::PaletteResources::BLOCK_1_7_7;
        case BlockType::BLOCK_2_7_11: return Constants::PaletteResources::BLOCK_1_7_11;
        case BlockType::BLOCK_2_7_12: return Constants::PaletteResources::BLOCK_1_7_12;
        case BlockType::BLOCK_2_7_13: return Constants::PaletteResources::BLOCK_1_7_13;
        case BlockType::BLOCK_2_7_14: return Constants::PaletteResources::BLOCK_1_7_14;
        case BlockType::BLOCK_2_7_15: return Constants::PaletteResources::BLOCK_1_7_15;
        case BlockType::BLOCK_2_7_16: return Constants::PaletteResources::BLOCK_1_7_16;
        case BlockType::BLOCK_2_13_1: return Constants::PaletteResources::BLOCK_1_13_1;
        case BlockType::BLOCK_2_13_2: return Constants::PaletteResources::BLOCK_1_13_2;
        case BlockType::BLOCK_2_13_3: return Constants::PaletteResources::BLOCK_1_13_3;
        case BlockType::BLOCK_2_13_4: return Constants::PaletteResources::BLOCK_1_13_4;
        case BlockType::BLOCK_2_13_5: return Constants::PaletteResources::BLOCK_1_13_5;
        case BlockType::BLOCK_2_13_6: return Constants::PaletteResources::BLOCK_1_13_6;
        case BlockType::BLOCK_2_13_7: return Constants::PaletteResources::BLOCK_1_13_7;
        case BlockType::BLOCK_2_13_8: return Constants::PaletteResources::BLOCK_1_13_8;
        case BlockType::BLOCK_2_13_9: return Constants::PaletteResources::BLOCK_1_13_9;
        case BlockType::BLOCK_2_13_10: return Constants::PaletteResources::BLOCK_1_13_10;
        case BlockType::BLOCK_2_13_11: return Constants::PaletteResources::BLOCK_1_13_11;
        case BlockType::BLOCK_2_13_12: return Constants::PaletteResources::BLOCK_1_13_12;
        case BlockType::BLOCK_2_13_13: return Constants::PaletteResources::BLOCK_1_13_13;
        case BlockType::BLOCK_2_13_14: return Constants::PaletteResources::BLOCK_1_13_14;
        case BlockType::BLOCK_2_13_15: return Constants::PaletteResources::BLOCK_1_13_15;
        case BlockType::BLOCK_2_13_16: return Constants::PaletteResources::BLOCK_1_13_16;
        case BlockType::BLOCK_2_14_1: return Constants::PaletteResources::BLOCK_1_14_1;
        case BlockType::BLOCK_2_14_2: return Constants::PaletteResources::BLOCK_1_14_2;
        case BlockType::BLOCK_2_14_3: return Constants::PaletteResources::BLOCK_1_14_3;
        case BlockType::BLOCK_2_14_4: return Constants::PaletteResources::BLOCK_1_14_4;
        case BlockType::BLOCK_2_14_5: return Constants::PaletteResources::BLOCK_1_14_5;
        case BlockType::BLOCK_2_14_6: return Constants::PaletteResources::BLOCK_1_14_6;
        case BlockType::BLOCK_2_14_7: return Constants::PaletteResources::BLOCK_1_14_7;
        case BlockType::BLOCK_2_14_8: return Constants::PaletteResources::BLOCK_1_14_8;
        case BlockType::BLOCK_2_14_9: return Constants::PaletteResources::BLOCK_1_14_9;
        case BlockType::BLOCK_2_14_10: return Constants::PaletteResources::BLOCK_1_14_10;
        case BlockType::BLOCK_2_14_11: return Constants::PaletteResources::BLOCK_1_14_11;
        case BlockType::BLOCK_2_14_12: return Constants::PaletteResources::BLOCK_1_14_12;
        case BlockType::BLOCK_2_14_13: return Constants::PaletteResources::BLOCK_1_14_13;
        case BlockType::BLOCK_2_14_14: return Constants::PaletteResources::BLOCK_1_14_14;
        case BlockType::BLOCK_2_14_15: return Constants::PaletteResources::BLOCK_1_14_15;
        case BlockType::BLOCK_2_14_16: return Constants::PaletteResources::BLOCK_1_14_16;
        case BlockType::BLOCK_2_15_1: return Constants::PaletteResources::BLOCK_1_15_1;
        case BlockType::BLOCK_2_15_2: return Constants::PaletteResources::BLOCK_1_15_2;
        case BlockType::BLOCK_2_15_3: return Constants::PaletteResources::BLOCK_1_15_3;
        case BlockType::BLOCK_2_15_4: return Constants::PaletteResources::BLOCK_1_15_4;
        case BlockType::BLOCK_2_15_5: return Constants::PaletteResources::BLOCK_1_15_5;
        case BlockType::BLOCK_2_15_6: return Constants::PaletteResources::BLOCK_1_15_6;
        case BlockType::BLOCK_2_15_7: return Constants::PaletteResources::BLOCK_1_15_7;
        case BlockType::BLOCK_2_15_8: return Constants::PaletteResources::BLOCK_1_15_8;
        case BlockType::BLOCK_2_15_9: return Constants::PaletteResources::BLOCK_1_15_9;
        case BlockType::BLOCK_2_15_10: return Constants::PaletteResources::BLOCK_1_15_10;
        case BlockType::BLOCK_2_15_11: return Constants::PaletteResources::BLOCK_1_15_11;
        case BlockType::BLOCK_2_15_12: return Constants::PaletteResources::BLOCK_1_15_12;
        case BlockType::BLOCK_2_15_13: return Constants::PaletteResources::BLOCK_1_15_13;
        case BlockType::BLOCK_2_15_14: return Constants::PaletteResources::BLOCK_1_15_14;
        case BlockType::BLOCK_2_15_15: return Constants::PaletteResources::BLOCK_1_15_15;
        case BlockType::BLOCK_2_15_16: return Constants::PaletteResources::BLOCK_1_15_16;
        case BlockType::BLOCK_2_16_1: return Constants::PaletteResources::BLOCK_1_16_1;
        case BlockType::BLOCK_2_16_2: return Constants::PaletteResources::BLOCK_1_16_2;
        case BlockType::BLOCK_2_16_3: return Constants::PaletteResources::BLOCK_1_16_3;
        case BlockType::BLOCK_2_16_4: return Constants::PaletteResources::BLOCK_1_16_4;
        case BlockType::BLOCK_2_16_5: return Constants::PaletteResources::BLOCK_1_16_5;
        case BlockType::BLOCK_2_16_6: return Constants::PaletteResources::BLOCK_1_16_6;
        case BlockType::BLOCK_2_16_7: return Constants::PaletteResources::BLOCK_1_16_7;
        case BlockType::BLOCK_2_16_8: return Constants::PaletteResources::BLOCK_1_16_8;
        case BlockType::BLOCK_2_16_9: return Constants::PaletteResources::BLOCK_1_16_9;
        case BlockType::BLOCK_2_16_10: return Constants::PaletteResources::BLOCK_1_16_10;
        case BlockType::BLOCK_2_16_11: return Constants::PaletteResources::BLOCK_1_16_11;
        case BlockType::BLOCK_2_16_12: return Constants::PaletteResources::BLOCK_1_16_12;
        case BlockType::BLOCK_2_16_13: return Constants::PaletteResources::BLOCK_1_16_13;
        case BlockType::BLOCK_2_16_14: return Constants::PaletteResources::BLOCK_1_16_14;
        case BlockType::BLOCK_2_16_15: return Constants::PaletteResources::BLOCK_1_16_15;
        case BlockType::BLOCK_2_16_16: return Constants::PaletteResources::BLOCK_1_16_16;

        case BlockType::BLOCK_3_1_2: return Constants::PaletteResources::BLOCK_1_1_2;
        case BlockType::BLOCK_3_1_3: {
            if (!itemSpawned) return Constants::PaletteResources::BLOCK_1_1_3;
            return Constants::PaletteResources::BLOCK_1_1_4;
        }
        case BlockType::BLOCK_3_1_12: return Constants::PaletteResources::BLOCK_1_1_12;
        case BlockType::BLOCK_3_1_13: return Constants::PaletteResources::BLOCK_1_1_13;
        case BlockType::BLOCK_3_1_14: return Constants::PaletteResources::BLOCK_1_1_14;
        case BlockType::BLOCK_3_1_15: return Constants::PaletteResources::BLOCK_1_1_15;
        case BlockType::BLOCK_3_1_16: return Constants::PaletteResources::BLOCK_1_1_16;
        case BlockType::BLOCK_3_2_7: return Constants::PaletteResources::BLOCK_1_2_7;
        case BlockType::BLOCK_3_2_8: return Constants::PaletteResources::BLOCK_1_2_8;
        case BlockType::BLOCK_3_2_12: return Constants::PaletteResources::BLOCK_1_2_12;
        case BlockType::BLOCK_3_2_13: return Constants::PaletteResources::BLOCK_1_2_13;
        case BlockType::BLOCK_3_2_14: return Constants::PaletteResources::BLOCK_1_2_14;
        case BlockType::BLOCK_3_2_15: return Constants::PaletteResources::BLOCK_1_2_15;
        case BlockType::BLOCK_3_2_16: return Constants::PaletteResources::BLOCK_1_2_16;
        case BlockType::BLOCK_3_3_4: return Constants::PaletteResources::BLOCK_1_3_4;
        case BlockType::BLOCK_3_3_5: return Constants::PaletteResources::BLOCK_1_3_5;
        case BlockType::BLOCK_3_3_6: return Constants::PaletteResources::BLOCK_1_3_6;
        case BlockType::BLOCK_3_3_7: return Constants::PaletteResources::BLOCK_1_3_7;
        case BlockType::BLOCK_3_3_8: return Constants::PaletteResources::BLOCK_1_3_8;
        case BlockType::BLOCK_3_3_15: return Constants::PaletteResources::BLOCK_1_3_15;
        case BlockType::BLOCK_3_3_16: return Constants::PaletteResources::BLOCK_1_3_16;
        case BlockType::BLOCK_3_4_4: return Constants::PaletteResources::BLOCK_1_4_4;
        case BlockType::BLOCK_3_4_5: return Constants::PaletteResources::BLOCK_1_4_5;
        case BlockType::BLOCK_3_4_6: return Constants::PaletteResources::BLOCK_1_4_6;
        case BlockType::BLOCK_3_4_7: return Constants::PaletteResources::BLOCK_1_4_7;
        case BlockType::BLOCK_3_4_11: return Constants::PaletteResources::BLOCK_1_4_11;
        case BlockType::BLOCK_3_4_12: return Constants::PaletteResources::BLOCK_1_4_12;
        case BlockType::BLOCK_3_4_13: return Constants::PaletteResources::BLOCK_1_4_13;
        case BlockType::BLOCK_3_4_14: return Constants::PaletteResources::BLOCK_1_4_14;
        case BlockType::BLOCK_3_4_15: return Constants::PaletteResources::BLOCK_1_4_15;
        case BlockType::BLOCK_3_4_16: return Constants::PaletteResources::BLOCK_1_4_16;
        case BlockType::BLOCK_3_5_4: return Constants::PaletteResources::BLOCK_1_5_4;
        case BlockType::BLOCK_3_5_5: return Constants::PaletteResources::BLOCK_1_5_5;
        case BlockType::BLOCK_3_5_6: return Constants::PaletteResources::BLOCK_1_5_6;
        case BlockType::BLOCK_3_5_8: return Constants::PaletteResources::BLOCK_1_5_8;
        case BlockType::BLOCK_3_5_9: return Constants::PaletteResources::BLOCK_1_5_9;
        case BlockType::BLOCK_3_5_10: return Constants::PaletteResources::BLOCK_1_5_10;
        case BlockType::BLOCK_3_5_11: return Constants::PaletteResources::BLOCK_1_5_11;
        case BlockType::BLOCK_3_5_12: return Constants::PaletteResources::BLOCK_1_5_12;
        case BlockType::BLOCK_3_5_13: return Constants::PaletteResources::BLOCK_1_5_13;
        case BlockType::BLOCK_3_5_14: return Constants::PaletteResources::BLOCK_1_5_14;
        case BlockType::BLOCK_3_5_15: return Constants::PaletteResources::BLOCK_1_5_15;
        case BlockType::BLOCK_3_5_16: return Constants::PaletteResources::BLOCK_1_5_16;
        case BlockType::BLOCK_3_6_5: return Constants::PaletteResources::BLOCK_1_6_5;
        case BlockType::BLOCK_3_6_6: return Constants::PaletteResources::BLOCK_1_6_6;
        case BlockType::BLOCK_3_7_1: return Constants::PaletteResources::BLOCK_1_7_1;
        case BlockType::BLOCK_3_7_2: return Constants::PaletteResources::BLOCK_1_7_2;
        case BlockType::BLOCK_3_7_3: return Constants::PaletteResources::BLOCK_1_7_3;
        case BlockType::BLOCK_3_7_4: return Constants::PaletteResources::BLOCK_1_7_4;
        case BlockType::BLOCK_3_7_5: return Constants::PaletteResources::BLOCK_1_7_5;
        case BlockType::BLOCK_3_7_6: return Constants::PaletteResources::BLOCK_1_7_6;
        case BlockType::BLOCK_3_7_7: return Constants::PaletteResources::BLOCK_1_7_7;
        case BlockType::BLOCK_3_7_11: return Constants::PaletteResources::BLOCK_1_7_11;
        case BlockType::BLOCK_3_7_12: return Constants::PaletteResources::BLOCK_1_7_12;
        case BlockType::BLOCK_3_7_13: return Constants::PaletteResources::BLOCK_1_7_13;
        case BlockType::BLOCK_3_7_14: return Constants::PaletteResources::BLOCK_1_7_14;
        case BlockType::BLOCK_3_7_15: return Constants::PaletteResources::BLOCK_1_7_15;
        case BlockType::BLOCK_3_7_16: return Constants::PaletteResources::BLOCK_1_7_16;
        case BlockType::BLOCK_3_13_1: return Constants::PaletteResources::BLOCK_1_13_1;
        case BlockType::BLOCK_3_13_2: return Constants::PaletteResources::BLOCK_1_13_2;
        case BlockType::BLOCK_3_13_3: return Constants::PaletteResources::BLOCK_1_13_3;
        case BlockType::BLOCK_3_13_4: return Constants::PaletteResources::BLOCK_1_13_4;
        case BlockType::BLOCK_3_13_5: return Constants::PaletteResources::BLOCK_1_13_5;
        case BlockType::BLOCK_3_13_6: return Constants::PaletteResources::BLOCK_1_13_6;
        case BlockType::BLOCK_3_13_7: return Constants::PaletteResources::BLOCK_1_13_7;
        case BlockType::BLOCK_3_13_8: return Constants::PaletteResources::BLOCK_1_13_8;
        case BlockType::BLOCK_3_13_9: return Constants::PaletteResources::BLOCK_1_13_9;
        case BlockType::BLOCK_3_13_10: return Constants::PaletteResources::BLOCK_1_13_10;
        case BlockType::BLOCK_3_13_11: return Constants::PaletteResources::BLOCK_1_13_11;
        case BlockType::BLOCK_3_13_12: return Constants::PaletteResources::BLOCK_1_13_12;
        case BlockType::BLOCK_3_13_13: return Constants::PaletteResources::BLOCK_1_13_13;
        case BlockType::BLOCK_3_13_14: return Constants::PaletteResources::BLOCK_1_13_14;
        case BlockType::BLOCK_3_13_15: return Constants::PaletteResources::BLOCK_1_13_15;
        case BlockType::BLOCK_3_13_16: return Constants::PaletteResources::BLOCK_1_13_16;
        case BlockType::BLOCK_3_14_1: return Constants::PaletteResources::BLOCK_1_14_1;
        case BlockType::BLOCK_3_14_2: return Constants::PaletteResources::BLOCK_1_14_2;
        case BlockType::BLOCK_3_14_3: return Constants::PaletteResources::BLOCK_1_14_3;
        case BlockType::BLOCK_3_14_4: return Constants::PaletteResources::BLOCK_1_14_4;
        case BlockType::BLOCK_3_14_5: return Constants::PaletteResources::BLOCK_1_14_5;
        case BlockType::BLOCK_3_14_6: return Constants::PaletteResources::BLOCK_1_14_6;
        case BlockType::BLOCK_3_14_7: return Constants::PaletteResources::BLOCK_1_14_7;
        case BlockType::BLOCK_3_14_8: return Constants::PaletteResources::BLOCK_1_14_8;
        case BlockType::BLOCK_3_14_9: return Constants::PaletteResources::BLOCK_1_14_9;
        case BlockType::BLOCK_3_14_10: return Constants::PaletteResources::BLOCK_1_14_10;
        case BlockType::BLOCK_3_14_11: return Constants::PaletteResources::BLOCK_1_14_11;
        case BlockType::BLOCK_3_14_12: return Constants::PaletteResources::BLOCK_1_14_12;
        case BlockType::BLOCK_3_14_13: return Constants::PaletteResources::BLOCK_1_14_13;
        case BlockType::BLOCK_3_14_14: return Constants::PaletteResources::BLOCK_1_14_14;
        case BlockType::BLOCK_3_14_15: return Constants::PaletteResources::BLOCK_1_14_15;
        case BlockType::BLOCK_3_14_16: return Constants::PaletteResources::BLOCK_1_14_16;
        case BlockType::BLOCK_3_15_1: return Constants::PaletteResources::BLOCK_1_15_1;
        case BlockType::BLOCK_3_15_2: return Constants::PaletteResources::BLOCK_1_15_2;
        case BlockType::BLOCK_3_15_3: return Constants::PaletteResources::BLOCK_1_15_3;
        case BlockType::BLOCK_3_15_4: return Constants::PaletteResources::BLOCK_1_15_4;
        case BlockType::BLOCK_3_15_5: return Constants::PaletteResources::BLOCK_1_15_5;
        case BlockType::BLOCK_3_15_6: return Constants::PaletteResources::BLOCK_1_15_6;
        case BlockType::BLOCK_3_15_7: return Constants::PaletteResources::BLOCK_1_15_7;
        case BlockType::BLOCK_3_15_8: return Constants::PaletteResources::BLOCK_1_15_8;
        case BlockType::BLOCK_3_15_9: return Constants::PaletteResources::BLOCK_1_15_9;
        case BlockType::BLOCK_3_15_10: return Constants::PaletteResources::BLOCK_1_15_10;
        case BlockType::BLOCK_3_15_11: return Constants::PaletteResources::BLOCK_1_15_11;
        case BlockType::BLOCK_3_15_12: return Constants::PaletteResources::BLOCK_1_15_12;
        case BlockType::BLOCK_3_15_13: return Constants::PaletteResources::BLOCK_1_15_13;
        case BlockType::BLOCK_3_15_14: return Constants::PaletteResources::BLOCK_1_15_14;
        case BlockType::BLOCK_3_15_15: return Constants::PaletteResources::BLOCK_1_15_15;
        case BlockType::BLOCK_3_15_16: return Constants::PaletteResources::BLOCK_1_15_16;
        case BlockType::BLOCK_3_16_1: return Constants::PaletteResources::BLOCK_1_16_1;
        case BlockType::BLOCK_3_16_2: return Constants::PaletteResources::BLOCK_1_16_2;
        case BlockType::BLOCK_3_16_3: return Constants::PaletteResources::BLOCK_1_16_3;
        case BlockType::BLOCK_3_16_4: return Constants::PaletteResources::BLOCK_1_16_4;
        case BlockType::BLOCK_3_16_5: return Constants::PaletteResources::BLOCK_1_16_5;
        case BlockType::BLOCK_3_16_6: return Constants::PaletteResources::BLOCK_1_16_6;
        case BlockType::BLOCK_3_16_7: return Constants::PaletteResources::BLOCK_1_16_7;
        case BlockType::BLOCK_3_16_8: return Constants::PaletteResources::BLOCK_1_16_8;
        case BlockType::BLOCK_3_16_9: return Constants::PaletteResources::BLOCK_1_16_9;
        case BlockType::BLOCK_3_16_10: return Constants::PaletteResources::BLOCK_1_16_10;
        case BlockType::BLOCK_3_16_11: return Constants::PaletteResources::BLOCK_1_16_11;
        case BlockType::BLOCK_3_16_12: return Constants::PaletteResources::BLOCK_1_16_12;
        case BlockType::BLOCK_3_16_13: return Constants::PaletteResources::BLOCK_1_16_13;
        case BlockType::BLOCK_3_16_14: return Constants::PaletteResources::BLOCK_1_16_14;
        case BlockType::BLOCK_3_16_15: return Constants::PaletteResources::BLOCK_1_16_15;
        case BlockType::BLOCK_3_16_16: return Constants::PaletteResources::BLOCK_1_16_16;

        case BlockType::BLOCK_4_1_2: return Constants::PaletteResources::BLOCK_1_1_2;
        case BlockType::BLOCK_4_1_3: {
            if (!itemSpawned) return Constants::PaletteResources::BLOCK_1_1_3;
            return Constants::PaletteResources::BLOCK_1_1_4;
        }
        case BlockType::BLOCK_4_1_12: return Constants::PaletteResources::BLOCK_1_1_12;
        case BlockType::BLOCK_4_1_13: return Constants::PaletteResources::BLOCK_1_1_13;
        case BlockType::BLOCK_4_1_14: return Constants::PaletteResources::BLOCK_1_1_14;
        case BlockType::BLOCK_4_1_15: return Constants::PaletteResources::BLOCK_1_1_15;
        case BlockType::BLOCK_4_1_16: return Constants::PaletteResources::BLOCK_1_1_16;
        case BlockType::BLOCK_4_2_7: return Constants::PaletteResources::BLOCK_1_2_7;
        case BlockType::BLOCK_4_2_8: return Constants::PaletteResources::BLOCK_1_2_8;
        case BlockType::BLOCK_4_2_12: return Constants::PaletteResources::BLOCK_1_2_12;
        case BlockType::BLOCK_4_2_13: return Constants::PaletteResources::BLOCK_1_2_13;
        case BlockType::BLOCK_4_2_14: return Constants::PaletteResources::BLOCK_1_2_14;
        case BlockType::BLOCK_4_2_15: return Constants::PaletteResources::BLOCK_1_2_15;
        case BlockType::BLOCK_4_2_16: return Constants::PaletteResources::BLOCK_1_2_16;
        case BlockType::BLOCK_4_3_4: return Constants::PaletteResources::BLOCK_1_3_4;
        case BlockType::BLOCK_4_3_5: return Constants::PaletteResources::BLOCK_1_3_5;
        case BlockType::BLOCK_4_3_6: return Constants::PaletteResources::BLOCK_1_3_6;
        case BlockType::BLOCK_4_3_7: return Constants::PaletteResources::BLOCK_1_3_7;
        case BlockType::BLOCK_4_3_8: return Constants::PaletteResources::BLOCK_1_3_8;
        case BlockType::BLOCK_4_3_15: return Constants::PaletteResources::BLOCK_1_3_15;
        case BlockType::BLOCK_4_3_16: return Constants::PaletteResources::BLOCK_1_3_16;
        case BlockType::BLOCK_4_4_4: return Constants::PaletteResources::BLOCK_1_4_4;
        case BlockType::BLOCK_4_4_5: return Constants::PaletteResources::BLOCK_1_4_5;
        case BlockType::BLOCK_4_4_6: return Constants::PaletteResources::BLOCK_1_4_6;
        case BlockType::BLOCK_4_4_7: return Constants::PaletteResources::BLOCK_1_4_7;
        case BlockType::BLOCK_4_4_11: return Constants::PaletteResources::BLOCK_1_4_11;
        case BlockType::BLOCK_4_4_12: return Constants::PaletteResources::BLOCK_1_4_12;
        case BlockType::BLOCK_4_4_13: return Constants::PaletteResources::BLOCK_1_4_13;
        case BlockType::BLOCK_4_4_14: return Constants::PaletteResources::BLOCK_1_4_14;
        case BlockType::BLOCK_4_4_15: return Constants::PaletteResources::BLOCK_1_4_15;
        case BlockType::BLOCK_4_4_16: return Constants::PaletteResources::BLOCK_1_4_16;
        case BlockType::BLOCK_4_5_4: return Constants::PaletteResources::BLOCK_1_5_4;
        case BlockType::BLOCK_4_5_5: return Constants::PaletteResources::BLOCK_1_5_5;
        case BlockType::BLOCK_4_5_6: return Constants::PaletteResources::BLOCK_1_5_6;
        case BlockType::BLOCK_4_5_8: return Constants::PaletteResources::BLOCK_1_5_8;
        case BlockType::BLOCK_4_5_9: return Constants::PaletteResources::BLOCK_1_5_9;
        case BlockType::BLOCK_4_5_10: return Constants::PaletteResources::BLOCK_1_5_10;
        case BlockType::BLOCK_4_5_11: return Constants::PaletteResources::BLOCK_1_5_11;
        case BlockType::BLOCK_4_5_12: return Constants::PaletteResources::BLOCK_1_5_12;
        case BlockType::BLOCK_4_5_13: return Constants::PaletteResources::BLOCK_1_5_13;
        case BlockType::BLOCK_4_5_14: return Constants::PaletteResources::BLOCK_1_5_14;
        case BlockType::BLOCK_4_5_15: return Constants::PaletteResources::BLOCK_1_5_15;
        case BlockType::BLOCK_4_5_16: return Constants::PaletteResources::BLOCK_1_5_16;
        case BlockType::BLOCK_4_6_5: return Constants::PaletteResources::BLOCK_1_6_5;
        case BlockType::BLOCK_4_6_6: return Constants::PaletteResources::BLOCK_1_6_6;
        case BlockType::BLOCK_4_7_1: return Constants::PaletteResources::BLOCK_1_7_1;
        case BlockType::BLOCK_4_7_2: return Constants::PaletteResources::BLOCK_1_7_2;
        case BlockType::BLOCK_4_7_3: return Constants::PaletteResources::BLOCK_1_7_3;
        case BlockType::BLOCK_4_7_4: return Constants::PaletteResources::BLOCK_1_7_4;
        case BlockType::BLOCK_4_7_5: return Constants::PaletteResources::BLOCK_1_7_5;
        case BlockType::BLOCK_4_7_6: return Constants::PaletteResources::BLOCK_1_7_6;
        case BlockType::BLOCK_4_7_7: return Constants::PaletteResources::BLOCK_1_7_7;
        case BlockType::BLOCK_4_7_11: return Constants::PaletteResources::BLOCK_1_7_11;
        case BlockType::BLOCK_4_7_12: return Constants::PaletteResources::BLOCK_1_7_12;
        case BlockType::BLOCK_4_7_13: return Constants::PaletteResources::BLOCK_1_7_13;
        case BlockType::BLOCK_4_7_14: return Constants::PaletteResources::BLOCK_1_7_14;
        case BlockType::BLOCK_4_7_15: return Constants::PaletteResources::BLOCK_1_7_15;
        case BlockType::BLOCK_4_7_16: return Constants::PaletteResources::BLOCK_1_7_16;
        case BlockType::BLOCK_4_13_1: return Constants::PaletteResources::BLOCK_1_13_1;
        case BlockType::BLOCK_4_13_2: return Constants::PaletteResources::BLOCK_1_13_2;
        case BlockType::BLOCK_4_13_3: return Constants::PaletteResources::BLOCK_1_13_3;
        case BlockType::BLOCK_4_13_4: return Constants::PaletteResources::BLOCK_1_13_4;
        case BlockType::BLOCK_4_13_5: return Constants::PaletteResources::BLOCK_1_13_5;
        case BlockType::BLOCK_4_13_6: return Constants::PaletteResources::BLOCK_1_13_6;
        case BlockType::BLOCK_4_13_7: return Constants::PaletteResources::BLOCK_1_13_7;
        case BlockType::BLOCK_4_13_8: return Constants::PaletteResources::BLOCK_1_13_8;
        case BlockType::BLOCK_4_13_9: return Constants::PaletteResources::BLOCK_1_13_9;
        case BlockType::BLOCK_4_13_10: return Constants::PaletteResources::BLOCK_1_13_10;
        case BlockType::BLOCK_4_13_11: return Constants::PaletteResources::BLOCK_1_13_11;
        case BlockType::BLOCK_4_13_12: return Constants::PaletteResources::BLOCK_1_13_12;
        case BlockType::BLOCK_4_13_13: return Constants::PaletteResources::BLOCK_1_13_13;
        case BlockType::BLOCK_4_13_14: return Constants::PaletteResources::BLOCK_1_13_14;
        case BlockType::BLOCK_4_13_15: return Constants::PaletteResources::BLOCK_1_13_15;
        case BlockType::BLOCK_4_13_16: return Constants::PaletteResources::BLOCK_1_13_16;
        case BlockType::BLOCK_4_14_1: return Constants::PaletteResources::BLOCK_1_14_1;
        case BlockType::BLOCK_4_14_2: return Constants::PaletteResources::BLOCK_1_14_2;
        case BlockType::BLOCK_4_14_3: return Constants::PaletteResources::BLOCK_1_14_3;
        case BlockType::BLOCK_4_14_4: return Constants::PaletteResources::BLOCK_1_14_4;
        case BlockType::BLOCK_4_14_5: return Constants::PaletteResources::BLOCK_1_14_5;
        case BlockType::BLOCK_4_14_6: return Constants::PaletteResources::BLOCK_1_14_6;
        case BlockType::BLOCK_4_14_7: return Constants::PaletteResources::BLOCK_1_14_7;
        case BlockType::BLOCK_4_14_8: return Constants::PaletteResources::BLOCK_1_14_8;
        case BlockType::BLOCK_4_14_9: return Constants::PaletteResources::BLOCK_1_14_9;
        case BlockType::BLOCK_4_14_10: return Constants::PaletteResources::BLOCK_1_14_10;
        case BlockType::BLOCK_4_14_11: return Constants::PaletteResources::BLOCK_1_14_11;
        case BlockType::BLOCK_4_14_12: return Constants::PaletteResources::BLOCK_1_14_12;
        case BlockType::BLOCK_4_14_13: return Constants::PaletteResources::BLOCK_1_14_13;
        case BlockType::BLOCK_4_14_14: return Constants::PaletteResources::BLOCK_1_14_14;
        case BlockType::BLOCK_4_14_15: return Constants::PaletteResources::BLOCK_1_14_15;
        case BlockType::BLOCK_4_14_16: return Constants::PaletteResources::BLOCK_1_14_16;
        case BlockType::BLOCK_4_15_1: return Constants::PaletteResources::BLOCK_1_15_1;
        case BlockType::BLOCK_4_15_2: return Constants::PaletteResources::BLOCK_1_15_2;
        case BlockType::BLOCK_4_15_3: return Constants::PaletteResources::BLOCK_1_15_3;
        case BlockType::BLOCK_4_15_4: return Constants::PaletteResources::BLOCK_1_15_4;
        case BlockType::BLOCK_4_15_5: return Constants::PaletteResources::BLOCK_1_15_5;
        case BlockType::BLOCK_4_15_6: return Constants::PaletteResources::BLOCK_1_15_6;
        case BlockType::BLOCK_4_15_7: return Constants::PaletteResources::BLOCK_1_15_7;
        case BlockType::BLOCK_4_15_8: return Constants::PaletteResources::BLOCK_1_15_8;
        case BlockType::BLOCK_4_15_9: return Constants::PaletteResources::BLOCK_1_15_9;
        case BlockType::BLOCK_4_15_10: return Constants::PaletteResources::BLOCK_1_15_10;
        case BlockType::BLOCK_4_15_11: return Constants::PaletteResources::BLOCK_1_15_11;
        case BlockType::BLOCK_4_15_12: return Constants::PaletteResources::BLOCK_1_15_12;
        case BlockType::BLOCK_4_15_13: return Constants::PaletteResources::BLOCK_1_15_13;
        case BlockType::BLOCK_4_15_14: return Constants::PaletteResources::BLOCK_1_15_14;
        case BlockType::BLOCK_4_15_15: return Constants::PaletteResources::BLOCK_1_15_15;
        case BlockType::BLOCK_4_15_16: return Constants::PaletteResources::BLOCK_1_15_16;
        case BlockType::BLOCK_4_16_1: return Constants::PaletteResources::BLOCK_1_16_1;
        case BlockType::BLOCK_4_16_2: return Constants::PaletteResources::BLOCK_1_16_2;
        case BlockType::BLOCK_4_16_3: return Constants::PaletteResources::BLOCK_1_16_3;
        case BlockType::BLOCK_4_16_4: return Constants::PaletteResources::BLOCK_1_16_4;
        case BlockType::BLOCK_4_16_5: return Constants::PaletteResources::BLOCK_1_16_5;
        case BlockType::BLOCK_4_16_6: return Constants::PaletteResources::BLOCK_1_16_6;
        case BlockType::BLOCK_4_16_7: return Constants::PaletteResources::BLOCK_1_16_7;
        case BlockType::BLOCK_4_16_8: return Constants::PaletteResources::BLOCK_1_16_8;
        case BlockType::BLOCK_4_16_9: return Constants::PaletteResources::BLOCK_1_16_9;
        case BlockType::BLOCK_4_16_10: return Constants::PaletteResources::BLOCK_1_16_10;
        case BlockType::BLOCK_4_16_11: return Constants::PaletteResources::BLOCK_1_16_11;
        case BlockType::BLOCK_4_16_12: return Constants::PaletteResources::BLOCK_1_16_12;
        case BlockType::BLOCK_4_16_13: return Constants::PaletteResources::BLOCK_1_16_13;
        case BlockType::BLOCK_4_16_14: return Constants::PaletteResources::BLOCK_1_16_14;
        case BlockType::BLOCK_4_16_15: return Constants::PaletteResources::BLOCK_1_16_15;
        case BlockType::BLOCK_4_16_16: return Constants::PaletteResources::BLOCK_1_16_16;
        default:
            return Constants::PaletteResources::BLOCK_1_1_2;
    }
}

std::vector<Rectangle> Block::getHitBox() const {
   return {hitbox};
}

ObjectCategory Block::getObjectCategory() const {
    return ObjectCategory::BLOCK;
}

std::vector<ObjectCategory> Block::getCollisionTargets() const {
    return {ObjectCategory::CHARACTER, ObjectCategory::ENEMY, ObjectCategory::PROJECTILE, ObjectCategory::SHELL};
}

void Block::onCollision(std::shared_ptr<Object> other, Direction direction) {
    if (other->getObjectCategory() == ObjectCategory::CHARACTER && direction == Direction::DOWN && !isMoving) {
        physicsBody->SetLinearVelocity(b2Vec2{ 0.0f, -2.0f });
        isMoving = true;
        resetTimer = 0.0f;
        auto character = std::dynamic_pointer_cast<Character>(other);
        if (character->getPowerState() != PowerState::SMALL && !isSolid()) {
            Vector2 centerPos = { position.x + hitbox.width * 0.5f, position.y + hitbox.height * 0.5f };
            BrokenBlockEffect* p = new BrokenBlockEffect(centerPos, {30, 30}, {-200, -400}, {200, -400}, {0, 1000}, 5.0f, 0.01f, getTextureSprite(), getSrcRect());
            ParticleSystem::getInstance().addEffect(p);
            GameContext::getInstance().mark_for_deletion_Object(GameContext::getInstance().getSharedPtrFromRaw(this));
        }
        if (blockType == BlockType::BLOCK_1_1_3 || blockType == BlockType::BLOCK_2_1_3 || blockType == BlockType::BLOCK_3_1_3 || blockType == BlockType::BLOCK_4_1_3) {
            itemSpawned = true;
            if (itemType == "FIRE_FLOWER") {
                GameContext::getInstance().addObject(ItemType::FIRE_FLOWER, {position.x + Constants::TILE_SIZE*0.5f, position.y}, {1, 1});
                itemType = "";
            }
            else if (itemType == "MUSHROOM") {
                GameContext::getInstance().addObject(ItemType::MUSHROOM, {position.x + Constants::TILE_SIZE*0.5f, position.y}, {1, 1});
                itemType = "";
            }
            else if (itemType == "ONE_UP") {
                GameContext::getInstance().addObject(ItemType::ONE_UP, {position.x + Constants::TILE_SIZE*0.5f, position.y}, {1, 1});
                itemType = "";
            }
        } 
    }
}

void Block::update(float deltaTime) {
    if (physicsBody) {
        auto newPos = Box2DWorldManager::b2ToRaylib(physicsBody->GetPosition());
        position = { newPos.x - hitbox.width / 2.0f, newPos.y - hitbox.height / 2.0f };

        if (isMoving) {
            resetTimer += deltaTime;
            if (resetTimer > RESET_TIME) {
                physicsBody->SetLinearVelocity(b2Vec2({ 0.0f, 0.0f }));
                Vector2 centrePos = { originalPos.x + hitbox.width / 2.0f, originalPos.y + hitbox.height / 2.0f };
                physicsBody->SetTransform(Box2DWorldManager::raylibToB2(centrePos), 0.0f);
            }
            if (resetTimer > 3 * RESET_TIME) {
                isMoving = false;
                resetTimer = 0.0f;
            }
        }
    }
}
    
bool Block::isActive() const {
    return active;
}

void Block::setActive(bool flag) {
    active = flag;
}

bool Block::isCollided() const {
    return collided;
}

void Block::setCollided(bool flag) {
    collided = flag;
}

Vector2 Block::getPosition() const {
    return position;
}

void Block::setPosition(Vector2 newPos) {
    position = newPos;
    hitbox.x = position.x;
    hitbox.y = position.y;
    
    if (physicsBody) {
        physicsBody->SetTransform(Box2DWorldManager::raylibToB2(newPos), physicsBody->GetAngle());
    }
}

bool Block::isSolid() const {
    return solid;
}

ObjectType Block::getObjectType() const {
    return getType();
}

BlockType Block::getType() const {
    return blockType;
}

Vector2 Block::getSize() const {
    return size;
}
Block_1_1_2Block::Block_1_1_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_1_2, { 1,1 }) { solid = false; }
Block_1_1_3Block::Block_1_1_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_1_3, { 1,1 }) { solid = true; }
Block_1_1_12Block::Block_1_1_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_1_12, { 1,1 }) { solid = false; }
Block_1_1_13Block::Block_1_1_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_1_13, { 1,1 }) { solid = false; }
Block_1_1_14Block::Block_1_1_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_1_14, { 1,1 }) { solid = false; }
Block_1_1_15Block::Block_1_1_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_1_15, { 1,1 }) { solid = false; }
Block_1_1_16Block::Block_1_1_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_1_16, { 1,1 }) { solid = false; }
Block_1_2_7Block::Block_1_2_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_2_7, { 1,1 }) { solid = false; }
Block_1_2_8Block::Block_1_2_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_2_8, { 1,1 }) { solid = false; }
Block_1_2_12Block::Block_1_2_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_2_12, { 1,1 }) { solid = false; }
Block_1_2_13Block::Block_1_2_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_2_13, { 1,1 }) { solid = false; }
Block_1_2_14Block::Block_1_2_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_2_14, { 1,1 }) { solid = false; }
Block_1_2_15Block::Block_1_2_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_2_15, { 1,1 }) { solid = false; }
Block_1_2_16Block::Block_1_2_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_2_16, { 1,1 }) { solid = false; }
Block_1_3_4Block::Block_1_3_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_3_4, { 1,1 }) { solid = false; }
Block_1_3_5Block::Block_1_3_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_3_5, { 1,1 }) { solid = false; }
Block_1_3_6Block::Block_1_3_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_3_6, { 1,1 }) { solid = false; }
Block_1_3_7Block::Block_1_3_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_3_7, { 1,1 }) { solid = false; }
Block_1_3_8Block::Block_1_3_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_3_8, { 1,1 }) { solid = false; }
Block_1_3_15Block::Block_1_3_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_3_15, { 1,1 }) { solid = false; }
Block_1_3_16Block::Block_1_3_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_3_16, { 1,1 }) { solid = false; }
Block_1_4_4Block::Block_1_4_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_4_4, { 1,1 }) { solid = false; }
Block_1_4_5Block::Block_1_4_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_4_5, { 1,1 }) { solid = false; }
Block_1_4_6Block::Block_1_4_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_4_6, { 1,1 }) { solid = false; }
Block_1_4_7Block::Block_1_4_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_4_7, { 1,1 }) { solid = false; }
Block_1_4_11Block::Block_1_4_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_4_11, { 1,1 }) { solid = false; }
Block_1_4_12Block::Block_1_4_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_4_12, { 1,1 }) { solid = false; }
Block_1_4_13Block::Block_1_4_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_4_13, { 1,1 }) { solid = false; }
Block_1_4_14Block::Block_1_4_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_4_14, { 1,1 }) { solid = false; }
Block_1_4_15Block::Block_1_4_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_4_15, { 1,1 }) { solid = false; }
Block_1_4_16Block::Block_1_4_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_4_16, { 1,1 }) { solid = false; }
Block_1_5_4Block::Block_1_5_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_5_4, { 1,1 }) { solid = false; }
Block_1_5_5Block::Block_1_5_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_5_5, { 1,1 }) { solid = false; }
Block_1_5_6Block::Block_1_5_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_5_6, { 1,1 }) { solid = false; }
Block_1_5_8Block::Block_1_5_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_5_8, { 1,1 }) { solid = false; }
Block_1_5_9Block::Block_1_5_9Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_5_9, { 1,1 }) { solid = false; }
Block_1_5_10Block::Block_1_5_10Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_5_10, { 1,1 }) { solid = false; }
Block_1_5_11Block::Block_1_5_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_5_11, { 1,1 }) { solid = false; }
Block_1_5_12Block::Block_1_5_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_5_12, { 1,1 }) { solid = false; }
Block_1_5_13Block::Block_1_5_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_5_13, { 1,1 }) { solid = false; }
Block_1_5_14Block::Block_1_5_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_5_14, { 1,1 }) { solid = false; }
Block_1_5_15Block::Block_1_5_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_5_15, { 1,1 }) { solid = false; }
Block_1_5_16Block::Block_1_5_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_5_16, { 1,1 }) { solid = false; }
Block_1_6_5Block::Block_1_6_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_6_5, { 1,1 }) { solid = false; }
Block_1_6_6Block::Block_1_6_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_6_6, { 1,1 }) { solid = false; }
Block_1_7_1Block::Block_1_7_1Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_7_1, { 1,1 }) { solid = false; }
Block_1_7_2Block::Block_1_7_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_7_2, { 1,1 }) { solid = false; }
Block_1_7_3Block::Block_1_7_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_7_3, { 1,1 }) { solid = false; }
Block_1_7_4Block::Block_1_7_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_7_4, { 1,1 }) { solid = false; }
Block_1_7_5Block::Block_1_7_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_7_5, { 1,1 }) { solid = false; }
Block_1_7_6Block::Block_1_7_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_7_6, { 1,1 }) { solid = false; }
Block_1_7_7Block::Block_1_7_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_7_7, { 1,1 }) { solid = false; }
Block_1_7_11Block::Block_1_7_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_7_11, { 1,1 }) { solid = false; }
Block_1_7_12Block::Block_1_7_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_7_12, { 1,1 }) { solid = false; }
Block_1_7_13Block::Block_1_7_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_7_13, { 1,1 }) { solid = false; }
Block_1_7_14Block::Block_1_7_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_7_14, { 1,1 }) { solid = false; }
Block_1_7_15Block::Block_1_7_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_7_15, { 1,1 }) { solid = false; }
Block_1_7_16Block::Block_1_7_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_7_16, { 1,1 }) { solid = false; }
Block_1_13_1Block::Block_1_13_1Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_13_1, { 1,1 }) { solid = false; }
Block_1_13_2Block::Block_1_13_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_13_2, { 1,1 }) { solid = false; }
Block_1_13_3Block::Block_1_13_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_13_3, { 1,1 }) { solid = false; }
Block_1_13_4Block::Block_1_13_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_13_4, { 1,1 }) { solid = false; }
Block_1_13_5Block::Block_1_13_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_13_5, { 1,1 }) { solid = false; }
Block_1_13_6Block::Block_1_13_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_13_6, { 1,1 }) { solid = false; }
Block_1_13_7Block::Block_1_13_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_13_7, { 1,1 }) { solid = false; }
Block_1_13_8Block::Block_1_13_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_13_8, { 1,1 }) { solid = false; }
Block_1_13_9Block::Block_1_13_9Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_13_9, { 1,1 }) { solid = false; }
Block_1_13_10Block::Block_1_13_10Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_13_10, { 1,1 }) { solid = false; }
Block_1_13_11Block::Block_1_13_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_13_11, { 1,1 }) { solid = false; }
Block_1_13_12Block::Block_1_13_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_13_12, { 1,1 }) { solid = false; }
Block_1_13_13Block::Block_1_13_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_13_13, { 1,1 }) { solid = false; }
Block_1_13_14Block::Block_1_13_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_13_14, { 1,1 }) { solid = false; }
Block_1_13_15Block::Block_1_13_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_13_15, { 1,1 }) { solid = false; }
Block_1_13_16Block::Block_1_13_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_13_16, { 1,1 }) { solid = false; }
Block_1_14_1Block::Block_1_14_1Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_14_1, { 1,1 }) { solid = false; }
Block_1_14_2Block::Block_1_14_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_14_2, { 1,1 }) { solid = false; }
Block_1_14_3Block::Block_1_14_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_14_3, { 1,1 }) { solid = false; }
Block_1_14_4Block::Block_1_14_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_14_4, { 1,1 }) { solid = false; }
Block_1_14_5Block::Block_1_14_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_14_5, { 1,1 }) { solid = false; }
Block_1_14_6Block::Block_1_14_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_14_6, { 1,1 }) { solid = false; }
Block_1_14_7Block::Block_1_14_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_14_7, { 1,1 }) { solid = false; }
Block_1_14_8Block::Block_1_14_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_14_8, { 1,1 }) { solid = false; }
Block_1_14_9Block::Block_1_14_9Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_14_9, { 1,1 }) { solid = false; }
Block_1_14_10Block::Block_1_14_10Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_14_10, { 1,1 }) { solid = false; }
Block_1_14_11Block::Block_1_14_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_14_11, { 1,1 }) { solid = false; }
Block_1_14_12Block::Block_1_14_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_14_12, { 1,1 }) { solid = false; }
Block_1_14_13Block::Block_1_14_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_14_13, { 1,1 }) { solid = false; }
Block_1_14_14Block::Block_1_14_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_14_14, { 1,1 }) { solid = false; }
Block_1_14_15Block::Block_1_14_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_14_15, { 1,1 }) { solid = false; }
Block_1_14_16Block::Block_1_14_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_14_16, { 1,1 }) { solid = false; }
Block_1_15_1Block::Block_1_15_1Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_15_1, { 1,1 }) { solid = false; }
Block_1_15_2Block::Block_1_15_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_15_2, { 1,1 }) { solid = false; }
Block_1_15_3Block::Block_1_15_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_15_3, { 1,1 }) { solid = false; }
Block_1_15_4Block::Block_1_15_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_15_4, { 1,1 }) { solid = false; }
Block_1_15_5Block::Block_1_15_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_15_5, { 1,1 }) { solid = false; }
Block_1_15_6Block::Block_1_15_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_15_6, { 1,1 }) { solid = false; }
Block_1_15_7Block::Block_1_15_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_15_7, { 1,1 }) { solid = false; }
Block_1_15_8Block::Block_1_15_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_15_8, { 1,1 }) { solid = false; }
Block_1_15_9Block::Block_1_15_9Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_15_9, { 1,1 }) { solid = false; }
Block_1_15_10Block::Block_1_15_10Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_15_10, { 1,1 }) { solid = false; }
Block_1_15_11Block::Block_1_15_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_15_11, { 1,1 }) { solid = false; }
Block_1_15_12Block::Block_1_15_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_15_12, { 1,1 }) { solid = false; }
Block_1_15_13Block::Block_1_15_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_15_13, { 1,1 }) { solid = false; }
Block_1_15_14Block::Block_1_15_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_15_14, { 1,1 }) { solid = false; }
Block_1_15_15Block::Block_1_15_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_15_15, { 1,1 }) { solid = false; }
Block_1_15_16Block::Block_1_15_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_15_16, { 1,1 }) { solid = false; }
Block_1_16_1Block::Block_1_16_1Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_16_1, { 1,1 }) { solid = false; }
Block_1_16_2Block::Block_1_16_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_16_2, { 1,1 }) { solid = false; }
Block_1_16_3Block::Block_1_16_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_16_3, { 1,1 }) { solid = false; }
Block_1_16_4Block::Block_1_16_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_16_4, { 1,1 }) { solid = false; }
Block_1_16_5Block::Block_1_16_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_16_5, { 1,1 }) { solid = false; }
Block_1_16_6Block::Block_1_16_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_16_6, { 1,1 }) { solid = false; }
Block_1_16_7Block::Block_1_16_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_16_7, { 1,1 }) { solid = false; }
Block_1_16_8Block::Block_1_16_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_16_8, { 1,1 }) { solid = false; }
Block_1_16_9Block::Block_1_16_9Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_16_9, { 1,1 }) { solid = false; }
Block_1_16_10Block::Block_1_16_10Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_16_10, { 1,1 }) { solid = false; }
Block_1_16_11Block::Block_1_16_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_16_11, { 1,1 }) { solid = false; }
Block_1_16_12Block::Block_1_16_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_16_12, { 1,1 }) { solid = false; }
Block_1_16_13Block::Block_1_16_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_16_13, { 1,1 }) { solid = false; }
Block_1_16_14Block::Block_1_16_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_16_14, { 1,1 }) { solid = false; }
Block_1_16_15Block::Block_1_16_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_16_15, { 1,1 }) { solid = false; }
Block_1_16_16Block::Block_1_16_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_1_16_16, { 1,1 }) { solid = false; }





Block_2_1_2Block::Block_2_1_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_1_2, { 1,1 }) { solid = false; }
Block_2_1_3Block::Block_2_1_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_1_3, { 1,1 }) { solid = true; }
Block_2_1_12Block::Block_2_1_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_1_12, { 1,1 }) { solid = false; }
Block_2_1_13Block::Block_2_1_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_1_13, { 1,1 }) { solid = false; }
Block_2_1_14Block::Block_2_1_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_1_14, { 1,1 }) { solid = false; }
Block_2_1_15Block::Block_2_1_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_1_15, { 1,1 }) { solid = false; }
Block_2_1_16Block::Block_2_1_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_1_16, { 1,1 }) { solid = false; }
Block_2_2_7Block::Block_2_2_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_2_7, { 1,1 }) { solid = false; }
Block_2_2_8Block::Block_2_2_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_2_8, { 1,1 }) { solid = false; }
Block_2_2_12Block::Block_2_2_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_2_12, { 1,1 }) { solid = false; }
Block_2_2_13Block::Block_2_2_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_2_13, { 1,1 }) { solid = false; }
Block_2_2_14Block::Block_2_2_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_2_14, { 1,1 }) { solid = false; }
Block_2_2_15Block::Block_2_2_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_2_15, { 1,1 }) { solid = false; }
Block_2_2_16Block::Block_2_2_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_2_16, { 1,1 }) { solid = false; }
Block_2_3_4Block::Block_2_3_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_3_4, { 1,1 }) { solid = false; }
Block_2_3_5Block::Block_2_3_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_3_5, { 1,1 }) { solid = false; }
Block_2_3_6Block::Block_2_3_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_3_6, { 1,1 }) { solid = false; }
Block_2_3_7Block::Block_2_3_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_3_7, { 1,1 }) { solid = false; }
Block_2_3_8Block::Block_2_3_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_3_8, { 1,1 }) { solid = false; }
Block_2_3_15Block::Block_2_3_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_3_15, { 1,1 }) { solid = false; }
Block_2_3_16Block::Block_2_3_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_3_16, { 1,1 }) { solid = false; }
Block_2_4_4Block::Block_2_4_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_4_4, { 1,1 }) { solid = false; }
Block_2_4_5Block::Block_2_4_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_4_5, { 1,1 }) { solid = false; }
Block_2_4_6Block::Block_2_4_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_4_6, { 1,1 }) { solid = false; }
Block_2_4_7Block::Block_2_4_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_4_7, { 1,1 }) { solid = false; }
Block_2_4_11Block::Block_2_4_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_4_11, { 1,1 }) { solid = false; }
Block_2_4_12Block::Block_2_4_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_4_12, { 1,1 }) { solid = false; }
Block_2_4_13Block::Block_2_4_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_4_13, { 1,1 }) { solid = false; }
Block_2_4_14Block::Block_2_4_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_4_14, { 1,1 }) { solid = false; }
Block_2_4_15Block::Block_2_4_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_4_15, { 1,1 }) { solid = false; }
Block_2_4_16Block::Block_2_4_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_4_16, { 1,1 }) { solid = false; }
Block_2_5_4Block::Block_2_5_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_5_4, { 1,1 }) { solid = false; }
Block_2_5_5Block::Block_2_5_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_5_5, { 1,1 }) { solid = false; }
Block_2_5_6Block::Block_2_5_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_5_6, { 1,1 }) { solid = false; }
Block_2_5_8Block::Block_2_5_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_5_8, { 1,1 }) { solid = false; }
Block_2_5_9Block::Block_2_5_9Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_5_9, { 1,1 }) { solid = false; }
Block_2_5_10Block::Block_2_5_10Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_5_10, { 1,1 }) { solid = false; }
Block_2_5_11Block::Block_2_5_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_5_11, { 1,1 }) { solid = false; }
Block_2_5_12Block::Block_2_5_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_5_12, { 1,1 }) { solid = false; }
Block_2_5_13Block::Block_2_5_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_5_13, { 1,1 }) { solid = false; }
Block_2_5_14Block::Block_2_5_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_5_14, { 1,1 }) { solid = false; }
Block_2_5_15Block::Block_2_5_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_5_15, { 1,1 }) { solid = false; }
Block_2_5_16Block::Block_2_5_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_5_16, { 1,1 }) { solid = false; }
Block_2_6_5Block::Block_2_6_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_6_5, { 1,1 }) { solid = false; }
Block_2_6_6Block::Block_2_6_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_6_6, { 1,1 }) { solid = false; }
Block_2_7_1Block::Block_2_7_1Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_7_1, { 1,1 }) { solid = false; }
Block_2_7_2Block::Block_2_7_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_7_2, { 1,1 }) { solid = false; }
Block_2_7_3Block::Block_2_7_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_7_3, { 1,1 }) { solid = false; }
Block_2_7_4Block::Block_2_7_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_7_4, { 1,1 }) { solid = false; }
Block_2_7_5Block::Block_2_7_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_7_5, { 1,1 }) { solid = false; }
Block_2_7_6Block::Block_2_7_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_7_6, { 1,1 }) { solid = false; }
Block_2_7_7Block::Block_2_7_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_7_7, { 1,1 }) { solid = false; }
Block_2_7_11Block::Block_2_7_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_7_11, { 1,1 }) { solid = false; }
Block_2_7_12Block::Block_2_7_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_7_12, { 1,1 }) { solid = false; }
Block_2_7_13Block::Block_2_7_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_7_13, { 1,1 }) { solid = false; }
Block_2_7_14Block::Block_2_7_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_7_14, { 1,1 }) { solid = false; }
Block_2_7_15Block::Block_2_7_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_7_15, { 1,1 }) { solid = false; }
Block_2_7_16Block::Block_2_7_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_7_16, { 1,1 }) { solid = false; }
Block_2_13_1Block::Block_2_13_1Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_13_1, { 1,1 }) { solid = false; }
Block_2_13_2Block::Block_2_13_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_13_2, { 1,1 }) { solid = false; }
Block_2_13_3Block::Block_2_13_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_13_3, { 1,1 }) { solid = false; }
Block_2_13_4Block::Block_2_13_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_13_4, { 1,1 }) { solid = false; }
Block_2_13_5Block::Block_2_13_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_13_5, { 1,1 }) { solid = false; }
Block_2_13_6Block::Block_2_13_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_13_6, { 1,1 }) { solid = false; }
Block_2_13_7Block::Block_2_13_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_13_7, { 1,1 }) { solid = false; }
Block_2_13_8Block::Block_2_13_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_13_8, { 1,1 }) { solid = false; }
Block_2_13_9Block::Block_2_13_9Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_13_9, { 1,1 }) { solid = false; }
Block_2_13_10Block::Block_2_13_10Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_13_10, { 1,1 }) { solid = false; }
Block_2_13_11Block::Block_2_13_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_13_11, { 1,1 }) { solid = false; }
Block_2_13_12Block::Block_2_13_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_13_12, { 1,1 }) { solid = false; }
Block_2_13_13Block::Block_2_13_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_13_13, { 1,1 }) { solid = false; }
Block_2_13_14Block::Block_2_13_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_13_14, { 1,1 }) { solid = false; }
Block_2_13_15Block::Block_2_13_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_13_15, { 1,1 }) { solid = false; }
Block_2_13_16Block::Block_2_13_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_13_16, { 1,1 }) { solid = false; }
Block_2_14_1Block::Block_2_14_1Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_14_1, { 1,1 }) { solid = false; }
Block_2_14_2Block::Block_2_14_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_14_2, { 1,1 }) { solid = false; }
Block_2_14_3Block::Block_2_14_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_14_3, { 1,1 }) { solid = false; }
Block_2_14_4Block::Block_2_14_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_14_4, { 1,1 }) { solid = false; }
Block_2_14_5Block::Block_2_14_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_14_5, { 1,1 }) { solid = false; }
Block_2_14_6Block::Block_2_14_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_14_6, { 1,1 }) { solid = false; }
Block_2_14_7Block::Block_2_14_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_14_7, { 1,1 }) { solid = false; }
Block_2_14_8Block::Block_2_14_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_14_8, { 1,1 }) { solid = false; }
Block_2_14_9Block::Block_2_14_9Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_14_9, { 1,1 }) { solid = false; }
Block_2_14_10Block::Block_2_14_10Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_14_10, { 1,1 }) { solid = false; }
Block_2_14_11Block::Block_2_14_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_14_11, { 1,1 }) { solid = false; }
Block_2_14_12Block::Block_2_14_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_14_12, { 1,1 }) { solid = false; }
Block_2_14_13Block::Block_2_14_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_14_13, { 1,1 }) { solid = false; }
Block_2_14_14Block::Block_2_14_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_14_14, { 1,1 }) { solid = false; }
Block_2_14_15Block::Block_2_14_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_14_15, { 1,1 }) { solid = false; }
Block_2_14_16Block::Block_2_14_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_14_16, { 1,1 }) { solid = false; }
Block_2_15_1Block::Block_2_15_1Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_15_1, { 1,1 }) { solid = false; }
Block_2_15_2Block::Block_2_15_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_15_2, { 1,1 }) { solid = false; }
Block_2_15_3Block::Block_2_15_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_15_3, { 1,1 }) { solid = false; }
Block_2_15_4Block::Block_2_15_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_15_4, { 1,1 }) { solid = false; }
Block_2_15_5Block::Block_2_15_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_15_5, { 1,1 }) { solid = false; }
Block_2_15_6Block::Block_2_15_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_15_6, { 1,1 }) { solid = false; }
Block_2_15_7Block::Block_2_15_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_15_7, { 1,1 }) { solid = false; }
Block_2_15_8Block::Block_2_15_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_15_8, { 1,1 }) { solid = false; }
Block_2_15_9Block::Block_2_15_9Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_15_9, { 1,1 }) { solid = false; }
Block_2_15_10Block::Block_2_15_10Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_15_10, { 1,1 }) { solid = false; }
Block_2_15_11Block::Block_2_15_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_15_11, { 1,1 }) { solid = false; }
Block_2_15_12Block::Block_2_15_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_15_12, { 1,1 }) { solid = false; }
Block_2_15_13Block::Block_2_15_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_15_13, { 1,1 }) { solid = false; }
Block_2_15_14Block::Block_2_15_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_15_14, { 1,1 }) { solid = false; }
Block_2_15_15Block::Block_2_15_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_15_15, { 1,1 }) { solid = false; }
Block_2_15_16Block::Block_2_15_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_15_16, { 1,1 }) { solid = false; }
Block_2_16_1Block::Block_2_16_1Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_16_1, { 1,1 }) { solid = false; }
Block_2_16_2Block::Block_2_16_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_16_2, { 1,1 }) { solid = false; }
Block_2_16_3Block::Block_2_16_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_16_3, { 1,1 }) { solid = false; }
Block_2_16_4Block::Block_2_16_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_16_4, { 1,1 }) { solid = false; }
Block_2_16_5Block::Block_2_16_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_16_5, { 1,1 }) { solid = false; }
Block_2_16_6Block::Block_2_16_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_16_6, { 1,1 }) { solid = false; }
Block_2_16_7Block::Block_2_16_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_16_7, { 1,1 }) { solid = false; }
Block_2_16_8Block::Block_2_16_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_16_8, { 1,1 }) { solid = false; }
Block_2_16_9Block::Block_2_16_9Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_16_9, { 1,1 }) { solid = false; }
Block_2_16_10Block::Block_2_16_10Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_16_10, { 1,1 }) { solid = false; }
Block_2_16_11Block::Block_2_16_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_16_11, { 1,1 }) { solid = false; }
Block_2_16_12Block::Block_2_16_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_16_12, { 1,1 }) { solid = false; }
Block_2_16_13Block::Block_2_16_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_16_13, { 1,1 }) { solid = false; }
Block_2_16_14Block::Block_2_16_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_16_14, { 1,1 }) { solid = false; }
Block_2_16_15Block::Block_2_16_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_16_15, { 1,1 }) { solid = false; }
Block_2_16_16Block::Block_2_16_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_16_16, { 1,1 }) { solid = false; }





Block_3_1_2Block::Block_3_1_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_1_2, { 1,1 }) { solid = false; }
Block_3_1_3Block::Block_3_1_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_1_3, { 1,1 }) { solid = true; }
Block_3_1_12Block::Block_3_1_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_1_12, { 1,1 }) { solid = false; }
Block_3_1_13Block::Block_3_1_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_1_13, { 1,1 }) { solid = false; }
Block_3_1_14Block::Block_3_1_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_1_14, { 1,1 }) { solid = false; }
Block_3_1_15Block::Block_3_1_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_1_15, { 1,1 }) { solid = false; }
Block_3_1_16Block::Block_3_1_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_1_16, { 1,1 }) { solid = false; }
Block_3_2_7Block::Block_3_2_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_2_7, { 1,1 }) { solid = false; }
Block_3_2_8Block::Block_3_2_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_2_8, { 1,1 }) { solid = false; }
Block_3_2_12Block::Block_3_2_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_2_12, { 1,1 }) { solid = false; }
Block_3_2_13Block::Block_3_2_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_2_13, { 1,1 }) { solid = false; }
Block_3_2_14Block::Block_3_2_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_2_14, { 1,1 }) { solid = false; }
Block_3_2_15Block::Block_3_2_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_2_15, { 1,1 }) { solid = false; }
Block_3_2_16Block::Block_3_2_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_2_16, { 1,1 }) { solid = false; }
Block_3_3_4Block::Block_3_3_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_3_4, { 1,1 }) { solid = false; }
Block_3_3_5Block::Block_3_3_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_3_5, { 1,1 }) { solid = false; }
Block_3_3_6Block::Block_3_3_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_3_6, { 1,1 }) { solid = false; }
Block_3_3_7Block::Block_3_3_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_3_7, { 1,1 }) { solid = false; }
Block_3_3_8Block::Block_3_3_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_3_8, { 1,1 }) { solid = false; }
Block_3_3_15Block::Block_3_3_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_3_15, { 1,1 }) { solid = false; }
Block_3_3_16Block::Block_3_3_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_3_16, { 1,1 }) { solid = false; }
Block_3_4_4Block::Block_3_4_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_4_4, { 1,1 }) { solid = false; }
Block_3_4_5Block::Block_3_4_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_4_5, { 1,1 }) { solid = false; }
Block_3_4_6Block::Block_3_4_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_4_6, { 1,1 }) { solid = false; }
Block_3_4_7Block::Block_3_4_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_4_7, { 1,1 }) { solid = false; }
Block_3_4_11Block::Block_3_4_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_4_11, { 1,1 }) { solid = false; }
Block_3_4_12Block::Block_3_4_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_4_12, { 1,1 }) { solid = false; }
Block_3_4_13Block::Block_3_4_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_4_13, { 1,1 }) { solid = false; }
Block_3_4_14Block::Block_3_4_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_4_14, { 1,1 }) { solid = false; }
Block_3_4_15Block::Block_3_4_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_4_15, { 1,1 }) { solid = false; }
Block_3_4_16Block::Block_3_4_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_4_16, { 1,1 }) { solid = false; }
Block_3_5_4Block::Block_3_5_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_5_4, { 1,1 }) { solid = false; }
Block_3_5_5Block::Block_3_5_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_5_5, { 1,1 }) { solid = false; }
Block_3_5_6Block::Block_3_5_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_5_6, { 1,1 }) { solid = false; }
Block_3_5_8Block::Block_3_5_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_5_8, { 1,1 }) { solid = false; }
Block_3_5_9Block::Block_3_5_9Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_5_9, { 1,1 }) { solid = false; }
Block_3_5_10Block::Block_3_5_10Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_5_10, { 1,1 }) { solid = false; }
Block_3_5_11Block::Block_3_5_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_5_11, { 1,1 }) { solid = false; }
Block_3_5_12Block::Block_3_5_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_5_12, { 1,1 }) { solid = false; }
Block_3_5_13Block::Block_3_5_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_5_13, { 1,1 }) { solid = false; }
Block_3_5_14Block::Block_3_5_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_5_14, { 1,1 }) { solid = false; }
Block_3_5_15Block::Block_3_5_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_5_15, { 1,1 }) { solid = false; }
Block_3_5_16Block::Block_3_5_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_5_16, { 1,1 }) { solid = false; }
Block_3_6_5Block::Block_3_6_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_6_5, { 1,1 }) { solid = false; }
Block_3_6_6Block::Block_3_6_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_6_6, { 1,1 }) { solid = false; }
Block_3_7_1Block::Block_3_7_1Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_7_1, { 1,1 }) { solid = false; }
Block_3_7_2Block::Block_3_7_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_7_2, { 1,1 }) { solid = false; }
Block_3_7_3Block::Block_3_7_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_7_3, { 1,1 }) { solid = false; }
Block_3_7_4Block::Block_3_7_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_7_4, { 1,1 }) { solid = false; }
Block_3_7_5Block::Block_3_7_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_7_5, { 1,1 }) { solid = false; }
Block_3_7_6Block::Block_3_7_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_7_6, { 1,1 }) { solid = false; }
Block_3_7_7Block::Block_3_7_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_7_7, { 1,1 }) { solid = false; }
Block_3_7_11Block::Block_3_7_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_7_11, { 1,1 }) { solid = false; }
Block_3_7_12Block::Block_3_7_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_7_12, { 1,1 }) { solid = false; }
Block_3_7_13Block::Block_3_7_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_7_13, { 1,1 }) { solid = false; }
Block_3_7_14Block::Block_3_7_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_7_14, { 1,1 }) { solid = false; }
Block_3_7_15Block::Block_3_7_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_7_15, { 1,1 }) { solid = false; }
Block_3_7_16Block::Block_3_7_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_7_16, { 1,1 }) { solid = false; }
Block_3_13_1Block::Block_3_13_1Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_13_1, { 1,1 }) { solid = false; }
Block_3_13_2Block::Block_3_13_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_13_2, { 1,1 }) { solid = false; }
Block_3_13_3Block::Block_3_13_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_13_3, { 1,1 }) { solid = false; }
Block_3_13_4Block::Block_3_13_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_13_4, { 1,1 }) { solid = false; }
Block_3_13_5Block::Block_3_13_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_13_5, { 1,1 }) { solid = false; }
Block_3_13_6Block::Block_3_13_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_13_6, { 1,1 }) { solid = false; }
Block_3_13_7Block::Block_3_13_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_13_7, { 1,1 }) { solid = false; }
Block_3_13_8Block::Block_3_13_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_13_8, { 1,1 }) { solid = false; }
Block_3_13_9Block::Block_3_13_9Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_13_9, { 1,1 }) { solid = false; }
Block_3_13_10Block::Block_3_13_10Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_13_10, { 1,1 }) { solid = false; }
Block_3_13_11Block::Block_3_13_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_13_11, { 1,1 }) { solid = false; }
Block_3_13_12Block::Block_3_13_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_13_12, { 1,1 }) { solid = false; }
Block_3_13_13Block::Block_3_13_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_13_13, { 1,1 }) { solid = false; }
Block_3_13_14Block::Block_3_13_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_13_14, { 1,1 }) { solid = false; }
Block_3_13_15Block::Block_3_13_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_13_15, { 1,1 }) { solid = false; }
Block_3_13_16Block::Block_3_13_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_13_16, { 1,1 }) { solid = false; }
Block_3_14_1Block::Block_3_14_1Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_14_1, { 1,1 }) { solid = false; }
Block_3_14_2Block::Block_3_14_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_14_2, { 1,1 }) { solid = false; }
Block_3_14_3Block::Block_3_14_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_14_3, { 1,1 }) { solid = false; }
Block_3_14_4Block::Block_3_14_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_14_4, { 1,1 }) { solid = false; }
Block_3_14_5Block::Block_3_14_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_14_5, { 1,1 }) { solid = false; }
Block_3_14_6Block::Block_3_14_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_14_6, { 1,1 }) { solid = false; }
Block_3_14_7Block::Block_3_14_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_14_7, { 1,1 }) { solid = false; }
Block_3_14_8Block::Block_3_14_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_14_8, { 1,1 }) { solid = false; }
Block_3_14_9Block::Block_3_14_9Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_14_9, { 1,1 }) { solid = false; }
Block_3_14_10Block::Block_3_14_10Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_14_10, { 1,1 }) { solid = false; }
Block_3_14_11Block::Block_3_14_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_14_11, { 1,1 }) { solid = false; }
Block_3_14_12Block::Block_3_14_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_14_12, { 1,1 }) { solid = false; }
Block_3_14_13Block::Block_3_14_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_14_13, { 1,1 }) { solid = false; }
Block_3_14_14Block::Block_3_14_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_14_14, { 1,1 }) { solid = false; }
Block_3_14_15Block::Block_3_14_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_14_15, { 1,1 }) { solid = false; }
Block_3_14_16Block::Block_3_14_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_14_16, { 1,1 }) { solid = false; }
Block_3_15_1Block::Block_3_15_1Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_15_1, { 1,1 }) { solid = false; }
Block_3_15_2Block::Block_3_15_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_15_2, { 1,1 }) { solid = false; }
Block_3_15_3Block::Block_3_15_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_15_3, { 1,1 }) { solid = false; }
Block_3_15_4Block::Block_3_15_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_15_4, { 1,1 }) { solid = false; }
Block_3_15_5Block::Block_3_15_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_15_5, { 1,1 }) { solid = false; }
Block_3_15_6Block::Block_3_15_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_15_6, { 1,1 }) { solid = false; }
Block_3_15_7Block::Block_3_15_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_15_7, { 1,1 }) { solid = false; }
Block_3_15_8Block::Block_3_15_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_15_8, { 1,1 }) { solid = false; }
Block_3_15_9Block::Block_3_15_9Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_15_9, { 1,1 }) { solid = false; }
Block_3_15_10Block::Block_3_15_10Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_15_10, { 1,1 }) { solid = false; }
Block_3_15_11Block::Block_3_15_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_15_11, { 1,1 }) { solid = false; }
Block_3_15_12Block::Block_3_15_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_15_12, { 1,1 }) { solid = false; }
Block_3_15_13Block::Block_3_15_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_15_13, { 1,1 }) { solid = false; }
Block_3_15_14Block::Block_3_15_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_15_14, { 1,1 }) { solid = false; }
Block_3_15_15Block::Block_3_15_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_15_15, { 1,1 }) { solid = false; }
Block_3_15_16Block::Block_3_15_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_15_16, { 1,1 }) { solid = false; }
Block_3_16_1Block::Block_3_16_1Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_16_1, { 1,1 }) { solid = false; }
Block_3_16_2Block::Block_3_16_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_16_2, { 1,1 }) { solid = false; }
Block_3_16_3Block::Block_3_16_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_16_3, { 1,1 }) { solid = false; }
Block_3_16_4Block::Block_3_16_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_16_4, { 1,1 }) { solid = false; }
Block_3_16_5Block::Block_3_16_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_16_5, { 1,1 }) { solid = false; }
Block_3_16_6Block::Block_3_16_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_16_6, { 1,1 }) { solid = false; }
Block_3_16_7Block::Block_3_16_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_16_7, { 1,1 }) { solid = false; }
Block_3_16_8Block::Block_3_16_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_16_8, { 1,1 }) { solid = false; }
Block_3_16_9Block::Block_3_16_9Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_16_9, { 1,1 }) { solid = false; }
Block_3_16_10Block::Block_3_16_10Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_16_10, { 1,1 }) { solid = false; }
Block_3_16_11Block::Block_3_16_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_16_11, { 1,1 }) { solid = false; }
Block_3_16_12Block::Block_3_16_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_16_12, { 1,1 }) { solid = false; }
Block_3_16_13Block::Block_3_16_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_16_13, { 1,1 }) { solid = false; }
Block_3_16_14Block::Block_3_16_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_16_14, { 1,1 }) { solid = false; }
Block_3_16_15Block::Block_3_16_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_16_15, { 1,1 }) { solid = false; }
Block_3_16_16Block::Block_3_16_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_16_16, { 1,1 }) { solid = false; }





Block_4_1_2Block::Block_4_1_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_1_2, { 1,1 }) { solid = false; }
Block_4_1_3Block::Block_4_1_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_1_3, { 1,1 }) { solid = true; }
Block_4_1_12Block::Block_4_1_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_1_12, { 1,1 }) { solid = false; }
Block_4_1_13Block::Block_4_1_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_1_13, { 1,1 }) { solid = false; }
Block_4_1_14Block::Block_4_1_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_1_14, { 1,1 }) { solid = false; }
Block_4_1_15Block::Block_4_1_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_1_15, { 1,1 }) { solid = false; }
Block_4_1_16Block::Block_4_1_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_1_16, { 1,1 }) { solid = false; }
Block_4_2_7Block::Block_4_2_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_2_7, { 1,1 }) { solid = false; }
Block_4_2_8Block::Block_4_2_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_2_8, { 1,1 }) { solid = false; }
Block_4_2_12Block::Block_4_2_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_2_12, { 1,1 }) { solid = false; }
Block_4_2_13Block::Block_4_2_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_2_13, { 1,1 }) { solid = false; }
Block_4_2_14Block::Block_4_2_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_2_14, { 1,1 }) { solid = false; }
Block_4_2_15Block::Block_4_2_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_2_15, { 1,1 }) { solid = false; }
Block_4_2_16Block::Block_4_2_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_2_16, { 1,1 }) { solid = false; }
Block_4_3_4Block::Block_4_3_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_3_4, { 1,1 }) { solid = false; }
Block_4_3_5Block::Block_4_3_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_3_5, { 1,1 }) { solid = false; }
Block_4_3_6Block::Block_4_3_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_3_6, { 1,1 }) { solid = false; }
Block_4_3_7Block::Block_4_3_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_3_7, { 1,1 }) { solid = false; }
Block_4_3_8Block::Block_4_3_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_3_8, { 1,1 }) { solid = false; }
Block_4_3_15Block::Block_4_3_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_3_15, { 1,1 }) { solid = false; }
Block_4_3_16Block::Block_4_3_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_3_16, { 1,1 }) { solid = false; }
Block_4_4_4Block::Block_4_4_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_4_4, { 1,1 }) { solid = false; }
Block_4_4_5Block::Block_4_4_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_4_5, { 1,1 }) { solid = false; }
Block_4_4_6Block::Block_4_4_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_4_6, { 1,1 }) { solid = false; }
Block_4_4_7Block::Block_4_4_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_4_7, { 1,1 }) { solid = false; }
Block_4_4_11Block::Block_4_4_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_4_11, { 1,1 }) { solid = false; }
Block_4_4_12Block::Block_4_4_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_4_12, { 1,1 }) { solid = false; }
Block_4_4_13Block::Block_4_4_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_4_13, { 1,1 }) { solid = false; }
Block_4_4_14Block::Block_4_4_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_4_14, { 1,1 }) { solid = false; }
Block_4_4_15Block::Block_4_4_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_4_15, { 1,1 }) { solid = false; }
Block_4_4_16Block::Block_4_4_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_4_16, { 1,1 }) { solid = false; }
Block_4_5_4Block::Block_4_5_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_5_4, { 1,1 }) { solid = false; }
Block_4_5_5Block::Block_4_5_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_5_5, { 1,1 }) { solid = false; }
Block_4_5_6Block::Block_4_5_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_5_6, { 1,1 }) { solid = false; }
Block_4_5_8Block::Block_4_5_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_5_8, { 1,1 }) { solid = false; }
Block_4_5_9Block::Block_4_5_9Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_5_9, { 1,1 }) { solid = false; }
Block_4_5_10Block::Block_4_5_10Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_5_10, { 1,1 }) { solid = false; }
Block_4_5_11Block::Block_4_5_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_5_11, { 1,1 }) { solid = false; }
Block_4_5_12Block::Block_4_5_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_5_12, { 1,1 }) { solid = false; }
Block_4_5_13Block::Block_4_5_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_5_13, { 1,1 }) { solid = false; }
Block_4_5_14Block::Block_4_5_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_5_14, { 1,1 }) { solid = false; }
Block_4_5_15Block::Block_4_5_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_5_15, { 1,1 }) { solid = false; }
Block_4_5_16Block::Block_4_5_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_5_16, { 1,1 }) { solid = false; }
Block_4_6_5Block::Block_4_6_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_6_5, { 1,1 }) { solid = false; }
Block_4_6_6Block::Block_4_6_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_6_6, { 1,1 }) { solid = false; }
Block_4_7_1Block::Block_4_7_1Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_7_1, { 1,1 }) { solid = false; }
Block_4_7_2Block::Block_4_7_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_7_2, { 1,1 }) { solid = false; }
Block_4_7_3Block::Block_4_7_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_7_3, { 1,1 }) { solid = false; }
Block_4_7_4Block::Block_4_7_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_7_4, { 1,1 }) { solid = false; }
Block_4_7_5Block::Block_4_7_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_7_5, { 1,1 }) { solid = false; }
Block_4_7_6Block::Block_4_7_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_7_6, { 1,1 }) { solid = false; }
Block_4_7_7Block::Block_4_7_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_7_7, { 1,1 }) { solid = false; }
Block_4_7_11Block::Block_4_7_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_7_11, { 1,1 }) { solid = false; }
Block_4_7_12Block::Block_4_7_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_7_12, { 1,1 }) { solid = false; }
Block_4_7_13Block::Block_4_7_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_7_13, { 1,1 }) { solid = false; }
Block_4_7_14Block::Block_4_7_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_7_14, { 1,1 }) { solid = false; }
Block_4_7_15Block::Block_4_7_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_7_15, { 1,1 }) { solid = false; }
Block_4_7_16Block::Block_4_7_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_7_16, { 1,1 }) { solid = false; }
Block_4_13_1Block::Block_4_13_1Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_13_1, { 1,1 }) { solid = false; }
Block_4_13_2Block::Block_4_13_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_13_2, { 1,1 }) { solid = false; }
Block_4_13_3Block::Block_4_13_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_13_3, { 1,1 }) { solid = false; }
Block_4_13_4Block::Block_4_13_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_13_4, { 1,1 }) { solid = false; }
Block_4_13_5Block::Block_4_13_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_13_5, { 1,1 }) { solid = false; }
Block_4_13_6Block::Block_4_13_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_13_6, { 1,1 }) { solid = false; }
Block_4_13_7Block::Block_4_13_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_13_7, { 1,1 }) { solid = false; }
Block_4_13_8Block::Block_4_13_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_13_8, { 1,1 }) { solid = false; }
Block_4_13_9Block::Block_4_13_9Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_13_9, { 1,1 }) { solid = false; }
Block_4_13_10Block::Block_4_13_10Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_13_10, { 1,1 }) { solid = false; }
Block_4_13_11Block::Block_4_13_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_13_11, { 1,1 }) { solid = false; }
Block_4_13_12Block::Block_4_13_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_13_12, { 1,1 }) { solid = false; }
Block_4_13_13Block::Block_4_13_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_13_13, { 1,1 }) { solid = false; }
Block_4_13_14Block::Block_4_13_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_13_14, { 1,1 }) { solid = false; }
Block_4_13_15Block::Block_4_13_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_13_15, { 1,1 }) { solid = false; }
Block_4_13_16Block::Block_4_13_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_13_16, { 1,1 }) { solid = false; }
Block_4_14_1Block::Block_4_14_1Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_14_1, { 1,1 }) { solid = false; }
Block_4_14_2Block::Block_4_14_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_14_2, { 1,1 }) { solid = false; }
Block_4_14_3Block::Block_4_14_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_14_3, { 1,1 }) { solid = false; }
Block_4_14_4Block::Block_4_14_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_14_4, { 1,1 }) { solid = false; }
Block_4_14_5Block::Block_4_14_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_14_5, { 1,1 }) { solid = false; }
Block_4_14_6Block::Block_4_14_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_14_6, { 1,1 }) { solid = false; }
Block_4_14_7Block::Block_4_14_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_14_7, { 1,1 }) { solid = false; }
Block_4_14_8Block::Block_4_14_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_14_8, { 1,1 }) { solid = false; }
Block_4_14_9Block::Block_4_14_9Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_14_9, { 1,1 }) { solid = false; }
Block_4_14_10Block::Block_4_14_10Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_14_10, { 1,1 }) { solid = false; }
Block_4_14_11Block::Block_4_14_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_14_11, { 1,1 }) { solid = false; }
Block_4_14_12Block::Block_4_14_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_14_12, { 1,1 }) { solid = false; }
Block_4_14_13Block::Block_4_14_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_14_13, { 1,1 }) { solid = false; }
Block_4_14_14Block::Block_4_14_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_14_14, { 1,1 }) { solid = false; }
Block_4_14_15Block::Block_4_14_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_14_15, { 1,1 }) { solid = false; }
Block_4_14_16Block::Block_4_14_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_14_16, { 1,1 }) { solid = false; }
Block_4_15_1Block::Block_4_15_1Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_15_1, { 1,1 }) { solid = false; }
Block_4_15_2Block::Block_4_15_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_15_2, { 1,1 }) { solid = false; }
Block_4_15_3Block::Block_4_15_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_15_3, { 1,1 }) { solid = false; }
Block_4_15_4Block::Block_4_15_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_15_4, { 1,1 }) { solid = false; }
Block_4_15_5Block::Block_4_15_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_15_5, { 1,1 }) { solid = false; }
Block_4_15_6Block::Block_4_15_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_15_6, { 1,1 }) { solid = false; }
Block_4_15_7Block::Block_4_15_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_15_7, { 1,1 }) { solid = false; }
Block_4_15_8Block::Block_4_15_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_15_8, { 1,1 }) { solid = false; }
Block_4_15_9Block::Block_4_15_9Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_15_9, { 1,1 }) { solid = false; }
Block_4_15_10Block::Block_4_15_10Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_15_10, { 1,1 }) { solid = false; }
Block_4_15_11Block::Block_4_15_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_15_11, { 1,1 }) { solid = false; }
Block_4_15_12Block::Block_4_15_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_15_12, { 1,1 }) { solid = false; }
Block_4_15_13Block::Block_4_15_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_15_13, { 1,1 }) { solid = false; }
Block_4_15_14Block::Block_4_15_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_15_14, { 1,1 }) { solid = false; }
Block_4_15_15Block::Block_4_15_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_15_15, { 1,1 }) { solid = false; }
Block_4_15_16Block::Block_4_15_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_15_16, { 1,1 }) { solid = false; }
Block_4_16_1Block::Block_4_16_1Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_16_1, { 1,1 }) { solid = false; }
Block_4_16_2Block::Block_4_16_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_16_2, { 1,1 }) { solid = false; }
Block_4_16_3Block::Block_4_16_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_16_3, { 1,1 }) { solid = false; }
Block_4_16_4Block::Block_4_16_4Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_16_4, { 1,1 }) { solid = false; }
Block_4_16_5Block::Block_4_16_5Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_16_5, { 1,1 }) { solid = false; }
Block_4_16_6Block::Block_4_16_6Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_16_6, { 1,1 }) { solid = false; }
Block_4_16_7Block::Block_4_16_7Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_16_7, { 1,1 }) { solid = false; }
Block_4_16_8Block::Block_4_16_8Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_16_8, { 1,1 }) { solid = false; }
Block_4_16_9Block::Block_4_16_9Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_16_9, { 1,1 }) { solid = false; }
Block_4_16_10Block::Block_4_16_10Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_16_10, { 1,1 }) { solid = false; }
Block_4_16_11Block::Block_4_16_11Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_16_11, { 1,1 }) { solid = false; }
Block_4_16_12Block::Block_4_16_12Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_16_12, { 1,1 }) { solid = false; }
Block_4_16_13Block::Block_4_16_13Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_16_13, { 1,1 }) { solid = false; }
Block_4_16_14Block::Block_4_16_14Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_16_14, { 1,1 }) { solid = false; }
Block_4_16_15Block::Block_4_16_15Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_16_15, { 1,1 }) { solid = false; }
Block_4_16_16Block::Block_4_16_16Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_16_16, { 1,1 }) { solid = false; }

json Block::toJson() const {
    json data;
    data["saveType"] = getSaveType();
    data["blockType"] = static_cast<int>(blockType);
    data["position"] = { position.x, position.y };
    data["gridPosition"] = { gridPosition.x, gridPosition.y };
    data["size"] = { size.x, size.y };
    data["solid"] = solid;
    return data;
}

void Block::fromJson(const json& data) {
    blockType = static_cast<BlockType>(data["blockType"]);
    position = { data["position"][0], data["position"][1] };
    gridPosition = { data["gridPosition"][0], data["gridPosition"][1] };
    size = { data["size"][0], data["size"][1] };
    solid = data["solid"];
}

std::string Block::getSaveType() const {
    return "Block";
}

void Block::setItemType(std::string type) {
    itemType = type;
} 