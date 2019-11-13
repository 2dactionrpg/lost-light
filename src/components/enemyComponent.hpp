#pragma once
#include <common.hpp>

enum actionType
{
    SHOOT,
    TURN,
    PICK_DEST,
    TURN_TO_DEST,
    MOVE,
    IDLE,
};

struct enemyStateMachine {
    /* Behaviour */
    bool alert;
    float alert_cooldown_ms;
    bool enemy_on_sight;
    bool line_of_sight;
    actionType action;
    bool is_turn_complete;
    float turn_sum;
    float turn_step;
    float turn_target;
};

struct triangleComponent {
    int id;
};

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
    enemyStateMachine es;
};

