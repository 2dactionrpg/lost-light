#pragma once
#include <common.hpp>

struct enemyComponent
{
    /* data */
    int id;
    bool is_alive;
    vec2 destination;
    vec2 target;
};