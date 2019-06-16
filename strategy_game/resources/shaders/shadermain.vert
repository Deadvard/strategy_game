#version 430
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 offset;
layout(location = 3) in vec3 color;

out vec3 fcolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	fcolor = color;
	gl_Position = projection * view * model * vec4(position + offset, 1.0);
}