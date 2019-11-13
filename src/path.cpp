#define _USE_MATH_DEFINES

#include "path.hpp"

#include <cmath>
#include <iostream>

constexpr int NUM_SEGMENTS = 12;

bool Path::init(int id)
{
    std::vector<GLfloat> screen_vertex_buffer_data;
	constexpr float z = -0.1;

	for (int i = 0; i < NUM_SEGMENTS; i++) {
		screen_vertex_buffer_data.push_back(std::cos(M_PI * 2.0 * float(i) / (float)NUM_SEGMENTS));
		screen_vertex_buffer_data.push_back(std::sin(M_PI * 2.0 * float(i) / (float)NUM_SEGMENTS));
		screen_vertex_buffer_data.push_back(z);

		screen_vertex_buffer_data.push_back(std::cos(M_PI * 2.0 * float(i + 1) / (float)NUM_SEGMENTS));
		screen_vertex_buffer_data.push_back(std::sin(M_PI * 2.0 * float(i + 1) / (float)NUM_SEGMENTS));
		screen_vertex_buffer_data.push_back(z);

		screen_vertex_buffer_data.push_back(0);
		screen_vertex_buffer_data.push_back(0);
		screen_vertex_buffer_data.push_back(z);
	}

    // Clearing errors
	gl_flush_errors();

	// Vertex Buffer creation
	glGenBuffers(1, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, screen_vertex_buffer_data.size()*sizeof(GLfloat), screen_vertex_buffer_data.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_instance_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_instance_vbo);

	if (gl_has_errors())
		return false;

	// Loading shaders
	if (!effect.load_from_file(shader_path("pebble.vs.glsl"), shader_path("pebble.fs.glsl")))
		return false;

    // m_dots = {
    //     {true, {150.f, 150.f}, 3.f},
    //     {true, {250.f, 250.f}, 3.f},
    //     {true, {350.f, 350.f}, 3.f},
    //     {true, {450.f, 450.f}, 3.f}
    // };

	path_id = id;
	steps = 20;

	in_color[0] = 0.0;
    in_color[1] = 1.0;
    in_color[2] = 0.0;

	is_blocked = false;

	return true;
}

void Path::destroy() {
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &m_instance_vbo);

    glDeleteShader(effect.vertex);
    glDeleteShader(effect.fragment);
    glDeleteShader(effect.program);

    m_dots.clear();
}

std::vector<Dot> Path::form(vec2 start, vec2 self_box,vec2 end) {
    m_dots.clear();

	in_color[0] = 0.0;
    in_color[1] = 1.0;

	is_blocked = false;

	float dy = (end.y - start.y) / steps;
	float dx = (end.x - start.x) / steps;

    float idx = start.x;
    float idy = start.y;
    while ( !(len({end.x - idx + dx, end.y - idy + dy}) < 70))
    {
		idx += dx;
		idy += dy;
        Dot d = {true, {idx, idy}, 3.f};
        m_dots.emplace_back(d);
    }
	return m_dots;
	// fprintf(stderr,"line formed\n");
}

void Path::blocked()
{
	is_blocked = true;
	in_color[0] = 1.0;
    in_color[1] = 0.0;

}
void Path::draw(const mat3& projection)
{
    glUseProgram(effect.program);

    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

    // GLint transform_uloc = glGetUniformLocation(effect.program, "transform");
    GLint projection_uloc = glGetUniformLocation(effect.program, "projection");
    GLint color_uloc = glGetUniformLocation(effect.program, "color");

    float color[] = { in_color[0], in_color[1], in_color[2] };
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribDivisor(0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_instance_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_dots.size() * sizeof(Dot), m_dots.data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Dot), (GLvoid*)offsetof(Dot, position));
	glVertexAttribDivisor(1, 1);

    glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Dot), (GLvoid*)offsetof(Dot, radius));
	glVertexAttribDivisor(2, 1);

    glDrawArraysInstanced(GL_TRIANGLES, 0, NUM_SEGMENTS*3, m_dots.size());

    glVertexAttribDivisor(1, 0);
	glVertexAttribDivisor(2, 0);
}