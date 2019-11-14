#pragma once
#include "../common.hpp"
#include "../components/levelComponent.hpp"
#include "../data.hpp"
#include "../enemy.hpp"
#include "../factory.hpp"
#include "../potion.hpp"
#include "menuSystem.hpp"

#include <entt/entity/registry.hpp>
#include <fstream>
#include <iostream>
#include <filesystem>

using namespace std;

class LevelSystem
{
private:
    // globa info
    int enemy_id = 0;
    int lvl_num = 1;
    int enemy_total;
    int enemy_killed;
    float enemy_spawn_delay;
    float next_enemy_spawn_counter;

    // minion info
    int minion_num;
    int minion_count;
    int minion_max_on_screen;
    vector<vec2> minion_init_pos;
    vector<bool> minion_is_movable;

    // boss info
    int boss_num;
    int boss_count;
    int boss_max_on_screen;
    vector<vec2> boss_init_pos;
    vector<bool> boss_is_movable;

    vector<vec2> wallVector;

    // additional info
    MenuSystem menuSystem;

public:
    bool init_level(entt::registry &registry);
    int update(entt::registry &registry, float elapsed_ms, vector<Enemy> *m_enemies, vector<Projectile> *m_projectiles);
    void increment_enemy_killed(entt::registry &registry);
    int get_next_enemy_id();
    vec2 get_next_minion_pos();
    bool get_next_minion_is_movable();
    vec2 get_next_boss_pos();
    bool get_next_boss_is_movable();
    void reset_enemy(entt::registry &registry);
    bool should_spawn_minion(int enemy_size);
    bool should_spawn_boss(entt::registry &registry);
    std::vector<vec2> get_wall_orientation();
    int getLevel();
};
