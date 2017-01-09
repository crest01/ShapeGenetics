/*
 * GPUVolumeEvaluator.cu
 *
 *  Created on: Dec 19, 2015
 *      Author: Karl Haubenwallner
 */

#include "GpuVolumeEvaluator.h"
#include "Grammar_IF.h"
#include "Genome_IF.h"
#include "Algorithm_IF.h"
#include "Mutation_IF.h"
#include "Population_IF.h"
#include "Recombination_IF.h"
#include "Selection_IF.h"
#include "Statistics_IF.h"
#include "GeneticAlgoConfig.h"
#include "GeometryBufferInstanced.h"
#include "Smoothstep.h"
#include <cuda.h>
#include <vector_types.h>
#include "NVRTC/program.h"
#include <string>
#include <fstream>
#include <streambuf>
#include "CuCheckError.h"
#include "CUDA/module.h"
#include "CUDA/launch.h"

namespace PGA {
namespace GPU {

using math::float3;
using math::int3;


bool read_file_as_string(const std::string& filename, std::string& contents)
{
	std::ifstream t(filename);

	if (!t.good()) {
		return false;
	}

	contents.clear();

	t.seekg(0, std::ios::end);
	contents.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	contents.assign((std::istreambuf_iterator<char>(t)),
	            std::istreambuf_iterator<char>());
	return true;
}


void GpuVolumeEvaluator::initCUDA()
{
	size_t     totalGlobalMem;

    int deviceCount = 0;
    CUresult err = cuInit(0);
    int major = 0, minor = 0;

    if (err == CUDA_SUCCESS)
    	cuCheckError(cuDeviceGetCount(&deviceCount));

    if (deviceCount == 0) {
        std::cout << "Error: no devices supporting CUDA" << std::endl;
        exit(-1);
    }

    // get first CUDA device
    cuCheckError(cuDeviceGet(&_device, 0));
    char name[100];
    cuDeviceGetName(name, 100, _device);
    std::cout << "> Using device 0: " << name << std::endl;

    // get compute capabilities and the devicename
    cuCheckError( cuDeviceComputeCapability(&major, &minor, _device) );
    std::cout << "> GPU Device has SM " << major << "." << minor << " compute capability" << std::endl;

    cuCheckError( cuDeviceTotalMem(&totalGlobalMem, _device) );
    std::cout << "> Total amount of global memory: " << (unsigned long long)totalGlobalMem << " bytes" << std::endl;
    std::cout << "> 64-bit Memory Address: " << ((totalGlobalMem > (unsigned long long)4*1024*1024*1024L) ?"YES" : "NO") << std::endl;

    err = cuCtxCreate(&_context, 0, _device);
    if (err != CUDA_SUCCESS) {
        std::cout << "* Error initializing the CUDA context."  << std::endl;
        cuCtxDetach(_context);
        exit(-1);
    }
}

void GpuVolumeEvaluator::finalizeCUDA()
{
    cuCtxDetach(_context);
}

GpuVolumeEvaluator::GpuVolumeEvaluator(ProceduralAlgorithm* base):
				_good_voxels(0),
				_bad_voxels(0),
				_overlap_voxels(0),
				_target_voxels(0),
				_base(base),
				_target_genome(nullptr),
				_target_initialized(false),
				_calc_time(0.0f),
				_prod_time(0.0f)
{
	_runs = 0;
}

GpuVolumeEvaluator::~GpuVolumeEvaluator()
{
	finalizeCUDA();
}



void GpuVolumeEvaluator::generateTargetGeometry()
{
	_target_grammar->impl()->createGeometry(_target_genome);
	std::cerr << "Display Target" << std::endl;
}



float GpuVolumeEvaluator::calcRating(Genome_IF* genome)
{

	auto t0 = std::chrono::high_resolution_clock::now();
	_grammar->impl()->createGeometry(genome);
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);
	_prod_time = time_span.count();


