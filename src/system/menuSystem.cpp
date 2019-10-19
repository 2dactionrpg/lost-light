#include "menuSystem.hpp"
#include <iostream>

void MenuSystem::update(entt::registry& registry, Menu& m_menu)
{
    auto menu = registry.view<menuComponent>();

    for (auto entity : menu) {
        auto& state = menu.get(entity).state;
        m_menu.load_texture(state);
    }
}

void MenuSystem::sync(entt::registry& registry, int m_state)
{
    auto menu = registry.view<menuComponent>();

    for (auto entity : menu) {
        auto& state = menu.get(entity).state;
        state = m_state;
    }
}

int MenuSystem::get_state(entt::registry& registry)
{
    auto menu = registry.view<menuComponent>();

    int m_state = -1;
    for (auto entity : menu) {
        auto& state = menu.get(entity).state;
        m_state = state;
    }
    return m_state;
}
