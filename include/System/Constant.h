#pragma once // Constants.h
#include "Interface.h"
#include <raylib.h>
#include <vector>
#include <box2d/box2d.h>

struct CharacterStats {
    float baseSpeed;
    float jumpVel;
};

namespace Constants {
    constexpr float GRAVITY = 980.0f;
    constexpr int TILE_SIZE = 64;
    constexpr float KOOPA_SPEED = 200.0f;
    constexpr float WORLDBOUNDS_WIDTH = 50000.0f;
    constexpr float WORLDBOUNDS_HEIGHT = 50000.0f;
    constexpr float COYOTE_TIME_DURATION = 150.0f;

    namespace GreenKoopa {
		constexpr Vector2 standardSize = { 0.75, 1};
        constexpr float WANDERING_SPEED = 100.0f;
        constexpr float STOMPED_DURATION = 0.3f;
        constexpr float KNOCK_VELOCITY = -300.0f;
    }
    namespace RedKoopa {
        constexpr Vector2 standardSize = {0.75, 1};
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
        constexpr Vector2 standardSize = { 0.75, 0.75 };
        constexpr float MOVING_SPEED = 500.0f;
        constexpr float IDLE_DURATION = 4.5f;
        constexpr float REVIVE_DURATION = 1.5f;
        constexpr float MOVING_DURATION = 7.5f;
	}
    namespace Character {
        constexpr Vector2 SMALL_STANDARD_SIZE = { 0.75f, 1.0f };
        constexpr Vector2 SUPER_STANDARD_SIZE = { 1.0f, 1.5f };
        constexpr float BOUNCE_AFTER_STRIKE_VELOCITY = 7.0f;
        constexpr float INVINCIBLE_TIME_AFTER_STRIKE = 0.2f;
        constexpr float INVINCIBLE_TIME_AFTER_TRANSFORM = 1.0f;
        constexpr float STUNNED_STATE_DURATION = 1.0f;
        constexpr float REVIVE_TIME = 3.0f;
        constexpr float TRANSFORM_DURATION = 0.8f;
        constexpr CharacterStats MARIO_STATS = { 3.5f, 10.0f };
        constexpr CharacterStats LUIGI_STATS = { 2.5f, 12.0f };
        const std::vector<std::vector<Rectangle>> SMALL_MARIO_FRAME_DATA = {
            {{11, 23, 12, 15}}, // idle state
            {{118, 23, 12, 15}, {149, 22, 15, 16}, {186, 22, 16, 16}, {219, 22, 16, 16}}, // moving state
            {{293, 22, 16, 16}, {326, 22, 16, 16}}, // jumping state
            {{11, 23, 12, 15}, {49, 26, 11, 12}}, // stunned state
            {{713, 22, 16, 16}}, // knocked state
            {{11, 23, 12, 15}, {149, 22, 15, 16}, {293, 22, 16, 16}, {360, 106, 16, 26}, {11, 106, 14, 27}}, // super transform
            {{11, 23, 12, 15}, {149, 22, 15, 16}, {293, 22, 16, 16}, {360, 262, 16, 26}, {11, 262, 14, 27}} // fire transform
        };
        const std::vector<std::vector<Rectangle>> SUPER_MARIO_FRAME_DATA = {
            {{11, 106, 14, 27}}, // idle state
            {{116, 106, 16, 27}, {149, 106, 16, 26}, {184, 106, 14, 27}}, // moving state
            {{360, 106, 16, 26}, {393, 106, 16, 26}}, // jumping state
            {{11, 106, 14, 27}, {48, 115, 14, 18}}, // stunned state
            {{713, 22, 16, 16}}, // none
            {{11, 106, 14, 27}, {360, 106, 16, 26}, {293, 22, 16, 16}, {149, 22, 15, 16}, {11, 23, 12, 15}}, // small transform
            {{11, 23, 12, 15}, {149, 22, 15, 16}, {293, 22, 16, 16}, {360, 262, 16, 26}, {11, 262, 14, 27}} // fire transform
        };
        const std::vector<std::vector<Rectangle>> FIRE_MARIO_FRAME_DATA = {
            {{11, 262, 14, 27}}, // idle state
            {{116, 262, 16, 27}, {149, 262, 16, 26}, {184, 262, 14, 27}}, // moving state
            {{360, 262, 16, 26}, {393, 262, 16, 26}}, // jumping state
            {{11, 262, 14, 27}, {48, 271, 14, 18}}, // stunned state 
            {}, // none
            {{11, 262, 14, 27}, {360, 262, 16, 26}, {293, 22, 16, 16}, {149, 22, 15, 16}, {11, 23, 12, 15}}, // small transform
            {{11, 317, 14, 27}, {44, 317, 14, 27}} // attack state
        };
        const std::vector<std::vector<Rectangle>> LUIGI_FRAME_DATA = {
            {{11, 893, 12, 15}}, // idle state
            {{118, 893, 12, 15}, {149, 892, 15, 16}}, // moving state
            {{293, 892, 16, 16}, {326, 892, 16, 16}} // jumping state
        };
    }
    namespace Projectile {
        constexpr Vector2 STANDARD_SIZE = { 0.5f, 0.5f };
        namespace FireBall {
            constexpr float DURATION = 5.0f;
        }
    }
    namespace DryBowser{
		constexpr float BASIC_ATTACK_COOLDOWN = 2.f;
        constexpr float WalkTurnDuration = 0.25f;  
		constexpr float JUMP_VELOCITY = -800.0f;
        constexpr float WallJumpFriction = 0.95f;
        constexpr float RUN_SPEED = 300.0f; 
		constexpr float BASIC_ATTACK_DURATION = 2.0f;
        constexpr float BASIC_ATTACK_RANGE = 150.0f; 
		constexpr float SPIN_ATTACK_COOLDOWN = 15.0f;
        constexpr float SPIN_ATTACK_WINDUP_DURATION = 1.f;
        constexpr float SPIN_ATTACK_DURATION =0.5f;
        constexpr float SPIN_ATTACK_WINDDOWN_DURATION = 0.75f;
		constexpr float SPIN_ATTACK_RANGE = 300.0f;
        constexpr float TAKE_DAMAGE_DURATION = 0.5f; 
		constexpr float INTRO_DURATION = 3.0f;  
        constexpr float DEAD_DURATION = 3.0f;  
        constexpr float WALL_JUMP_DURATION = 0.25f;
		constexpr float AERIAL_ATTACK_DURATION = 1.5f;
        constexpr float AERIAL_ATTACK_RANGE = 500;
        constexpr float AERIAL_ATTACK_COOLDOWN = 10.0f; 
    } 
    namespace Spring {
        constexpr float BOUNCE_VELOCITY = 14.0f;
        constexpr Vector2 STANDARD_SIZE = { 1, 1 };
        constexpr float BOUNCE_DURATION = 0.8f;
    }
    namespace Torch {
        constexpr Vector2 STANDARD_SIZE = { 1, 1 };
        constexpr float STANDARD_LIGHT_RADIUS = 0.2f;
        constexpr float STANDARD_BRIGHTNESS = 0.5f;
    }
    namespace PaletteResources {
        constexpr Rectangle BRICK = { 12, 111, 128, 86 };
        constexpr Rectangle GROUND = { 786, 111, 128, 86 };

