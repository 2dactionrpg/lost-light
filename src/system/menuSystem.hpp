#pragma once

#include "../common.hpp"
#include "../components/menuComponent.hpp"
#include "../data.hpp"
#include "../menu.hpp"
#include "soundSystem.hpp"
#include <entt/entity/registry.hpp>

using namespace std;

class MenuSystem {
private:
    SoundSystem soundSystem;

public:
    void update(entt::registry& registry, Menu& m_menu);
    void sync(entt::registry& registry, int m_state);
    int get_state(entt::registry& registry);
    bool get_debug_mode(entt::registry& registry);
};
