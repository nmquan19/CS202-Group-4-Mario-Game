#include "../../include/System/ContactListener.h"
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/Characters/Character.h"
#include "../../include/Enemy/Enemy.h"
#include "../../include/System/Interface.h"
#include "../../include/Game/GameContext.h"
#include <iostream>

void GameContactListener::BeginContact(b2Contact* contact) {
    PhysicsBody* bodyA = getPhysicsBody(contact->GetFixtureA()->GetBody());
    PhysicsBody* bodyB = getPhysicsBody(contact->GetFixtureB()->GetBody());
    
    if (!bodyA || !bodyB || !shouldProcessCollision(bodyA, bodyB)) {
        return;
    }
    
    std::shared_ptr<Object> objA = bodyA->getGameObject();
    std::shared_ptr<Object> objB = bodyB->getGameObject();
    
    if (!objA || !objB) {
        return;
    }
    
    // Track the contact
    b2Body* bA = contact->GetFixtureA()->GetBody();
    b2Body* bB = contact->GetFixtureB()->GetBody();
    activeContacts.insert({bA, bB});
    activeContacts.insert({bB, bA});  // Both directions
    
    // Handle specific collision types
    ObjectCategory catA = bodyA->category;
    ObjectCategory catB = bodyB->category;
    
    // Character-Ground collision (for jump mechanics)
    if ((catA == ObjectCategory::CHARACTER && catB == ObjectCategory::BLOCK) ||
        (catA == ObjectCategory::BLOCK && catB == ObjectCategory::CHARACTER)) {
        
        auto character = (catA == ObjectCategory::CHARACTER) ? objA : objB;
        auto ground = (catA == ObjectCategory::BLOCK) ? objA : objB;
        
        // Check if character is landing on top of ground
        b2WorldManifold worldManifold;
        contact->GetWorldManifold(&worldManifold);
        
        Vector2 normal = {worldManifold.normal.x, worldManifold.normal.y};
        if (catB == ObjectCategory::CHARACTER) {
            normal.x = -normal.x;
            normal.y = -normal.y;
        }
        
        // If normal points upward (character landing on ground)
        if (normal.y < -0.7f) {
            handleCharacterGroundContact(character, ground, true);
        }
    }
    
    // Character-Enemy collision
    else if ((catA == ObjectCategory::CHARACTER && catB == ObjectCategory::ENEMY) ||
             (catA == ObjectCategory::ENEMY && catB == ObjectCategory::CHARACTER)) {
        
        auto character = (catA == ObjectCategory::CHARACTER) ? objA : objB;
        auto enemy = (catA == ObjectCategory::ENEMY) ? objA : objB;
        handleCharacterEnemyCollision(character, enemy, true);
    }
    
    // Enemy-Enemy collision
    else if (catA == ObjectCategory::ENEMY && catB == ObjectCategory::ENEMY) {
        handleEnemyCollision(objA, objB, true);
    }
    
    // Character-Item collision
    else if ((catA == ObjectCategory::CHARACTER && catB == ObjectCategory::ITEM) ||
             (catA == ObjectCategory::ITEM && catB == ObjectCategory::CHARACTER)) {
        
        auto character = (catA == ObjectCategory::CHARACTER) ? objA : objB;
        auto item = (catA == ObjectCategory::ITEM) ? objA : objB;
        handleItemCollection(character, item);
    }
    
    // Enemy-Block collision (for AI bouncing)
    else if ((catA == ObjectCategory::ENEMY && catB == ObjectCategory::BLOCK) ||
             (catA == ObjectCategory::BLOCK && catB == ObjectCategory::ENEMY)) {
        
        auto enemy = (catA == ObjectCategory::ENEMY) ? objA : objB;
        auto block = (catA == ObjectCategory::BLOCK) ? objA : objB;
        
        // Get collision normal
        b2WorldManifold worldManifold;
        contact->GetWorldManifold(&worldManifold);
        
        Vector2 normal = {worldManifold.normal.x, worldManifold.normal.y};
        if (catB == ObjectCategory::ENEMY) {
            normal.x = -normal.x;
            normal.y = -normal.y;
        }
        
        // If it's a side collision (enemy hits wall), reverse direction
        if (std::abs(normal.x) > 0.7f) {
            if (auto enemyPtr = std::dynamic_pointer_cast<Enemy>(enemy)) {
                Vector2 vel = enemyPtr->getVelocity();
                vel.x = -vel.x;
                enemyPtr->setVelocity(vel);
            }
        }
    }
    
    // Call the objects' collision handlers
    std::vector<std::shared_ptr<Object>> candidatesA = {objB};
    std::vector<std::shared_ptr<Object>> candidatesB = {objA};
    
    objA->checkCollision(candidatesA);
    objB->checkCollision(candidatesB);
}

