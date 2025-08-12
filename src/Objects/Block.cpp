#include "../../include/Objects/Block.h"
#include "../../include/System/Grid.h"
#include "../../include/System/TextureManager.h"
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
                                static_cast<uint16>(ObjectCategory::BLOCK);
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

    Rectangle destRec = { position.x, position.y, hitbox.width, hitbox.height};
    Rectangle srcRec;

    switch (blockType) {
        case(BlockType::GROUND): srcRec = Constants::PaletteResources::GROUND;
            break;
        case(BlockType::BRICK): srcRec = Constants::PaletteResources::BRICK;
            break;
        case(BlockType::BLOCK_1_1_2):
            srcRec = Constants::PaletteResources::BLOCK_1_1_2;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_1_3):
            srcRec = Constants::PaletteResources::BLOCK_1_1_3;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_1_1_12):
            srcRec = Constants::PaletteResources::BLOCK_1_1_12;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_1_13):
            srcRec = Constants::PaletteResources::BLOCK_1_1_13;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_1_14):
            srcRec = Constants::PaletteResources::BLOCK_1_1_14;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_1_15):
            srcRec = Constants::PaletteResources::BLOCK_1_1_15;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_1_16):
            srcRec = Constants::PaletteResources::BLOCK_1_1_16;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_1_2_7):
            srcRec = Constants::PaletteResources::BLOCK_1_2_7;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_2_8):
            srcRec = Constants::PaletteResources::BLOCK_1_2_8;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_1_2_12):
            srcRec = Constants::PaletteResources::BLOCK_1_2_12;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_2_13):
            srcRec = Constants::PaletteResources::BLOCK_1_2_13;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_2_14):
            srcRec = Constants::PaletteResources::BLOCK_1_2_14;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_2_15):
            srcRec = Constants::PaletteResources::BLOCK_1_2_15;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_2_16):
            srcRec = Constants::PaletteResources::BLOCK_1_2_16;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_1_3_4):
            srcRec = Constants::PaletteResources::BLOCK_1_3_4;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_3_5):
            srcRec = Constants::PaletteResources::BLOCK_1_3_5;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_3_6):
            srcRec = Constants::PaletteResources::BLOCK_1_3_6;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_3_7):
            srcRec = Constants::PaletteResources::BLOCK_1_3_7;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_3_8):
            srcRec = Constants::PaletteResources::BLOCK_1_3_8;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_1_3_15):
            srcRec = Constants::PaletteResources::BLOCK_1_3_15;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_3_16):
            srcRec = Constants::PaletteResources::BLOCK_1_3_16;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_1_4_4):
            srcRec = Constants::PaletteResources::BLOCK_1_4_4;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_4_5):
            srcRec = Constants::PaletteResources::BLOCK_1_4_5;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_4_6):
            srcRec = Constants::PaletteResources::BLOCK_1_4_6;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_4_7):
            srcRec = Constants::PaletteResources::BLOCK_1_4_7;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_1_4_11):
            srcRec = Constants::PaletteResources::BLOCK_1_4_11;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_4_12):
            srcRec = Constants::PaletteResources::BLOCK_1_4_12;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_4_13):
            srcRec = Constants::PaletteResources::BLOCK_1_4_13;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_4_14):
            srcRec = Constants::PaletteResources::BLOCK_1_4_14;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_4_15):
            srcRec = Constants::PaletteResources::BLOCK_1_4_15;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_4_16):
            srcRec = Constants::PaletteResources::BLOCK_1_4_16;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_1_5_4):
            srcRec = Constants::PaletteResources::BLOCK_1_5_4;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_5_5):
            srcRec = Constants::PaletteResources::BLOCK_1_5_5;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_5_6):
            srcRec = Constants::PaletteResources::BLOCK_1_5_6;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_1_5_8):
            srcRec = Constants::PaletteResources::BLOCK_1_5_8;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_5_9):
            srcRec = Constants::PaletteResources::BLOCK_1_5_9;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_5_10):
            srcRec = Constants::PaletteResources::BLOCK_1_5_10;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_5_11):
            srcRec = Constants::PaletteResources::BLOCK_1_5_11;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_5_12):
            srcRec = Constants::PaletteResources::BLOCK_1_5_12;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_5_13):
            srcRec = Constants::PaletteResources::BLOCK_1_5_13;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_5_14):
            srcRec = Constants::PaletteResources::BLOCK_1_5_14;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_5_15):
            srcRec = Constants::PaletteResources::BLOCK_1_5_15;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_5_16):
            srcRec = Constants::PaletteResources::BLOCK_1_5_16;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_1_6_5):
            srcRec = Constants::PaletteResources::BLOCK_1_6_5;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_6_6):
            srcRec = Constants::PaletteResources::BLOCK_1_6_6;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_1_7_1):
            srcRec = Constants::PaletteResources::BLOCK_1_7_1;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_7_2):
            srcRec = Constants::PaletteResources::BLOCK_1_7_2;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_7_3):
            srcRec = Constants::PaletteResources::BLOCK_1_7_3;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_7_4):
            srcRec = Constants::PaletteResources::BLOCK_1_7_4;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_7_5):
            srcRec = Constants::PaletteResources::BLOCK_1_7_5;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_7_6):
            srcRec = Constants::PaletteResources::BLOCK_1_7_6;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_7_7):
            srcRec = Constants::PaletteResources::BLOCK_1_7_7;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_1_7_11):
            srcRec = Constants::PaletteResources::BLOCK_1_7_11;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_7_12):
            srcRec = Constants::PaletteResources::BLOCK_1_7_12;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_7_13):
            srcRec = Constants::PaletteResources::BLOCK_1_7_13;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_7_14):
            srcRec = Constants::PaletteResources::BLOCK_1_7_14;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_7_15):
            srcRec = Constants::PaletteResources::BLOCK_1_7_15;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_1_7_16):
            srcRec = Constants::PaletteResources::BLOCK_1_7_16;
            DrawTexturePro(TextureManager::day_groundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_1_2):
            srcRec = Constants::PaletteResources::BLOCK_1_1_2;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_1_3):
            srcRec = Constants::PaletteResources::BLOCK_1_1_3;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_2_1_12):
            srcRec = Constants::PaletteResources::BLOCK_1_1_12;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_1_13):
            srcRec = Constants::PaletteResources::BLOCK_1_1_13;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_1_14):
            srcRec = Constants::PaletteResources::BLOCK_1_1_14;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_1_15):
            srcRec = Constants::PaletteResources::BLOCK_1_1_15;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_1_16):
            srcRec = Constants::PaletteResources::BLOCK_1_1_16;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_2_2_7):
            srcRec = Constants::PaletteResources::BLOCK_1_2_7;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_2_8):
            srcRec = Constants::PaletteResources::BLOCK_1_2_8;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_2_2_12):
            srcRec = Constants::PaletteResources::BLOCK_1_2_12;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_2_13):
            srcRec = Constants::PaletteResources::BLOCK_1_2_13;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_2_14):
            srcRec = Constants::PaletteResources::BLOCK_1_2_14;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_2_15):
            srcRec = Constants::PaletteResources::BLOCK_1_2_15;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_2_16):
            srcRec = Constants::PaletteResources::BLOCK_1_2_16;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_2_3_4):
            srcRec = Constants::PaletteResources::BLOCK_1_3_4;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_3_5):
            srcRec = Constants::PaletteResources::BLOCK_1_3_5;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_3_6):
            srcRec = Constants::PaletteResources::BLOCK_1_3_6;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_3_7):
            srcRec = Constants::PaletteResources::BLOCK_1_3_7;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_3_8):
            srcRec = Constants::PaletteResources::BLOCK_1_3_8;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_2_3_15):
            srcRec = Constants::PaletteResources::BLOCK_1_3_15;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_3_16):
            srcRec = Constants::PaletteResources::BLOCK_1_3_16;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_2_4_4):
            srcRec = Constants::PaletteResources::BLOCK_1_4_4;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_4_5):
            srcRec = Constants::PaletteResources::BLOCK_1_4_5;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_4_6):
            srcRec = Constants::PaletteResources::BLOCK_1_4_6;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_4_7):
            srcRec = Constants::PaletteResources::BLOCK_1_4_7;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_2_4_11):
            srcRec = Constants::PaletteResources::BLOCK_1_4_11;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_4_12):
            srcRec = Constants::PaletteResources::BLOCK_1_4_12;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_4_13):
            srcRec = Constants::PaletteResources::BLOCK_1_4_13;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_4_14):
            srcRec = Constants::PaletteResources::BLOCK_1_4_14;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_4_15):
            srcRec = Constants::PaletteResources::BLOCK_1_4_15;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_4_16):
            srcRec = Constants::PaletteResources::BLOCK_1_4_16;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_2_5_4):
            srcRec = Constants::PaletteResources::BLOCK_1_5_4;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_5_5):
            srcRec = Constants::PaletteResources::BLOCK_1_5_5;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_5_6):
            srcRec = Constants::PaletteResources::BLOCK_1_5_6;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_2_5_8):
            srcRec = Constants::PaletteResources::BLOCK_1_5_8;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_5_9):
            srcRec = Constants::PaletteResources::BLOCK_1_5_9;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_5_10):
            srcRec = Constants::PaletteResources::BLOCK_1_5_10;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_5_11):
            srcRec = Constants::PaletteResources::BLOCK_1_5_11;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_5_12):
            srcRec = Constants::PaletteResources::BLOCK_1_5_12;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_5_13):
            srcRec = Constants::PaletteResources::BLOCK_1_5_13;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_5_14):
            srcRec = Constants::PaletteResources::BLOCK_1_5_14;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_5_15):
            srcRec = Constants::PaletteResources::BLOCK_1_5_15;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_5_16):
            srcRec = Constants::PaletteResources::BLOCK_1_5_16;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_2_6_5):
            srcRec = Constants::PaletteResources::BLOCK_1_6_5;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_6_6):
            srcRec = Constants::PaletteResources::BLOCK_1_6_6;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_2_7_1):
            srcRec = Constants::PaletteResources::BLOCK_1_7_1;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_7_2):
            srcRec = Constants::PaletteResources::BLOCK_1_7_2;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_7_3):
            srcRec = Constants::PaletteResources::BLOCK_1_7_3;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_7_4):
            srcRec = Constants::PaletteResources::BLOCK_1_7_4;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_7_5):
            srcRec = Constants::PaletteResources::BLOCK_1_7_5;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_7_6):
            srcRec = Constants::PaletteResources::BLOCK_1_7_6;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_7_7):
            srcRec = Constants::PaletteResources::BLOCK_1_7_7;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_2_7_11):
            srcRec = Constants::PaletteResources::BLOCK_1_7_11;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_7_12):
            srcRec = Constants::PaletteResources::BLOCK_1_7_12;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_7_13):
            srcRec = Constants::PaletteResources::BLOCK_1_7_13;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_7_14):
            srcRec = Constants::PaletteResources::BLOCK_1_7_14;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_7_15):
            srcRec = Constants::PaletteResources::BLOCK_1_7_15;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_2_7_16):
            srcRec = Constants::PaletteResources::BLOCK_1_7_16;
            DrawTexturePro(TextureManager::day_undergroundTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_3_1_2):
            srcRec = Constants::PaletteResources::BLOCK_1_1_2;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_1_3):
            srcRec = Constants::PaletteResources::BLOCK_1_1_3;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_3_1_12):
            srcRec = Constants::PaletteResources::BLOCK_1_1_12;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_1_13):
            srcRec = Constants::PaletteResources::BLOCK_1_1_13;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_1_14):
            srcRec = Constants::PaletteResources::BLOCK_1_1_14;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_1_15):
            srcRec = Constants::PaletteResources::BLOCK_1_1_15;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_1_16):
            srcRec = Constants::PaletteResources::BLOCK_1_1_16;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_3_2_7):
            srcRec = Constants::PaletteResources::BLOCK_1_2_7;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_2_8):
            srcRec = Constants::PaletteResources::BLOCK_1_2_8;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_3_2_12):
            srcRec = Constants::PaletteResources::BLOCK_1_2_12;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_2_13):
            srcRec = Constants::PaletteResources::BLOCK_1_2_13;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_2_14):
            srcRec = Constants::PaletteResources::BLOCK_1_2_14;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_2_15):
            srcRec = Constants::PaletteResources::BLOCK_1_2_15;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_2_16):
            srcRec = Constants::PaletteResources::BLOCK_1_2_16;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_3_3_4):
            srcRec = Constants::PaletteResources::BLOCK_1_3_4;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_3_5):
            srcRec = Constants::PaletteResources::BLOCK_1_3_5;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_3_6):
            srcRec = Constants::PaletteResources::BLOCK_1_3_6;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_3_7):
            srcRec = Constants::PaletteResources::BLOCK_1_3_7;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_3_8):
            srcRec = Constants::PaletteResources::BLOCK_1_3_8;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_3_3_15):
            srcRec = Constants::PaletteResources::BLOCK_1_3_15;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_3_16):
            srcRec = Constants::PaletteResources::BLOCK_1_3_16;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_3_4_4):
            srcRec = Constants::PaletteResources::BLOCK_1_4_4;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_4_5):
            srcRec = Constants::PaletteResources::BLOCK_1_4_5;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_4_6):
            srcRec = Constants::PaletteResources::BLOCK_1_4_6;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_4_7):
            srcRec = Constants::PaletteResources::BLOCK_1_4_7;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_3_4_11):
            srcRec = Constants::PaletteResources::BLOCK_1_4_11;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_4_12):
            srcRec = Constants::PaletteResources::BLOCK_1_4_12;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_4_13):
            srcRec = Constants::PaletteResources::BLOCK_1_4_13;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_4_14):
            srcRec = Constants::PaletteResources::BLOCK_1_4_14;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_4_15):
            srcRec = Constants::PaletteResources::BLOCK_1_4_15;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_4_16):
            srcRec = Constants::PaletteResources::BLOCK_1_4_16;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_3_5_4):
            srcRec = Constants::PaletteResources::BLOCK_1_5_4;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_5_5):
            srcRec = Constants::PaletteResources::BLOCK_1_5_5;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_5_6):
            srcRec = Constants::PaletteResources::BLOCK_1_5_6;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_3_5_8):
            srcRec = Constants::PaletteResources::BLOCK_1_5_8;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_5_9):
            srcRec = Constants::PaletteResources::BLOCK_1_5_9;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_5_10):
            srcRec = Constants::PaletteResources::BLOCK_1_5_10;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_5_11):
            srcRec = Constants::PaletteResources::BLOCK_1_5_11;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_5_12):
            srcRec = Constants::PaletteResources::BLOCK_1_5_12;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_5_13):
            srcRec = Constants::PaletteResources::BLOCK_1_5_13;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_5_14):
            srcRec = Constants::PaletteResources::BLOCK_1_5_14;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_5_15):
            srcRec = Constants::PaletteResources::BLOCK_1_5_15;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_5_16):
            srcRec = Constants::PaletteResources::BLOCK_1_5_16;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_3_6_5):
            srcRec = Constants::PaletteResources::BLOCK_1_6_5;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_6_6):
            srcRec = Constants::PaletteResources::BLOCK_1_6_6;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_3_7_1):
            srcRec = Constants::PaletteResources::BLOCK_1_7_1;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_7_2):
            srcRec = Constants::PaletteResources::BLOCK_1_7_2;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_7_3):
            srcRec = Constants::PaletteResources::BLOCK_1_7_3;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_7_4):
            srcRec = Constants::PaletteResources::BLOCK_1_7_4;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_7_5):
            srcRec = Constants::PaletteResources::BLOCK_1_7_5;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_7_6):
            srcRec = Constants::PaletteResources::BLOCK_1_7_6;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_7_7):
            srcRec = Constants::PaletteResources::BLOCK_1_7_7;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_3_7_11):
            srcRec = Constants::PaletteResources::BLOCK_1_7_11;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_7_12):
            srcRec = Constants::PaletteResources::BLOCK_1_7_12;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_7_13):
            srcRec = Constants::PaletteResources::BLOCK_1_7_13;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_7_14):
            srcRec = Constants::PaletteResources::BLOCK_1_7_14;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_7_15):
            srcRec = Constants::PaletteResources::BLOCK_1_7_15;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_3_7_16):
            srcRec = Constants::PaletteResources::BLOCK_1_7_16;
            DrawTexturePro(TextureManager::night_airshipTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_4_1_2):
            srcRec = Constants::PaletteResources::BLOCK_1_1_2;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_1_3):
            srcRec = Constants::PaletteResources::BLOCK_1_1_3;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_4_1_12):
            srcRec = Constants::PaletteResources::BLOCK_1_1_12;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_1_13):
            srcRec = Constants::PaletteResources::BLOCK_1_1_13;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_1_14):
            srcRec = Constants::PaletteResources::BLOCK_1_1_14;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_1_15):
            srcRec = Constants::PaletteResources::BLOCK_1_1_15;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_1_16):
            srcRec = Constants::PaletteResources::BLOCK_1_1_16;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_4_2_7):
            srcRec = Constants::PaletteResources::BLOCK_1_2_7;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_2_8):
            srcRec = Constants::PaletteResources::BLOCK_1_2_8;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_4_2_12):
            srcRec = Constants::PaletteResources::BLOCK_1_2_12;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_2_13):
            srcRec = Constants::PaletteResources::BLOCK_1_2_13;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_2_14):
            srcRec = Constants::PaletteResources::BLOCK_1_2_14;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_2_15):
            srcRec = Constants::PaletteResources::BLOCK_1_2_15;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_2_16):
            srcRec = Constants::PaletteResources::BLOCK_1_2_16;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_4_3_4):
            srcRec = Constants::PaletteResources::BLOCK_1_3_4;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_3_5):
            srcRec = Constants::PaletteResources::BLOCK_1_3_5;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_3_6):
            srcRec = Constants::PaletteResources::BLOCK_1_3_6;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_3_7):
            srcRec = Constants::PaletteResources::BLOCK_1_3_7;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_3_8):
            srcRec = Constants::PaletteResources::BLOCK_1_3_8;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_4_3_15):
            srcRec = Constants::PaletteResources::BLOCK_1_3_15;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_3_16):
            srcRec = Constants::PaletteResources::BLOCK_1_3_16;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_4_4_4):
            srcRec = Constants::PaletteResources::BLOCK_1_4_4;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_4_5):
            srcRec = Constants::PaletteResources::BLOCK_1_4_5;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_4_6):
            srcRec = Constants::PaletteResources::BLOCK_1_4_6;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_4_7):
            srcRec = Constants::PaletteResources::BLOCK_1_4_7;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_4_4_11):
            srcRec = Constants::PaletteResources::BLOCK_1_4_11;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_4_12):
            srcRec = Constants::PaletteResources::BLOCK_1_4_12;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_4_13):
            srcRec = Constants::PaletteResources::BLOCK_1_4_13;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_4_14):
            srcRec = Constants::PaletteResources::BLOCK_1_4_14;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_4_15):
            srcRec = Constants::PaletteResources::BLOCK_1_4_15;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_4_16):
            srcRec = Constants::PaletteResources::BLOCK_1_4_16;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_4_5_4):
            srcRec = Constants::PaletteResources::BLOCK_1_5_4;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_5_5):
            srcRec = Constants::PaletteResources::BLOCK_1_5_5;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_5_6):
            srcRec = Constants::PaletteResources::BLOCK_1_5_6;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_4_5_8):
            srcRec = Constants::PaletteResources::BLOCK_1_5_8;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_5_9):
            srcRec = Constants::PaletteResources::BLOCK_1_5_9;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_5_10):
            srcRec = Constants::PaletteResources::BLOCK_1_5_10;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_5_11):
            srcRec = Constants::PaletteResources::BLOCK_1_5_11;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_5_12):
            srcRec = Constants::PaletteResources::BLOCK_1_5_12;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_5_13):
            srcRec = Constants::PaletteResources::BLOCK_1_5_13;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_5_14):
            srcRec = Constants::PaletteResources::BLOCK_1_5_14;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_5_15):
            srcRec = Constants::PaletteResources::BLOCK_1_5_15;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_5_16):
            srcRec = Constants::PaletteResources::BLOCK_1_5_16;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_4_6_5):
            srcRec = Constants::PaletteResources::BLOCK_1_6_5;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_6_6):
            srcRec = Constants::PaletteResources::BLOCK_1_6_6;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_4_7_1):
            srcRec = Constants::PaletteResources::BLOCK_1_7_1;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_7_2):
            srcRec = Constants::PaletteResources::BLOCK_1_7_2;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_7_3):
            srcRec = Constants::PaletteResources::BLOCK_1_7_3;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_7_4):
            srcRec = Constants::PaletteResources::BLOCK_1_7_4;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_7_5):
            srcRec = Constants::PaletteResources::BLOCK_1_7_5;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_7_6):
            srcRec = Constants::PaletteResources::BLOCK_1_7_6;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_7_7):
            srcRec = Constants::PaletteResources::BLOCK_1_7_7;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;

        case(BlockType::BLOCK_4_7_11):
            srcRec = Constants::PaletteResources::BLOCK_1_7_11;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_7_12):
            srcRec = Constants::PaletteResources::BLOCK_1_7_12;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_7_13):
            srcRec = Constants::PaletteResources::BLOCK_1_7_13;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_7_14):
            srcRec = Constants::PaletteResources::BLOCK_1_7_14;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_7_15):
            srcRec = Constants::PaletteResources::BLOCK_1_7_15;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        case(BlockType::BLOCK_4_7_16):
            srcRec = Constants::PaletteResources::BLOCK_1_7_16;
            DrawTexturePro(TextureManager::night_snowTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
            break;
        default: srcRec = Constants::PaletteResources::BRICK;
    }

    DrawTexturePro(TextureManager::blocksTexture, srcRec, destRec, { 0, 0 }, 0.0f, WHITE);
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
    if (other->getObjectCategory() == ObjectCategory::CHARACTER) {
        if (direction == Direction::DOWN && !isMoving && !isSolid()) {
            physicsBody->SetLinearVelocity(b2Vec2{ 0.0f, -2.0f });
            isMoving = true;
            resetTimer = 0.0f;
            // if large character
            Vector2 centerPos = { position.x + hitbox.width * 0.5f, position.y + hitbox.height * 0.5f };
            BrokenBlockEffect* p = new BrokenBlockEffect(centerPos, { 30, 30 }, { -200, -400 }, { 200, -400 }, { 0, 1000 }, 5.0f, 0.01f, TextureManager::blocksTexture);
            ParticleSystem::getInstance().addEffect(p);
            GameContext::getInstance().mark_for_deletion_Object(GameContext::getInstance().getSharedPtrFromRaw(this));
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

GroundBlock::GroundBlock(Vector2 gridPos) : Block(gridPos, BlockType::GROUND, {1, 1}) { solid = false; }
BrickBlock::BrickBlock(Vector2 gridPos) : Block(gridPos, BlockType::BRICK, {1, 1}) { solid = false; }
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

Block_2_1_2Block::Block_2_1_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_1_2, { 1,1 }) { solid = false; }
Block_2_1_3Block::Block_2_1_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_2_1_3, { 1,1 }) { solid = false; }
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

Block_3_1_2Block::Block_3_1_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_1_2, { 1,1 }) { solid = false; }
Block_3_1_3Block::Block_3_1_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_3_1_3, { 1,1 }) { solid = false; }
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

Block_4_1_2Block::Block_4_1_2Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_1_2, { 1,1 }) { solid = false; }
Block_4_1_3Block::Block_4_1_3Block(Vector2 gridPos) : Block(gridPos, BlockType::BLOCK_4_1_3, { 1,1 }) { solid = false; }
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

