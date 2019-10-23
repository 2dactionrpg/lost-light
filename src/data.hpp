#pragma once

#include "common.hpp"

namespace {
// SaveGameData {
// TODO

// InitialData
// Character Initial data
vec2 c_init_pos = { 500.f, 300.f };
float c_init_radians = 20.f;
float c_init_speed = 250.f;
vec2 c_init_scale = { 0.1f, 0.1f };
float c_init_cooldown = 50.f;
float c_init_dash_distance = 100.f;

// Shield Initial data
vec2 s_init_pos = { 50.f, 300.f };
float s_init_radians = 20.f;
float s_init_speed = 200.f;
vec2 s_init_scale = { 0.25f, -0.4f };
float s_init_duration = 20.f;
float s_init_cooldown = 50.f;

// Others
enum soundType {
    C_DEAD,
    S_REFLECT,
    MENU_POPUP,
    MENU_CLOSE
};

enum gameState {
    STATE_START,
    STATE_TUTORIAL,
    STATE_PLAYING,
    STATE_PAUSE,
    STATE_GAMEOVER,
    STATE_WIN,
    STATE_TERMINATE,
};

}
