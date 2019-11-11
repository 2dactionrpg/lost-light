#pragma once

#include "entity.hpp"

class Projectile : public Entity {
    // Shared between all projectiles, no need to load one for each instance
    static Texture projectile_texture;

public:
    // Creates all the associated render resources and default transform
    bool init(int id);

    // Releases all the associated resources
    void destroy();

    // Renders the projectile
    // projection is the 2D orthographic projection matrix
    void draw(const mat3& projection) override;

    vec2 get_direction();

    void set_direction(vec2 direction);

    void set_rotation(float rad);

    // Returns the current projectile position
    vec2 get_position() const;

    // Sets the new projectile position
    void set_position(vec2 position);

    // Returns the projectile' bounding box for collision detection, called by collides_with()
    vec2 get_bounding_box() const;

    void set_scale(vec2 scale);

    int get_id() const;

private:
    int projectile_id;
};
