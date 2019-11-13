#pragma once
#include <vector>
#include "common.hpp"
#include "entity.hpp"

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// These for walls to build prison for every digital being
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

struct EndPoints { uint16_t a, b; };

class Line : public Entity
{
    public:
        bool init();
        void destroy();
        void draw(const mat3& projection) override;
        std::vector<vec3> get_vertices();
    private:
        std::vector<Vertex> m_vertices;
        std::vector<EndPoints> m_indicies;
};