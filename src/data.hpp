#pragma once

#include "common.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace
{
// SaveGameData {
// TODO

// InitialData
// Character Initial data
vec2 c_init_pos = {500.f, 300.f};
float c_init_radians = 20.f;
float c_init_speed = 250.f;
vec2 c_init_distortion = {1.0f, 1.0f};
float c_init_sheer = 0.f;
vec2 c_init_scale = {0.1f, 0.1f};
float c_init_cooldown = 50.f;
float c_init_dash_speed = 500.f;
vec2 c_init_dash_direction = {0.f, 0.f};
float c_init_dash_duration = 200.f;
float c_init_dash_cooldown = 500.f;
vec2 c_boss_init_pos = {1000.f, 500.f};

static const vec2 minion_level1_pos_array[] = {{1000.f, 400.f}};
static const bool minion_level1_movable_array[] = {false};

static const vec2 minion_level2_pos_array[] = {{1000.f, 200.f}, {1000.f, 400.f}, {1000.f, 600.f}};
static const bool minion_level2_movable_array[] = {false, false, false};

static const vec2 minion_level3_pos_array[] = {{200.f, 600.f}, {1000.f, 200.f}, {1000.f, 600.f}};
static const bool minion_level3_movable_array[] = {true, true, true};

static const vec2 boss_level4_pos_array[] = {{1000.f, 400.f}};
static const bool boss_level4_movable_array[] = {true};

static const vec2 wall_level1_pos_array[] = {{-100.f, -100.f}};
static const vec2 wall_level2_pos_array[] = {{14.f, 10.f},
                                             {14.f, 9.f},
                                             {14.f, 8.f}};
static const vec2 wall_level3_pos_array[] = {{14.f, 12.f},
                                             {14.f, 13.f},
                                             {14.f, 14.f},
                                             {14.f, 15.f},
                                             {14.f, 16.f},
                                             {14.f, 17.f},
                                             {14.f, 18.f},
                                             {14.f, 19.f},
                                             {20.f, 9.f},
                                             {21.f, 9.f},
                                             {22.f, 9.f},
                                             {23.f, 9.f},
                                             {24.f, 9.f},
                                             {25.f, 9.f},
                                             {26.f, 9.f},
                                             {27.f, 9.f},
                                             {28.f, 9.f}};
static const vec2 wall_level4_pos_array[] = {{10.f, 3.f},
                                             {10.f, 4.f},
                                             {10.f, 5.f},
                                             {10.f, 6.f},
                                             {10.f, 7.f},
                                             {10.f, 12.f},
                                             {10.f, 13.f},
                                             {10.f, 14.f},
                                             {10.f, 15.f},
                                             {10.f, 16.f}};

// Shield Initial data
vec2 s_init_pos = {50.f, 300.f};
float s_init_radians = 20.f;
float s_init_speed = 200.f;
vec2 s_init_scale = {0.25f, -0.4f};
float s_init_duration = 20.f;
float s_init_cooldown = 50.f;

// Enemy Initial data
float e_minion_init_shoot_frequency = 3000.f;
float e_boss_init_shoot_frequency = 1000.f;

bool debug_mode = false;

// Others
enum soundType
{
    C_DEAD,
    S_REFLECT,
    MENU_POPUP,
    MENU_CLOSE
};

enum enemyType
{
    MINION,
    BOSS
};

enum gameState
{
    STATE_START,
    STATE_TUTORIAL,
    STATE_PLAYING,
    STATE_PAUSE,
    STATE_GAMEOVER,
    STATE_WIN,
    STATE_TERMINATE,
};

} // namespace
