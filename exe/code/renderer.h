#ifndef RENDERER_H
#define RENDERER_h

#include "../../dll/code/game.h"

typedef struct renderer
{
	unsigned int primitive_shader;
	unsigned int primitive_buffer;
	unsigned int primitive_index_buffer;
	unsigned int primitive_vertex_array;

	unsigned int primitive_arrow;
} renderer;

void initialize(renderer* r);
void render(game_memory* memory, renderer* r);

#endif
