#pragma once
#include "raylib.h"
#include "../System/Interface.h"
#include <vector>
#include "..\..\include\System\PhysicsManager.h"
#include <cstdlib>
#include <math.h>
#include <iostream>

class Item : public ICollidable, public Object {
public:
    Item();
    virtual ~Item();

    std::vector<Rectangle> getHitBox() const override;
    ObjectCategory getObjectCategory() const override;
    std::vector<ObjectCategory> getCollisionTargets() const override;
    void checkCollision(const std::vector<std::shared_ptr<Object>>& candidates) override;
    void onCollision(std::shared_ptr<Object> other) override;

    bool isActive();

    int getCurrentFrame();

protected:
    bool isGrounded;

    std::vector<std::pair<int, int>> num_sprites;

    Rectangle hitbox;
    Rectangle spriteBox;
    Texture2D texture;
    bool IsActive;
};
