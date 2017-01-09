/*
 * VolumeEvaluator.cu
 *
 *  Created on: Dec 20, 2015
 *      Author: Karl Haubenwallner
 */

#include "combined_header.cuh"

#define EPSILON 0.0000001f

struct Object {
	unsigned int triangle_offset;
	unsigned int num_triangles;
	unsigned int matrix_offset;
};

struct Triangle {
	math::float3 v0;
	math::float3 v1;
	math::float3 v2;
};


__device__ bool ray_triangle_intersect(const math::float3& O, const math::float3& D, const math::float3& v0, const math::float3& v1, const math::float3& v2)
{

	// Moeller / Trumbore ray-triangle intersection algorithm

	math::float3 E1 = v1 - v0;
	math::float3 E2 = v2 - v0;

	// q = P
	math::float3 P = cross(D, E2);

	// a = det
	float det = dot(E1, P);

	if ((det > -EPSILON && det < EPSILON)) {
		return false;;
	}

	math::float3 T = (O - v0);
	math::float3 Q = cross(T, E1);

	math::float3 result(dot(E2, Q), dot(T, P), dot(D, Q));
	result = result * 1.0f/det;

	bool miss = false;

	miss = (result.y < 0.0f || result.y > 1.0f);

	miss |= (result.z < 0.0f || (result.y + result.z) > 1.0f);

	miss |= (result.x < 0.0f);

	return !miss;
}

#ifdef not_used
__global__ void d_calc_voxels_raystabbing(	const Triangle* triangles,
		const unsigned int num_triangles,
		unsigned int* voxel_outline,
		unsigned int* voxel_volume,
		math::VoxelCoords *vc)
{

	math::int3 voxel;
	voxel.x = threadIdx.x + blockIdx.x * blockDim.x;
	voxel.y = threadIdx.y + blockIdx.y * blockDim.y;
	voxel.z = 0;

	// find all triangles that intersect with this voxel line

	math::float3 origin = vc->toWorld(voxel);
	math::float3 direction(0.0f, 0.0f, 1.0f);

	__syncthreads();
	for (int i = 0; i < num_triangles; ++i) {
		math::float3 intersection;

		if (ray_triangle_intersect(origin, direction, triangles[i].v0, triangles[i].v1, triangles[i].v2, intersection)) {
			int index = vc->toIndex(vc->toVoxel(intersection));
			voxel_outline[index] = 1;
		}
	}

	__syncthreads();


	// do first flood-fill

	unsigned short val = 0;
	for (voxel.z = 0; voxel.z < vc->voxelPerDimension().z; voxel.z++) {

		int index = vc->toIndex(voxel);

		val += voxel_outline[index];

		voxel_volume[index] = clamp(val, 0, 1);

		val = val % 2;
	}

	// second flood fill from the other direction, to prevent single-voxel-triangles from flood-filling to the end
	val = 0;
	for (voxel.z = vc->voxelPerDimension().z -1; voxel.z >= 0; voxel.z--) {
		int index = vc->toIndex(voxel);

		val += voxel_outline[index];

		voxel_volume[index] += clamp(val, 0, 1);

		val = val % 2;
	}
}

#endif

__global__ void d_calc_voxels_objects(
		const Object* objects,
		int num_objects,
		Triangle* triangles,
		math::float3x4* matrices,
		unsigned int* voxel_volume,
		int *outside_voxels,
		math::VoxelCoords *vc)
{

	math::VoxelCoords local_vc = *vc;
	math::float3x4 instance = matrices[blockIdx.x];
	Object object = objects[0];
	for (int i = 0; i < num_objects; ++i) {
		if (objects[i].triangle_offset < blockIdx.x) {
			object = objects[i];
		}
	}
	__shared__ math::int3 s_min;
	__shared__ math::int3 s_max;

	Triangle triangle = triangles[object.triangle_offset];

	// apply the matrix to the triangle
	triangle.v0 = instance * math::float4(triangle.v0, 1.0f);
	triangle.v1 = instance * math::float4(triangle.v1, 1.0f);
	triangle.v2 = instance * math::float4(triangle.v2, 1.0f);

	if (threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0) {
		s_min = local_vc.toVoxel(triangle.v0);
		s_max = local_vc.toVoxel(triangle.v0);
	}

	__syncthreads();


	// calc the bounding box of the object
	if (threadIdx.y == 0) {

		for (int offset = threadIdx.x; offset < object.num_triangles; offset +=  blockDim.x) {
			int triangle_index = object.triangle_offset + offset;
			Triangle t = triangles[triangle_index];

			// apply the matrix to the triangle
			t.v0 = instance * math::float4(t.v0, 1.0f);
			t.v1 = instance * math::float4(t.v1, 1.0f);
			t.v2 = instance * math::float4(t.v2, 1.0f);

			math::int3 v0 = local_vc.toVoxel(t.v0);
			math::int3 v1 = local_vc.toVoxel(t.v1);
			math::int3 v2 = local_vc.toVoxel(t.v2);

			atomicMin(&s_min.x, min(v0.x, min(v1.x, v2.x)));
			atomicMin(&s_min.y, min(v0.y, min(v1.y, v2.y)));
			atomicMin(&s_min.z, min(v0.z, min(v1.z, v2.z)));

			atomicMax(&s_max.x, max(v0.x, max(v1.x, v2.x)));
			atomicMax(&s_max.y, max(v0.y, max(v1.y, v2.y)));
			atomicMax(&s_max.z, max(v0.z, max(v1.z, v2.z)));

		}
	}
	__syncthreads();

	// find out how many voxels we have to check
	math::int3 span = (s_max - s_min) + math::int3(1, 1, 1);
	int num_voxels = local_vc.calcNumVoxels(span);
	math::float3 ray_direction(0.0f, 1.0f, 0.0f);
	
	//if(threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0) {
	//	printf("span(%d, %d, %d), num voxels = %d, s_min(%d, %d, %d)\n", span.x, span.y, span.z, num_voxels, s_min.x, s_min.y, s_min.z);
	//}
	

	for (int index = threadIdx.x; index < num_voxels; index += blockDim.x) {
		math::int3 voxel_coord = local_vc.getVoxelCoordsInsideBox(index, span) + s_min;
		if (local_vc.check(voxel_coord) == false) {
			atomicAdd(outside_voxels, 1);
		}
		else {
			//int voxel_index = local_vc.toIndex(voxel_coord);
			math::float3 voxel_center = local_vc.toWorld(voxel_coord);
			bool hits = false;
			for (int i = 0; i < object.num_triangles; ++i) {
				Triangle t = triangles[object.triangle_offset + i];
				// apply the matrix to the triangle
				t.v0 = instance * math::float4(t.v0, 1.0f);
				t.v1 = instance * math::float4(t.v1, 1.0f);
				t.v2 = instance * math::float4(t.v2, 1.0f);
				// count triangle check if the voxel is inside the object
				if (ray_triangle_intersect(voxel_center, ray_direction, t.v0, t.v1, t.v2) == true) {
					hits  = !hits;
				}
			}
			if (hits) {
				//printf("%d %d %d -> %d\n", voxel_coord.x, voxel_coord.y, voxel_coord.z, local_vc.toIndex(voxel_coord));
				atomicAdd(&voxel_volume[local_vc.toIndex(voxel_coord)], 1);
			}
		}
	}

//	__syncthreads();

}


