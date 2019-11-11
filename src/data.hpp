#pragma once

#include "common.hpp"

namespace
{
// SaveGameData {
// TODO

// InitialData
// Character Initial data
vec2 c_init_pos = {500.f, 300.f};
float c_init_radians = 20.f;
float c_init_speed = 250.f;
vec2 c_init_scale = {0.1f, 0.1f};
float c_init_cooldown = 50.f;
float c_init_dash_speed = 500.f;
vec2 c_init_dash_direction = {0.f, 0.f};
float c_init_dash_duration = 200.f;
float c_init_dash_cooldown = 500.f;
vec2 c_boss_init_pos = {1000.f, 500.f};

static const vec2 init_pos_array[] = {{1000.f, 100.f}, {1000.f, 400.f}, {1000.f, 700.f}};
static const vec2 wall_level1_pos_array[] = {{2.f, 3.f}, {2.f, 4.f}, {2.f, 5.f}};
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
