#pragma once

#include "entity.hpp"

class Ground : public Entity
{
    // Shared between all grounds, no need to load one for each instance
    static Texture ground_texture;

public:
    // Creates all the associated render resources and default transform
    bool init(int id);

    // Releases all the associated resources
    void destroy();

    // Renders the ground
    // projection is the 2D orthographic projection matrix
    void draw(const mat3 &projection) override;

    vec2 get_direction();

    void set_direction(vec2 direction);

    void set_rotation(float rad);

    void set_scale(vec2 scale);

    void load_texture(int level);

    // Returns the current ground position
    vec2 get_position() const;

    // Sets the new ground position
    void set_position(vec2 position);

    // Returns the ground' bounding box for collision detection, called by collides_with()
    vec2 get_bounding_box() const;

    int get_id();

private:
    int ground_id;
};
