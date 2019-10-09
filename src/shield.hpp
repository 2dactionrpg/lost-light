#pragma once

// #include "common.hpp"
#include "entity.hpp"
#include <vector>

// class Turtle;
// class Fish;
class Projectile;

class Shield : public Entity {
    static Texture shield_texture;

public:
    bool init();

    void destroy();

    void draw(const mat3& projection) override;

    vec2 get_position() const;

    void set_position(vec2 position);

    float get_rotation() const;

    void set_rotation(float radians);

    vec2 get_scale() const;

    void set_scale(vec2 scale);

    void hide();

    vec2 getDirection();

    void scaleMultiplier(float x, float y);

    vec2 get_bounding_box() const;

    bool collides_with(const Projectile& projectile);

    float triangleArea(vec2 p1, vec2 p2, vec2 p3);

    float trianglesArea(vec2 p1, vec2 p2, vec2 p3, vec2 p4, vec2 projectileP);

    float squareArea(vec2 p1, vec2 p2, vec2 p3);

private:
    std::vector<Vertex> m_vertices;
    std::vector<uint16_t> m_indices;
};
