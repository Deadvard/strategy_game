#include "primitives.h"
#include "shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

static const float quad[]
{
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
};

static const unsigned int quad_indices[]
{
	0, 1, 2,
	3, 2, 1

};

void initialize(renderer* r)
{
	r->primitive_shader = createShader("primitive.vert", "primitive.frag");
	glGenBuffers(1, &r->primitive_buffer);
	glGenBuffers(1, &r->primitive_index_buffer);
	glGenVertexArrays(1, &r->primitive_vertex_array);
	
	glBindVertexArray(r->primitive_vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, r->primitive_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r->primitive_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void render(renderer* r)
{
	glUseProgram(r->primitive_shader);
	glBindVertexArray(r->primitive_vertex_array);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}