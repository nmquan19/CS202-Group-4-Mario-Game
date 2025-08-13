#include "../../include/Objects/ObjectFactory.h"
#include "../../include/Objects/Block.h"
#include "../../include/Characters/Character.h"
#include "../../include/Enemy/Enemy.h"
#include "../../include/Enemy/Goomba/Goomba.h"
#include "../../include/Enemy/Boo/Boo.h"
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
#include <numbers>
#include "../../include/Objects/Torch.h"
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
        case BlockType::BLOCK_1_1_2:
            return std::make_unique<Block_1_1_2Block>(gridPos);
        case BlockType::BLOCK_1_1_3:
            return std::make_unique<Block_1_1_3Block>(gridPos);
        case BlockType::BLOCK_1_1_12:
            return std::make_unique<Block_1_1_12Block>(gridPos);
        case BlockType::BLOCK_1_1_13:
            return std::make_unique<Block_1_1_13Block>(gridPos);
        case BlockType::BLOCK_1_1_14:
            return std::make_unique<Block_1_1_14Block>(gridPos);
        case BlockType::BLOCK_1_1_15:
            return std::make_unique<Block_1_1_15Block>(gridPos);
        case BlockType::BLOCK_1_1_16:
            return std::make_unique<Block_1_1_16Block>(gridPos);
        case BlockType::BLOCK_1_2_7:
            return std::make_unique<Block_1_2_7Block>(gridPos);
        case BlockType::BLOCK_1_2_8:
            return std::make_unique<Block_1_2_8Block>(gridPos);
        case BlockType::BLOCK_1_2_12:
            return std::make_unique<Block_1_2_12Block>(gridPos);
        case BlockType::BLOCK_1_2_13:
            return std::make_unique<Block_1_2_13Block>(gridPos);
        case BlockType::BLOCK_1_2_14:
            return std::make_unique<Block_1_2_14Block>(gridPos);
        case BlockType::BLOCK_1_2_15:
            return std::make_unique<Block_1_2_15Block>(gridPos);
        case BlockType::BLOCK_1_2_16:
            return std::make_unique<Block_1_2_16Block>(gridPos);
        case BlockType::BLOCK_1_3_4:
            return std::make_unique<Block_1_3_4Block>(gridPos);
        case BlockType::BLOCK_1_3_5:
            return std::make_unique<Block_1_3_5Block>(gridPos);
        case BlockType::BLOCK_1_3_6:
            return std::make_unique<Block_1_3_6Block>(gridPos);
        case BlockType::BLOCK_1_3_7:
            return std::make_unique<Block_1_3_7Block>(gridPos);
        case BlockType::BLOCK_1_3_8:
            return std::make_unique<Block_1_3_8Block>(gridPos);
        case BlockType::BLOCK_1_3_15:
            return std::make_unique<Block_1_3_15Block>(gridPos);
        case BlockType::BLOCK_1_3_16:
            return std::make_unique<Block_1_3_16Block>(gridPos);
        case BlockType::BLOCK_1_4_4:
            return std::make_unique<Block_1_4_4Block>(gridPos);
        case BlockType::BLOCK_1_4_5:
            return std::make_unique<Block_1_4_5Block>(gridPos);
        case BlockType::BLOCK_1_4_6:
            return std::make_unique<Block_1_4_6Block>(gridPos);
        case BlockType::BLOCK_1_4_7:
            return std::make_unique<Block_1_4_7Block>(gridPos);
        case BlockType::BLOCK_1_4_11:
            return std::make_unique<Block_1_4_11Block>(gridPos);
        case BlockType::BLOCK_1_4_12:
            return std::make_unique<Block_1_4_12Block>(gridPos);
        case BlockType::BLOCK_1_4_13:
            return std::make_unique<Block_1_4_13Block>(gridPos);
        case BlockType::BLOCK_1_4_14:
            return std::make_unique<Block_1_4_14Block>(gridPos);
        case BlockType::BLOCK_1_4_15:
            return std::make_unique<Block_1_4_15Block>(gridPos);
        case BlockType::BLOCK_1_4_16:
            return std::make_unique<Block_1_4_16Block>(gridPos);
        case BlockType::BLOCK_1_5_4:
            return std::make_unique<Block_1_5_4Block>(gridPos);
        case BlockType::BLOCK_1_5_5:
            return std::make_unique<Block_1_5_5Block>(gridPos);
        case BlockType::BLOCK_1_5_6:
            return std::make_unique<Block_1_5_6Block>(gridPos);
        case BlockType::BLOCK_1_5_8:
            return std::make_unique<Block_1_5_8Block>(gridPos);
        case BlockType::BLOCK_1_5_9:
            return std::make_unique<Block_1_5_9Block>(gridPos);
        case BlockType::BLOCK_1_5_10:
            return std::make_unique<Block_1_5_10Block>(gridPos);
        case BlockType::BLOCK_1_5_11:
            return std::make_unique<Block_1_5_11Block>(gridPos);
        case BlockType::BLOCK_1_5_12:
            return std::make_unique<Block_1_5_12Block>(gridPos);
        case BlockType::BLOCK_1_5_13:
            return std::make_unique<Block_1_5_13Block>(gridPos);
        case BlockType::BLOCK_1_5_14:
            return std::make_unique<Block_1_5_14Block>(gridPos);
        case BlockType::BLOCK_1_5_15:
            return std::make_unique<Block_1_5_15Block>(gridPos);
        case BlockType::BLOCK_1_5_16:
            return std::make_unique<Block_1_5_16Block>(gridPos);
        case BlockType::BLOCK_1_6_5:
            return std::make_unique<Block_1_6_5Block>(gridPos);
        case BlockType::BLOCK_1_6_6:
            return std::make_unique<Block_1_6_6Block>(gridPos);
        case BlockType::BLOCK_1_7_1:
            return std::make_unique<Block_1_7_1Block>(gridPos);
        case BlockType::BLOCK_1_7_2:
            return std::make_unique<Block_1_7_2Block>(gridPos);
        case BlockType::BLOCK_1_7_3:
            return std::make_unique<Block_1_7_3Block>(gridPos);
        case BlockType::BLOCK_1_7_4:
            return std::make_unique<Block_1_7_4Block>(gridPos);
        case BlockType::BLOCK_1_7_5:
            return std::make_unique<Block_1_7_5Block>(gridPos);
        case BlockType::BLOCK_1_7_6:
            return std::make_unique<Block_1_7_6Block>(gridPos);
        case BlockType::BLOCK_1_7_7:
            return std::make_unique<Block_1_7_7Block>(gridPos);
        case BlockType::BLOCK_1_7_11:
            return std::make_unique<Block_1_7_11Block>(gridPos);
        case BlockType::BLOCK_1_7_12:
            return std::make_unique<Block_1_7_12Block>(gridPos);
        case BlockType::BLOCK_1_7_13:
            return std::make_unique<Block_1_7_13Block>(gridPos);
        case BlockType::BLOCK_1_7_14:
            return std::make_unique<Block_1_7_14Block>(gridPos);
        case BlockType::BLOCK_1_7_15:
            return std::make_unique<Block_1_7_15Block>(gridPos);
        case BlockType::BLOCK_1_7_16:
            return std::make_unique<Block_1_7_16Block>(gridPos);

        case BlockType::BLOCK_2_1_2:
            return std::make_unique<Block_2_1_2Block>(gridPos);
        case BlockType::BLOCK_2_1_3:
            return std::make_unique<Block_2_1_3Block>(gridPos);
        case BlockType::BLOCK_2_1_12:
            return std::make_unique<Block_2_1_12Block>(gridPos);
        case BlockType::BLOCK_2_1_13:
            return std::make_unique<Block_2_1_13Block>(gridPos);
        case BlockType::BLOCK_2_1_14:
            return std::make_unique<Block_2_1_14Block>(gridPos);
        case BlockType::BLOCK_2_1_15:
            return std::make_unique<Block_2_1_15Block>(gridPos);
        case BlockType::BLOCK_2_1_16:
            return std::make_unique<Block_2_1_16Block>(gridPos);
        case BlockType::BLOCK_2_2_7:
            return std::make_unique<Block_2_2_7Block>(gridPos);
        case BlockType::BLOCK_2_2_8:
            return std::make_unique<Block_2_2_8Block>(gridPos);
        case BlockType::BLOCK_2_2_12:
            return std::make_unique<Block_2_2_12Block>(gridPos);
        case BlockType::BLOCK_2_2_13:
            return std::make_unique<Block_2_2_13Block>(gridPos);
        case BlockType::BLOCK_2_2_14:
            return std::make_unique<Block_2_2_14Block>(gridPos);
        case BlockType::BLOCK_2_2_15:
            return std::make_unique<Block_2_2_15Block>(gridPos);
        case BlockType::BLOCK_2_2_16:
            return std::make_unique<Block_2_2_16Block>(gridPos);
        case BlockType::BLOCK_2_3_4:
            return std::make_unique<Block_2_3_4Block>(gridPos);
        case BlockType::BLOCK_2_3_5:
            return std::make_unique<Block_2_3_5Block>(gridPos);
        case BlockType::BLOCK_2_3_6:
            return std::make_unique<Block_2_3_6Block>(gridPos);
        case BlockType::BLOCK_2_3_7:
            return std::make_unique<Block_2_3_7Block>(gridPos);
        case BlockType::BLOCK_2_3_8:
            return std::make_unique<Block_2_3_8Block>(gridPos);
        case BlockType::BLOCK_2_3_15:
            return std::make_unique<Block_2_3_15Block>(gridPos);
        case BlockType::BLOCK_2_3_16:
            return std::make_unique<Block_2_3_16Block>(gridPos);
        case BlockType::BLOCK_2_4_4:
            return std::make_unique<Block_2_4_4Block>(gridPos);
        case BlockType::BLOCK_2_4_5:
            return std::make_unique<Block_2_4_5Block>(gridPos);
        case BlockType::BLOCK_2_4_6:
            return std::make_unique<Block_2_4_6Block>(gridPos);
        case BlockType::BLOCK_2_4_7:
            return std::make_unique<Block_2_4_7Block>(gridPos);
        case BlockType::BLOCK_2_4_11:
            return std::make_unique<Block_2_4_11Block>(gridPos);
        case BlockType::BLOCK_2_4_12:
            return std::make_unique<Block_2_4_12Block>(gridPos);
        case BlockType::BLOCK_2_4_13:
            return std::make_unique<Block_2_4_13Block>(gridPos);
        case BlockType::BLOCK_2_4_14:
            return std::make_unique<Block_2_4_14Block>(gridPos);
        case BlockType::BLOCK_2_4_15:
            return std::make_unique<Block_2_4_15Block>(gridPos);
        case BlockType::BLOCK_2_4_16:
            return std::make_unique<Block_2_4_16Block>(gridPos);
        case BlockType::BLOCK_2_5_4:
            return std::make_unique<Block_2_5_4Block>(gridPos);
        case BlockType::BLOCK_2_5_5:
            return std::make_unique<Block_2_5_5Block>(gridPos);
        case BlockType::BLOCK_2_5_6:
            return std::make_unique<Block_2_5_6Block>(gridPos);
        case BlockType::BLOCK_2_5_8:
            return std::make_unique<Block_2_5_8Block>(gridPos);
        case BlockType::BLOCK_2_5_9:
            return std::make_unique<Block_2_5_9Block>(gridPos);
        case BlockType::BLOCK_2_5_10:
            return std::make_unique<Block_2_5_10Block>(gridPos);
        case BlockType::BLOCK_2_5_11:
            return std::make_unique<Block_2_5_11Block>(gridPos);
        case BlockType::BLOCK_2_5_12:
            return std::make_unique<Block_2_5_12Block>(gridPos);
        case BlockType::BLOCK_2_5_13:
            return std::make_unique<Block_2_5_13Block>(gridPos);
        case BlockType::BLOCK_2_5_14:
            return std::make_unique<Block_2_5_14Block>(gridPos);
        case BlockType::BLOCK_2_5_15:
            return std::make_unique<Block_2_5_15Block>(gridPos);
        case BlockType::BLOCK_2_5_16:
            return std::make_unique<Block_2_5_16Block>(gridPos);
        case BlockType::BLOCK_2_6_5:
            return std::make_unique<Block_2_6_5Block>(gridPos);
        case BlockType::BLOCK_2_6_6:
            return std::make_unique<Block_2_6_6Block>(gridPos);
        case BlockType::BLOCK_2_7_1:
            return std::make_unique<Block_2_7_1Block>(gridPos);
        case BlockType::BLOCK_2_7_2:
            return std::make_unique<Block_2_7_2Block>(gridPos);
        case BlockType::BLOCK_2_7_3:
            return std::make_unique<Block_2_7_3Block>(gridPos);
        case BlockType::BLOCK_2_7_4:
            return std::make_unique<Block_2_7_4Block>(gridPos);
        case BlockType::BLOCK_2_7_5:
            return std::make_unique<Block_2_7_5Block>(gridPos);
        case BlockType::BLOCK_2_7_6:
            return std::make_unique<Block_2_7_6Block>(gridPos);
        case BlockType::BLOCK_2_7_7:
            return std::make_unique<Block_2_7_7Block>(gridPos);
        case BlockType::BLOCK_2_7_11:
            return std::make_unique<Block_2_7_11Block>(gridPos);
        case BlockType::BLOCK_2_7_12:
            return std::make_unique<Block_2_7_12Block>(gridPos);
        case BlockType::BLOCK_2_7_13:
            return std::make_unique<Block_2_7_13Block>(gridPos);
        case BlockType::BLOCK_2_7_14:
            return std::make_unique<Block_2_7_14Block>(gridPos);
        case BlockType::BLOCK_2_7_15:
            return std::make_unique<Block_2_7_15Block>(gridPos);
        case BlockType::BLOCK_2_7_16:
            return std::make_unique<Block_2_7_16Block>(gridPos);

        case BlockType::BLOCK_3_1_2:
            return std::make_unique<Block_3_1_2Block>(gridPos);
        case BlockType::BLOCK_3_1_3:
            return std::make_unique<Block_3_1_3Block>(gridPos);
        case BlockType::BLOCK_3_1_12:
            return std::make_unique<Block_3_1_12Block>(gridPos);
        case BlockType::BLOCK_3_1_13:
            return std::make_unique<Block_3_1_13Block>(gridPos);
        case BlockType::BLOCK_3_1_14:
            return std::make_unique<Block_3_1_14Block>(gridPos);
        case BlockType::BLOCK_3_1_15:
            return std::make_unique<Block_3_1_15Block>(gridPos);
        case BlockType::BLOCK_3_1_16:
            return std::make_unique<Block_3_1_16Block>(gridPos);
        case BlockType::BLOCK_3_2_7:
            return std::make_unique<Block_3_2_7Block>(gridPos);
        case BlockType::BLOCK_3_2_8:
            return std::make_unique<Block_3_2_8Block>(gridPos);
        case BlockType::BLOCK_3_2_12:
            return std::make_unique<Block_3_2_12Block>(gridPos);
        case BlockType::BLOCK_3_2_13:
            return std::make_unique<Block_3_2_13Block>(gridPos);
        case BlockType::BLOCK_3_2_14:
            return std::make_unique<Block_3_2_14Block>(gridPos);
        case BlockType::BLOCK_3_2_15:
            return std::make_unique<Block_3_2_15Block>(gridPos);
        case BlockType::BLOCK_3_2_16:
            return std::make_unique<Block_3_2_16Block>(gridPos);
        case BlockType::BLOCK_3_3_4:
            return std::make_unique<Block_3_3_4Block>(gridPos);
        case BlockType::BLOCK_3_3_5:
            return std::make_unique<Block_3_3_5Block>(gridPos);
        case BlockType::BLOCK_3_3_6:
            return std::make_unique<Block_3_3_6Block>(gridPos);
        case BlockType::BLOCK_3_3_7:
            return std::make_unique<Block_3_3_7Block>(gridPos);
        case BlockType::BLOCK_3_3_8:
            return std::make_unique<Block_3_3_8Block>(gridPos);
        case BlockType::BLOCK_3_3_15:
            return std::make_unique<Block_3_3_15Block>(gridPos);
        case BlockType::BLOCK_3_3_16:
            return std::make_unique<Block_3_3_16Block>(gridPos);
        case BlockType::BLOCK_3_4_4:
            return std::make_unique<Block_3_4_4Block>(gridPos);
        case BlockType::BLOCK_3_4_5:
            return std::make_unique<Block_3_4_5Block>(gridPos);
        case BlockType::BLOCK_3_4_6:
            return std::make_unique<Block_3_4_6Block>(gridPos);
        case BlockType::BLOCK_3_4_7:
            return std::make_unique<Block_3_4_7Block>(gridPos);
        case BlockType::BLOCK_3_4_11:
            return std::make_unique<Block_3_4_11Block>(gridPos);
        case BlockType::BLOCK_3_4_12:
            return std::make_unique<Block_3_4_12Block>(gridPos);
        case BlockType::BLOCK_3_4_13:
            return std::make_unique<Block_3_4_13Block>(gridPos);
        case BlockType::BLOCK_3_4_14:
            return std::make_unique<Block_3_4_14Block>(gridPos);
        case BlockType::BLOCK_3_4_15:
            return std::make_unique<Block_3_4_15Block>(gridPos);
        case BlockType::BLOCK_3_4_16:
            return std::make_unique<Block_3_4_16Block>(gridPos);
        case BlockType::BLOCK_3_5_4:
            return std::make_unique<Block_3_5_4Block>(gridPos);
        case BlockType::BLOCK_3_5_5:
            return std::make_unique<Block_3_5_5Block>(gridPos);
        case BlockType::BLOCK_3_5_6:
            return std::make_unique<Block_3_5_6Block>(gridPos);
        case BlockType::BLOCK_3_5_8:
            return std::make_unique<Block_3_5_8Block>(gridPos);
        case BlockType::BLOCK_3_5_9:
            return std::make_unique<Block_3_5_9Block>(gridPos);
        case BlockType::BLOCK_3_5_10:
            return std::make_unique<Block_3_5_10Block>(gridPos);
        case BlockType::BLOCK_3_5_11:
            return std::make_unique<Block_3_5_11Block>(gridPos);
        case BlockType::BLOCK_3_5_12:
            return std::make_unique<Block_3_5_12Block>(gridPos);
        case BlockType::BLOCK_3_5_13:
            return std::make_unique<Block_3_5_13Block>(gridPos);
        case BlockType::BLOCK_3_5_14:
            return std::make_unique<Block_3_5_14Block>(gridPos);
        case BlockType::BLOCK_3_5_15:
            return std::make_unique<Block_3_5_15Block>(gridPos);
        case BlockType::BLOCK_3_5_16:
            return std::make_unique<Block_3_5_16Block>(gridPos);
        case BlockType::BLOCK_3_6_5:
            return std::make_unique<Block_3_6_5Block>(gridPos);
        case BlockType::BLOCK_3_6_6:
            return std::make_unique<Block_3_6_6Block>(gridPos);
        case BlockType::BLOCK_3_7_1:
            return std::make_unique<Block_3_7_1Block>(gridPos);
        case BlockType::BLOCK_3_7_2:
            return std::make_unique<Block_3_7_2Block>(gridPos);
        case BlockType::BLOCK_3_7_3:
            return std::make_unique<Block_3_7_3Block>(gridPos);
        case BlockType::BLOCK_3_7_4:
            return std::make_unique<Block_3_7_4Block>(gridPos);
        case BlockType::BLOCK_3_7_5:
            return std::make_unique<Block_3_7_5Block>(gridPos);
        case BlockType::BLOCK_3_7_6:
            return std::make_unique<Block_3_7_6Block>(gridPos);
        case BlockType::BLOCK_3_7_7:
            return std::make_unique<Block_3_7_7Block>(gridPos);
        case BlockType::BLOCK_3_7_11:
            return std::make_unique<Block_3_7_11Block>(gridPos);
        case BlockType::BLOCK_3_7_12:
            return std::make_unique<Block_3_7_12Block>(gridPos);
        case BlockType::BLOCK_3_7_13:
            return std::make_unique<Block_3_7_13Block>(gridPos);
        case BlockType::BLOCK_3_7_14:
            return std::make_unique<Block_3_7_14Block>(gridPos);
        case BlockType::BLOCK_3_7_15:
            return std::make_unique<Block_3_7_15Block>(gridPos);
        case BlockType::BLOCK_3_7_16:
            return std::make_unique<Block_3_7_16Block>(gridPos);

        case BlockType::BLOCK_4_1_2:
            return std::make_unique<Block_4_1_2Block>(gridPos);
        case BlockType::BLOCK_4_1_3:
            return std::make_unique<Block_4_1_3Block>(gridPos);
        case BlockType::BLOCK_4_1_12:
            return std::make_unique<Block_4_1_12Block>(gridPos);
        case BlockType::BLOCK_4_1_13:
            return std::make_unique<Block_4_1_13Block>(gridPos);
        case BlockType::BLOCK_4_1_14:
            return std::make_unique<Block_4_1_14Block>(gridPos);
        case BlockType::BLOCK_4_1_15:
            return std::make_unique<Block_4_1_15Block>(gridPos);
        case BlockType::BLOCK_4_1_16:
            return std::make_unique<Block_4_1_16Block>(gridPos);
        case BlockType::BLOCK_4_2_7:
            return std::make_unique<Block_4_2_7Block>(gridPos);
        case BlockType::BLOCK_4_2_8:
            return std::make_unique<Block_4_2_8Block>(gridPos);
        case BlockType::BLOCK_4_2_12:
            return std::make_unique<Block_4_2_12Block>(gridPos);
        case BlockType::BLOCK_4_2_13:
            return std::make_unique<Block_4_2_13Block>(gridPos);
        case BlockType::BLOCK_4_2_14:
            return std::make_unique<Block_4_2_14Block>(gridPos);
        case BlockType::BLOCK_4_2_15:
            return std::make_unique<Block_4_2_15Block>(gridPos);
        case BlockType::BLOCK_4_2_16:
            return std::make_unique<Block_4_2_16Block>(gridPos);
        case BlockType::BLOCK_4_3_4:
            return std::make_unique<Block_4_3_4Block>(gridPos);
        case BlockType::BLOCK_4_3_5:
            return std::make_unique<Block_4_3_5Block>(gridPos);
        case BlockType::BLOCK_4_3_6:
            return std::make_unique<Block_4_3_6Block>(gridPos);
        case BlockType::BLOCK_4_3_7:
            return std::make_unique<Block_4_3_7Block>(gridPos);
        case BlockType::BLOCK_4_3_8:
            return std::make_unique<Block_4_3_8Block>(gridPos);
        case BlockType::BLOCK_4_3_15:
            return std::make_unique<Block_4_3_15Block>(gridPos);
        case BlockType::BLOCK_4_3_16:
            return std::make_unique<Block_4_3_16Block>(gridPos);
        case BlockType::BLOCK_4_4_4:
            return std::make_unique<Block_4_4_4Block>(gridPos);
        case BlockType::BLOCK_4_4_5:
            return std::make_unique<Block_4_4_5Block>(gridPos);
        case BlockType::BLOCK_4_4_6:
            return std::make_unique<Block_4_4_6Block>(gridPos);
        case BlockType::BLOCK_4_4_7:
            return std::make_unique<Block_4_4_7Block>(gridPos);
        case BlockType::BLOCK_4_4_11:
            return std::make_unique<Block_4_4_11Block>(gridPos);
        case BlockType::BLOCK_4_4_12:
            return std::make_unique<Block_4_4_12Block>(gridPos);
        case BlockType::BLOCK_4_4_13:
            return std::make_unique<Block_4_4_13Block>(gridPos);
        case BlockType::BLOCK_4_4_14:
            return std::make_unique<Block_4_4_14Block>(gridPos);
        case BlockType::BLOCK_4_4_15:
            return std::make_unique<Block_4_4_15Block>(gridPos);
        case BlockType::BLOCK_4_4_16:
            return std::make_unique<Block_4_4_16Block>(gridPos);
        case BlockType::BLOCK_4_5_4:
            return std::make_unique<Block_4_5_4Block>(gridPos);
        case BlockType::BLOCK_4_5_5:
            return std::make_unique<Block_4_5_5Block>(gridPos);
        case BlockType::BLOCK_4_5_6:
            return std::make_unique<Block_4_5_6Block>(gridPos);
        case BlockType::BLOCK_4_5_8:
            return std::make_unique<Block_4_5_8Block>(gridPos);
        case BlockType::BLOCK_4_5_9:
            return std::make_unique<Block_4_5_9Block>(gridPos);
        case BlockType::BLOCK_4_5_10:
            return std::make_unique<Block_4_5_10Block>(gridPos);
        case BlockType::BLOCK_4_5_11:
            return std::make_unique<Block_4_5_11Block>(gridPos);
        case BlockType::BLOCK_4_5_12:
            return std::make_unique<Block_4_5_12Block>(gridPos);
        case BlockType::BLOCK_4_5_13:
            return std::make_unique<Block_4_5_13Block>(gridPos);
        case BlockType::BLOCK_4_5_14:
            return std::make_unique<Block_4_5_14Block>(gridPos);
        case BlockType::BLOCK_4_5_15:
            return std::make_unique<Block_4_5_15Block>(gridPos);
        case BlockType::BLOCK_4_5_16:
            return std::make_unique<Block_4_5_16Block>(gridPos);
        case BlockType::BLOCK_4_6_5:
            return std::make_unique<Block_4_6_5Block>(gridPos);
        case BlockType::BLOCK_4_6_6:
            return std::make_unique<Block_4_6_6Block>(gridPos);
        case BlockType::BLOCK_4_7_1:
            return std::make_unique<Block_4_7_1Block>(gridPos);
        case BlockType::BLOCK_4_7_2:
            return std::make_unique<Block_4_7_2Block>(gridPos);
        case BlockType::BLOCK_4_7_3:
            return std::make_unique<Block_4_7_3Block>(gridPos);
        case BlockType::BLOCK_4_7_4:
            return std::make_unique<Block_4_7_4Block>(gridPos);
        case BlockType::BLOCK_4_7_5:
            return std::make_unique<Block_4_7_5Block>(gridPos);
        case BlockType::BLOCK_4_7_6:
            return std::make_unique<Block_4_7_6Block>(gridPos);
        case BlockType::BLOCK_4_7_7:
            return std::make_unique<Block_4_7_7Block>(gridPos);
        case BlockType::BLOCK_4_7_11:
            return std::make_unique<Block_4_7_11Block>(gridPos);
        case BlockType::BLOCK_4_7_12:
            return std::make_unique<Block_4_7_12Block>(gridPos);
        case BlockType::BLOCK_4_7_13:
            return std::make_unique<Block_4_7_13Block>(gridPos);
        case BlockType::BLOCK_4_7_14:
            return std::make_unique<Block_4_7_14Block>(gridPos);
        case BlockType::BLOCK_4_7_15:
            return std::make_unique<Block_4_7_15Block>(gridPos);
        case BlockType::BLOCK_4_7_16:
            return std::make_unique<Block_4_7_16Block>(gridPos);
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
    case EnemyType::DRY_BOWSER: 
        return std::make_unique<DryBowser>(startPosition, size);
    case EnemyType::BOO:
        return std::make_unique<Boo>(startPosition, size);
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

