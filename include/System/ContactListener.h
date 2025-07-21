#pragma once

#include "../box2d/box2d.h"
#include "PhysicsBody.h"
#include <memory>
#include <unordered_set>
#include <unordered_map>

class Object;

struct ContactPairHash {
    size_t operator()(const std::pair<b2Body*, b2Body*>& p) const {
        return std::hash<void*>{}(p.first) ^ (std::hash<void*>{}(p.second) << 1);
    }
};

class GameContactListener : public b2ContactListener {
public:
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
    
    // Get collision information
    bool areObjectsInContact(std::shared_ptr<Object> obj1, std::shared_ptr<Object> obj2) const;
    std::vector<std::shared_ptr<Object>> getContactsFor(std::shared_ptr<Object> obj) const;
    
    // Ground detection for characters
    bool isObjectOnGround(std::shared_ptr<Object> obj) const;
    void clearGroundContacts(std::shared_ptr<Object> obj);
    
private:
    // Helper methods
    PhysicsBody* getPhysicsBody(b2Body* body) const;
    std::shared_ptr<Object> getGameObject(b2Body* body) const;
    bool shouldProcessCollision(PhysicsBody* bodyA, PhysicsBody* bodyB) const;
    void handleCharacterGroundContact(std::shared_ptr<Object> character, std::shared_ptr<Object> ground, bool isBegin);
    void handleEnemyCollision(std::shared_ptr<Object> enemy1, std::shared_ptr<Object> enemy2, bool isBegin);
    void handleCharacterEnemyCollision(std::shared_ptr<Object> character, std::shared_ptr<Object> enemy, bool isBegin);
    void handleItemCollection(std::shared_ptr<Object> character, std::shared_ptr<Object> item);
    
    // Active contacts tracking
    std::unordered_set<std::pair<b2Body*, b2Body*>, ContactPairHash> activeContacts;
    
    // Ground contacts for characters (for jump mechanics)
    std::unordered_map<std::shared_ptr<Object>, std::unordered_set<std::shared_ptr<Object>>> groundContacts;
};

