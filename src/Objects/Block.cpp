#include "../../include/Objects/Block.h"
#include "../../include/System/Grid.h"
#include "../../include/System/TextureManager.h"
#include <vector>
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/System/Interface.h"
#include <raylib.h>

Block::Block(Vector2 gridPos, BlockType type) : gridPosition(gridPos), blockType(type) {
    position = { gridPos.x * GridSystem::GRID_SIZE, gridPos.y * GridSystem::GRID_SIZE };
    size = { GridSystem::GRID_SIZE, GridSystem::GRID_SIZE };
}

Block::~Block() {}

void Block::draw() {
    if (!active) return;

    Texture2D texture = TextureManager::getInstance().getBlockTexture(getType());

    Rectangle destRec = { position.x, position.y, size.x, size.y };
    Rectangle sourceRec = { 0, 0, (float)texture.width, (float)texture.height };

    DrawTexturePro(texture, sourceRec, destRec, { 0, 0 }, 0.0f, WHITE);
}

Rectangle Block::getHitBox() const {
    return { position.x, position.y, size.x, size.y };
}

ObjectCategory Block::getObjectCategory() const {
    return ObjectCategory::BLOCK;
}

std::vector<ObjectCategory> Block::getCollisionTargets() const {
    return {};
}

void Block::checkCollision(const std::vector<Object*>& candidates) {
    return;
}

void Block::onCollision(Object* other) {}
    
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
}

bool Block::isSolid() const {
    return solid;
}

BlockType Block::getType() const {
    return blockType;
}

GroundBlock::GroundBlock(Vector2 gridPos) : Block(gridPos, BlockType::GROUND) {
    solid = true;
}

BrickBlock::BrickBlock(Vector2 gridPos) : Block(gridPos, BlockType::BRICK) {
    solid = true;
}