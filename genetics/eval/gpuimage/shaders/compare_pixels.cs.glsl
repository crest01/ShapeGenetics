#version 430

layout(local_size_x = 8, local_size_y = 8) in;

layout(std430, binding = 0) buffer pixels {
	uint good_pixels;
	uint bad_pixels;
};

layout(rgba8ui, binding = 0) uniform uimage2D texture_target_location;
layout(rgba8ui, binding = 0) uniform uimage2D texture_object_location;

uniform uint texture_width;
uniform uint texture_height;

shared uvec2 values[8 * 8];

void main()
{

	ivec2 texcoord;
	
	uint index = gl_LocalInvocationID.y * 8 + gl_LocalInvocationID.x;

	uvec2 lvalue;

	texcoord.x = int(gl_GlobalInvocationID.x);
	texcoord.y = int(gl_GlobalInvocationID.y);
	
			
	uint target = imageLoad(texture_target_location, texcoord).r;
	uint generated = imageLoad(texture_object_location, texcoord).r;

	bool v_t = (target.r == 0);
	bool v_o = (generated.r == 0);
	bool inside = (texcoord.x < texture_width && texcoord.y < texture_height);
			
	// if inside the texture coords: 
	// 		v_t ... target pixel is set
	//		v_o ... object pixel is set
	//    	good: target and object set
	//      bad: target not set, but object set 

	lvalue.x = int( (v_t && v_o) && inside);
	lvalue.y = int( (v_o && !v_t) && inside);

	values[index] = lvalue;

	uint offset = (8*8) / 2;

	for(; offset >  0; offset /= 2) {
		barrier();
		if(index < offset) {
			values[index].x += values[index + offset].x;
			values[index].y += values[index + offset].y;
		}
	}
	
	if (index == 0) {
		atomicAdd(good_pixels, values[index].x);
		atomicAdd(bad_pixels, values[index].y);
	}
}
