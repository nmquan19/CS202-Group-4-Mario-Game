#include "..\..\include\Enemy\Enemy.h"
#include <raymath.h>
#include <vector>
#include "../../include/Objects/ObjectFactory.h"
#include "../../include/System/Interface.h"
#include <raylib.h>
#include "../../include/Enemy/EnemyState.h"
#include <algorithm>
#include "../../include/System/Grid.h"
#include "../../include/System/Box2DWorldManager.h"
#include <utility>
#include "../../include/Enemy/EnemyAI/EnemyNavigator.h"

Enemy::Enemy(Vector2 startPos, Vector2 velocity, Vector2 accelleration,Texture2D texture) : position(startPos), active(true), velocity(velocity), accelleration(accelleration), texture(texture), aniTimer(0), aniSpeed(0.2f), collided(false) {
    isalive = true;
    hitbox = { position.x, position.y,  size.x * GridSystem::GRID_SIZE,
        size.y * GridSystem::GRID_SIZE};
    currentState = nullptr; 
    physicsBody = Box2DWorldManager::getInstance().createEnemyBody(position, { hitbox.width, hitbox.height });
    if (physicsBody) {
        physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
    }
}

Enemy::Enemy(Vector2 startPos,  Texture2D texture, Vector2 size) : position(startPos), active(true), velocity({0,0}), accelleration({0,0}), texture(texture), aniTimer(0), aniSpeed(0.2f), collided(false) {
    this->size = size; 
	isalive = true;
    this->spritebox = { 0, 0, 32, 32}; 
    hitbox = {position.x, position.y,  size.x * GridSystem::GRID_SIZE,
        size.y * GridSystem::GRID_SIZE };
    currentState = nullptr;
    physicsBody = Box2DWorldManager::getInstance().createEnemyBody(position, { hitbox.width, hitbox.height });
    if (physicsBody) {
        physicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
        for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            b2Filter filter = fixture->GetFilterData();
            filter.maskBits = static_cast<uint16>(ObjectCategory::ENEMY);
            filter.categoryBits = static_cast<uint16> (ObjectCategory::CHARACTER) | static_cast<uint16>(ObjectCategory::BLOCK) |
                static_cast<uint16>(ObjectCategory::PROJECTILE) | static_cast<uint16>(ObjectCategory::INTERACTIVE);
            fixture->SetFilterData(filter);
        }
    }
}

Enemy::~Enemy() {
    if (physicsBody) {
        Box2DWorldManager::getInstance().destroyBody(physicsBody);
        physicsBody = nullptr;
    }
}

std::vector<Rectangle> Enemy::getHitBox() const {
    return {hitbox};
}
std::vector<ObjectCategory> Enemy::getCollisionTargets() const 
{
	return { ObjectCategory::CHARACTER, ObjectCategory::BLOCK, ObjectCategory::PROJECTILE};
}

void Enemy::update(float deltaTime)
{
    aniTimer += deltaTime;
    if (aniTimer >= aniSpeed) {
        curFrame += 1;
        aniTimer = 0;
    }
    
    if (physicsBody) {
        b2Vec2 b2Pos = physicsBody->GetPosition();
        Vector2 bodyPos = Box2DWorldManager::b2ToRaylib(b2Pos);
        position.x = bodyPos.x - hitbox.width * 0.5f;
        position.y = bodyPos.y - hitbox.height * 0.5f;
        hitbox.x = position.x;
        hitbox.y = position.y;
    } 
}

void Enemy::draw() {
    Rectangle sourceRec = this->spritebox;      
    Vector2 origin = { 0, 0 };
    DrawTexturePro(this->texture, sourceRec, hitbox, origin, 0.0f, WHITE);
}

ObjectCategory Enemy::getObjectCategory() const {
	return ObjectCategory::ENEMY;
}
void Enemy::changeState(EnemyState* other)
{      
    if (currentState == other) {
        return;
    }

    if (currentState) {
        currentState->exit(this);
    }

    currentState =  other; 

    if (other) {
        currentState->enter(this);
    }
}

void Enemy::flipDirection()
{
    isFacingRight ^= 1; 
}

bool Enemy::isAlive() const {
    return isalive;
}

bool Enemy::isActive() const {
	return active;
}

void Enemy::setVelocity(Vector2 newVelocity) {
    if (physicsBody) {
        physicsBody->SetLinearVelocity(Box2DWorldManager::raylibToB2(newVelocity));
    }
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
    if (physicsBody) {
        physicsBody->SetTransform(Box2DWorldManager::raylibToB2(newPosition), physicsBody->GetAngle());
    }
}

