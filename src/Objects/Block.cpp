#include "../../include/Objects/Block.h"
#include "../../include/System/Grid.h"
#include "../../include/System/TextureManager.h"
#include <vector>
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/System/Interface.h"
#include "../../include/System/Box2DWorldManager.h"
#include <raylib.h>
#include <iostream>

Block::Block(Vector2 gridPos, BlockType type, Vector2 s) : gridPosition(gridPos), blockType(type), isMoving(false) {
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

    Rectangle destRec = { position.x, position.y, hitbox.width, hitbox.height};
    Rectangle srcRec;

    switch (blockType) {
        case(BlockType::GROUND): srcRec = Constants::PaletteResources::GROUND;
            break;
        case(BlockType::BRICK): srcRec = Constants::PaletteResources::BRICK;
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
    if (direction == Direction::DOWN && !isMoving && !isSolid()) {
        physicsBody->SetLinearVelocity(b2Vec2{ 0.0f, -2.0f });
        isMoving = true;
        resetTimer = 0.0f;
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

GroundBlock::GroundBlock(Vector2 gridPos) : Block(gridPos, BlockType::GROUND, {1, 1}) {
    solid = false;
}

BrickBlock::BrickBlock(Vector2 gridPos) : Block(gridPos, BlockType::BRICK, {1, 1}) {
    solid = false;
}
