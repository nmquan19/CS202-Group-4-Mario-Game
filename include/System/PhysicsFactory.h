#pragma once

#include "PhysicsBody.h"
#include "Interface.h"
#include <raylib.h>

class PhysicsFactory {
public:
    // Create physics body for character
    static BodyCreateInfo createCharacterBody(Vector2 position, Vector2 size) {
        BodyCreateInfo info;
        info.position = position;
        info.size = size;
        info.shape = PhysicsShape::BOX;
        info.material = PhysicsMaterial::Character();
        info.isStatic = false;
        info.fixedRotation = true;
        info.gravityScale = 1.0f;
        return info;
    }
    
    // Create physics body for ground blocks
    static BodyCreateInfo createGroundBody(Vector2 position, Vector2 size) {
        BodyCreateInfo info;
        info.position = position;
        info.size = size;
        info.shape = PhysicsShape::BOX;
        info.material = PhysicsMaterial::Ground();
        info.isStatic = true;
        info.fixedRotation = true;
        info.gravityScale = 0.0f;
        return info;
    }
    
    // Create physics body for enemies
    static BodyCreateInfo createEnemyBody(Vector2 position, Vector2 size, EnemyType type) {
        BodyCreateInfo info;
        info.position = position;
        info.size = size;
        info.shape = PhysicsShape::BOX;
        info.material = PhysicsMaterial::Enemy();
        info.isStatic = false;
        info.fixedRotation = true;
        info.gravityScale = 1.0f;
        
        // Customize based on enemy type
        switch (type) {
            case EnemyType::GOOMBA:
                info.material.friction = 0.1f;
                break;
            case EnemyType::GREEN_KOOPA:
            case EnemyType::RED_KOOPA:
                info.material.friction = 0.2f;
                break;
            default:
                break;
        }
        
        return info;
    }
    
    // Create physics body for items
    static BodyCreateInfo createItemBody(Vector2 position, Vector2 size, ItemType type) {
        BodyCreateInfo info;
        info.position = position;
        info.size = size;
        info.shape = PhysicsShape::BOX;
        info.material = PhysicsMaterial::Item();
        info.isStatic = false;
        info.fixedRotation = true;
        info.gravityScale = 0.8f; // Items fall a bit slower
        
        // Make items sensors so they don't physically collide
        info.material.isSensor = true;
        
        return info;
    }
    
    // Create physics body for projectiles
    static BodyCreateInfo createProjectileBody(Vector2 position, Vector2 size) {
        BodyCreateInfo info;
        info.position = position;
        info.size = size;
        info.shape = PhysicsShape::BOX;
        info.material = PhysicsMaterial::Projectile();
        info.isStatic = false;
        info.fixedRotation = false; // Projectiles can rotate
        info.gravityScale = 0.5f; // Reduced gravity for projectiles
        return info;
    }
    
    // Create physics body for koopa shells
    static BodyCreateInfo createKoopaShellBody(Vector2 position, Vector2 size) {
        BodyCreateInfo info;
        info.position = position;
        info.size = size;
        info.shape = PhysicsShape::BOX;
        info.material = PhysicsMaterial::Projectile();
        info.isStatic = false;
        info.fixedRotation = false; // Shells can roll/rotate
        info.gravityScale = 1.0f;
        
        // High restitution for bouncing
        info.material.restitution = 0.6f;
        info.material.friction = 0.1f; // Low friction for rolling
        
        return info;
    }
    
    // Create physics body for triggers/sensors
    static BodyCreateInfo createTriggerBody(Vector2 position, Vector2 size) {
        BodyCreateInfo info;
        info.position = position;
        info.size = size;
        info.shape = PhysicsShape::BOX;
        info.material = PhysicsMaterial::Sensor();
        info.isStatic = true;
        info.fixedRotation = true;
        info.gravityScale = 0.0f;
        return info;
    }
    
    // Create physics body for moving platforms
    static BodyCreateInfo createMovingPlatformBody(Vector2 position, Vector2 size) {
        BodyCreateInfo info;
        info.position = position;
        info.size = size;
        info.shape = PhysicsShape::BOX;
        info.material = PhysicsMaterial::Ground();
        info.isStatic = false; // Can move but controlled kinematically
        info.fixedRotation = true;
        info.gravityScale = 0.0f; // Platforms don't fall
        return info;
    }
};
