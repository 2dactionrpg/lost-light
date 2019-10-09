#pragma once

#include <vector>

#include "common.hpp"
#include "entity.hpp"

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// DON'T WORRY ABOUT THIS CLASS UNTIL ASSIGNMENT 3
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// Salmon pew-pews
class Pebbles : public Entity {
public:
    // Data structure for pebble contains information needed
    // to render and simulate a basic pebble (apart from mesh.vbo),
    // we will use this layout to pass information from m_pebbles to the pipeline.
    struct Pebble {
        float life = 0.0f; // remove pebble when its life reaches 0
        vec2 position;
        vec2 velocity;
        float radius;
    };

    // Creates all the associated render resources
    bool init();

    // Releases all associated resources
    void destroy();

    // Updates all pebbles
    // ms represents the number of milliseconds elapsed from the previous update() call
    void update(float ms);

    // Renders the pebbles
    // projection is the 2D orthographic projection matrix
    void draw(const mat3& projection) override;

    // Spawn new pebble
    void spawn_pebble(vec2 position);

    // Trigger collision checks
    void collides_with();

private:
    GLuint m_instance_vbo; // vbo for instancing pebbles
    std::vector<Pebble> m_pebbles; // vector of pebbles
};
