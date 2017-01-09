/*
 * GPUVolumeEvaluator.h
 *
 *  Created on: Dec 19, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_EVALUATION_GPU_GENERICVOLUMEEVALUATOR_H_
#define GPUPROCGENETICS_SRC_EVALUATION_GPU_GENERICVOLUMEEVALUATOR_H_

#include "math/vector.h"
#include "math/matrix.h"
#include "Evaluation_IF.h"
#include "VoxelCoords.h"
#include "VoxelSpace.h"
#include "GeneticAlgoConfig.h"

namespace PGA {

class VolumeEvaluationConf;
class GenomeConf;
class GeometryConf;

namespace CPU {


struct Triangle {
	math::float3 v0;
	math::float3 v1;
	math::float3 v2;
	unsigned int shape_id;
};

struct Object {
	int start_offset;
	int num_triangles;
	math::float3 bb_min;
	math::float3 bb_max;
};

class GenericVolumeEvaluator : public Evaluation_IF {
private:
	int									_good_voxels;
	int									_bad_voxels;
	int									_overlap_voxels;
	ProceduralAlgorithm*				_algo;
	VolumeEvaluationConf*				_conf;
	GrammarConf*						_target_grammar;
	GrammarConf*						_grammar;
	GenomeConf*							_genome;
	GeometryConf*						_geometry;
	Genome_IF*							_target_genome;
	VoxelCoords							_voxel_coords;
	bool								_target_initialized;
	float 								_calc_time;
	float 								_prod_time;


	VoxelSpace<unsigned short>	_target_voxels;
	VoxelSpace<unsigned short>	_object_voxels;

	bool ray_triangle_intersect(const float3& origin, const float3& direction, const float3& v0, const float3& v1, const float3& v2, float3& intersection);

	void createTriangles(GeometryBufferInstanced& buffer, std::vector<Triangle>& triangles, std::vector<Object>& objects);
	void compareVoxels(int& good, int& bad, int& overlap);
	void calcVoxels(std::vector<Triangle>& triangles, std::vector<Object>& objects, VoxelSpace<unsigned short>& vs);
	void calcTargetVoxels();
	void calcObjectVoxels(Genome_IF* genome);


public:
	GenericVolumeEvaluator(ProceduralAlgorithm* base);
	void init(ProceduralAlgorithm* base);
	float calcRating(Genome_IF* genome);

	void generateTargetVoxels(std::vector<math::float4>& center);
	void generateVoxels(const int index, std::vector<math::float4>& center);

	float getLastCalculationTime()
	{
		return _calc_time;
	}
	float getLastProductionTime()
	{
		return _prod_time;
	}


};
}  // namespace CPU
}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_EVALUATION_CPU_GenericVolumeEvaluator_H_ */
