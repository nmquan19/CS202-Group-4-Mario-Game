#include "../../include/System/PhysicsManager.h"
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/System/Interface.h"
#include "../../include/Characters/Character.h"
#include "../../include/Enemy/Enemy.h"
#include <raylib.h>
#include <raymath.h>
#include <memory>
#include <iostream>

PhysicsManager* PhysicsManager::instance = nullptr;

PhysicsManager& PhysicsManager::getInstance() {
    if (!instance) {
        instance = new PhysicsManager();
    }
    return *instance;
}

PhysicsManager::~PhysicsManager() {
    if (world) {
        // Clean up all bodies
        b2Body* body = world->GetBodyList();
        while (body) {
            b2Body* next = body->GetNext();
            
            // Clear user data pointer (PhysicsBody is managed by shared_ptr)
            if (body->GetUserData().pointer) {
                body->GetUserData().pointer = 0;
            }
            
            world->DestroyBody(body);
            body = next;
        }
    }
}

void PhysicsManager::initializeWorld(Vector2 gravity) {
    // Create Box2D world
    b2Vec2 b2Gravity(gravity.x, gravity.y);
    world = std::make_unique<b2World>(b2Gravity);
    
    // Create and set contact listener
    contactListener = std::make_unique<GameContactListener>();
    world->SetContactListener(contactListener.get());
    
    // Allow bodies to sleep for better performance
    world->SetAllowSleeping(true);
    
    std::cout << "Box2D Physics World initialized with gravity: (" 
              << gravity.x << ", " << gravity.y << ")" << std::endl;
}

void PhysicsManager::setGravity(Vector2 gravity) {
    if (world) {
        world->SetGravity(b2Vec2(gravity.x, gravity.y));
    }
}

Vector2 PhysicsManager::getGravity() const {
    if (world) {
        b2Vec2 gravity = world->GetGravity();
        return {gravity.x, gravity.y};
    }
    return {0, 0};
}

void PhysicsManager::setWorldBounds(Rectangle bounds) {
    worldBounds = bounds;
    
    if (world) {
        // Remove old boundary bodies
        for (b2Body* body : boundaryBodies) {
            if (body->GetUserData().pointer) {
                body->GetUserData().pointer = 0;  // Clear pointer (managed by shared_ptr)
            }
            world->DestroyBody(body);
        }
        boundaryBodies.clear();
        
        createWorldBoundaries(bounds);
    }
}

std::shared_ptr<PhysicsBody> PhysicsManager::addObject(std::shared_ptr<Object> object, const BodyCreateInfo& createInfo) {
    if (!world || !object) {
        return nullptr;
    }
    
    // Check if object already has a physics body
    auto it = objectToBody.find(object);
    if (it != objectToBody.end()) {
        std::cout << "Warning: Object already has a physics body!" << std::endl;
        return it->second;
    }
    
    // Create Box2D body
    b2Body* body = createBody(createInfo);
    if (!body) {
        return nullptr;
    }
    
    // Create physics body wrapper
    auto physicsBody = std::make_shared<PhysicsBody>(body, object, object->getObjectCategory(), 
                                                    createInfo.isStatic, createInfo.material.isSensor);
    
    // Set user data
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(physicsBody.get());
    
    // Store mappings
    objectToBody[object] = physicsBody;
    bodyToPhysicsBody[body] = physicsBody;
    
    // Setup fixture
    setupFixture(body, createInfo);
    
    std::cout << "Added physics body for object at (" 
              << createInfo.position.x << ", " << createInfo.position.y << ")" << std::endl;
    
    return physicsBody;
}

void PhysicsManager::removeObject(std::shared_ptr<Object> object) {
    if (!object) return;
    
    auto it = objectToBody.find(object);
    if (it != objectToBody.end()) {
        auto physicsBody = it->second;
        if (physicsBody && physicsBody->body) {
            // Remove from body mapping
            bodyToPhysicsBody.erase(physicsBody->body);
            
            // Clean up contact listener references
            contactListener->clearGroundContacts(object);
            
            // Mark body for destruction (will be destroyed in next world step)
            physicsBody->body->SetEnabled(false);
            
            // Delete user data
            if (physicsBody->body->GetUserData().pointer) {
                physicsBody->body->GetUserData().pointer = 0;
            }
            
            world->DestroyBody(physicsBody->body);
            physicsBody->body = nullptr;
        }
        
        objectToBody.erase(it);
    }
}

