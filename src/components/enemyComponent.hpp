#pragma once
#include <common.hpp>

struct enemyComponent
{
    /* data */
    int id;
    bool is_alive;
    float shoot_delay_ms;
    vec2 destination;
    vec2 target;
};