#pragma once

#include "../box2d/box2d.h"
#include "../System/Interface.h"
#include <memory>
#include <raylib.h>

class Object;

// Physics body wrapper that connects Box2D bodies with game objects
struct PhysicsBody {
    b2Body* body = nullptr;
    std::weak_ptr<Object> gameObject;
    ObjectCategory category;
    bool isStatic = false;
    bool isSensor = false;
    
    PhysicsBody() = default;
    PhysicsBody(b2Body* b, std::weak_ptr<Object> obj, ObjectCategory cat, bool staticBody = false, bool sensor = false)
        : body(b), gameObject(obj), category(cat), isStatic(staticBody), isSensor(sensor) {}
    
    // Get the game object if it's still valid
    std::shared_ptr<Object> getGameObject() const {
        return gameObject.lock();
    }
    
    // Check if the game object is still valid
    bool isValid() const {
        return !gameObject.expired() && body != nullptr;
    }
    
    // Convert Box2D position to Raylib position
    Vector2 getPosition() const {
        if (!body) return {0, 0};
        b2Vec2 pos = body->GetPosition();
        return {pos.x, pos.y};
    }
    
    // Convert Box2D velocity to Raylib velocity
    Vector2 getVelocity() const {
        if (!body) return {0, 0};
        b2Vec2 vel = body->GetLinearVelocity();
        return {vel.x, vel.y};
    }
    
    // Set velocity
    void setVelocity(Vector2 velocity) {
        if (body && !isStatic) {
            body->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
        }
    }
    
    // Apply force
    void applyForce(Vector2 force) {
        if (body && !isStatic) {
            body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
        }
    }
    
    // Apply impulse
    void applyImpulse(Vector2 impulse) {
        if (body && !isStatic) {
            body->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), true);
        }
    }
    
    // Set position
    void setPosition(Vector2 position) {
        if (body) {
            body->SetTransform(b2Vec2(position.x, position.y), body->GetAngle());
        }
    }
    
    // Enable/disable physics simulation
    void setEnabled(bool enabled) {
        if (body) {
            body->SetEnabled(enabled);
        }
    }
    
    // Set as sensor (no collision response, only detection)
    void setSensor(bool sensor) {
        if (body) {
            isSensor = sensor;
            b2Fixture* fixture = body->GetFixtureList();
            while (fixture) {
                fixture->SetSensor(sensor);
                fixture = fixture->GetNext();
            }
        }
    }
};

// Physics material properties
struct PhysicsMaterial {
    float density = 1.0f;
    float friction = 0.3f;
    float restitution = 0.0f;  // Bounciness
    bool isSensor = false;
    
    PhysicsMaterial() = default;
    PhysicsMaterial(float d, float f, float r, bool sensor = false)
        : density(d), friction(f), restitution(r), isSensor(sensor) {}
        
    // Predefined materials for Mario game
    static PhysicsMaterial Ground() { return PhysicsMaterial(0.0f, 0.7f, 0.0f); }
    static PhysicsMaterial Character() { return PhysicsMaterial(1.0f, 0.3f, 0.0f); }
    static PhysicsMaterial Enemy() { return PhysicsMaterial(1.0f, 0.1f, 0.1f); }
    static PhysicsMaterial Item() { return PhysicsMaterial(0.5f, 0.1f, 0.3f); }
    static PhysicsMaterial Projectile() { return PhysicsMaterial(0.3f, 0.1f, 0.8f); }
    static PhysicsMaterial Sensor() { return PhysicsMaterial(0.0f, 0.0f, 0.0f, true); }
};

// Body shape types
enum class PhysicsShape {
    BOX,
    CIRCLE,
    POLYGON
};

// Body creation parameters
struct BodyCreateInfo {
    Vector2 position = {0, 0};
    Vector2 size = {1, 1};
    PhysicsShape shape = PhysicsShape::BOX;
    PhysicsMaterial material;
    bool isStatic = false;
    bool fixedRotation = true;  // Most Mario objects don't rotate
    float gravityScale = 1.0f;
    std::vector<Vector2> vertices;  // For polygon shapes
    
    BodyCreateInfo() = default;
    
    // Convenience constructors
    static BodyCreateInfo StaticBox(Vector2 pos, Vector2 size, PhysicsMaterial mat = PhysicsMaterial::Ground()) {
        BodyCreateInfo info;
        info.position = pos;
        info.size = size;
        info.material = mat;
        info.isStatic = true;
        return info;
    }
    
    static BodyCreateInfo DynamicBox(Vector2 pos, Vector2 size, PhysicsMaterial mat = PhysicsMaterial::Character()) {
        BodyCreateInfo info;
        info.position = pos;
        info.size = size;
        info.material = mat;
        info.isStatic = false;
        return info;
    }
    
    static BodyCreateInfo DynamicCircle(Vector2 pos, float radius, PhysicsMaterial mat = PhysicsMaterial::Character()) {
        BodyCreateInfo info;
        info.position = pos;
        info.size = {radius, radius};
        info.shape = PhysicsShape::CIRCLE;
        info.material = mat;
        info.isStatic = false;
        return info;
    }
    
    static BodyCreateInfo Sensor(Vector2 pos, Vector2 size) {
        BodyCreateInfo info;
        info.position = pos;
        info.size = size;
        info.material = PhysicsMaterial::Sensor();
        info.isStatic = true;
        return info;
    }
};
