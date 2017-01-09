#version 430

#include <camera>

layout(location = 0) in vec4 v_voxel;


out vec4 color_v;

void main()
{
	vec3 v_position = v_voxel.xyz;
	vec3 view = camera.position - v_position;

	gl_Position = camera.VP * vec4(v_position, 1.0f);
	
	float type = v_voxel.w;

	if (type == 1.0f) {
		color_v = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else if (type == 2.0f) {
		color_v = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else {
		color_v = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	}
	gl_PointSize = clamp(2.0f, 20.0f, 20.0 * 1.0/length(view));

}
