#include "PlayerMovementSystem.hpp"
#include <iostream>

void PlayerMovementSystem::update(entt::registry &registry, float ms, Character &character, Shield &shield)
{
    auto view = registry.view<player, motion, input>();
    for (auto entity : view)
    {
        auto &position = view.get<motion>(entity).position;
        auto &radians = view.get<motion>(entity).radians;
        auto &speed = view.get<motion>(entity).speed;
        auto &xpos = view.get<input>(entity).xpos;
        auto &ypos = view.get<input>(entity).ypos;

        auto &upKeyPressed = view.get<input>(entity).upKeyPressed;
        auto &downKeyPressed = view.get<input>(entity).downKeyPressed;
        auto &leftKeyPressed = view.get<input>(entity).leftKeyPressed;
        auto &rightKeyPressed = view.get<input>(entity).rightKeyPressed;

        float step = speed * (ms / 1000);

        // Set movement for player
        if (upKeyPressed)
        {
            move(position, {0.f, -step});
            rotate(radians, 1.5f);
        }
        if (downKeyPressed)
        {
            move(position, {0.f, step});
            rotate(radians, -1.5f);
        }
        if (leftKeyPressed)
        {
            move(position, {-step, 0.f});
            rotate(radians, 0.f);
        }
        if (rightKeyPressed)
        {
            move(position, {step, 0.f});
            rotate(radians, 3.14f);
        }

        if (upKeyPressed && rightKeyPressed)
        {
            rotate(radians, 2.25f);
        }
        else if (rightKeyPressed && downKeyPressed)
        {
            rotate(radians, -2.25f);
        }
        else if (downKeyPressed && leftKeyPressed)
        {
            rotate(radians, -0.75f);
        }
        else if (leftKeyPressed && upKeyPressed)
        {
            rotate(radians, 0.75f);
        }

        character.set_position(position);
        character.set_rotation(radians);

        // Set movement for shield
        vec2 shieldVec = {0.f, 1.f};
        vec2 mouseVec = {
            (float)xpos - shield.get_position().x,
            (float)ypos - shield.get_position().y};

        int factor = 1;
        if (mouseVec.x > 0.f)
            factor = -1;

        float angle = acos(dot(mouseVec, shieldVec) / (lengthVec2(mouseVec) * lengthVec2(shieldVec)));
        shield.set_position(character.get_position());
        shield.set_rotation(factor * angle);
    }
}

void PlayerMovementSystem::move(vec2 &pos, vec2 off)
{
    pos.x += off.x;
    pos.y += off.y;
}

void PlayerMovementSystem::rotate(float &radians, float newRadians)
{
    radians = newRadians;
}

// Calculates the length of a vec2 vector
float PlayerMovementSystem::lengthVec2(vec2 v)
{
    return sqrt(pow(v.x, 2.f) + pow(v.y, 2.f));
}
