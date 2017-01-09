/*
 * GpuVolumeEvaluator.cuh
 *
 *  Created on: Apr 14, 2016
 *      Author: toe
 */

#ifndef GPUVOLUMEEVALUATOR_H_
#define GPUVOLUMEEVALUATOR_H_

#include "DualBuild.h"
#include "math/vector.h"
#include "math/matrix.h"
#include "GeneticAlgoConfig.h"
#include "CuBuffer.h"
#include "Evaluation_IF.h"
#include "VoxelCoords.h"
#include "CUDA/module.h"

namespace PGA {
namespace GPU {

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

class GpuVolumeEvaluator: public Evaluation_IF {
private:
	unsigned int									_target_voxels;
	int									_good_voxels;
	int									_bad_voxels;
	int									_overlap_voxels;
	ProceduralAlgorithm*				_base;
	VolumeEvaluationConf*				_conf;
	GrammarConf*						_target_grammar;
	GrammarConf*						_grammar;
	GenomeConf*							_genome;
	GeometryConf*						_geometry;
	PopulationConf*						_population;
	Genome_IF*							_target_genome;
	VoxelCoords							_h_voxel_coords;
	CUBuffer<VoxelCoords>				_d_voxel_coords;
	bool								_target_initialized;

	int									_runs;

	CUBuffer<unsigned int>			_d_target_voxels;
	CUBuffer<unsigned int>			_d_object_voxels;

	CUBuffer<math::float3x4>			_d_matrices;
	int									_num_matrices;

	CUBuffer<Triangle>				_d_triangles;
	CUBuffer<Object>					_d_objects;
	std::vector<Object>					_h_objects;

	void calcTargetVoxels();
	void calcObjectVoxels();

	float								_calc_time;
	float								_prod_time;

	CUdevice   _device;
	CUcontext  _context;
	CU::unique_module _module;

	CUfunction	_fct_count_voxels;
	CUfunction	_fct_compare_voxels;
	CUfunction	_fct_export_voxels;
	CUfunction	_fct_calc_voxels;

	void initCUDA();
	void finalizeCUDA();

public:
	GpuVolumeEvaluator(ProceduralAlgorithm* base);
	~GpuVolumeEvaluator();
	void init(ProceduralAlgorithm* base);
	void copyOjectsToDevice(GeometryBufferInstanced& buffer);
	void copyMatricesToDevice(GeometryBufferInstanced& buffer);
	void exportVoxels(CUBuffer<unsigned int>& d_voxels, std::vector<math::float4>& center);
	void generateTargetGeometry();
	float calcRating(Genome_IF* genome);
	void generateTargetVoxels(std::vector<math::float4>& center);
	void createObjectVoxelsOnly();
	void generateVoxels(const int index, std::vector<math::float4>& center);

	// GeometryGenerator-hooks
	void finalize() { }
	virtual void addShape(const PGG::Shapes::SBox& s) {}

	float getLastCalculationTime()
	{
		return _calc_time;
	}
	float getLastProductionTime()
	{
		return _prod_time;
	}

};
}  // namespace GPU
}  // namespace PGA

#endif /* GPUVOLUMEEVALUATOR_H_ */
