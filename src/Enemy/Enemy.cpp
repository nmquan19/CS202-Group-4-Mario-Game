#include "..\..\include\Enemy\Enemy.h"
#include "..\..\include\System\PhysicsManager.h"
#include <cstdlib>
#include <math.h>
#include <raymath.h>
#include <iostream>
Enemy::Enemy(Vector2 startPos, Vector2 velocity, Vector2 accelleration,Texture2D texture) : position(startPos), active(true), velocity(velocity), accelleration(accelleration), texture(texture) {
    hitbox = {0,0,0,0};
    num_sprites ={};
    currentState = nullptr;
    PhysicsManager::getInstance().addObject(this);

}

Enemy::~Enemy() {
    PhysicsManager::getInstance().markForDeletion(this);
}

Rectangle Enemy::getHitBox() const {
    return hitbox;
}
std::vector<ObjectCategory> Enemy::getCollisionTargets() const 
{
	return { ObjectCategory::CHARACTER, ObjectCategory::ENEMY, ObjectCategory::BLOCK };
}



void Enemy::draw() {
	 float  scale =5.0f;
    Rectangle sourceRec = this->spritebox;      
//    if(direction == 1)  
//     {
//      DrawText("FLIPPED", 60, 120, 20, RED);
//      sourceRec.x = sourceRec.x ;  
//      sourceRec.width = -sourceRec.width;           
//     }
    Rectangle destRec = {
    position.x,
    position.y,
    abs(sourceRec.width) * scale,  
    sourceRec.height * scale
    };
Vector2 origin = { 0, 0 };


 DrawText(TextFormat("spritebox_width: x =%f", spritebox.width), 100, 600, 20, RED);
  DrawText(TextFormat("position: x=%.1f y=%.1f", position.x, position.y), 100, 620, 20, RED); 
  DrawText(TextFormat("velocity: x=%.1f y=%.1f", velocity.x, velocity.y), 100, 640, 20, RED); 
  DrawText(TextFormat("hitbox: x=%.1f y=%.1f", hitbox.width, hitbox.height), 100, 660, 20, RED); 

 DrawText(TextFormat("curFrame: x =%d", curFrame), 100, 580, 20, RED);
 DrawText(TextFormat("Rec: x=%.1f witdth=%.1f", sourceRec.x, sourceRec.width), 100, 540, 20, RED);
 DrawText(TextFormat("destRec: x=%.1f width=%.1f", destRec.x, destRec.width), 100, 560, 20, RED); 
DrawTexturePro(this->texture, sourceRec, destRec, origin, 0.0f, WHITE);
}

ObjectCategory Enemy::getObjectCategory() const {
	return ObjectCategory::ENEMY;
}
void Enemy::changeState(EnemyState* other)
{      
     if(currentState) currentState->exit(this);
      currentState =  other; 
      std::cout<<"\n Entering"<<"A"<<"\n";
      currentState->enter(this);
}
void Enemy::onCollision(Object* other) {
	if (other->getObjectCategory() == ObjectCategory::CHARACTER) {
            // velocity = {0,0} ;  
            // accelleration = {0,0};
    }
    if (other->getObjectCategory() == ObjectCategory::ENEMY) {
        // Push enemy away from Mario slightly to prevent sticking
        Rectangle otherHitBox = other->getHitBox();
        Vector2 pushDirection = {
            position.x - otherHitBox.x,
            position.y - otherHitBox.y
        };

        // Normalize and apply small push
        float length = pushDirection.x * pushDirection.x + pushDirection.y * pushDirection.y;
        if (length > 0) {
            pushDirection.x /= length;
            pushDirection.y /= length;
            position.x += pushDirection.x * 500.0f; // Small push away
            position.y += pushDirection.y * 500.0f;
        }
    }
}
void Enemy::checkCollision(const std::vector<Object*>& candidates) {
    for (auto* candidate : candidates) {
        switch (candidate->getObjectCategory()) {
        case ObjectCategory::ENEMY:
            //handleEnemyCollision(candidate);
            break;
        case ObjectCategory::BLOCK:
            handleEnvironmentCollision(candidate);
            break;
        case ObjectCategory::PROJECTILE:
            // implement
            break;
        case ObjectCategory::ITEM:
            // implement
            break;
        }
    }
}

void Enemy::handleEnvironmentCollision(Object* other) {
    Rectangle playerHitBox = getHitBox();
    Rectangle otherHitBox = other->getHitBox();

    // Calculate overlap amounts for each direction
    float overlapLeft = (playerHitBox.x + playerHitBox.width) - otherHitBox.x;
    float overlapRight = (otherHitBox.x + otherHitBox.width) - playerHitBox.x;
    float overlapTop = (playerHitBox.y + playerHitBox.height) - otherHitBox.y;
    float overlapBottom = (otherHitBox.y + otherHitBox.height) - playerHitBox.y;

    if (overlapTop < overlapBottom && overlapTop < overlapLeft && overlapTop < overlapRight) {
        // Collision from top - player is on top of block
        position.y = otherHitBox.y - (spritebox.height * scale);  // ? Use sprite dimensions, not hitbox
        isGrounded = true; 
    }
    else if (overlapBottom < overlapTop && overlapBottom < overlapLeft && overlapBottom < overlapRight) {
        // Collision from bottom
        position.y = otherHitBox.y + otherHitBox.height;
        if (velocity.y < 0) {
            velocity.y = 0;
        }
    }
    else if (overlapLeft < overlapRight) {
        // Collision from left side
        position.x = otherHitBox.x - (spritebox.width * scale);  // ? Use sprite dimensions
        velocity.x = 0;
    }
    else {
        // Collision from right side
        position.x = otherHitBox.x + otherHitBox.width;
        velocity.x = 0;
    }
}
bool Enemy::isActive() const {
	return active;
}
void Enemy::setVelocity(Vector2 newVelocity) {
    velocity = newVelocity;
}

Vector2 Enemy::getVelocity() {
    return velocity;
}

void Enemy::setSpeed(float newSpeed) {
    speed = newSpeed;
}

float Enemy::getSpeed() {
    return speed;
}

void Enemy::setPosition(Vector2 newPosition) {
    position = newPosition;
}

Vector2 Enemy::getPosition() const {
    return position;
}


void Enemy::setActive(bool flag) {
    active = flag;
}

bool Enemy::isCollided() const {
    return collided;
}

void Enemy::setCollided(bool flag) {
    collided = flag;
}
