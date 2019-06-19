#ifndef RENDERER_H
#define RENDERER_h

typedef struct renderer renderer;

enum render_type
{
	R_PRIMITIVE,
	R_GUI,
	R_MESH
};

void begin_render(unsigned int type, renderer* r);
void end_render();


#endif
