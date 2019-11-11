#pragma once
#include <common.hpp>

struct characterComponent
{
    /* data */
    bool is_movable;
    bool is_dashable;
    float dash_speed;
    vec2 dash_direction;
    float dash_duration;
    float dash_cooldown;
};