void PhysicsManager::markForDeletion(std::shared_ptr<Object> object) {
    if (object) {
        object->setActive(false);
        objectsToRemove.push_back(object);
    }
}

void PhysicsManager::update(float deltaTime) {
    if (!world || physicsPaused) {
        return;
    }
    
    auto frameStart = GetTime();
    
    // Clean up objects marked for deletion
    for (auto obj : objectsToRemove) {
        removeObject(obj);
    }
    objectsToRemove.clear();
    
    // Remove invalid bodies
    cleanupInvalidBodies();
    
    // Step physics simulation using fixed timestep
    accumulator += deltaTime;
    
    while (accumulator >= fixedTimeStep) {
        step(fixedTimeStep);
        accumulator -= fixedTimeStep;
    }
    
    // Update game object positions from physics bodies
    updateObjectPositions();
    
    lastFrameTime = GetTime() - frameStart;
}

void PhysicsManager::step(float timeStep, int velocityIterations, int positionIterations) {
    if (world) {
        world->Step(timeStep, velocityIterations, positionIterations);
    }
}

std::vector<std::shared_ptr<Object>> PhysicsManager::getObjectsInLayer(ObjectCategory objectCategory) {
    std::vector<std::shared_ptr<Object>> result;
    
    for (const auto& pair : objectToBody) {
        auto object = pair.first;
        auto physicsBody = pair.second;
        
        if (object && object->isActive() && physicsBody && 
            physicsBody->category == objectCategory) {
            result.push_back(object);
        }
    }
    
    return result;
}

std::vector<std::shared_ptr<Object>> PhysicsManager::getObjectsInArea(Rectangle area) {
    std::vector<std::shared_ptr<Object>> result;
    
    if (!world) return result;
    
    // Convert area to Box2D AABB
    b2AABB aabb;
    aabb.lowerBound = b2Vec2(area.x, area.y);
    aabb.upperBound = b2Vec2(area.x + area.width, area.y + area.height);
    
    // Query the world
    class AreaQueryCallback : public b2QueryCallback {
    public:
        std::vector<std::shared_ptr<Object>>* objects;
        PhysicsManager* manager;
        
        AreaQueryCallback(std::vector<std::shared_ptr<Object>>* objs, PhysicsManager* mgr) 
            : objects(objs), manager(mgr) {}
        
        bool ReportFixture(b2Fixture* fixture) override {
            auto physicsBody = manager->bodyToPhysicsBody.find(fixture->GetBody());
            if (physicsBody != manager->bodyToPhysicsBody.end()) {
                auto gameObject = physicsBody->second->getGameObject();
                if (gameObject && gameObject->isActive()) {
                    objects->push_back(gameObject);
                }
            }
            return true; // Continue query
        }
    };
    
    AreaQueryCallback callback(&result, this);
    world->QueryAABB(&callback, aabb);
    
    return result;
}

std::vector<std::shared_ptr<Object>> PhysicsManager::getObjectsNearPoint(Vector2 point, float radius) {
    Rectangle area = {
        point.x - radius,
        point.y - radius,
        radius * 2,
        radius * 2
    };
    
    auto nearbyObjects = getObjectsInArea(area);
    std::vector<std::shared_ptr<Object>> result;
    
    // Filter by actual distance
    for (auto obj : nearbyObjects) {
        Vector2 objPos = obj->getPosition();
        float distance = Vector2Distance(point, objPos);
        if (distance <= radius) {
            result.push_back(obj);
        }
    }
    
    return result;
}

std::shared_ptr<Object> PhysicsManager::raycast(Vector2 start, Vector2 end, ObjectCategory targetCategory) {
    if (!world) return nullptr;
    
    class RaycastCallback : public b2RayCastCallback {
    public:
        PhysicsManager* manager;
        ObjectCategory target;
        std::shared_ptr<Object> hitObject = nullptr;
        float closestFraction = 1.0f;
        
        RaycastCallback(PhysicsManager* mgr, ObjectCategory cat) : manager(mgr), target(cat) {}
        
        float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override {
            auto physicsBodyIt = manager->bodyToPhysicsBody.find(fixture->GetBody());
            if (physicsBodyIt != manager->bodyToPhysicsBody.end()) {
                auto physicsBody = physicsBodyIt->second;
                if (physicsBody->category == target) {
                    auto gameObject = physicsBody->getGameObject();
                    if (gameObject && gameObject->isActive() && fraction < closestFraction) {
                        hitObject = gameObject;
                        closestFraction = fraction;
                    }
                }
            }
            return fraction; // Continue with current fraction
        }
    };
    
    RaycastCallback callback(this, targetCategory);
    world->RayCast(&callback, raylibToBox2D(start), raylibToBox2D(end));
    
    return callback.hitObject;
}

