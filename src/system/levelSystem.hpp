#pragma once
#include "../common.hpp"
#include "../components/levelComponent.hpp"

#include <entt/entity/registry.hpp>

using namespace std;

class LevelSystem {
private:
    int enemy_id = 0;
    int lvl_num;
    int minion_max_on_screen;
    int boss_max_on_screen;
    int minion_num;
    int boss_num;
    int enemy_total;
    vector<vec2> minion_init_pos;
    vector<bool> minion_is_movable;
    vector<vec2> boss_init_pos;
    vector<bool> boss_is_movable;

public:
    bool init_level(entt::registry& registry, int m_lvl_num);
    void load_level_info(entt::registry& registry, int& m_lvl_num, int& m_minion_max_on_screen,
        int& m_boss_max_on_screen, int& m_minion_num, int& m_boss_num, int& m_enemy_total, int& m_enemy_killed);
    void update(entt::registry& registry, int& m_enemy_killed);
    void increment_enemy_killed(entt::registry& registry);
    int get_next_enemy_id();
    vec2 get_next_minion_pos();
    bool get_next_minion_is_movable();
    vec2 get_next_boss_pos();
    bool get_next_boss_is_movable();
};
