#include "../../include/Enemy/LedgeDetector.h"
#include "raylib.h"
#include <vector>
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/System/Interface.h"
#include "../../include/Enemy/Enemy.h"
LedgeDetector::LedgeDetector(float castLength)
    : nearLedge(false), castLength(castLength), hitBox({ 0,0,0,0 }){

}

void LedgeDetector::update(Enemy* enemy, float deltaTime) 
{
    std::vector<Rectangle> hitboxes = enemy->getHitBox();
    if (hitboxes.empty()) return;
    
    Rectangle hitbox = hitboxes[0]; // Use the first hitbox
    
    Vector2 origin = { hitbox.x + enemy->FacingRight()*hitbox.width, hitbox.y + hitbox.height};
    hitBox = {
        origin.x+1.0f*(enemy->FacingRight()? 1 : -1),
        origin.y,               
        1.0f,                   
        castLength              
    };
    
}

bool LedgeDetector::isNearLedge() const
{
    return nearLedge;
}

std::vector<Rectangle> LedgeDetector::getHitBox() const
{
    return {hitBox}; 
}

ObjectCategory LedgeDetector::getObjectCategory() const
{
    return ObjectCategory::TRIGGER;
}

std::vector<ObjectCategory> LedgeDetector::getCollisionTargets() const
{
    return {ObjectCategory::BLOCK};
}

void LedgeDetector::onCollision(std::shared_ptr<Object> other, Direction direction)
{
    if (!other) {
        nearLedge = false;
        return;
    }
    else
    {
        nearLedge = true;
    }
}

LedgeDetector& LedgeDetector::getInstance()
{
    static LedgeDetector instance(10.0f); // Default values; change as needed
    return instance;
}

bool LedgeDetector::isActive() const
{
    return true; 
}
void LedgeDetector::setActive(bool active)
{}
void LedgeDetector::draw()
{}
void LedgeDetector::setPosition(Vector2 newpos)
{
}
bool LedgeDetector::isCollided() const  
{
    return collided;
}
void LedgeDetector::setCollided(bool collided)
{
    this->collided = collided;
}
Vector2 LedgeDetector::getPosition() const
{
    return { hitBox.x,hitBox.y }; 
}
LedgeDetector :: ~LedgeDetector()
{
    this->active = false;  
    // PhysicsManager::getInstance().markForDeletion(this);
}

ObjectType LedgeDetector::getObjectType() const {
    return TriggerType::CHECKPOINT;
}

Vector2 LedgeDetector::getSize() const {
    return size;
}