	calcObjectVoxels();

	float rating = 0.0f;

	if (_conf->getSeparateOverlap() == false) {
		float good_rating = _conf->getGoodWeight() * smoothstep(0.0f, _target_voxels, _good_voxels);
		float bad_rating = _conf->getBadWeight() * smoothstep(0.0f, _target_voxels * 2, _bad_voxels + _overlap_voxels);
		float length_rating = _conf->getLengthWeight() * smoothstep(_genome->getMinimalLength(), _genome->getMaximalLength(), genome->length());
		rating = good_rating - bad_rating - length_rating;
	}
	else {
		float good_rating = _conf->getGoodWeight() * smoothstep(0.0f, _target_voxels, _good_voxels);
		float bad_rating = _conf->getBadWeight() * smoothstep(0.0f, _target_voxels*3, _bad_voxels);
		float overlap_rating = _conf->getOverlapWeight() * smoothstep(0.0f, _good_voxels + _bad_voxels, _overlap_voxels);
		float length_rating = _conf->getLengthWeight() * smoothstep(_genome->getMinimalLength(), _genome->getMaximalLength(), genome->length());
		rating = good_rating - bad_rating - overlap_rating - length_rating;
	}

	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span_2 = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	_calc_time = time_span_2.count();

	return rating;
}

void calcNumThreadForAllVoxels(CU::dim& threads, CU::dim& blocks, const int3& voxels)
{

	threads[0] = std::min(voxels.x, 10);
	threads[1] = std::min(voxels.y, 10);
	threads[2] = std::min(voxels.z, 10);

	blocks[0] = voxels.x / threads[0];
	blocks[1] = voxels.y / threads[1];
	blocks[2] = voxels.z / threads[2];

}

void GpuVolumeEvaluator::calcTargetVoxels()
{
	// copy the matrices for the objects to the device
	copyMatricesToDevice(*(_geometry->getInstancedTargetMeshBuffer()));

	if (_num_matrices == 0) {
		std::cout << "Warning: no target geometry created!" << std::endl;
		_target_voxels = 0;
		return;
	}

	CUBuffer<int> d_outside_voxels;
	d_outside_voxels.init(1);
	d_outside_voxels.setToZero();

	CU::dim threads(256, 1, 1);
	CU::dim blocks(_num_matrices, 1, 1);


	CU::call(_fct_calc_voxels, blocks, threads, 0, nullptr,
			_d_objects(),
			_h_objects.size(),
			_d_triangles(),
			_d_matrices(),
			_d_target_voxels(),
			d_outside_voxels(),
			_d_voxel_coords());
	std::cout << "Launch cuda: blocks ( " << blocks[0] << ", " << blocks[1] << ", " << blocks[2] << " ), threads: ( " << threads[0] << ", " << threads[1] << ", " << threads[2] << ") " << std::endl;
	cuCheckError( cuCtxSynchronize() );

	CUBuffer<unsigned int> d_num_voxels;
	d_num_voxels.assureSize(1);
	d_num_voxels.setToZero();

	calcNumThreadForAllVoxels(threads, blocks, _h_voxel_coords.voxelPerDimension());

	CU::call(_fct_count_voxels, blocks, threads, 0, nullptr, _d_target_voxels(), d_num_voxels(), _d_voxel_coords());

	cuCheckError( cuCtxSynchronize() );

	d_num_voxels.copyToHost(&_target_voxels, 1);

	int outside_voxels;
	d_outside_voxels.copyToHost(&outside_voxels, 1);

}

void GpuVolumeEvaluator::init(ProceduralAlgorithm* base)
{

	_target_grammar = _base->get<GrammarConf*>("target");
	_conf = _base->get<VolumeEvaluationConf*>("evaluation");
	_grammar = _base->get<GrammarConf*>("grammar");
	_genome = _base->get<GenomeConf*>("genome");
	_geometry = _base->get<GeometryConf*>("geometry");
	_population = _base->get<PopulationConf*>("population");

	std::string source_folder("FIXME/genetics/eval/gpuvolume/nvrtc_filesC:/ProcGen/gpuprocgenetics/run/nvrtc_files/");
	std::string source;
	std::string source_file("VolumeEvaluator.cu");
	std::string header_file("combined_header.cuh");
	std::string header;

	if (read_file_as_string(source_folder+source_file, source) && read_file_as_string(source_folder+header_file, header)) {

		initCUDA();

		//unique_program createProgram(const char* source, const char* name, const char* const * headers, int num_headers, const char* const * include_names);

		const char* header_names = header_file.c_str();
		const char* header_data = header.c_str();
		NVRTC::unique_program prog = NVRTC::createProgram(source.c_str(), "volume_eval", &header_data, 1, &header_names);

		if (NVRTC::compileProgram(prog, { "-arch=compute_52", "-maxrregcount=63", "-use_fast_math", "-std=c++11"}) != NVRTC_SUCCESS) {
//		if (NVRTC::compileProgram(prog, { "-arch=compute_52", "-maxrregcount=63", "-use_fast_math", "-std=c++11" , "-G", "-lineinfo"}) != NVRTC_SUCCESS) {
				std::cout << "error compiling file " << source_file << std::endl;
			std::cout << "compile log: " <<  NVRTC::getProgramLog(prog) << std::endl;
		}
		else {
			NVRTC::PTXObject ptx = NVRTC::getPTX(prog);
			_module = CU::loadModule(ptx.data.get());
			_fct_count_voxels = CU::getFunction(_module, "_fct_count_voxels");
			_fct_calc_voxels = CU::getFunction(_module, "_fct_calc_voxels");
			_fct_compare_voxels = CU::getFunction(_module, "_fct_compare_voxels");
			_fct_export_voxels = CU::getFunction(_module, "_fct_export_voxels");
		}

	}
	else {
		std::cerr << "error loading CUDA source file " << source_file << std::endl;
		throw std::invalid_argument("CUDA source not found.");
	}

	_h_voxel_coords.init(_conf->getBBmin(), _conf->getBBmax(), _conf->getVoxels());

	_d_voxel_coords.init(1);
	_d_voxel_coords.copyToDevice(&_h_voxel_coords, 1);

	_d_target_voxels.init(_h_voxel_coords.numVoxels());
	_d_target_voxels.setToZero();
	_d_object_voxels.init(_h_voxel_coords.numVoxels());
	_d_object_voxels.setToZero();

	_target_genome = _genome->factory()->createGenomeFromString(*_target_grammar, _conf->getTargetString());
	_target_grammar->impl()->createGeometry(_target_genome);

	// copy the shapes for the target to the device
	copyOjectsToDevice(*(_geometry->getInstancedTargetMeshBuffer()));

	// calculate the target voxels
	calcTargetVoxels();

	// copy the shapes for the object to the device
	copyOjectsToDevice(*(_geometry->getInstancedObjectMeshBuffer()));


	_target_initialized = true;

	if (_conf->getSeparateOverlap() == true) {
		std::cout << "Volume based Evaluation, separate overlap weight: " << std::endl;
		std::cout << "\tgood weight: " << _conf->getGoodWeight() << std::endl;
		std::cout << "\tbad weight: " << _conf->getBadWeight() << std::endl;
		std::cout << "\toverlap weight: " << _conf->getOverlapWeight() << std::endl;
		std::cout << "\tlength weight: " << _conf->getLengthWeight() << std::endl;
		std::cout << "\ttarget voxels: " << _target_voxels << std::endl;
	}
	else {
		std::cout << "Volume based Evaluation: " << std::endl;
		std::cout << "\tgood weight: " << _conf->getGoodWeight() << std::endl;
		std::cout << "\tbad weight: " << _conf->getBadWeight() << std::endl;
		std::cout << "\tlength weight: " << _conf->getLengthWeight() << std::endl;
		std::cout << "\ttarget voxels: " << _target_voxels << std::endl;
	}

}

void GpuVolumeEvaluator::copyOjectsToDevice(GeometryBufferInstanced& buffer)
{
	std::vector<Triangle>				h_triangles;

	h_triangles.clear();
	_h_objects.clear();

	for (int s = 0; s < buffer.getNumShapes(); s++) {
		ObjShape* shape = buffer.getShape(s);

		Object object;
		object.triangle_offset = h_triangles.size();

		for (int i = 0; i < shape->vertices.size(); i+=3) {
			Triangle t;
			t.v0 = shape->vertices[i].p;
			t.v1 = shape->vertices[i+1].p;
			t.v2 = shape->vertices[i+2].p;
			h_triangles.push_back(t);
		}

		object.num_triangles = h_triangles.size() - object.triangle_offset;
		object.matrix_offset = 0;
		_h_objects.push_back(object);
	}

	_d_triangles.assureSize(h_triangles.size());

	_d_triangles.copyToDevice(&h_triangles[0], h_triangles.size());

	cuCheckError( cuCtxSynchronize() );

}

void GpuVolumeEvaluator::copyMatricesToDevice(GeometryBufferInstanced& buffer)
{
	std::vector<math::float3x4> h_matrices;
	h_matrices.clear();
	for (int s = 0; s < buffer.getNumShapes(); ++s) {
		int matrix_offset = h_matrices.size();
		// update the matrix offset for the shape object: i'm assuming here that all matrices for a shape are in order
		_h_objects[s].matrix_offset = matrix_offset;
		int new_matrices = buffer.getMatrixBuffer(s).size();
		if (new_matrices == 0) {
			continue;
		}
		h_matrices.resize(h_matrices.size() + new_matrices);
		buffer.getMatrixBuffer(s).copy(&h_matrices[matrix_offset], 0, buffer.getMatrixBuffer(s).size());
	}

	// copy the matrices to the device
	if (h_matrices.size() > 0) {
		_d_matrices.assureSize(h_matrices.size());
		_d_matrices.copyToDevice(&h_matrices[0], h_matrices.size());
	}

	// update the matrix offset for the objects
	_d_objects.assureSize(_h_objects.size());
	_d_objects.copyToDevice(&_h_objects[0], _h_objects.size());

	_num_matrices = h_matrices.size();

}

void GpuVolumeEvaluator::exportVoxels(CUBuffer<unsigned int>& d_voxel_space, std::vector<math::float4>& center)
{

	// note: here i copy the voxels from the GPU to the host, and then copy them to a OpenGL-Buffer again later.
	// this is terribly inefficient, but the Voxel-display is only supposed to be a debug-feature, so i won't change this for now

	center.clear();

	CUBuffer<unsigned int> d_num_voxels;

	d_num_voxels.init(1);

	d_num_voxels.setToZero();

	CU::dim threads(1, 1, 1);
	CU::dim blocks(1, 1, 1);

	calcNumThreadForAllVoxels(threads, blocks, _h_voxel_coords.voxelPerDimension());

	CU::call(_fct_count_voxels, blocks, threads, 0, nullptr, d_voxel_space(), d_num_voxels(), _d_voxel_coords());

	cuCheckError(cuCtxSynchronize());

	unsigned int num_voxels = 0;
	d_num_voxels.copyToHost(&num_voxels, 1);

	if (num_voxels == 0) {
		return;
	}

	CUBuffer<math::float4> d_centers;
	d_centers.init(num_voxels);

	d_num_voxels.setToZero();

	CU::call(_fct_export_voxels, blocks, threads, 0, nullptr, d_voxel_space(), d_centers(), d_num_voxels(), _d_voxel_coords());

	cuCheckError(cuCtxSynchronize());

	center.resize(num_voxels);
	d_centers.copyToHost(&center[0], num_voxels);
}


void GpuVolumeEvaluator::generateTargetVoxels(std::vector<math::float4>& center)
{
	exportVoxels(_d_target_voxels, center);
}

void GpuVolumeEvaluator::generateVoxels(const int index, std::vector<math::float4>& center)
{
	Genome_IF* genome;
	if (index < 0) {
		genome = _population->impl()->getBestGenome();
	}
	else {
		genome = _population->impl()->getGenome(index);
	}

	_grammar->impl()->createGeometry(genome);

	// calculate the voxels
	createObjectVoxelsOnly();
	
	// export them
	exportVoxels(_d_object_voxels, center);

	// clear the voxels (usually happens in compare_voxels on the gpu)
	_d_object_voxels.setToZero();
}


void GpuVolumeEvaluator::createObjectVoxelsOnly()
{
	copyMatricesToDevice(*(_geometry->getInstancedObjectMeshBuffer()));

	if (_num_matrices == 0) {
		return;
	}

	CUBuffer<int> d_outside_voxels;
	d_outside_voxels.init(1);
	d_outside_voxels.setToZero();

	int outside_voxels = 0;

	CU::dim threads(256, 1, 1);
	CU::dim blocks(_num_matrices, 1, 1);

	CU::call(_fct_calc_voxels, blocks, threads, 0 /* shared mem */, nullptr,
		_d_objects(),
		_h_objects.size(),
		_d_triangles(),
		_d_matrices(),
		_d_object_voxels(),
		d_outside_voxels(),
		_d_voxel_coords());

	cuCheckError(cuCtxSynchronize());

}

void GpuVolumeEvaluator::calcObjectVoxels()
{

	copyMatricesToDevice(*(_geometry->getInstancedObjectMeshBuffer()));

	if (_num_matrices == 0) {
		// no geometry created, don't evaluate it
		_good_voxels = 0;
		_bad_voxels = 0;
		_overlap_voxels = 0;
		return;
	}

	CUBuffer<int> d_outside_voxels;
	d_outside_voxels.init(1);
	d_outside_voxels.setToZero();

	int outside_voxels = 0;

	CU::dim threads(256, 1, 1);
	CU::dim blocks(_num_matrices, 1, 1);

	CU::call(_fct_calc_voxels, blocks, threads, 0 /* shared mem */, nullptr,
							_d_objects(),
							_h_objects.size(),
							_d_triangles(),
							_d_matrices(),
							_d_object_voxels(),
							d_outside_voxels(),
							_d_voxel_coords());

	cuCheckError( cuCtxSynchronize() );

	d_outside_voxels.copyToHost(&outside_voxels, 1);

	CUBuffer<int> d_ratings;
	d_ratings.assureSize(3);
	d_ratings.setToZero();

	calcNumThreadForAllVoxels(threads, blocks, _h_voxel_coords.voxelPerDimension());


	CU::call(_fct_compare_voxels, blocks, threads, 0 /* shared mem */, nullptr,
						_d_target_voxels(),
						_d_object_voxels(),
						d_ratings(),
						_d_voxel_coords());

	cuCheckError( cuCtxSynchronize() );

	int h_ratings[3];

	d_ratings.copyToHost(h_ratings, 3);

	_good_voxels = h_ratings[0];
	_bad_voxels = h_ratings[1];
	_overlap_voxels = h_ratings[2] + outside_voxels;


//    size_t free_byte ;
//    size_t total_byte ;
//
//    cudaCheckError(cudaMemGetInfo( &free_byte, &total_byte )) ;
//
//    std::cout << "after calcObjectVoxels: CUDA memory: " << free_byte / (1024 * 1024) << "MB free, " << total_byte / (1024 * 1024) << "MB used" << std::endl;

}


} // namespace GPU

} // namespace PGA


