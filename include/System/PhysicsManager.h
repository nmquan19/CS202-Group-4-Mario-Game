#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include "../box2d/box2d.h"
#include "Interface.h"
#include "PhysicsBody.h"
#include "ContactListener.h"
#include "../Objects/ObjectFactory.h"
#include <raylib.h>

class PhysicsManager {
public:
    static PhysicsManager& getInstance();
    static void cleanup();

    // World management
    void initializeWorld(Vector2 gravity = {0.0f, 980.0f});
    void setGravity(Vector2 gravity);
    Vector2 getGravity() const;
    void setWorldBounds(Rectangle bounds);
    
    // Object management
    std::shared_ptr<PhysicsBody> addObject(std::shared_ptr<Object> object, const BodyCreateInfo& createInfo);
    void removeObject(std::shared_ptr<Object> object);
    void markForDeletion(std::shared_ptr<Object> object);
    
    // Physics simulation
    void update(float deltaTime);
    void step(float timeStep, int velocityIterations = 8, int positionIterations = 3);
    
    // Query methods
    std::vector<std::shared_ptr<Object>> getObjectsInLayer(ObjectCategory objectCategory);
    std::vector<std::shared_ptr<Object>> getObjectsInArea(Rectangle area);
    std::vector<std::shared_ptr<Object>> getObjectsNearPoint(Vector2 point, float radius);
    
    // Raycast and queries
    std::shared_ptr<Object> raycast(Vector2 start, Vector2 end, ObjectCategory targetCategory = ObjectCategory::BLOCK);
    std::vector<std::shared_ptr<Object>> raycastAll(Vector2 start, Vector2 end);
    
    // Ground detection
    bool isObjectOnGround(std::shared_ptr<Object> object) const;
    
    // Physics body access
    std::shared_ptr<PhysicsBody> getPhysicsBody(std::shared_ptr<Object> object) const;
    
    // Utility methods
    static Vector2 box2DToRaylib(const b2Vec2& vec);
    static b2Vec2 raylibToBox2D(const Vector2& vec);
    static Rectangle box2DToRaylib(const b2AABB& aabb);
    
    // Debug rendering
    void drawDebug() const;
    void setDebugDraw(bool enabled);
    bool isDebugDrawEnabled() const;
    
    // Statistics
    void getDebugStats(int& bodyCount, int& contactCount, float& frameTime) const;
    
    // Pause/Resume physics
    void pausePhysics();
    void resumePhysics();
    bool isPhysicsPaused() const;

private:
    PhysicsManager() = default;
    ~PhysicsManager();
    
    // Helper methods
    void createWorldBoundaries(Rectangle bounds);
    void updateObjectPositions();
    void processCollisions();
    void cleanupInvalidBodies();
    b2Body* createBody(const BodyCreateInfo& createInfo);
    void setupFixture(b2Body* body, const BodyCreateInfo& createInfo);
    
    // Box2D world and components
    std::unique_ptr<b2World> world;
    std::unique_ptr<GameContactListener> contactListener;
    
    // Object tracking
    std::unordered_map<std::shared_ptr<Object>, std::shared_ptr<PhysicsBody>> objectToBody;
    std::unordered_map<b2Body*, std::shared_ptr<PhysicsBody>> bodyToPhysicsBody;
    std::vector<std::shared_ptr<Object>> objectsToRemove;
    
    // World bounds
    Rectangle worldBounds = {0, 0, 1920, 1080};
    std::vector<b2Body*> boundaryBodies;
    
    // Simulation settings
    bool physicsPaused = false;
    bool debugDraw = false;
    float accumulator = 0.0f;
    const float fixedTimeStep = 1.0f / 60.0f;  // 60 FPS physics
    
    // Debug statistics
    mutable float lastFrameTime = 0.0f;
    
    static PhysicsManager* instance;
};