#version 430

layout (location = 0) in vec2 position;
out vec2 uv;

void main()
{
	uv = position;
	gl_Position = vec4(position, -1.0, 1.0);
}



