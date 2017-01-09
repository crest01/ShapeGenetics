/*
 * VoxelSpace.h
 *
 *  Created on: Dec 15, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_EVALUATION_CPU_VOXELSPACE_H_
#define GPUPROCGENETICS_SRC_EVALUATION_CPU_VOXELSPACE_H_

#include <iostream>
#include <atomic>
#include <vector>
#include "math/vector.h"
#include "math/matrix.h"
#include "VoxelCoords.h"

namespace PGA {
namespace CPU {

template <typename T>
struct atomwrapper
{
  std::atomic<T> _a;

  atomwrapper()
    :_a()
  {}

  atomwrapper(const std::atomic<T> &a)
    :_a(a.load())
  {}

  atomwrapper(const atomwrapper &other)
    :_a(other._a.load())
  {}

  T load() {
	  return _a.load();
  }

  void store(T val) {
	  _a.store(val);
  }

  void inc() {
	  _a ++;
  }

  void clear() {
	  _a.store(0);
  }

  atomwrapper &operator=(const atomwrapper &other)
  {
    _a.store(other._a.load());
  }
};


template <typename VoxelType>
class VoxelSpace {
private:
	std::vector<atomwrapper<VoxelType> > 	_volume;
	std::vector<atomwrapper<VoxelType> > 	_oct_tree;

	VoxelCoords				_coords;
	VoxelCoords				_oct_coords;

	int						_set_voxels;
	bool					_use_octree;
	bool 					_compare_at_insert;



	VoxelType getOctTree(const math::int3& coords)
	{
		return _oct_tree[_oct_coords.toIndex(coords)].load();
	}

	VoxelType get(const math::float3& coords)
	{
		return get(_coords.toVoxel(coords));
	}

	void clear(const int index)
	{
		_volume[index].clear();
	}

	void clear_octTree(const int index)
	{
		_oct_tree[index].clear();
	}


	bool equal(VoxelType a, VoxelType b) {
		return (a == b);
	}

	bool isSet(VoxelType a) {
		return (a != 0);
	}

	void inc(const math::int3& coords)
	{
		int vol_index = _coords.toIndex(coords);
		_volume[vol_index].inc();
		if (_use_octree) {
			int oct_index = _oct_coords.toIndex(coords / 10);
			_oct_tree[oct_index].inc();
		}
	}

	bool isSet(bool a) {
		return a;
	}

public:
	VoxelSpace() {}
	~VoxelSpace() {};


	int getNumSetVoxels() {
		return _set_voxels;
	}

	VoxelType get(const math::int3& coords)
	{
		return _volume[_coords.toIndex(coords)].load();
	}

	void exportVoxels(std::vector<math::float4>& center)
	{
		math::int3 min(0, 0, 0);
		math::int3 max(_coords.voxelPerDimension());

		for (int x = min.x; x < max.x; ++x) {
			for (int y = min.y; y < max.y; ++y) {
				for (int z = min.z; z < max.z; ++z) {
					math::int3 coords(x, y, z);
					VoxelType v = get(coords);
					if (v != 0) {
						math::float3 world(_coords.toWorld(coords));
						math::float4 voxel(world.x, world.y, world.z, static_cast<float>(v));
						center.push_back(voxel);
					}
				}
			}
		}
	}


	void setVoxel(const math::int3& coords) {
		_set_voxels ++;
		inc(coords);
	}

	void addBox(const math::float3& size, const math::float3& center)
	{
		math::int3 min = _coords.toVoxel(center - size / 2.0f);
		math::int3 max = _coords.toVoxel(center + size / 2.0f);

		for (int x = min.x; x <= max.x; ++x) {
			for (int y = min.y; y <= max.y; ++y) {
				for (int z = min.z; z <= max.z; ++z) {
					inc(math::int3(x, y, z));
					_set_voxels ++;
				}
			}
		}
	}

	void addAndCompareBox(const math::float3& size, const math::float3& center, VoxelSpace& other_vs, int& good, int& bad, int& overlap)
	{
		math::int3 min = _coords.toVoxel(center - size / 2.0f);
		math::int3 max = _coords.toVoxel(center + size / 2.0f);

		for (int x = min.x; x <= max.x; ++x) {
			for (int y = min.y; y <= max.y; ++y) {
				for (int z = min.z; z <= max.z; ++z) {
					math::int3 coords(x, y, z);
					if (_coords.check(coords) == false) {
						bad ++;
						continue;
					}
					if (isSet(get(coords))) {
						overlap ++;
					}
					if (isSet(other_vs.get(coords))) {
						good ++;
					}
					else {
						bad ++;
					}
					inc(coords);
					_set_voxels ++;
				}
			}
		}
	}


	void compareBox(const math::float3& size, const math::float3& center, int& inside_voxels, int& outside_voxels)
	{
		int good_count = 0;
		int bad_count = 0;

		math::int3 min = _coords.toVoxel(center - size / 2.0f);
		math::int3 max = _coords.toVoxel(center + size / 2.0f);

		for (int x = min.x; x <= max.x; ++x) {
			for (int y = min.y; y <= max.y; ++y) {
				for (int z = min.z; z <= max.z; ++z) {
					if (!_coords.check(math::int3(x, y, z))) {
						bad_count ++;
					}
					else {
						if (get(math::int3(x, y, z)) == 0) {
							bad_count ++;
						}
						else {
							good_count ++;
						}
					}
				}
			}
		}
		inside_voxels = good_count;
		outside_voxels = bad_count;
	}



	void reset()
	{
		for (int i = 0; i < _coords.numVoxels(); ++i) {
			clear(i);
		}
		if (_use_octree) {
			for (int i = 0; i < _oct_coords.numVoxels(); ++i) {
				clear_octTree(i);
			}
		}
		_set_voxels = 0;
	}


	void init(const VoxelCoords& vc) {
		_use_octree = false;
		_coords = vc;
		_volume.resize(_coords.numVoxels());
	}

	void init(const math::float3& bb_min, const math::float3& bb_max, const math::int3& voxel_per_dimension)
	{
		_coords.init(bb_min, bb_max, voxel_per_dimension);
		_volume.resize(_coords.numVoxels());

		if (_use_octree) {
			_oct_coords.init(bb_min, bb_max, voxel_per_dimension / 10);
			_oct_tree.resize(_oct_coords.numVoxels());
		}
	}

};


} // namespace CPU
}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_EVALUATION_CPU_VOXELSPACE_H_ */