std::vector<std::shared_ptr<Object>> PhysicsManager::raycastAll(Vector2 start, Vector2 end) {
    std::vector<std::shared_ptr<Object>> result;
    
    if (!world) return result;
    
    class RaycastAllCallback : public b2RayCastCallback {
    public:
        PhysicsManager* manager;
        std::vector<std::shared_ptr<Object>>* objects;
        
        RaycastAllCallback(PhysicsManager* mgr, std::vector<std::shared_ptr<Object>>* objs) 
            : manager(mgr), objects(objs) {}
        
        float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override {
            auto physicsBodyIt = manager->bodyToPhysicsBody.find(fixture->GetBody());
            if (physicsBodyIt != manager->bodyToPhysicsBody.end()) {
                auto gameObject = physicsBodyIt->second->getGameObject();
                if (gameObject && gameObject->isActive()) {
                    objects->push_back(gameObject);
                }
            }
            return 1.0f; // Continue to get all hits
        }
    };
    
    RaycastAllCallback callback(this, &result);
    world->RayCast(&callback, raylibToBox2D(start), raylibToBox2D(end));
    
    return result;
}

bool PhysicsManager::isObjectOnGround(std::shared_ptr<Object> object) const {
    return contactListener && contactListener->isObjectOnGround(object);
}

std::shared_ptr<PhysicsBody> PhysicsManager::getPhysicsBody(std::shared_ptr<Object> object) const {
    auto it = objectToBody.find(object);
    return (it != objectToBody.end()) ? it->second : nullptr;
}

// Utility conversion methods
Vector2 PhysicsManager::box2DToRaylib(const b2Vec2& vec) {
    return {vec.x, vec.y};
}

b2Vec2 PhysicsManager::raylibToBox2D(const Vector2& vec) {
    return b2Vec2(vec.x, vec.y);
}

Rectangle PhysicsManager::box2DToRaylib(const b2AABB& aabb) {
    return {
        aabb.lowerBound.x,
        aabb.lowerBound.y,
        aabb.upperBound.x - aabb.lowerBound.x,
        aabb.upperBound.y - aabb.lowerBound.y
    };
}

void PhysicsManager::drawDebug() const {
    if (!debugDraw || !world) return;
    
    // Draw all physics bodies
    for (b2Body* body = world->GetBodyList(); body; body = body->GetNext()) {
        for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            b2Shape* shape = fixture->GetShape();
            
            Color color = (body->GetType() == b2_staticBody) ? BLUE : (body->IsAwake() ? GREEN : GRAY);
            
            if (shape->GetType() == b2Shape::e_polygon) {
                b2PolygonShape* poly = static_cast<b2PolygonShape*>(shape);
                for (int i = 0; i < poly->m_count; ++i) {
                    b2Vec2 v1 = body->GetWorldPoint(poly->m_vertices[i]);
                    b2Vec2 v2 = body->GetWorldPoint(poly->m_vertices[(i + 1) % poly->m_count]);
                    
                    DrawLine(v1.x, v1.y, v2.x, v2.y, color);
                }
            } else if (shape->GetType() == b2Shape::e_circle) {
                b2CircleShape* circle = static_cast<b2CircleShape*>(shape);
                b2Vec2 center = body->GetWorldPoint(circle->m_p);
                
                DrawCircleLines(center.x, center.y, circle->m_radius, color);
            }
        }
    }
}

void PhysicsManager::setDebugDraw(bool enabled) {
    debugDraw = enabled;
}

bool PhysicsManager::isDebugDrawEnabled() const {
    return debugDraw;
}

void PhysicsManager::getDebugStats(int& bodyCount, int& contactCount, float& frameTime) const {
    bodyCount = world ? world->GetBodyCount() : 0;
    contactCount = world ? world->GetContactCount() : 0;
    frameTime = lastFrameTime;
}

void PhysicsManager::pausePhysics() {
    physicsPaused = true;
}

void PhysicsManager::resumePhysics() {
    physicsPaused = false;
}

bool PhysicsManager::isPhysicsPaused() const {
    return physicsPaused;
}

void PhysicsManager::cleanup() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

// Private helper methods

