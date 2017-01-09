#version 430

layout(location = 0) out vec4 color;

in vec4 color_v;


void main()
{
	color = color_v;
}
