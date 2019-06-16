#version 430

out vec4 frag_color;

in vec3 fcolor;


void main()
{	

	frag_color = vec4(fcolor, 1.0);
} 