void GameContactListener::EndContact(b2Contact* contact) {
    b2Body* bA = contact->GetFixtureA()->GetBody();
    b2Body* bB = contact->GetFixtureB()->GetBody();
    
    // Remove from active contacts
    activeContacts.erase({bA, bB});
    activeContacts.erase({bB, bA});
    
    PhysicsBody* bodyA = getPhysicsBody(bA);
    PhysicsBody* bodyB = getPhysicsBody(bB);
    
    if (!bodyA || !bodyB) {
        return;
    }
    
    std::shared_ptr<Object> objA = bodyA->getGameObject();
    std::shared_ptr<Object> objB = bodyB->getGameObject();
    
    if (!objA || !objB) {
        return;
    }
    
    // Handle ground contact ending
    ObjectCategory catA = bodyA->category;
    ObjectCategory catB = bodyB->category;
    
    if ((catA == ObjectCategory::CHARACTER && catB == ObjectCategory::BLOCK) ||
        (catA == ObjectCategory::BLOCK && catB == ObjectCategory::CHARACTER)) {
        
        auto character = (catA == ObjectCategory::CHARACTER) ? objA : objB;
        auto ground = (catA == ObjectCategory::BLOCK) ? objA : objB;
        handleCharacterGroundContact(character, ground, false);
    }
}

void GameContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
    PhysicsBody* bodyA = getPhysicsBody(contact->GetFixtureA()->GetBody());
    PhysicsBody* bodyB = getPhysicsBody(contact->GetFixtureB()->GetBody());
    
    if (!bodyA || !bodyB) {
        return;
    }
    
    // Disable collision response for sensors
    if (bodyA->isSensor || bodyB->isSensor) {
        contact->SetEnabled(false);
        return;
    }
    
    ObjectCategory catA = bodyA->category;
    ObjectCategory catB = bodyB->category;
    
    // Custom collision responses
    
    // Items should not physically collide with characters (only trigger collection)
    if ((catA == ObjectCategory::CHARACTER && catB == ObjectCategory::ITEM) ||
        (catA == ObjectCategory::ITEM && catB == ObjectCategory::CHARACTER)) {
        contact->SetEnabled(false);
    }
    
    // Projectiles vs enemies should have special handling
    else if ((catA == ObjectCategory::PROJECTILE && catB == ObjectCategory::ENEMY) ||
             (catA == ObjectCategory::ENEMY && catB == ObjectCategory::PROJECTILE)) {
        
        // Let the collision happen but mark enemy for destruction
        auto enemy = (catA == ObjectCategory::ENEMY) ? bodyA->getGameObject() : bodyB->getGameObject();
        auto projectile = (catA == ObjectCategory::PROJECTILE) ? bodyA->getGameObject() : bodyB->getGameObject();
        
        if (enemy && projectile) {
            GameContext::getInstance().mark_for_deletion_Object(enemy);
            GameContext::getInstance().mark_for_deletion_Object(projectile);
        }
    }
}

void GameContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
    // Handle post-collision effects here if needed
    // For example, damage based on impact force, sound effects, etc.
    
    float maxImpulse = 0.0f;
    for (int i = 0; i < impulse->count; ++i) {
        maxImpulse = std::max(maxImpulse, impulse->normalImpulses[i]);
    }
    
    // If impact is strong enough, could trigger effects
    if (maxImpulse > 10.0f) {
        // Strong impact - could play sound, create particles, etc.
    }
}

bool GameContactListener::areObjectsInContact(std::shared_ptr<Object> obj1, std::shared_ptr<Object> obj2) const {
    // This would require maintaining a reverse lookup from game objects to bodies
    // For now, implement a simpler version
    return false; // TODO: Implement proper lookup
}

std::vector<std::shared_ptr<Object>> GameContactListener::getContactsFor(std::shared_ptr<Object> obj) const {
    std::vector<std::shared_ptr<Object>> contacts;
    // TODO: Implement proper lookup from game object to its contacts
    return contacts;
}

bool GameContactListener::isObjectOnGround(std::shared_ptr<Object> obj) const {
    auto it = groundContacts.find(obj);
    return it != groundContacts.end() && !it->second.empty();
}

void GameContactListener::clearGroundContacts(std::shared_ptr<Object> obj) {
    groundContacts.erase(obj);
}

PhysicsBody* GameContactListener::getPhysicsBody(b2Body* body) const {
    if (!body || !body->GetUserData().pointer) return nullptr;
    return reinterpret_cast<PhysicsBody*>(body->GetUserData().pointer);
}

std::shared_ptr<Object> GameContactListener::getGameObject(b2Body* body) const {
    PhysicsBody* physicsBody = getPhysicsBody(body);
    if (!physicsBody) return nullptr;
    return physicsBody->getGameObject();
}

