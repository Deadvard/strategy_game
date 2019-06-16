#version 430

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texture;

out vertex_data
{
	vec3 world_pos;
	vec2 tex_coord;
	vec3 world_normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vs_out.world_pos = (model * vec4(vertex_position, 1)).xyz;
    vs_out.tex_coord = vertex_texture;
	vs_out.world_normal = (model * vec4(vertex_normal, 0)).xyz;

    gl_Position = projection * view * model * vec4(vertex_position, 1.0);
}
