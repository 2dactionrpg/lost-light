#include "menuSystem.hpp"

void MenuSystem::update(entt::registry& registry, Menu& m_menu)
{
    auto menu = registry.view<menuComponent>();

    for (auto entity : menu) {
        auto& state = menu.get(entity).state;
        auto& old_state = menu.get(entity).old_state;
        if (state != old_state) {
            if (state == STATE_PAUSE) {
                soundSystem.play_sound(MENU_POPUP);
            }
            if (old_state == STATE_PAUSE && state == STATE_PLAYING) {
                soundSystem.play_sound(MENU_CLOSE);
            }
            m_menu.load_texture(state);
            old_state = state;
        }
    }
}

void MenuSystem::sync(entt::registry& registry, int m_state)
{
    auto menu = registry.view<menuComponent>();

    for (auto entity : menu) {
        auto& state = menu.get(entity).state;
        auto& old_state = menu.get(entity).old_state;
        if (state != m_state) {
            state = m_state;
        }
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

bool MenuSystem::get_debug_mode(entt::registry& registry)
{
    auto menu = registry.view<menuComponent>();

    bool m_debug = false;
    for (auto entity : menu) {
        auto& debug = menu.get(entity).debug;
        m_debug = debug;
    }
    return m_debug;
}