bool GameContactListener::shouldProcessCollision(PhysicsBody* bodyA, PhysicsBody* bodyB) const {
    if (!bodyA || !bodyB) return false;
    
    std::shared_ptr<Object> objA = bodyA->getGameObject();
    std::shared_ptr<Object> objB = bodyB->getGameObject();
    
    if (!objA || !objB) return false;
    if (!objA->isActive() || !objB->isActive()) return false;
    
    // Check if these object types should collide
    auto targetsA = objA->getCollisionTargets();
    auto targetsB = objB->getCollisionTargets();
    
    bool shouldCollide = false;
    
    // Check if A wants to collide with B's category
    for (auto target : targetsA) {
        if (target == bodyB->category) {
            shouldCollide = true;
            break;
        }
    }
    
    // Check if B wants to collide with A's category
    if (!shouldCollide) {
        for (auto target : targetsB) {
            if (target == bodyA->category) {
                shouldCollide = true;
                break;
            }
        }
    }
    
    return shouldCollide;
}

void GameContactListener::handleCharacterGroundContact(std::shared_ptr<Object> character, std::shared_ptr<Object> ground, bool isBegin) {
    if (isBegin) {
        groundContacts[character].insert(ground);
        
        // Set character as on ground
        if (auto charPtr = std::dynamic_pointer_cast<Character>(character)) {
            charPtr->setOnGround(true);
        }
    } else {
        groundContacts[character].erase(ground);
        
        // If no more ground contacts, character is airborne
        if (groundContacts[character].empty()) {
            if (auto charPtr = std::dynamic_pointer_cast<Character>(character)) {
                charPtr->setOnGround(false);
            }
        }
    }
}

void GameContactListener::handleEnemyCollision(std::shared_ptr<Object> enemy1, std::shared_ptr<Object> enemy2, bool isBegin) {
    if (!isBegin) return;
    
    // Basic enemy collision - push apart
    Vector2 pos1 = enemy1->getPosition();
    Vector2 pos2 = enemy2->getPosition();
    
    Vector2 direction = {pos1.x - pos2.x, pos1.y - pos2.y};
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
        
        // Small push force
        float pushForce = 50.0f;
        
        if (auto enemy1Ptr = std::dynamic_pointer_cast<Enemy>(enemy1)) {
            Vector2 vel1 = enemy1Ptr->getVelocity();
            vel1.x += direction.x * pushForce;
            enemy1Ptr->setVelocity(vel1);
        }
        
        if (auto enemy2Ptr = std::dynamic_pointer_cast<Enemy>(enemy2)) {
            Vector2 vel2 = enemy2Ptr->getVelocity();
            vel2.x -= direction.x * pushForce;
            enemy2Ptr->setVelocity(vel2);
        }
    }
}

void GameContactListener::handleCharacterEnemyCollision(std::shared_ptr<Object> character, std::shared_ptr<Object> enemy, bool isBegin) {
    if (!isBegin) return;
    
    auto charPtr = std::dynamic_pointer_cast<Character>(character);
    auto enemyPtr = std::dynamic_pointer_cast<Enemy>(enemy);
    
    if (!charPtr || !enemyPtr) return;
    
    // Determine collision type based on relative positions
    Vector2 charPos = charPtr->getPosition();
    Vector2 enemyPos = enemyPtr->getPosition();
    
    Rectangle charHitbox = charPtr->getHitBox();
    Rectangle enemyHitbox = enemyPtr->getHitBox();
    
    // Check if character is above enemy (stomp)
    float charBottom = charPos.y + charHitbox.height;
    float enemyTop = enemyPos.y;
    
    if (charBottom <= enemyTop + 10.0f && charPtr->getVelocity().y > 0) {
        // Character stomps on enemy
        GameContext::getInstance().mark_for_deletion_Object(enemy);
        
        // Give character a little bounce
        Vector2 vel = charPtr->getVelocity();
        vel.y = -200.0f; // Small bounce
        charPtr->setVelocity(vel);
    } else {
        // Character takes damage
        charPtr->takeDamage(1);
        
        // Knockback
        Vector2 direction = {charPos.x - enemyPos.x, 0};
        if (direction.x != 0) {
            direction.x = (direction.x > 0) ? 1.0f : -1.0f;
            Vector2 vel = charPtr->getVelocity();
            vel.x = direction.x * 300.0f; // Knockback force
            vel.y = -150.0f; // Small upward force
            charPtr->setVelocity(vel);
        }
    }
}

void GameContactListener::handleItemCollection(std::shared_ptr<Object> character, std::shared_ptr<Object> item) {
    // Mark item for deletion (collected)
    GameContext::getInstance().mark_for_deletion_Object(item);
    
    // Add score, power-up effects, etc. based on item type
    // This could be handled by the item itself or the character
}
