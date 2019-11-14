#pragma once
#include <common.hpp>

struct projectileComponent
{
    int id;
    bool is_out_of_screen;
    bool is_alive;
    float radius;
};