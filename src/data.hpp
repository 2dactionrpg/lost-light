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

static const vec2 zombie_level1_pos_array[] = {{800.f, 400.f}};
static const bool zombie_level1_movable_array[] = {true};

static const vec2 minion_level2_pos_array[] = {{1000.f, 200.f}, {1000.f, 400.f}, {1000.f, 600.f}};
static const bool minion_level2_movable_array[] = {false, false, false};

static const vec2 minion_level3_pos_array[] = {{200.f, 600.f}, {1000.f, 200.f}, {1000.f, 600.f}};
static const bool minion_level3_movable_array[] = {true, true, true};

static const vec2 boss_level4_pos_array[] = {{1000.f, 400.f}};
static const bool boss_level4_movable_array[] = {true};
static const vec2 wall_level1_pos_array[] = {{-100.f, -100.f}};
static const vec2 surrounding_wall_pos_array[] = {{0.5f, 0.5f},
                                                  {1.5f, 0.5f},
                                                  {2.5f, 0.5f},
                                                  {3.5f, 0.5f},
                                                  {4.5f, 0.5f},
                                                  {5.5f, 0.5f},
                                                  {6.5f, 0.5f},
                                                  {7.5f, 0.5f},
                                                  {8.5f, 0.5f},
                                                  {9.5f, 0.5f},
                                                  {10.5f, 0.5f},
                                                  {11.5f, 0.5f},
                                                  {12.5f, 0.5f},
                                                  {13.5f, 0.5f},
                                                  {14.5f, 0.5f},
                                                  {15.5f, 0.5f},
                                                  {16.5f, 0.5f},
                                                  {17.5f, 0.5f},
                                                  {18.5f, 0.5f},
                                                  {19.5f, 0.5f},
                                                  {20.5f, 0.5f},
                                                  {21.5f, 0.5f},
                                                  {22.5f, 0.5f},
                                                  {23.5f, 0.5f},
                                                  {24.5f, 0.5f},
                                                  {25.5f, 0.5f},
                                                  {26.5f, 0.5f},
                                                  {27.5f, 0.5f},
                                                  {28.f, 1.5f},
                                                  {28.f, 2.5f},
                                                  {28.f, 6.5f},
                                                  {28.f, 7.5f},
                                                  {28.f, 8.5f},
                                                  {28.f, 9.5f},
                                                  {28.f, 10.5f},
                                                  {28.f, 11.5f},
                                                  {28.f, 12.5f},
                                                  {28.f, 13.5f},
                                                  {28.f, 14.5f},
                                                  {28.f, 15.5f},
                                                  {28.f, 16.5f},
                                                  {28.f, 17.5},
                                                  {28.f, 18.5},
                                                  {0.5f, 18.5f},
                                                  {1.5f, 18.5f},
                                                  {2.5f, 18.5f},
                                                  {3.5f, 18.5f},
                                                  {4.5f, 18.5f},
                                                  {5.5f, 18.5f},
                                                  {6.5f, 18.5f},
                                                  {7.5f, 18.5f},
                                                  {8.5f, 18.5f},
                                                  {9.5f, 18.5f},
                                                  {10.5f, 18.5f},
                                                  {11.5f, 18.5f},
                                                  {12.5f, 18.5f},
                                                  {13.5f, 18.5f},
                                                  {14.5f, 18.5f},
                                                  {15.5f, 18.5f},
                                                  {16.5f, 18.5f},
                                                  {17.5f, 18.5f},
                                                  {18.5f, 18.5f},
                                                  {19.5f, 18.5f},
                                                  {20.5f, 18.5f},
                                                  {21.5f, 18.5f},
                                                  {22.5f, 18.5f},
                                                  {23.5f, 18.5f},
                                                  {24.5f, 18.5f},
                                                  {25.5f, 18.5f},
                                                  {26.5f, 18.5f},
                                                  {27.5f, 18.5f},
                                                  {0.5f, 0.5f},
                                                  {0.5f, 1.5f},
                                                  {0.5f, 2.5f},
                                                  {0.5f, 6.5f},
                                                  {0.5f, 7.5f},
                                                  {0.5f, 8.5f},
                                                  {0.5f, 9.5f},
                                                  {0.5f, 10.5f},
                                                  {0.5f, 11.5f},
                                                  {0.5f, 12.5f},
                                                  {0.5f, 13.5f},
                                                  {0.5f, 14.5f},
                                                  {0.5f, 15.5f},
                                                  {0.5f, 16.5f},
                                                  {0.5f, 17.5},
                                                  {0.5f, 18.5}};
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
    C_HIT,
    Z_KILLED,
    E_KILLED,
    S_REFLECT,
    MENU_POPUP,
    MENU_CLOSE
};

enum enemyType
{
    MINION,
    ZOMBIE,
    BOSS
};

enum gameState
{
    STATE_START,
    STATE_TUTORIAL,
    STATE_SLIDE_1,
    STATE_SLIDE_2,
    STATE_PLAYING,
    STATE_PAUSE,
    STATE_GAMEOVER,
    STATE_WIN,
    STATE_TERMINATE,
};

} // namespace
