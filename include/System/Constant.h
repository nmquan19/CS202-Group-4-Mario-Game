#pragma once// Constants.h
#include <raylib.h>
#include <vector>

struct CharacterStats {
    float baseSpeed;
    float jumpForce;
};

namespace Constants {
    constexpr float GRAVITY = 980.0f;
    constexpr int TILE_SIZE = 64;
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
        constexpr Vector2 standardSize = { 0.75, 0.75 };
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
    namespace Character {
        constexpr float BOUNCE_AFTER_STRIKE_VELOCITY = -300.0f;
        constexpr CharacterStats MARIO_STATS = {220.0f, 550.0f};
        constexpr CharacterStats LUIGI_STATS = {175.0f, 730.0f};
        const std::vector<std::vector<Rectangle>> MARIO_FRAME_DATA = {
            {{11, 23, 12, 15}}, // idle state
            {{118, 23, 12, 15}, {149, 22, 15, 16}}, // moving state
            {{293, 22, 16, 16}, {326, 22, 16, 16}} // jumping state
        };
        const std::vector<std::vector<Rectangle>> LUIGI_FRAME_DATA = {
            {{11, 893, 12, 15}}, // idle state
            {{118, 893, 12, 15}, {149, 892, 15, 16}}, // moving state
            {{293, 892, 16, 16}, {326, 892, 16, 16}} // jumping state
        };
    }
    namespace DryBowser{
        constexpr float WalkTurnDuration = 0.5f;  
		constexpr float JUMP_VELOCITY = -600.0f;
        constexpr float WallJumpFriction = 0.95f;
        constexpr float RUN_SPEED = 500.0f; 
		constexpr float BASIC_ATTACK_DURATION = 2.0f;
        constexpr float BASIC_ATTACK_RANGE = 100.0f; 
    } 
    namespace Spring {
        constexpr float BOUNCE_VELOCITY = -800.0f;
    }
}
