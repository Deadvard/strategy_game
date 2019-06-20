#ifndef RENDERER_H
#define RENDERER_h

typedef struct renderer
{
	unsigned int primitive_shader;
	unsigned int primitive_buffer;
	unsigned int primitive_index_buffer;
	unsigned int primitive_vertex_array;
} renderer;

void initialize(renderer* r);
void render(renderer* r);

#endif
