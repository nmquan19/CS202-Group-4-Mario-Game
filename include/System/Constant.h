#pragma once// Constants.h
#include <raylib.h>
namespace Constants {
    constexpr float GRAVITY = 980.0f;
    constexpr int TILE_SIZE = 128;
    constexpr float KOOPA_SPEED = 200.0f;

    namespace GreenKoopa {
		constexpr Vector2 standardSize = { 1, 1.5};
        constexpr float WANDERING_SPEED = 100.0f;
        constexpr float STOMPED_DURATION = 0.3f;
        constexpr float KNOCK_VELOCITY = -300.0f;
    }
    namespace RedKoopa {
        constexpr Vector2 standardSize = {1, 1.5};
        constexpr float WANDERING_SPEED = 150.0f;
        constexpr float STOMPED_DURATION = 0.3f;
        constexpr float KNOCK_VELOCITY = -300.0f;
    }
    namespace Goomba {
        constexpr Vector2 standardSize = { 1, 1 };
        constexpr float WANDERING_SPEED = 100.0f;
        constexpr float STOMPED_DURATION = 0.3f;
        constexpr float KNOCK_VELOCITY = -300.0f;
	}
    namespace KoopaShell {
        constexpr Vector2 standardSize = { 1, 1 };
        constexpr float MOVING_SPEED = 500.0f;
        constexpr float IDLE_DURATION = 4.5f;
        constexpr float REVIVE_DURATION = 1.5f;
	}
    namespace DryBowser{
        constexpr float WalkTurnDuration = 0.5f;  
		constexpr float JUMP_VELOCITY = -600.0f;
        constexpr float WallJumpFriction = 0.95f;
        constexpr float RUN_SPEED = 500.0f; 
		constexpr float BASIC_ATTACK_DURATION = 2.0f;
        constexpr float BASIC_ATTACK_RANGE = 100.0f; 
    } 
}
