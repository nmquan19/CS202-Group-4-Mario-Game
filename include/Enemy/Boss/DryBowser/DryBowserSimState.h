#pragma once
#include <raylib.h>
#include <string>
#include <memory>
#include "../SimulateState.h"
class DryBowserSimState : public BaseSimState {
public:
    Vector2 bossPos;
    Vector2 bossVelocity;
    Vector2 playerPos;
    Vector2 playerVelocity;
    bool isOnGround = true;
    bool isInAir = false;
    bool isNearWall = false;


    int bossHP = 100;
    int playerHP = 1;
    
	bool IsPlayerInSpinAttackRange = false;  
	bool isPlayerInMeleeRange = false;   
	bool isPlayerInFireBreathRange = false;
    bool isPlayerAtHigherGround = false; 

    bool playerInRange = false;
    bool canAttack = true;
    bool isVulnerable = true;

    float timeElapsed = 0.0f;
    float moveCooldown = 0.0f;
    float animationTimer = 0.0f;

    std::string currentMove = "none";
    float moveDuration = 0.0f;
    float moveRange = 0.0f;

    std::unique_ptr<BaseSimState> clone() const override {
        return std::make_unique<DryBowserSimState>(*this);
    }
};