Vector2 Enemy::getDirection() const {
    return direction;
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

float Enemy::getWidth() const {
    return spritebox.width * scale;
}

float Enemy::getHeight() const {
    return spritebox.height * scale;
}

float Enemy::getCenterX() const {
    return position.x + (spritebox.width * scale) / 2;
}
bool Enemy::FacingRight() const
{
    return isFacingRight;
}
float Enemy::getBottom() const {
    return position.y + hitbox.height;
}
float Enemy::getCenterY() const {
    return position.y + (spritebox.height * scale) / 2;
}

Vector2 Enemy::getCenter() const {
    return Vector2{ getCenterX(), getCenterY() };
}

std::vector<std::pair<int, int>> Enemy::getSpriteData() {
   switch(getType()) {
        case EnemyType::GOOMBA:
            return { {0,1},{2,3},{5,5} }; 
        case EnemyType::GREEN_KOOPA:
            return  {{45,46}};
        case EnemyType::RED_KOOPA:
			return { {53,54} };
        default:
            return {};
   }
}


bool Enemy::isPlayerAtHigherGround() const {
  
    float selfBottomY = position.y + hitbox.height;
    float targetBottomY = targetPosition.y + targetHitboxes.height +5.0f;

    return selfBottomY > targetBottomY;
}
Vector2 Enemy::getSize() const {
    switch(getType()) {
        case EnemyType::GOOMBA:
            return Constants::Goomba::standardSize;
        case EnemyType::GREEN_KOOPA:
            return  Constants::GreenKoopa::standardSize;
        case EnemyType::RED_KOOPA:
			return Constants::RedKoopa::standardSize;
        default:
            return {1, 1};
    }
}
bool Enemy::moveToTarget() {
    auto& nav = NavGraph::getInstance();
    Vector2 centerPos = {
        this->getPosition().x + this->getHitBox()[0].width / 2,
        this->getPosition().y + this->getHitBox()[0].height / 2
    };
    std::shared_ptr<NavGraphNode> curNode = nav.getNearestNode(centerPos, NodeType::Ground);
    std::shared_ptr<NavGraphNode> targetNode = nav.getNearestNode(targetPosition, NodeType::Ground);
    if (!curNode || !targetNode)
        return false;
  
    DrawText(TextFormat("Current Pos: %.2f, %.2f", getPosition().x, getPosition().y), 200, 350, 20, WHITE);
    if (!currentPath.empty() && pathIndex < currentPath.size()) {
        DrawText(TextFormat("Target Node: %.2f, %.2f", currentPath[pathIndex]->getPosition().x, currentPath[pathIndex]->getPosition().y), 200, 370, 20, GREEN);
    }

    bool containsCurrentNode = false;
    for (const auto& node : currentPath) {
        if (Vector2Distance(node->getPosition(), curNode->getPosition()) < 1.0f) {
            containsCurrentNode = true;
            break;
        }
    }
    bool shouldReplan = (!lastTargetNode||targetNode != lastTargetNode || !containsCurrentNode);
    if (true) {
        currentPath = nav.getShortestPath(this, targetPosition, NodeType::Ground);
        pathIndex = 0;
        lastTargetNode = targetNode;
        if (currentPath.empty()) return false; 
        //isTraversing = false;
    }
    DrawText(TextFormat("IsTraversing: %d", isTraversing), 300, 300, 20, RED);
    currentNode = curNode;
    if (!isTraversing &&  !currentPath.empty() && pathIndex < currentPath.size() - 1) {
        std::shared_ptr<NavGraphNode> nextNode = currentPath[pathIndex + 1];
        if (Vector2Distance(nextNode->getPosition(), centerPos) < 0.5f /*std::max(1.f, getWalkVelocity() * GetFrameTime())*/) {
            ++pathIndex;
            isTraversing = false;
        }
        else {
            const Edge* edge = nullptr;
            for (const auto& e : curNode->getNeighbors()) {
                if (e.toNode == nextNode) {
                    edge = &e;
                    break;
                }
            }
            if (edge) {
                executeTraversal(*edge);
                onTraversalFinished();
            }
        }
    }
}

void Enemy::executeTraversal(const Edge& edge) {
    Vector2 fromPos = {
           this->getPosition().x + this->getHitBox()[0].width / 2,
           this->getPosition().y + this->getHitBox()[0].height / 2
    };
    Vector2 toPos = edge.toNode->getPosition();
    switch (edge.direction) {
    case EdgeDirection::Horizontal:
    {
       
        if(curAniName != "Run" || animController.isFinished()) setAnimation("Run");
        Vector2 dir = Vector2Normalize(Vector2Subtract(toPos, fromPos));
        this->direction = dir;
        float moveSpeed = getWalkVelocity() / 2;
        float distanceToTarget = Vector2Distance(fromPos, toPos);
        float moveStep = std::min(moveSpeed * GetFrameTime(), distanceToTarget);
         velocity.x = moveStep * dir.x / GetFrameTime();
 
        break;
    }
    case EdgeDirection::Jumping:
    {
        DrawText("Jumping", 200, 400, 30, BLUE);
        jumpTo(toPos, true);
        break;
    }
    case EdgeDirection::Falling:
    {
        DrawText("Falling", 200, 400, 30, BLUE);
        break;
    }
    default:
        break;
    }
}

void Enemy::onTraversalFinished() {
    pathIndex++;
    if(pathIndex<currentPath.size()) currentNode = currentPath[pathIndex];
    //setPosition(currentNode->getPosition()); k
    isTraversing = false;
}
float Enemy::jumpTo(Vector2 targetPos, bool apply) {
    float gravity = 980.0f;
    float maxWalkSpeed = getWalkVelocity();
    float maxJumpSpeed = std::fabs(getJumpVelocity());
    Vector2 startPos = {
        this->getPosition().x + this->getHitBox()[0].width / 2,
        this->getPosition().y + this->getHitBox()[0].height / 2
    };
    float dx = targetPos.x - startPos.x;
    float dy = targetPos.y - startPos.y;
    float minT = 0.01f; 
    float maxT = 2.0f * maxJumpSpeed / gravity; 
    float bestT = _FMAX;
    float bestVx = 0.0f, bestVy = 0.0f;
    bool found = false;
    float step = 0.01f;
    int count = 0; 
    for (float t = minT; t <= maxT; t += step) {
        float vy = (dy + 0.5f * gravity * t * t) / t;
        float vx = dx / t;
        if (std::fabs(vx) <= maxWalkSpeed && std::fabs(vy) <= maxJumpSpeed) {
            if (count == 0)
            {
                count++;
                continue;
            }
            bestT = t;
            bestVx = vx;
            bestVy = vy;
            found = true;
            break; 
        }
    }

    if (found && apply) {
        this->velocity = { bestVx, bestVy };
    }
    
    return found ? bestT : _FMAX;
}