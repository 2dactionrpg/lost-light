#pragma once

#include "entity.hpp"
#include "projectile.hpp"
#include <vector>

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

    vec2 get_direction();

    vec2 get_bounding_box() const;

    bool collides_with(const Projectile& projectile);

    float triangleArea(vec2 p1, vec2 p2, vec2 p3);

    float trianglesArea(vec2 p1, vec2 p2, vec2 p3, vec2 p4, vec2 projectileP);

    float squareArea(vec2 p1, vec2 p2, vec2 p3);

    bool onSegment(vec2 p, vec2 q, vec2 r);

    int orientation(vec2 p, vec2 q, vec2 r);

    bool doIntersect(vec2 p1, vec2 q1, vec2 p2, vec2 q2);

private:
    std::vector<Vertex> m_vertices;
    std::vector<uint16_t> m_indices;
};
