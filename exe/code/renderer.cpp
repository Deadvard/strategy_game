#include "primitives.h"
#include "shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

static const float arrow[]
{
	1,1,0,0,0,0,0,0,0,0,
	1,1,1,0,0,0,0,0,0,0,
	0,1,1,1,0,0,0,0,0,0,
	0,0,1,1,1,0,0,0,0,0,
	0,0,0,1,1,1,0,0,0,1,
	0,0,0,0,1,1,1,0,1,1,
	0,0,0,0,0,1,1,1,1,1,
	0,0,0,0,0,0,1,1,1,1,
	0,0,0,0,0,1,1,1,1,1,
	0,0,0,0,1,1,1,1,1,1,
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

	glGenTextures(1, &r->primitive_arrow);
	glBindTexture(GL_TEXTURE_2D, r->primitive_arrow);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 10, 10, 0, GL_RED, GL_FLOAT, arrow);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void render(renderer* r)
{
	glUseProgram(r->primitive_shader);
	uniform(r->primitive_shader, "view_projection", glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 1000.0f));
	uniform(r->primitive_shader, "model", glm::translate(glm::mat4(1.0f), glm::vec3(0,0,-2)));
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, r->primitive_arrow);
	
	uniform(r->primitive_shader, "texture1", 0);	

	glBindVertexArray(r->primitive_vertex_array);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}