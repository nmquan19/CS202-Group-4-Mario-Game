#include "../../include/Objects/ObjectFactory.h"
#include "../Objects/Block.h"
#include "../Characters/Character.h"
#include <memory>
#include "../../include/Enemy/Enemy.h"
#include "../../include/Enemy/Goomba/Goomba.h"
#include "../../include/Enemy/Koopa/Koopa.h"

#include "../../include/System/Interface.h"
#include <raylib.h>
#include <vector>


std::unique_ptr<Object> ObjectFactory::createBlock(BlockType type, Vector2 gridPos) {
    return createSpecificBlock(type, gridPos);
}

std::unique_ptr<Block> ObjectFactory::createSpecificBlock(BlockType type, Vector2 gridPos) {
    switch (type) {
        case BlockType::GROUND:
            return std::make_unique<GroundBlock>(gridPos);
        case BlockType::BRICK:
            return std::make_unique<BrickBlock>(gridPos);
    }
}

std::unique_ptr<Character> ObjectFactory::createCharacter(CharacterType type, Vector2 startPosition, float scale) {
    switch(type) {
        case CharacterType::MARIO:
            return createSpecificCharacter(CharacterType::MARIO, startPosition, scale);
        case CharacterType::LUIGI:
            return createSpecificCharacter(CharacterType::LUIGI, startPosition, scale);
        default: 
            return nullptr;
    }
}

std::unique_ptr<Character> ObjectFactory::createSpecificCharacter(CharacterType type, Vector2 startPosition, float scale) {
    CharacterStats stats = getStats(type);
    auto stateFrameData = getFrameData(type);
    
    return std::make_unique<Character>(startPosition, stats, stateFrameData, type, scale);
}

CharacterStats ObjectFactory::getStats(CharacterType type) {
    switch (type) {
        case CharacterType::MARIO:
            return {220.0f, 550.0f, 980.0f};
        case CharacterType::LUIGI:  
            return {175.0f, 730.0f, 980.0f};
        default:
            return {220.0f, 550.0f, 980.0f};
    }
}
std::unique_ptr<Object> ObjectFactory::createEnemy(EnemyType type, Vector2 gridPos, Vector2 size) {
    return createSpecificEnemy(type, gridPos, size);
}
std::unique_ptr<Enemy> ObjectFactory::createSpecificEnemy(EnemyType type, Vector2 startPosition, Vector2 size) {
        switch (type) {
            case EnemyType::GOOMBA:
                return std::make_unique<Goomba>(startPosition, size);
        case EnemyType::KOOPA:
            return std::make_unique<Koopa>(startPosition, size);
        default:
            return nullptr; 
        }
}   
std::vector<std::vector<Rectangle>> ObjectFactory::getFrameData(CharacterType type) {
    switch (type) {
        case CharacterType::MARIO: 
            return {
                {{31, 21, 17, 43}, {111, 21, 17, 43}, {191, 20, 17, 44}, {271, 20, 17, 44}, {351, 20, 17, 44}}, // idle state
                {{31, 85, 19, 43}, {112, 85, 16, 43}, {193, 84, 15, 44}, {272, 84, 16, 44}, {351, 85, 18, 43}, {432, 85, 16, 43}, {512, 84, 16, 44}, {592, 84, 16, 44}}, // moving state
                {{30, 209, 19, 46}, {110, 209, 19, 46}, {190, 209, 19, 46}, {270, 209, 19, 46}} // jumping state
            };
        case CharacterType::LUIGI:
            return {
                {{31, 21, 17, 43}, {111, 21, 17, 43}, {191, 20, 17, 44}, {271, 20, 17, 44}, {351, 20, 17, 44}}, // idle state
                {{31, 85, 19, 43}, {112, 85, 16, 43}, {193, 84, 15, 44}, {272, 84, 16, 44}, {351, 85, 18, 43}, {432, 85, 16, 43}, {512, 84, 16, 44}, {592, 84, 16, 44}}, // moving state
                {{30, 209, 19, 46}, {110, 209, 19, 46}, {190, 209, 19, 46}, {270, 209, 19, 46}} // jumping state
            };
        default:
            return {
                {{31, 21, 17, 43}, {111, 21, 17, 43}, {191, 20, 17, 44}, {271, 20, 17, 44}, {351, 20, 17, 44}}, // idle state
                {{31, 85, 19, 43}, {112, 85, 16, 43}, {193, 84, 15, 44}, {272, 84, 16, 44}, {351, 85, 18, 43}, {432, 85, 16, 43}, {512, 84, 16, 44}, {592, 84, 16, 44}}, // moving state
                {{30, 209, 19, 46}, {110, 209, 19, 46}, {190, 209, 19, 46}, {270, 209, 19, 46}} // jumping state
            };
    }
}