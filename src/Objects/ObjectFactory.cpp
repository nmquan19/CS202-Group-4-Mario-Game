#include "../../include/Objects/ObjectFactory.h"
#include "../../include/Objects/Block.h"


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