#pragma once

#include "common.hpp"
#include "entity.hpp"

class Redzone : public Entity
{
    static Texture redzone_texture;

public:
    bool init();

    void destroy();

    void draw(const mat3& projection) override;

    void set_position(vec2 pos);

    vec2 get_bounding_box() const;

    vec2 get_position() const;

};