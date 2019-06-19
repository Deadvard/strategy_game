#include "primitives.h"
#include "shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

typedef unsigned char byte;

typedef struct primitive_vertex
{
	glm::vec3 position;
	glm::vec3 color;
} primitive_vertex;

typedef struct memory_stack
{
	byte* bottom;
	unsigned int top;
	unsigned int size;
};

typedef struct renderer
{
	unsigned int shader[3];
	unsigned int primitive_buffer;
	unsigned int primitive_vertex_array;

	memory_stack stack;
} renderer;


static renderer* static_renderer;

void initialize(renderer* r)
{
	r->shader[R_PRIMITIVE] = createShader("primitive.vert", "primitive.frag");
	glGenBuffers(1, &r->primitive_buffer);
	glGenVertexArrays(1, &r->primitive_vertex_array);
	
	glBindVertexArray(r->primitive_vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, r->primitive_buffer);
	glBufferData(GL_ARRAY_BUFFER, r->stack.size, 0, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	unsigned int stride = 6 * sizeof(float);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * stride, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * stride, (void*)(3 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void begin_render(unsigned int type, renderer* r)
{
	static_renderer = r;
	glUseProgram(static_renderer->shader[type]);
}

void end_render()
{
	glBindBuffer(GL_ARRAY_BUFFER, static_renderer->primitive_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		static_renderer->stack.top,
		static_renderer->stack.bottom);

	glBindVertexArray(static_renderer->primitive_vertex_array);

	glDrawArrays(GL_TRIANGLES, 0, static_renderer->stack.top);
	glUseProgram(0);
}

static const float box[]
{
	//pos, color
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 

	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 
	-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, -1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, -1.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, -1.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f
};


void push_cube(glm::vec3 position, glm::vec3 color, float size)
{
	primitive_vertex vertices[36];
	
	for (int i = 0; i < 36; ++i)
	{
		//static_renderer->stack.
	}


}

void push_cylinder()
{

}

void push_circle()
{

}

void push_cone()
{

}