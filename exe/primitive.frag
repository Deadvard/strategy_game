#version 430

in vec2 uv;
out vec4 frag_color;

uniform sampler2D texture1;

void main()
{
	frag_color = texture(texture1, uv);
}