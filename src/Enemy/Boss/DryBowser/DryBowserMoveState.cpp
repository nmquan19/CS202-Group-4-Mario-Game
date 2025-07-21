#include "../../../../include/Enemy/Boss/DryBowser/DryBowserMoveState.h"
#include <string>
#include "../../../../include/Enemy/Boss/Boss.h"
#include "../../../../include/System/Constant.h"

// ----------------------------
// BowserStandingState
// ----------------------------
void DryBowserStandingState::enter(Boss* boss) {
    boss->setAnimation("Idle");
}

void DryBowserStandingState::update(Boss* boss, float dt) {

}

void DryBowserStandingState::exit(Boss* boss) {
    // Clean up if needed
}

bool DryBowserStandingState::isFinished() const {
    return false;
}

void DryBowserRunningState::enter(Boss* boss) {
    float speed = boss->FacingRight() ? Constants::DryBowser::RUN_SPEED: -Constants::DryBowser::RUN_SPEED;
    boss->setAnimation("Run");
}

void DryBowserRunningState::update(Boss* boss, float dt) {

}

void DryBowserRunningState::exit(Boss* boss) {
}

bool DryBowserRunningState::isFinished() const {
    return false;
}

// ----------------------------
// BowserWallJumpMoveState
// ----------------------------
void DryBowserWallJumpMoveState::enter(Boss* boss) {
    boss->setAnimation("WallJump");
    timer = 0.0f;
}

void DryBowserWallJumpMoveState::update(Boss* boss, float dt) {
    timer += dt;
    auto v = boss->getVelocity();
    v.y  *= 1+friction;
    boss->setVelocity(v);
}

void DryBowserWallJumpMoveState::exit(Boss* boss) {
}

bool DryBowserWallJumpMoveState::isFinished() const {
    return timer >= 0.6f;
}

std::string DryBowserWallJumpMoveState::getName() const {
    return "WallJump";
}

bool DryBowserWallJumpMoveState::canBeInterrupted() {
    return false;
}

// ----------------------------
// BowserWalkTurnState
// ----------------------------
void DryBowserWalkTurnState::enter(Boss* boss) {
    timer = 0.0f;
    boss->setAnimation("WalkTurn");
}

void DryBowserWalkTurnState::update(Boss* boss, float dt) {
    timer += dt;
}

void DryBowserWalkTurnState::exit(Boss* boss) {
    boss->flipDirection();
}

bool DryBowserWalkTurnState::isFinished() const {
    return timer >= turnDuration;
}

// ----------------------------
// BowserJumpingState
// ----------------------------
void DryBowserJumpingState::enter(Boss* boss) {
    boss->setAnimation("Jump");
}

void DryBowserJumpingState::update(Boss* boss, float dt) {
}

void DryBowserJumpingState::exit(Boss* boss) {
    // none
}

bool DryBowserJumpingState::isFinished() const {
    return false; 
}

// ----------------------------
// BowserAttackState
// ----------------------------
void DryBowserMeleeAttack1State::enter(Boss* boss) {

    boss->setAnimation("MeleeAttack1");
    timer = 0.0f;
}

void DryBowserMeleeAttack1State::update(Boss* boss, float dt) {
    timer += dt;
}

void DryBowserMeleeAttack1State::exit(Boss* boss) {
}

bool DryBowserMeleeAttack1State::isFinished() const {
    return timer >= attackDuration;
}
