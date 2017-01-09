#version 430

layout(location = 0) uniform vec4 color0;

layout(location = 0) out vec4 color;

void main()
{
    color = color0;
}
