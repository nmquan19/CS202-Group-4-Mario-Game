#pragma once
#include "../Enemy.h"
#include "../Boss/BehaviorTree.h"
//class HammerBro: public Enemy {
//private:
//    int HP;
//    bool alive = true;
//    std::shared_ptr<BehaviorTreeNode> root; 
//public:
//    HammerBro(Vector2 spawnPosition, Vector2 size = { 2,2 });
//
//    // Game loop
//    void update(float dt) override;
//    void draw() override;
//
//    // Collision & physics
//    void handleCharacterCollision(std::shared_ptr<Object> other) override;
//    void handleEnvironmentCollision(std::shared_ptr<Object> other) override;
//    void checkCollision(const std::vector<std::shared_ptr<Object>>& candidates) override;
//    void onCollision(std::shared_ptr<Object> other) override;
//    std::vector<Rectangle> getHitBox() const override; // Override to provide multiple hitboxes
//    std::string getCurAnimation() const { return curAniName; }
//    // Logic
//    void takeDamage(int amount) override;
//    void die() override;
//    bool isAlive() const override;
//
//    // Planning / AI interface
//    void changePhase(std::unique_ptr<BossPhaseState> newPhase) override;
//    const WorldState& getWorldState() const override;
//    std::unique_ptr<BaseSimState> getCurrentSimState() const;
//    // Typing
//    ObjectType getObjectType() const override;
//    EnemyType getType() const override;
//    void setAnimation(const std::string& animationName) override;
//
//    //
//
//    ///Behavior Tree implementation
//    void walkToTarget();
//    void attack() override;
//    bool isAttacking();
//    bool isNearTarget() const;
//    void idle() override;
//    bool isTurning() const override;
//    void walkTurn() override;
//    bool canUseSpin() const;
//    bool isTargetInRange(BowserAttackType type) const;
//    void spinAttack();
//    bool isInSpinAttack();
//    void spinAttackWindup();
//    void spinAttackWinddown();
//private:
//    void updateWorldState();
//};
