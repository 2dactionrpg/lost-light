#pragma once

#include "entity.hpp"

class Health : public Entity
{
    // Shared between all grounds, no need to load one for each instance
    static Texture health_texture;

public:
    // Creates all the associated render resources and default transform
    bool init();
    // Releases all the associated resources
    void destroy();

    void draw(const mat3 &projection) override;

    void set_scale(vec2 scale);

    void load_texture(int level);

    void set_position(vec2 position);
};