void PhysicsManager::createWorldBoundaries(Rectangle bounds) {
    if (!world) return;
    
    // Create static bodies for world boundaries
    float thickness = 50.0f;
    
    // Ground
    BodyCreateInfo groundInfo = BodyCreateInfo::StaticBox(
        {bounds.x + bounds.width / 2, bounds.y + bounds.height + thickness / 2},
        {bounds.width, thickness}
    );
    b2Body* ground = createBody(groundInfo);
    setupFixture(ground, groundInfo);
    boundaryBodies.push_back(ground);
    
    // Left wall
    BodyCreateInfo leftWallInfo = BodyCreateInfo::StaticBox(
        {bounds.x - thickness / 2, bounds.y + bounds.height / 2},
        {thickness, bounds.height}
    );
    b2Body* leftWall = createBody(leftWallInfo);
    setupFixture(leftWall, leftWallInfo);
    boundaryBodies.push_back(leftWall);
    
    // Right wall
    BodyCreateInfo rightWallInfo = BodyCreateInfo::StaticBox(
        {bounds.x + bounds.width + thickness / 2, bounds.y + bounds.height / 2},
        {thickness, bounds.height}
    );
    b2Body* rightWall = createBody(rightWallInfo);
    setupFixture(rightWall, rightWallInfo);
    boundaryBodies.push_back(rightWall);
    
    // Ceiling (optional)
    BodyCreateInfo ceilingInfo = BodyCreateInfo::StaticBox(
        {bounds.x + bounds.width / 2, bounds.y - thickness / 2},
        {bounds.width, thickness}
    );
    b2Body* ceiling = createBody(ceilingInfo);
    setupFixture(ceiling, ceilingInfo);
    boundaryBodies.push_back(ceiling);
}

void PhysicsManager::updateObjectPositions() {
    for (const auto& pair : objectToBody) {
        auto object = pair.first;
        auto physicsBody = pair.second;
        
        if (object && object->isActive() && physicsBody && physicsBody->body) {
            // Update game object position from physics body
            Vector2 physicsPos = physicsBody->getPosition();
            object->setPosition(physicsPos);
        }
    }
}

void PhysicsManager::cleanupInvalidBodies() {
    std::vector<std::shared_ptr<Object>> toRemove;
    
    for (const auto& pair : objectToBody) {
        auto object = pair.first;
        auto physicsBody = pair.second;
        
        if (!object || !object->isActive() || !physicsBody || !physicsBody->body) {
            toRemove.push_back(object);
        }
    }
    
    for (auto obj : toRemove) {
        removeObject(obj);
    }
}

b2Body* PhysicsManager::createBody(const BodyCreateInfo& createInfo) {
    if (!world) return nullptr;
    
    b2BodyDef bodyDef;
    bodyDef.type = createInfo.isStatic ? b2_staticBody : b2_dynamicBody;
    bodyDef.position.Set(createInfo.position.x, createInfo.position.y);
    bodyDef.fixedRotation = createInfo.fixedRotation;
    bodyDef.gravityScale = createInfo.gravityScale;
    
    return world->CreateBody(&bodyDef);
}

void PhysicsManager::setupFixture(b2Body* body, const BodyCreateInfo& createInfo) {
    if (!body) return;
    
    b2FixtureDef fixtureDef;
    fixtureDef.density = createInfo.material.density;
    fixtureDef.friction = createInfo.material.friction;
    fixtureDef.restitution = createInfo.material.restitution;
    fixtureDef.isSensor = createInfo.material.isSensor;
    
    b2Shape* shape = nullptr;
    b2PolygonShape boxShape;
    b2CircleShape circleShape;
    
    switch (createInfo.shape) {
        case PhysicsShape::BOX:
            boxShape.SetAsBox(createInfo.size.x / 2.0f, createInfo.size.y / 2.0f);
            shape = &boxShape;
            break;
            
        case PhysicsShape::CIRCLE:
            circleShape.m_radius = createInfo.size.x; // Use x as radius
            shape = &circleShape;
            break;
            
        case PhysicsShape::POLYGON:
            if (!createInfo.vertices.empty()) {
                std::vector<b2Vec2> vertices;
                for (const auto& vertex : createInfo.vertices) {
                    vertices.push_back(b2Vec2(vertex.x, vertex.y));
                }
                boxShape.Set(vertices.data(), vertices.size());
                shape = &boxShape;
            } else {
                // Fallback to box
                boxShape.SetAsBox(createInfo.size.x / 2.0f, createInfo.size.y / 2.0f);
                shape = &boxShape;
            }
            break;
    }
    
    if (shape) {
        fixtureDef.shape = shape;
        body->CreateFixture(&fixtureDef);
    }
}