__global__ void d_count_voxels(unsigned int* voxels, unsigned int* num_voxel, math::VoxelCoords* vc)
{
	math::int3 voxel;
	voxel.x = threadIdx.x + blockIdx.x * blockDim.x;
	voxel.y = threadIdx.y + blockIdx.y * blockDim.y;
	voxel.z = threadIdx.z + blockIdx.z * blockDim.z;

	__shared__ int num;

	int index = vc->toIndex(voxel);
	if (voxels[index] > 0) {
		atomicAdd(&num, 1);
	}
	__syncthreads();

	if (threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0) {
		atomicAdd(num_voxel, num);
	}
}


__global__ void d_compare_voxels(unsigned int* voxels_target, unsigned int* voxels_object, int* ratings, math::VoxelCoords* vc)
{
	math::int3 voxel;
	voxel.x = threadIdx.x + blockIdx.x * blockDim.x;
	voxel.y = threadIdx.y + blockIdx.y * blockDim.y;
	voxel.z = threadIdx.z + blockIdx.z * blockDim.z;

	__shared__ int good;
	__shared__ int bad;
	__shared__ int overlap;

	if (threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0) {
		good = 0;
		bad = 0;
		overlap = 0;
	}

	__syncthreads();

	int index = vc->toIndex(voxel);

	bool v_t = (voxels_target[index] != 0);
	bool v_o = (voxels_object[index] != 0);
	bool v_overlap = (voxels_object[index] > 1);

	voxels_object[index] = 0;

	if (v_t && v_o) {
		atomicAdd(&good, 1);
	}
	else if (v_o && !v_t) {
		atomicAdd(&bad, 1);
	}
	if (v_overlap) {
		atomicAdd(&overlap, 1);
	}

	__syncthreads();

	if (threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0) {
		atomicAdd(&ratings[0], good);
		atomicAdd(&ratings[1], bad);
		atomicAdd(&ratings[2], overlap);
	}

}

__global__ void d_export_voxels(unsigned int* voxel_space, math::float4* voxels, unsigned int* num_voxels, math::VoxelCoords* vc)
{
	math::int3 voxel;
	voxel.x = threadIdx.x + blockIdx.x * blockDim.x;
	voxel.y = threadIdx.y + blockIdx.y * blockDim.y;
	voxel.z = threadIdx.z + blockIdx.z * blockDim.z;

	unsigned int index = vc->toIndex(voxel);
	unsigned int voxel_val = voxel_space[index];
	if (voxel_val > 0) {
		unsigned int offset = atomicAdd(num_voxels, 1);
		math::float3 world_coords = vc->toWorld(voxel);
		math::float4 voxel_data;
		voxel_data.x = world_coords.x;
		voxel_data.y = world_coords.y;
		voxel_data.z = world_coords.z;
		voxel_data.w = voxel_val * 1.0f;
		voxels[offset] = voxel_data;
	}
}

extern "C" {


__global__  void _fct_count_voxels(unsigned int* voxels, unsigned int* num_voxel, math::VoxelCoords *vc) {
	d_count_voxels(voxels, num_voxel, vc);
}

__global__ void _fct_compare_voxels(unsigned int* voxels_target, unsigned int* voxels_object, int* ratings, math::VoxelCoords *vc) {
	d_compare_voxels(voxels_target, voxels_object, ratings, vc);
}

__global__ void _fct_export_voxels(unsigned int* voxels, math::float4* voxel_centers, unsigned int* num_voxels, math::VoxelCoords *vc) {
	d_export_voxels(voxels, voxel_centers, num_voxels, vc);
}

__global__ void _fct_calc_voxels(
		const Object* objects,
		int num_objects,
		Triangle* triangles,
		math::float3x4* matrices,
		unsigned int* voxel_volume,
		int *outside_voxels,
		math::VoxelCoords* vc)
{
	d_calc_voxels_objects(objects, num_objects, triangles, matrices, voxel_volume, outside_voxels, vc);
}

}

