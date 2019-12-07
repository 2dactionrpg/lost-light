#pragma once

#include "data.hpp"
#include "entity.hpp"

class Overlay : public Entity {
    // Shared between all overlays, no need to load one for each instance
    static Texture overlay_texture;

public:
    // Creates all the associated render resources and default transform
    bool init();

    // Releases all the associated resources
    void destroy();

    // Update overlay due to current
    // ms represents the number of milliseconds elapsed from the previous update() call
    void update(float ms, int state);

    // Renders the overlay
    // projection is the 2D orthographic projection matrix
    void draw(const mat3& projection) override;

    vec2 get_direction();

    void set_direction(vec2 direction);

    void set_rotation(float rad);

    // Returns the current overlay position
    vec2 get_position() const;

    void set_light_sources(vec2 positions[], int size);

    // Sets the new overlay position
    void set_position(vec2 position);
    void load_texture(int state);

    // Returns the overlay' bounding box for collision detection, called by collides_with()
    vec2 get_bounding_box() const;

    vec2 light_source;
    GLfloat xs[20];
    GLfloat ys[20];
    GLfloat radius[20];
};
