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
std::string DryBowserStandingState::getName() const {
    return "Idle";
}
void DryBowserRunningState::enter(Boss* boss) {
    boss->setAnimation("Run");
}

void DryBowserRunningState::update(Boss* boss, float dt) {

}

void DryBowserRunningState::exit(Boss* boss) {
    boss->setVelocity({ 0, 0 });
}

bool DryBowserRunningState::isFinished() const {
    return false;
}
std::string DryBowserRunningState::getName() const {
    return "Run";
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
	Vector2 d = boss->getDirection();
	boss->setDirection({ -d.x, d.y });
}

bool DryBowserWalkTurnState::isFinished() const {
    return timer >= turnDuration;
}
std::string DryBowserWalkTurnState::getName() const
{
    return "WalkTurn"; 
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
    boss->flipDirection();
    boss->setVelocity({ -boss->getVelocity().x, boss->getVelocity().y });
}

bool DryBowserJumpingState::isFinished() const {
    return false; 
}

// ----------------------------
// BowserAttackState
// ----------------------------
void DryBowserMeleeAttack1State::enter(Boss* boss) {

    boss->setVelocity({ 0,0 }); 
    boss->setAnimation("MeleeAttack1");
    timer = 0.0f;
}

void DryBowserMeleeAttack1State::update(Boss* boss, float dt) {
    timer += dt;
}

void DryBowserMeleeAttack1State::exit(Boss* boss) {
	boss->setCooldown("MeleeAttack1", Constants::DryBowser::BASIC_ATTACK_COOLDOWN);
}

bool DryBowserMeleeAttack1State::isFinished() const {
    return timer >= attackDuration;
}

std::string DryBowserMeleeAttack1State::getName() const {
    return "MeleeAttack1";
}





void DryBowserSpinAttackState::enter(Boss* boss) {

    boss->setAnimation("SpinAttack");
    timer = 0.0f;
}

void DryBowserSpinAttackState::update(Boss* boss, float dt) {
    timer += dt;
}

void DryBowserSpinAttackState::exit(Boss* boss) {
    boss->setCooldown("SpinAttack", Constants::DryBowser::SPIN_ATTACK_COOLDOWN);
}

bool DryBowserSpinAttackState::isFinished() const {
    return timer >= Constants::DryBowser::SPIN_ATTACK_DURATION;
}

std::string DryBowserSpinAttackState::getName() const {
    return "SpinAttack";
}