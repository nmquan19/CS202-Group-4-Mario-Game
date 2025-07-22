#include "../../include/Objects/Block.h"
#include "../../include/System/Grid.h"
#include "../../include/System/TextureManager.h"
#include <vector>
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/System/Interface.h"
#include <raylib.h>

Block::Block(Vector2 gridPos, BlockType type, Vector2 s) : gridPosition(gridPos), blockType(type) {
    position = { gridPos.x * GridSystem::GRID_SIZE, gridPos.y * GridSystem::GRID_SIZE };
    size = s;
    hitbox = {position.x, position.y, size.x*GridSystem::GRID_SIZE, size.y*GridSystem::GRID_SIZE};
}

Block::~Block() {}

void Block::draw() {
    if (!active) return;

    Texture2D texture = TextureManager::getInstance().getBlockTexture(getType());

    Rectangle destRec = { position.x, position.y, hitbox.width, hitbox.height};
    Rectangle sourceRec = { 0, 0, (float)texture.width, (float)texture.height };

    DrawTexturePro(texture, sourceRec, destRec, { 0, 0 }, 0.0f, WHITE);
}

std::vector<Rectangle> Block::getHitBox() const {
   return {hitbox};
}

ObjectCategory Block::getObjectCategory() const {
    return ObjectCategory::BLOCK;
}

std::vector<ObjectCategory> Block::getCollisionTargets() const {
    return {};
}

void Block::checkCollision(const std::vector<std::shared_ptr<Object>>& candidates) {
    return;
}

void Block::onCollision(std::shared_ptr<Object> other) {}
    
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

ObjectType Block::getObjectType() const {
    return getType();
}

BlockType Block::getType() const {
    return blockType;
}

Vector2 Block::getSize() const {
    return size;
}

GroundBlock::GroundBlock(Vector2 gridPos) : Block(gridPos, BlockType::GROUND, {1, 1}) {
    solid = true;
}

BrickBlock::BrickBlock(Vector2 gridPos) : Block(gridPos, BlockType::BRICK, {1, 1}) {
    solid = true;
}
