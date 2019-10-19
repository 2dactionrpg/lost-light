#pragma once
#include "common.hpp"

// SaveGameData {
// TODO

// InitialData
// Character Initial data
vec2 c_init_pos = { 50.f, 300.f };
float c_init_radians = 20.f;
float c_init_speed = 200.f;
vec2 c_init_scale = { 0.1f, 0.1f };
bool c_init_keyboard_up = false;
bool c_init_keyboard_down = false;
bool c_init_keyboard_left = false;
bool c_init_keyboard_right = false;
bool c_init_keyboard_reset = false;
double c_init_mouse_xpos = 0.0;
double c_init_mouse_ypos = 0.0;

// Others
int itemTypeId;
int Durability; // also used as a 'uses' count for potions and the like
int strength; // damage of a weapon, protection of armor, effectiveness of a potion