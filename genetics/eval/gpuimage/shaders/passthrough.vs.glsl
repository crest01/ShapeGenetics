#version 430

#include <camera>

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;

out vec3 normal;
out vec3 view;

void main()
{
	normal = v_normal;
	view = camera.position - v_position;

	gl_Position = camera.VP * vec4(v_position, 1.0f);
}
