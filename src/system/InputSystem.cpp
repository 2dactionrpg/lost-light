#include "InputSystem.hpp"

void InputSystem::update(entt::registry &registry, int key, int action, int mod)
{
    auto view = registry.view<player, input, motion>();
    for (auto entity : view)
    {
        auto &upKeyPressed = view.get<input>(entity).upKeyPressed;
        auto &downKeyPressed = view.get<input>(entity).downKeyPressed;
        auto &leftKeyPressed = view.get<input>(entity).leftKeyPressed;
        auto &rightKeyPressed = view.get<input>(entity).rightKeyPressed;
        if (action == GLFW_RELEASE && key == GLFW_KEY_R)
        {
            registry.reset<motion>(entity);
            // m_character.destroy();
            // m_character.init();
            // m_pebbles_emitter.destroy();
            // m_pebbles_emitter.init();
            // m_projectiles.clear();
            // // m_turtles.clear();
            // // m_fish.clear();
            // m_water.reset_salmon_dead_time();
            // m_current_speed = 1.f;
        }
    }
}