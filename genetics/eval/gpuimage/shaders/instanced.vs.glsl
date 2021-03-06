#version 430

#include <camera>

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_tex;

layout(location = 3) in mat4 i_matrix;

out vec3 normal;
out vec3 view;

void main()
{
	view = camera.position - v_position;
	vec4 pos = i_matrix * vec4(v_position, 1.0);
	gl_Position = camera.VP * pos;
		
}
