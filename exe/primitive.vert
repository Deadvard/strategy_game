#version 430

layout (location = 0) in vec2 position;
out vec2 uv;

uniform mat4 model;
uniform mat4 view_projection;

void main()
{
	uv = position;
	gl_Position = view_projection * model * vec4(position, 0.0, 1.0);
}



