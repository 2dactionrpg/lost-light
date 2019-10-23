#pragma once
#include <common.hpp>

struct enemyComponent {
    /* data */
    int id;
    int health;
    bool is_alive;
    bool is_movable;
    float shoot_delay_ms;
    vec2 destination;
    vec2 target;
};
