#version 430

in vec3 normal;
in vec3 view;

layout(location = 0) uniform vec4 color0;

layout(location = 0) out vec4 color;

void main()
{
	vec3 n = normalize(normal);
	vec3 v = normalize(view);

	float lambert = max(dot(n, v), 0.0f);

	color = vec4(lambert) * color0;
}
