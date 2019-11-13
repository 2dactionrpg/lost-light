#pragma once

#include <vector>

#include "common.hpp"
#include "entity.hpp"

struct Line { uint16_t a, b; };

class Triangle : public Entity
{
    public:
        bool init(int id);
        void destroy();
        void draw(const mat3& projection) override;
        void spawn_dot(vec2 position);
        void set_position(vec2 pos);
        void set_rotation(float rad);
        void set_scale(vec2 scale);
        bool is_inside(vec2 pos);
        float area(vec2 p1, vec2 p2, vec2 p3);
        void green();
        void red();
        void white();


    private:
        int trig_id;
        float in_color[3];
        std::vector<Vertex> m_vertices;
        std::vector<Line> m_indicies;
};