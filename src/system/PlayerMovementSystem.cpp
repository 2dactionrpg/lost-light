#include "PlayerMovementSystem.hpp"

void PlayerMovementSystem::update(entt::registry &registry, float ms)
{
    auto view = registry.view<player, motion, input>();
    for (auto entity : view)
    {
        auto &position = view.get<motion>(entity).position;
        auto &radians = view.get<motion>(entity).radians;
        auto &speed = view.get<motion>(entity).speed;
        
        auto &upKeyPressed = view.get<input>(entity).upKeyPressed;
        auto &downKeyPressed = view.get<input>(entity).downKeyPressed;
        auto &leftKeyPressed = view.get<input>(entity).leftKeyPressed;
        auto &rightKeyPressed = view.get<input>(entity).rightKeyPressed;

        float step = speed * (ms / 1000);

        if (upKeyPressed) {
            move(position, { 0.f, -step });
            set_rotation(radians, 1.5f);
        }
        if (downKeyPressed) {
            move(position, { 0.f, step });
            set_rotation(radians, -1.5f);
        }
        if (leftKeyPressed) {
            move(position, { -step, 0.f });
            set_rotation(radians, 0.f);
        }
        if (rightKeyPressed) {
            move(position, { step, 0.f });
            set_rotation(radians, 3.14f);
        }

        if (upKeyPressed && rightKeyPressed) {
            set_rotation(radians, 2.25f);
        } else if (rightKeyPressed && downKeyPressed) {
            set_rotation(radians, -2.25f);
        } else if (downKeyPressed && leftKeyPressed) {
            set_rotation(radians, -0.75f);
        } else if (leftKeyPressed && upKeyPressed) {
            set_rotation(radians, 0.75f);
        }
    }
}

void PlayerMovementSystem::move(vec2 pos, vec2 off)
{
    pos.x += off.x;
    pos.y += off.y;
}


void PlayerMovementSystem::set_rotation(float radians, float newRadians)
{
    radians = newRadians;
}
