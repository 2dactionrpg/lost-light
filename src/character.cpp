// Header
#include "character.hpp"

// internal

// stlib
#include <algorithm>
#include <string>


bool Character::init()
{
    return true;
}

// Releases all graphics resources
void Character::destroy()
{
}

// Called on each frame by World::update()
void Character::update(float ms)
{
}

void Character::draw(const mat3& projection)
{
}

vec2 Character::get_position() const
{
    return vec2 { 0, 0 };
}

void Character::move(vec2 off)
{
}

void Character::set_rotation(float radians)
{
}

bool Character::is_alive() const
{
    return true;
}

void Character::kill()
{
}
