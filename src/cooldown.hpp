#pragma once

#include "entity.hpp"

class Cooldown : public Entity
{
    // Shared between all grounds, no need to load one for each instance
    static Texture cooldown_texture;

public:
    // Creates all the associated render resources and default transform
    bool init();
    // Releases all the associated resources
    void destroy();

    void draw(const mat3 &projection) override;

    void set_scale(vec2 scale);

    void load_texture(float cooldown);

    void set_position(vec2 position);
    private:
        int old_state;
};
