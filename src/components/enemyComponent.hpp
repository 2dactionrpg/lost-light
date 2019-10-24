#pragma once
#include <common.hpp>

struct enemyComponent {
    /* data */
    int id;
    int health;
    int enemy_type;
    bool is_alive;
    bool is_movable;
    float shoot_cooldown;
    float shoot_frequency;
    vec2 destination;
    vec2 target;
};