std::unique_ptr<Object> ObjectFactory::createProjectile(ProjectileType type, Vector2 position, int direction, Vector2 size) {
    return createSpecificProjectile(type, position, direction, size);
}

std::unique_ptr<Projectile> ObjectFactory::createSpecificProjectile(ProjectileType type, Vector2 position, int direction, Vector2 size) {
    switch (type) {
    case ProjectileType::FIRE_BALL:
        return std::make_unique<FireBall>(position, direction, size);
      // case...
    }
}
std::unique_ptr<Object> ObjectFactory::createTorch(Vector2 position, Vector2 size, float brightness, float radius, Color innerColor, Color outerColor) {
    return std::make_unique<Torch>(position,size,radius,brightness,innerColor,outerColor);
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

void Object::CircleMove(Vector2 center, float radius, float speed, float deltaTime) {
    totalTime += deltaTime;
    position.x = center.x + radius * cos(speed * totalTime);
    position.y = center.y + radius * sin(speed * totalTime);
    if (physicsBody) {
        physicsBody->SetTransform(b2Vec2(position.x / GridSystem::GRID_SIZE, position.y / GridSystem::GRID_SIZE),
            physicsBody->GetAngle());
    }
}

void Object::HarmonicOscillationMove(float amplitude, float frequency, float deltaTime) {
    totalTime += deltaTime;

    // Tần số góc ω = 2πf
    float omega = 2.0f * std::numbers::pi_v<float> *frequency;

    // Tính vị trí offset (pixel)
    float offsetX = amplitude * cos(omega * totalTime);

    // Tính vận tốc theo pixel/s
    float vxPixelsPerSecond = -omega * amplitude * sin(omega * totalTime);

    std::cout << "Amplitude: " << amplitude << ", Frequency: " << frequency 
              << ", DeltaTime: " << deltaTime << ", OffsetX: " << offsetX 
		<< ", VxPixelsPerSecond: " << vxPixelsPerSecond << std::endl;
	std::cout << "centerPosition: " << centerPosition.x << ", " << centerPosition.y << std::endl;

    // Đổi vận tốc sang mét/s cho Box2D
    float vxMetersPerSecond = Box2DWorldManager::raylibToB2(vxPixelsPerSecond);

    if (physicsBody) {
        b2Vec2 vel(vxMetersPerSecond, physicsBody->GetLinearVelocity().y);
        physicsBody->SetLinearVelocity(vel);
    }

    // Cập nhật vị trí hiển thị từ physics body
    Vector2 renderPos = Box2DWorldManager::b2ToRaylib(physicsBody->GetPosition());
    position.x = renderPos.x - Constants::TILE_SIZE / 2;
    position.y = renderPos.y - Constants::TILE_SIZE / 2;

}

void Object::StarShapeMove(Vector2 center, float deltaTime, float frequency) {
    totalTime += deltaTime;
    position.x = center.x - 0.25*(3 * cos(0.4 * totalTime) + 2 * sin(0.6 * totalTime));
    position.y = center.y - 0.25*(3 * sin(0.4 * totalTime) + 2 * cos(0.6 * totalTime));

    if (physicsBody) {
        physicsBody->SetTransform(b2Vec2(position.x / GridSystem::GRID_SIZE, position.y / GridSystem::GRID_SIZE),
            physicsBody->GetAngle());
    }
}

