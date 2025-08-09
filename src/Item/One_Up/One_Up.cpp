#include "../../../include/Item/One_Up/One_Up.h"
#include "../../../include/Game/GameContext.h"
#include "../../../include/System/Box2DWorldManager.h"
#include "../../../include/System/Grid.h"
#include <raylib.h>

One_Up::One_Up(Vector2 startPos) : Item(startPos) {
	gravity = 600.0f; // Set gravity for the item

    size = { 1, 1 };
    hitbox = {
        startPos.x,
        startPos.y,
        size.x * GridSystem::GRID_SIZE,
        size.y * GridSystem::GRID_SIZE
    };

    //physicsBody = Box2DWorldManager::getInstance().createItemBody(startPos, { hitbox.width, hitbox.height });
    //if (physicsBody) {
    //    physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
    //}
}

One_Up::~One_Up() {
    if (physicsBody) {
        Box2DWorldManager::getInstance().destroyBody(physicsBody);
        physicsBody = nullptr;
    }
}

void One_Up::update(float deltaTime) {
    aniTimer += deltaTime;
    if (aniTimer >= aniSpeed) {
        aniTimer = 0;
        animation->Update();
    }
}

void One_Up::Move(double dt) {
    // Gravity effect
	velocity.y += gravity * dt; // Adjust gravity effect as needed

	position.y += velocity.y * dt; // Update position based on velocity



    if (physicsBody) {
        b2Vec2 b2Pos = physicsBody->GetPosition();
        Vector2 bodyPos = Box2DWorldManager::b2ToRaylib(b2Pos);
        position.x = bodyPos.x - hitbox.width * 0.5f;
        position.y = bodyPos.y - hitbox.height * 0.5f;
        hitbox.x = position.x;
        hitbox.y = position.y;
    }

    if (position.y + hitbox.height < 0) {
        active = false;
        GameContext::getInstance().mark_for_deletion_Object(
            GameContext::getInstance().getSharedPtrFromRaw(this)
        );
        return;
    }


    // Update hitbox position
    hitbox.x = position.x;
    hitbox.y = position.y;
}

