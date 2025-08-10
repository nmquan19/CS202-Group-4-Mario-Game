#include "../../include/Objects/ObjectFactory.h"
#include "../../include/Objects/Block.h"
#include "../../include/Characters/Character.h"
#include "../../include/Enemy/Enemy.h"
#include "../../include/Enemy/Goomba/Goomba.h"
#include "../../include/Enemy/Koopa/Koopa.h"
#include "../../include/Item/Item.h"
#include "../../include/Item/Coin/Coin.h"
#include "../../include/Item/Mushroom/Mushroom.h"
#include "../../include/Item/Fire_Flower/Fire_Flower.h"
#include "../../include/Item/Star/Star.h"
#include "../../include/Item/One_Up/One_Up.h"
#include "../../include/Enemy/Boss/DryBowser/DryBowser.h"
#include "../../include/System/Interface.h"
#include "../../include/Enemy/Koopa/KoopaShell.h"
#include "../../include/System/Constant.h"
#include "../../include/Objects/Spring.h"
#include "../../include/Objects/Projectile/FireBall.h"
#include "../../include/System/Box2DWorldManager.h"
#include <raylib.h>
#include <vector>
#include <algorithm>
#include <memory>

Object::~Object() {
    if (physicsBody) {
        Box2DWorldManager::getInstance().destroyBody(physicsBody);
        physicsBody = nullptr;
    }
}

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

std::unique_ptr<Object> ObjectFactory::createCharacter(CharacterType type, PlayerID id, Vector2 startPosition, Vector2 size) {
    switch(type) {
        case CharacterType::MARIO:
            return createSpecificCharacter(CharacterType::MARIO, id, startPosition, size);
        case CharacterType::LUIGI:
            return createSpecificCharacter(CharacterType::LUIGI, id, startPosition, size);
        default: 
            return nullptr;
    }
}

std::unique_ptr<Character> ObjectFactory::createSpecificCharacter(CharacterType type, PlayerID id, Vector2 startPosition, Vector2 size) {
    CharacterStats stats = getStats(type);
    auto stateFrameData = getFrameData(type);
    
    return std::make_unique<Character>(startPosition, stats, stateFrameData, type, id, size);
}

CharacterStats ObjectFactory::getStats(CharacterType type) {
    switch (type) {
        case CharacterType::MARIO:
            return Constants::Character::MARIO_STATS;
        case CharacterType::LUIGI:  
            return Constants::Character::LUIGI_STATS;
    }
}
std::unique_ptr<Object> ObjectFactory::createEnemy(EnemyType type, Vector2 gridPos, Vector2 size) {
    return createSpecificEnemy(type, gridPos, size);
}

std::unique_ptr<Enemy> ObjectFactory::createSpecificEnemy(EnemyType type, Vector2 startPosition, Vector2 size) {
    switch (type) {
    case EnemyType::GOOMBA:
        return std::make_unique<Goomba>(startPosition, size);
    case EnemyType::GREEN_KOOPA:
        return std::make_unique<GreenKoopa>(startPosition, size);
    case EnemyType::RED_KOOPA:
        return std::make_unique<RedKoopa>(startPosition, size);
    default:
        return nullptr; 
    }
}   

std::unique_ptr<Object> ObjectFactory::createItem(ItemType type, Vector2 startPos, Vector2 size) {
    return createSpecificItem(type, startPos, size);
}

std::unique_ptr<Item> ObjectFactory::createSpecificItem(ItemType type, Vector2 startPos, Vector2 size) {
    switch (type) {
    case ItemType::COIN:
		return std::make_unique<Coin>(startPos);
        break;
    case ItemType::MUSHROOM:
        return std::make_unique<Mushroom>(startPos);
        break;
	case ItemType::FIRE_FLOWER:
        return std::make_unique<Fire_Flower>(startPos);
		break;
	case ItemType::STAR:
        return std::make_unique<Star>(startPos);
		break;
    case ItemType::ONE_UP:
	    return std::make_unique<One_Up>(startPos);
		break;
    default:
        return std::make_unique<Coin>(startPos);
    }
}


std::vector<std::vector<Rectangle>> ObjectFactory::getFrameData(CharacterType type) {
    switch (type) {
        case CharacterType::MARIO: 
            return Constants::Character::SMALL_MARIO_FRAME_DATA;
        case CharacterType::LUIGI:
            return Constants::Character::LUIGI_FRAME_DATA;
    }
}
std::unique_ptr<Object> ObjectFactory::createKoopaShell(KoopaShellType type, Vector2 gridPos, Vector2 size) {
    return std::make_unique<KoopaShell>(type, gridPos, size);
}

std::unique_ptr<Object> ObjectFactory::createSpring(Vector2 position, Vector2 size) {
    return std::make_unique<Spring>(position, size);
}

std::unique_ptr<Object> ObjectFactory::createProjectile(ProjectileType type, Vector2 position, Vector2 size) {
    return createSpecificProjectile(type, position, size);
}

std::unique_ptr<Projectile> ObjectFactory::createSpecificProjectile(ProjectileType type, Vector2 position, Vector2 size) {
    switch (type) {
    case ProjectileType::FIRE_BALL:
        return std::make_unique<FireBall>(position, size);
      // case...
    }
}

int Object::getCollidedPart(const Object& other){
    std::vector<Rectangle> playerHitBoxes = getHitBox();
    std::vector<Rectangle> otherHitBoxes = other.getHitBox();
    
    if (playerHitBoxes.empty() || otherHitBoxes.empty()) return 0;
    
    Rectangle playerHitBox = playerHitBoxes[0];
    Rectangle otherHitBox = otherHitBoxes[0];

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