        constexpr Rectangle BLOCK_1_1_2 = { 18, 1, 16, 16 };
        constexpr Rectangle BLOCK_1_1_3 = { 35, 1, 16, 16 };
        constexpr Rectangle BLOCK_1_1_12 = { 188, 1, 16, 16 };
        constexpr Rectangle BLOCK_1_1_13 = { 205, 1, 16, 16 };
        constexpr Rectangle BLOCK_1_1_14 = { 222, 1, 16, 16 };
        constexpr Rectangle BLOCK_1_1_15 = { 239, 1, 16, 16 };
        constexpr Rectangle BLOCK_1_1_16 = { 256, 1, 16, 16 };
        constexpr Rectangle BLOCK_1_2_7 = { 103, 18, 16, 16 };
        constexpr Rectangle BLOCK_1_2_8 = { 120, 18, 16, 16 };
        constexpr Rectangle BLOCK_1_2_12 = { 188, 18, 16, 16 };
        constexpr Rectangle BLOCK_1_2_13 = { 205, 18, 16, 16 };
        constexpr Rectangle BLOCK_1_2_14 = { 222, 18, 16, 16 };
        constexpr Rectangle BLOCK_1_2_15 = { 239, 18, 16, 16 };
        constexpr Rectangle BLOCK_1_2_16 = { 256, 18, 16, 16 };
        constexpr Rectangle BLOCK_1_3_4 = { 52, 35, 16, 16 };
        constexpr Rectangle BLOCK_1_3_5 = { 69, 35, 16, 16 };
        constexpr Rectangle BLOCK_1_3_6 = { 86, 35, 16, 16 };
        constexpr Rectangle BLOCK_1_3_7 = { 103, 35, 16, 16 };
        constexpr Rectangle BLOCK_1_3_8 = { 120, 35, 16, 16 };
        constexpr Rectangle BLOCK_1_3_15 = { 239, 35, 16, 16 };
        constexpr Rectangle BLOCK_1_3_16 = { 256, 35, 16, 16 };
        constexpr Rectangle BLOCK_1_4_4 = { 52, 52, 16, 16 };
        constexpr Rectangle BLOCK_1_4_5 = { 69, 52, 16, 16 };
        constexpr Rectangle BLOCK_1_4_6 = { 86, 52, 16, 16 };
        constexpr Rectangle BLOCK_1_4_7 = { 103, 52, 16, 16 };
        constexpr Rectangle BLOCK_1_4_11 = { 171, 52, 16, 16 };
        constexpr Rectangle BLOCK_1_4_12 = { 188, 52, 16, 16 };
        constexpr Rectangle BLOCK_1_4_13 = { 205, 52, 16, 16 };
        constexpr Rectangle BLOCK_1_4_14 = { 222, 52, 16, 16 };
        constexpr Rectangle BLOCK_1_4_15 = { 239, 52, 16, 16 };
        constexpr Rectangle BLOCK_1_4_16 = { 256, 52, 16, 16 };
        constexpr Rectangle BLOCK_1_5_4 = { 52, 69, 16, 16 };
        constexpr Rectangle BLOCK_1_5_5 = { 69, 69, 16, 16 };
        constexpr Rectangle BLOCK_1_5_6 = { 86, 69, 16, 16 };
        constexpr Rectangle BLOCK_1_5_8 = { 120, 69, 16, 16 };
        constexpr Rectangle BLOCK_1_5_9 = { 137, 69, 16, 16 };
        constexpr Rectangle BLOCK_1_5_10 = { 154, 69, 16, 16 };
        constexpr Rectangle BLOCK_1_5_11 = { 171, 69, 16, 16 };
        constexpr Rectangle BLOCK_1_5_12 = { 188, 69, 16, 16 };
        constexpr Rectangle BLOCK_1_5_13 = { 205, 69, 16, 16 };
        constexpr Rectangle BLOCK_1_5_14 = { 222, 69, 16, 16 };
        constexpr Rectangle BLOCK_1_5_15 = { 239, 69, 16, 16 };
        constexpr Rectangle BLOCK_1_5_16 = { 256, 69, 16, 16 };
        constexpr Rectangle BLOCK_1_6_5 = { 69, 86, 16, 16 };
        constexpr Rectangle BLOCK_1_6_6 = { 86, 86, 16, 16 };
        constexpr Rectangle BLOCK_1_7_1 = { 1, 103, 16, 16 };
        constexpr Rectangle BLOCK_1_7_2 = { 18, 103, 16, 16 };
        constexpr Rectangle BLOCK_1_7_3 = { 35, 103, 16, 16 };
        constexpr Rectangle BLOCK_1_7_4 = { 52, 103, 16, 16 };
        constexpr Rectangle BLOCK_1_7_5 = { 69, 103, 16, 16 };
        constexpr Rectangle BLOCK_1_7_6 = { 86, 103, 16, 16 };
        constexpr Rectangle BLOCK_1_7_7 = { 103, 103, 16, 16 };
        constexpr Rectangle BLOCK_1_7_11 = { 171, 103, 16, 16 };
        constexpr Rectangle BLOCK_1_7_12 = { 188, 103, 16, 16 };
        constexpr Rectangle BLOCK_1_7_13 = { 205, 103, 16, 16 };
        constexpr Rectangle BLOCK_1_7_14 = { 222, 103, 16, 16 };
        constexpr Rectangle BLOCK_1_7_15 = { 239, 103, 16, 16 };
        constexpr Rectangle BLOCK_1_7_16 = { 256, 103, 16, 16 };
    }
}

