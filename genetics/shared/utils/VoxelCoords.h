/*
 * VoxelCoords.h
 *
 *  Created on: Dec 19, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_EVALUATION_GPU_VOXELCOORDS_H_
#define GPUPROCGENETICS_SRC_EVALUATION_GPU_VOXELCOORDS_H_


#include "DualBuild.h"

namespace PGA {

DUAL_BUILD_FUNCTION static inline math::float4x4 translationMatrix(const math::float3& dst)
{
	return math::float4x4(1.0f, 0.0f, 0.0f, dst.x,
			0.0f, 1.0f, 0.0f, dst.y,
			0.0f, 0.0f, 1.0f, dst.z,
			0.0f, 0.0f, 0.0f, 1.0f);
}

DUAL_BUILD_FUNCTION static inline math::float4x4 scale(const math::float3& scale)
{
	math::float4x4 M(scale.x, 0.0f, 0.0f, 0.0f,
			0.0f, scale.y, 0.0f, 0.0f,
			0.0f, 0.0f, scale.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	return M;
}

struct VoxelCoords {
private:
	math::int3	  			_voxels;
	math::float3  			_bb_min;
	math::float3  			_bb_max;
	math::float3			_voxel_size;
	math::float4x4			_m;
	math::float4x4			_m_inv;
	math::float3 			_translation;
	math::float3 			_scale;



public:

	DUAL_BUILD_FUNCTION bool check(const math::int3& coords) {
		if (coords.x < 0 || coords.x >= _voxels.x || coords.y < 0 || coords.y >= _voxels.y || coords.z < 0 || coords.z >= _voxels.z) {
			return false;
		}
		return true;
	}

	DUAL_BUILD_FUNCTION int toIndex(const math::int3& coords)
	{
		if (coords.x < 0 || coords.x >= _voxels.x || coords.y < 0 || coords.y >= _voxels.y || coords.z < 0 || coords.z >= _voxels.z) {
			printf("Voxel Coords [%d, %d, %d] out of bounds!\n", coords.x, coords.y, coords.z);
		}
		return coords.x + _voxels.x * coords.y + _voxels.y * _voxels.x * coords.z;
	}

	DUAL_BUILD_FUNCTION int calcNumVoxels(const math::int3& size)
	{
		return ((size.x) * (size.y) * (size.z));
	}

	DUAL_BUILD_FUNCTION math::int3 getVoxelCoordsInsideBox(const unsigned int& index, const math::int3& size)
	{
		math::int3 coord(0, 0, 0);

		coord.x = index % (size.x);
		coord.y = (index / (size.x)) % (size.y);
		coord.z = (index / ((size.x) * (size.y)));

		return coord;
	}


	DUAL_BUILD_FUNCTION math::int3 toVoxel(const math::float3& coords)
	{
		math::float4 x(coords.x, coords.y, coords.z, 1.0f);
		x = _m * x;
		return math::int3(floor((x.x / x.w) + 0.5f), floor((x.y / x.w) + 0.5f), floor((x.z / x.w) + 0.5f));
	}

	DUAL_BUILD_FUNCTION math::float3 toWorld(const math::int3& coords)
	{
		math::float4 x(coords.x, coords.y, coords.z, 1.0f);
		x = _m_inv * x;
		return math::float3(x.x / x.w, x.y / x.w, x.z / x.w);
	}

	DUAL_BUILD_FUNCTION math::float3 voxelSize() {
		return _voxel_size;
	}


	DUAL_BUILD_FUNCTION math::int3& voxelPerDimension() {
		return _voxels;
	}

	DUAL_BUILD_FUNCTION int numVoxels() {
		return _voxels.x * _voxels.y * _voxels.z;
	}

	DUAL_BUILD_FUNCTION void init(const math::float3& bb_min, const math::float3& bb_max, const math::int3& voxel_per_dimension)
	{
		_bb_min = bb_min;
		_bb_max = bb_max;
		_voxels = voxel_per_dimension;

		_voxel_size = math::float3(	(bb_max.x - bb_min.x) / static_cast<float>(voxel_per_dimension.x),
									(bb_max.y - bb_min.y) / static_cast<float>(voxel_per_dimension.y),
									(bb_max.z - bb_min.z) / static_cast<float>(voxel_per_dimension.z));

		math::float3 translation = (bb_min * -1.0f) - _voxel_size / 2.0f;
		math::float3 scaling(1.0f/_voxel_size.x, 1.0f/_voxel_size.y, 1.0f/_voxel_size.z);


		math::float4x4 scale_w2v(scale(scaling));
		math::float4x4 translate_w2v(translationMatrix(translation));

		_m = scale_w2v * translate_w2v;

		math::float4x4 scale_v2w(scale(_voxel_size));
		math::float4x4 translate_v2w(translationMatrix(-1.0f * translation));

		_m_inv = translate_v2w * scale_v2w;
	}

	/*
	void test()
	{
		std::vector<math::int3> coords_v;
		std::vector<math::float3> coords_w;
		for (int x = 0; x < _voxels.x; ++x) {
			for (int y = 0; y < _voxels.y; ++y) {
				for (int z = 0; z < _voxels.z; ++z) {
					math::int3 voxel_coord(x, y, z);
					math::float3 world_coord = toWorld(voxel_coord);
					coords_w.push_back(world_coord);
					coords_v.push_back(voxel_coord);
				}
			}
		}
		for (int i = 0; i < coords_v.size(); ++i) {
			math::float3 w = coords_w[i];
			math::int3 v = coords_v[i];
			math::int3 w2v = toVoxel(coords_w[i]);
			math::float3 v2w = toWorld(coords_v[i]);
			if (w2v.x != v.x || w2v.y != v.y || w2v.z != v.z) {
				std::cout << "w2v - coords fishy" << std::endl;
				std::cout << "world (" << w.x << ", " << w.y << ", " << w.z << "), voxel (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
			}
			if (v2w.x != w.x || v2w.y != w.y || v2w.z != w.z) {
				std::cout << "v2w - coords fishy" << std::endl;
				std::cout << "world (" << w.x << ", " << w.y << ", " << w.z << "), voxel (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
			}
		}
		std::cout << "voxel coord test done " << std::endl;
	}

*/

};

}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_EVALUATION_GPU_VOXELCOORDS_H_ */
