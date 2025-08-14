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
    constexpr float WORLDBOUNDS_WIDTH = 5000.0f;
    constexpr float WORLDBOUNDS_HEIGHT = 5000.0f;
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
        constexpr float BOUNCE_AFTER_STRIKE_VELOCITY = 7.0f;
        constexpr float INVINCIBLE_TIME_AFTER_STRIKE = 0.2f;
        constexpr float INVINCIBLE_TIME_AFTER_TRANSFORM = 1.0f;
        constexpr float STUNNED_STATE_DURATION = 1.0f;
        constexpr float REVIVE_TIME = 3.0f;
        constexpr float TRANSFORM_DURATION = 0.8f;

        namespace Mario {
            const Vector2 SMALL_SIZE = { 0.75f, 1.0f };
            const Vector2 SUPER_SIZE = { 1.0f, 1.5f };
            const CharacterStats STATS = { 3.0f, 12.0f };
            const std::vector<std::vector<Rectangle>> SMALL_FRAME_DATA = {
                {{11, 23, 12, 15}}, // idle state
                {{118, 23, 12, 15}, {149, 22, 15, 16}, {186, 22, 16, 16}, {219, 22, 16, 16}}, // moving state
                {{293, 22, 16, 16}, {326, 22, 16, 16}}, // jumping state
                {{11, 23, 12, 15}, {49, 26, 11, 12}}, // stunned state
                {{713, 22, 16, 16}}, // knocked state
                {{11, 23, 12, 15}, {149, 22, 15, 16}, {293, 22, 16, 16}, {360, 106, 16, 26}, {11, 106, 14, 27}}, // super transform
                {{11, 23, 12, 15}, {149, 22, 15, 16}, {293, 22, 16, 16}, {360, 262, 16, 26}, {11, 262, 14, 27}} // fire transform
            };
            const std::vector<std::vector<Rectangle>> SUPER_FRAME_DATA = {
                {{11, 106, 14, 27}}, // idle state
                {{116, 106, 16, 27}, {149, 106, 16, 26}, {184, 106, 14, 27}}, // moving state
                {{360, 106, 16, 26}, {393, 106, 16, 26}}, // jumping state
                {{11, 106, 14, 27}, {48, 115, 14, 18}}, // stunned state
                {}, // none
                {{11, 106, 14, 27}, {360, 106, 16, 26}, {293, 22, 16, 16}, {149, 22, 15, 16}, {11, 23, 12, 15}}, // small transform
                {{11, 23, 12, 15}, {149, 22, 15, 16}, {293, 22, 16, 16}, {360, 262, 16, 26}, {11, 262, 14, 27}} // fire transform
            };
            const std::vector<std::vector<Rectangle>> FIRE_FRAME_DATA = {
                {{11, 262, 14, 27}}, // idle state
                {{116, 262, 16, 27}, {149, 262, 16, 26}, {184, 262, 14, 27}}, // moving state
                {{360, 262, 16, 26}, {393, 262, 16, 26}}, // jumping state
                {{11, 262, 14, 27}, {48, 271, 14, 18}}, // stunned state 
                {}, // none
                {{11, 262, 14, 27}, {360, 262, 16, 26}, {293, 22, 16, 16}, {149, 22, 15, 16}, {11, 23, 12, 15}}, // small transform
                {{11, 317, 14, 27}, {44, 317, 14, 27}} // attack state
            };
        }

        namespace Luigi {
            const Vector2 SMALL_SIZE = { 0.75f, 1.25f };
            const Vector2 SUPER_SIZE = { 1.0f, 1.75f };
            const CharacterStats STATS = { 2.0f, 14.0f };
            const std::vector<std::vector<Rectangle>> SMALL_FRAME_DATA = {
                {{11, 893, 12, 15}}, // idle state
                {{118, 893, 12, 15}, {149, 892, 15, 16}, {186, 892, 16, 16}, {219, 892, 16, 16}}, // moving state
                {{293, 892, 16, 16}, {326, 892, 16, 16}}, // jumping state
                {{11, 893, 12, 15}, {49, 896, 11, 12}}, // stunned state
                {{713, 892, 16, 16}}, // knocked state
                {{11, 893, 12, 15}, {149, 892, 15, 16}, {293, 892, 16, 16}, {360, 966, 16, 26}, {11, 966, 14, 27}}, // super transform
                {{11, 893, 12, 15}, {149, 892, 15, 16}, {293, 892, 16, 16}, {360, 1122, 16, 26}, {11, 1122, 14, 27}}, // fire transform
            };
            const std::vector<std::vector<Rectangle>> SUPER_FRAME_DATA = {
                {{11, 966, 14, 27}}, // idle state
                {{116, 966, 16, 27}, {149, 966, 16, 26}, {184, 966, 14, 27}}, // moving state
                {{360, 966, 16, 26}, {393, 966, 16, 26}}, // jumping state
                {{11, 966, 14, 27}, {48, 975, 14, 18}}, // stunned state
                {}, // none
                {{11, 966, 14, 27}, {360, 966, 16, 26}, {293, 892, 16, 16}, {149, 892, 15, 16}, {11, 893, 12, 15}}, // small transform
                {{11, 893, 12, 15}, {149, 892, 15, 16}, {293, 892, 16, 16}, {360, 1122, 16, 26}, {11, 1122, 14, 27}}, // fire transform
            };
            const std::vector<std::vector<Rectangle>> FIRE_FRAME_DATA = {
                {{11, 1122, 14, 27}}, // idle state
                {{116, 1122, 16, 27}, {149, 1122, 16, 26}, {184, 1122, 14, 27}}, // moving state
                {{360, 1122, 16, 26}, {393, 1122, 16, 26}}, // jumping state
                {{11, 1122, 14, 27}, {48, 1131, 14, 18}}, // stunned state 
                {}, // none
                {{11, 1122, 14, 27}, {360, 1122, 16, 26}, {293, 892, 16, 16}, {149, 892, 15, 16}, {11, 893, 12, 15}}, // small transform
                {{11, 1177, 14, 27}, {44, 1177, 14, 27}} // attack state
            };
        }  

        namespace Toad {
            const Vector2 SMALL_SIZE = { 0.75f, 1.0f };
            const Vector2 SUPER_SIZE = { 1.0f, 1.5f };
            const CharacterStats STATS = { 4.0f, 10.0f };
            const std::vector<std::vector<Rectangle>> SMALL_FRAME_DATA = {
                {{11, 1752, 12, 15}}, // idle state
                {{118, 1752, 12, 15}, {149, 1751, 14, 16}, {186, 1751, 16, 16}, {219, 1751, 16, 16}}, // moving state
                {{293, 1751, 16, 16}, {326, 1751, 16, 16}}, // jumping state
                {{11, 1752, 12, 15}, {49, 1754, 14, 13}}, // stunned state
                {{713, 1751, 16, 16}}, // knocked state
                {{11, 1752, 12, 15}, {149, 1751, 14, 16}, {293, 1751, 16, 16}, {359, 1826, 18, 25}, {9, 1826, 15, 26}}, // super transform
                {{11, 1752, 12, 15}, {149, 1751, 14, 16}, {293, 1751, 16, 16}, {359, 1982, 18, 25}, {9, 1982, 15, 26}} // fire transform
            };
            const std::vector<std::vector<Rectangle>> SUPER_FRAME_DATA = {
                {{9, 1826, 15, 26}}, // idle state
                {{116, 1826, 16, 26}, {149, 1826, 16, 25}, {182, 1826, 15, 26}}, // moving state
                {{359, 1826, 18, 25}, {392, 1826, 16, 25}}, // jumping state
                {{9, 1826, 15, 26}, {47, 1835, 15, 18}}, // stunned state
                {}, // none
                {{9, 1826, 15, 26}, {359, 1826, 18, 25}, {293, 1751, 16, 16}, {149, 1751, 14, 16}, {11, 1752, 12, 15}}, // small transform
                {{11, 1752, 12, 15}, {149, 1751, 14, 16}, {293, 1751, 16, 16}, {359, 1982, 18, 25}, {9, 1982, 15, 26}} // fire transform
                };
            const std::vector<std::vector<Rectangle>> FIRE_FRAME_DATA = {
                {{9, 1982, 15, 26}}, // idle state
                {{116, 1982, 16, 26}, {149, 1982, 16, 25}, {182, 1982, 15, 26}}, // moving state
                {{359, 1982, 18, 25}, {392, 1982, 16, 25}}, // jumping state
                {{9, 1982, 15, 26}, {47, 1991, 15, 18}}, // stunned state 
                {}, // none
                {{9, 1982, 15, 26}, {359, 1982, 18, 25}, {293, 1751, 16, 16}, {149, 1751, 14, 16}, {11, 1752, 12, 15}}, // small transform
                {{9, 2037, 15, 26}, {42, 2037, 15, 26}} // attack state
            };
        }

        namespace Toadette {
            const Vector2 SMALL_SIZE = { 1.0f, 1.0f };
            const Vector2 SUPER_SIZE = { 1.0f, 1.5f };
            const CharacterStats STATS = { 3.5f, 10.0f };
            const std::vector<std::vector<Rectangle>> SMALL_FRAME_DATA = {
                {{6, 2611, 17, 15}, {39, 2611, 17, 15}}, // idle state
                {{146, 2611, 17, 15}, {179, 2610, 17, 16}, {216, 2610, 19, 16}, {249, 2610, 19, 16}}, // moving state
                {{323, 2610, 19, 16}, {355, 2610, 20, 16}}, // jumping state
                {{6, 2611, 17, 15}, {76, 2613, 18, 13}}, // stunned state
                {{743, 2610, 22, 16}}, // knocked state
                {{6, 2611, 17, 15}, {179, 2610, 17, 16}, {323, 2610, 19, 16}, {423, 2685, 24, 25}, {5, 2685, 19, 26}}, // super transform
                {{6, 2611, 17, 15}, {179, 2610, 17, 16}, {323, 2610, 19, 16}, {424, 2841, 23, 25}, {5, 2841, 19, 26}} // fire transform
            };
            const std::vector<std::vector<Rectangle>> SUPER_FRAME_DATA = {
                {{3, 2685, 21, 26}, {36, 2685, 21, 26}}, // idle state
                {{181, 2685, 21, 26}, {213, 2685, 22, 26}, {246, 2685, 21, 26}}, // moving state
                {{423, 2685, 24, 25}, {455, 2685, 23, 25}}, // jumping state
                {{5, 2685, 19, 26}, {74, 2693, 21, 18}}, // stunned state
                {}, // none
                {{5, 2685, 19, 26}, {423, 2685, 24, 25}, {323, 2610, 19, 16}, {179, 2610, 17, 16}, {6, 2611, 17, 15}}, // small transform
                {{6, 2611, 17, 15}, {179, 2610, 17, 16}, {323, 2610, 19, 16}, {424, 2841, 23, 25}, {5, 2841, 19, 26}} // fire transform
            };
            const std::vector<std::vector<Rectangle>> FIRE_FRAME_DATA = {
                {{3, 2841, 21, 26}, {36, 2841, 21, 26}}, // idle state
                {{180, 2841, 21, 26}, {213, 2841, 22, 25}, {246, 2841, 21, 26}}, // moving state
                {{424, 2841, 23, 25}, {455, 2841, 23, 25}}, // jumping state
                {{5, 2841, 19, 26}, {74, 2849, 21, 18}}, // stunned state 
                {}, // none
                {{5, 2841, 19, 26}, {424, 2841, 23, 25}, {323, 2610, 19, 16}, {179, 2610, 17, 16}, {6, 2611, 17, 15}}, // small transform
                {{42, 2896, 19, 26}, {75, 2896, 19, 26}} // attack state
            };
        }
    }

    namespace Projectile {
        constexpr Vector2 STANDARD_SIZE = { 0.5f, 0.5f };
        namespace FireBall {
            constexpr float DURATION = 5.0f;
            constexpr Vector2 INITIAL_VELOCITY = { 7.0f, 5.0f };
        }
    }

    namespace Spring {
        constexpr float BOUNCE_VELOCITY = 14.0f;
        constexpr Vector2 STANDARD_SIZE = { 1, 1 };
        constexpr float BOUNCE_DURATION = 0.8f;
    }

    namespace MovingPlatform {
        const float AMPLITUDE = 1.5f * Constants::TILE_SIZE;
        const float PERIOD = 7.0f;
    }

    namespace Boo
    {
        constexpr float chaseRange = 400.0f;
        constexpr float patrolRange = 600.0f;

        constexpr float flySpeed = 300.f;
        constexpr float waitForAttackTime = 0.25f; 
        constexpr float attackDuration = 0.5f; 
        constexpr float retreatDuration = 0.75f;
        constexpr float patrolDuration = 1.f;
        constexpr float dieDuration = 0.25f;

        constexpr float attackRange = 200;
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
    namespace Torch {
        constexpr Vector2 STANDARD_SIZE = { 1, 1 };
        constexpr float STANDARD_LIGHT_RADIUS = 0.2f;
        constexpr float STANDARD_BRIGHTNESS = 0.5f;
    }
    namespace PaletteResources {
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
        constexpr Rectangle BLOCK_1_13_1 = { 1, 205, 16, 16 };
        constexpr Rectangle BLOCK_1_13_2 = { 18, 205, 16, 16 };
        constexpr Rectangle BLOCK_1_13_3 = { 35, 205, 16, 16 };
        constexpr Rectangle BLOCK_1_13_4 = { 52, 205, 16, 16 };
        constexpr Rectangle BLOCK_1_13_5 = { 69, 205, 16, 16 };
        constexpr Rectangle BLOCK_1_13_6 = { 86, 205, 16, 16 };
        constexpr Rectangle BLOCK_1_13_7 = { 103, 205, 16, 16 };
        constexpr Rectangle BLOCK_1_13_8 = { 120, 205, 16, 16 };
        constexpr Rectangle BLOCK_1_13_9 = { 137, 205, 16, 16 };
        constexpr Rectangle BLOCK_1_13_10 = { 154, 205, 16, 16 };
        constexpr Rectangle BLOCK_1_13_11 = { 171, 205, 16, 16 };
        constexpr Rectangle BLOCK_1_13_12 = { 188, 205, 16, 16 };
        constexpr Rectangle BLOCK_1_13_13 = { 205, 205, 16, 16 };
        constexpr Rectangle BLOCK_1_13_14 = { 222, 205, 16, 16 };
        constexpr Rectangle BLOCK_1_13_15 = { 239, 205, 16, 16 };
        constexpr Rectangle BLOCK_1_13_16 = { 256, 205, 16, 16 };
        constexpr Rectangle BLOCK_1_14_1 = { 1, 222, 16, 16 };
        constexpr Rectangle BLOCK_1_14_2 = { 18, 222, 16, 16 };
        constexpr Rectangle BLOCK_1_14_3 = { 35, 222, 16, 16 };
        constexpr Rectangle BLOCK_1_14_4 = { 52, 222, 16, 16 };
        constexpr Rectangle BLOCK_1_14_5 = { 69, 222, 16, 16 };
        constexpr Rectangle BLOCK_1_14_6 = { 86, 222, 16, 16 };
        constexpr Rectangle BLOCK_1_14_7 = { 103, 222, 16, 16 };
        constexpr Rectangle BLOCK_1_14_8 = { 120, 222, 16, 16 };
        constexpr Rectangle BLOCK_1_14_9 = { 137, 222, 16, 16 };
        constexpr Rectangle BLOCK_1_14_10 = { 154, 222, 16, 16 };
        constexpr Rectangle BLOCK_1_14_11 = { 171, 222, 16, 16 };
        constexpr Rectangle BLOCK_1_14_12 = { 188, 222, 16, 16 };
        constexpr Rectangle BLOCK_1_14_13 = { 205, 222, 16, 16 };
        constexpr Rectangle BLOCK_1_14_14 = { 222, 222, 16, 16 };
        constexpr Rectangle BLOCK_1_14_15 = { 239, 222, 16, 16 };
        constexpr Rectangle BLOCK_1_14_16 = { 256, 222, 16, 16 };
        constexpr Rectangle BLOCK_1_15_1 = { 1, 239, 16, 16 };
        constexpr Rectangle BLOCK_1_15_2 = { 18, 239, 16, 16 };
        constexpr Rectangle BLOCK_1_15_3 = { 35, 239, 16, 16 };
        constexpr Rectangle BLOCK_1_15_4 = { 52, 239, 16, 16 };
        constexpr Rectangle BLOCK_1_15_5 = { 69, 239, 16, 16 };
        constexpr Rectangle BLOCK_1_15_6 = { 86, 239, 16, 16 };
        constexpr Rectangle BLOCK_1_15_7 = { 103, 239, 16, 16 };
        constexpr Rectangle BLOCK_1_15_8 = { 120, 239, 16, 16 };
        constexpr Rectangle BLOCK_1_15_9 = { 137, 239, 16, 16 };
        constexpr Rectangle BLOCK_1_15_10 = { 154, 239, 16, 16 };
        constexpr Rectangle BLOCK_1_15_11 = { 171, 239, 16, 16 };
        constexpr Rectangle BLOCK_1_15_12 = { 188, 239, 16, 16 };
        constexpr Rectangle BLOCK_1_15_13 = { 205, 239, 16, 16 };
        constexpr Rectangle BLOCK_1_15_14 = { 222, 239, 16, 16 };
        constexpr Rectangle BLOCK_1_15_15 = { 239, 239, 16, 16 };
        constexpr Rectangle BLOCK_1_15_16 = { 256, 239, 16, 16 };
        constexpr Rectangle BLOCK_1_16_1 = { 1, 256, 16, 16 };
        constexpr Rectangle BLOCK_1_16_2 = { 18, 256, 16, 16 };
        constexpr Rectangle BLOCK_1_16_3 = { 35, 256, 16, 16 };
        constexpr Rectangle BLOCK_1_16_4 = { 52, 256, 16, 16 };
        constexpr Rectangle BLOCK_1_16_5 = { 69, 256, 16, 16 };
        constexpr Rectangle BLOCK_1_16_6 = { 86, 256, 16, 16 };
        constexpr Rectangle BLOCK_1_16_7 = { 103, 256, 16, 16 };
        constexpr Rectangle BLOCK_1_16_8 = { 120, 256, 16, 16 };
        constexpr Rectangle BLOCK_1_16_9 = { 137, 256, 16, 16 };
        constexpr Rectangle BLOCK_1_16_10 = { 154, 256, 16, 16 };
        constexpr Rectangle BLOCK_1_16_11 = { 171, 256, 16, 16 };
        constexpr Rectangle BLOCK_1_16_12 = { 188, 256, 16, 16 };
        constexpr Rectangle BLOCK_1_16_13 = { 205, 256, 16, 16 };
        constexpr Rectangle BLOCK_1_16_14 = { 222, 256, 16, 16 };
        constexpr Rectangle BLOCK_1_16_15 = { 239, 256, 16, 16 };
        constexpr Rectangle BLOCK_1_16_16 = { 256, 256, 16, 16 };
    }
}