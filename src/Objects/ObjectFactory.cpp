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
#include <algorithm>
#include "../../include/Enemy/Koopa/KoopaShell.h"


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
                {{11, 23, 12, 15}}, // idle state
                {{118, 23, 12, 15}, {149, 22, 15, 16}}, // moving state
                {{293, 22, 16, 16}, {326, 22, 16, 16}} // jumping state
            };
        case CharacterType::LUIGI:
            return {
                {{11, 893, 12, 15}}, // idle state
                {{118, 893, 12, 15}, {149, 892, 15, 16}}, // moving state
                {{293, 892, 16, 16}, {326, 892, 16, 16}} // jumping state
            };
        default:
            return {
                {{11, 23, 12, 15}}, // idle state
                {{118, 23, 12, 15}, {149, 22, 15, 16}}, // moving state
                {{293, 22, 16, 16}, {326, 22, 16, 16}} // jumping state
            };
    }
}
std::unique_ptr<Object> ObjectFactory::createKoopaShell(KoopaShellType type, Vector2 gridPos, Vector2 size) {
    return createSpecificKoopaShell(type, gridPos, size);
}
std::unique_ptr<KoopaShell> ObjectFactory::createSpecificKoopaShell(KoopaShellType type, Vector2 startPosition, Vector2 size) {
    switch (type) {
    case KoopaShellType::GREEN_KOOPA_SHELL:
        return std::make_unique<KoopaShell>(startPosition, size);
    //case EnemyType::KOOPA:
    //    return std::make_unique<Koopa>(startPosition, size);
    default:
        return nullptr;
    }
}
int Object::getCollidedPart(const Object& other)
{
    Rectangle playerHitBox = getHitBox();
    Rectangle otherHitBox = other.getHitBox();

    float overlapLeft = (playerHitBox.x + playerHitBox.width) - otherHitBox.x;
    float overlapRight = (otherHitBox.x + otherHitBox.width) - playerHitBox.x;
    float overlapTop = (playerHitBox.y + playerHitBox.height) - otherHitBox.y;
    float overlapBottom = (otherHitBox.y + otherHitBox.height) - playerHitBox.y;

    const float MIN_OVERLAP = 2.0f;

    if (overlapTop < MIN_OVERLAP && overlapBottom < MIN_OVERLAP && overlapLeft < MIN_OVERLAP && overlapRight < MIN_OVERLAP) {
        return 0 ;
    }

    float minOverlap = std::min({ overlapTop, overlapBottom, overlapLeft, overlapRight });
    if(minOverlap == overlapTop) {
        return 1; 
    } else if(minOverlap == overlapBottom) {
        return 2; 
    } else if(minOverlap == overlapLeft) {
        return 3; 
    } else if(minOverlap == overlapRight) {
        return 4; 
	}
}