#pragma once

#include "common.hpp"
#include <vector>

class Character : public Entity {
public:
    // Creates all the associated render resources and default transform
    bool init();

    // Releases all associated resources
    void destroy();

    // Update character position based on direction
    // ms represents the number of milliseconds elapsed from the previous update() call
    void update(float ms);

    // Renders the character
    void draw(const mat3& projection) override;

    // Returns the current character position
    vec2 get_position() const;

    // Moves the character's position by the specified offset
    void move(vec2 off);

    // Set character rotation in radians
    void set_rotation(float radians);

    // True if the character is alive
    bool is_alive() const;

    // Kills the character, changing its alive state and triggering on death events
    void kill();

private:
    bool c_is_alive; // True if the character is alive

    std::vector<Vertex> c_vertices;
    std::vector<uint16_t> c_indices;
};
