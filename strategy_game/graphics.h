#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <glm/glm.hpp>

#include "game_stack.h"

typedef struct
{
	unsigned int shader;
	unsigned int quad_vao;
	unsigned int quad_vbo;
	unsigned int instance_vbo;

	int num_quads;

	glm::mat4 view;
	glm::mat4 projection;
} graphics;

graphics create_graphics();
void render(graphics* g, game_data* gd);

#endif

