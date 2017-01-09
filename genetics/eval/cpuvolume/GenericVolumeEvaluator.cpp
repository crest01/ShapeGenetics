/*
 * GPUVolumeEvaluator.cu
 *
 *  Created on: Dec 19, 2015
 *      Author: Karl Haubenwallner
 */

#include "GenericVolumeEvaluator.h"
#include "GeneticAlgoConfig.h"
#include "Grammar_IF.h"
#include "Genome_IF.h"
#include "Algorithm_IF.h"
#include "Mutation_IF.h"
#include "Population_IF.h"
#include "Recombination_IF.h"
#include "Selection_IF.h"
#include "Statistics_IF.h"
#include "GeometryBufferInstanced.h"
#include "Smoothstep.h"


namespace PGA {
namespace CPU {

#define EPSILON 0.0000001f
using math::float3;
using math::int3;
using ::PGA::smoothstep;

GenericVolumeEvaluator::GenericVolumeEvaluator(ProceduralAlgorithm* base):
		_good_voxels(0),
		_bad_voxels(0),
		_overlap_voxels(0),
		_target_genome(nullptr),
		_target_initialized(false),
		_algo(base)
{

}
		

bool GenericVolumeEvaluator::ray_triangle_intersect(const float3& O, const float3& D, const float3& v0, const float3& v1, const float3& v2, float3& intersection)
{

	// Moeller / Trumbore ray-triangle intersection algorithm

	float3 E1 = v1 - v0;
	float3 E2 = v2 - v0;

	// q = P
	float3 P = cross(D, E2);

	// a = det
	float det = dot(E1, P);

	if ((det > -EPSILON && det < EPSILON)) {
		return false;;
	}

	float3 T = (O - v0);
	float3 Q = cross(T, E1);

	float3 result(dot(E2, Q), dot(T, P), dot(D, Q));
	result = result * 1.0f/det;

	if(result.y < 0.0f || result.y > 1.0f) {
		return false;
	}

	if (result.z < 0.0f || (result.y + result.z) > 1.0f) {
		return false;
	}

	if (result.x < 0.0f) {
		return false;
	}

	intersection = O + result.x * D;

	return true;
}

void GenericVolumeEvaluator::calcVoxels(std::vector<Triangle>& triangles, std::vector<Object>& objects, VoxelSpace<unsigned short>& vs)
{

	if (triangles.size() == 0) {
		return;
	}


	int tested_objects = 0;
	for (int index = 0; index < objects.size(); ++index) {

		Object* object = &objects[index];
		int hit_voxels = 0;
		int tested_voxels = 0;
		int invalid_voxels = 0;
		tested_objects ++;
		math::int3 vbb_min = _voxel_coords.toVoxel(object->bb_min);
		math::int3 vbb_max = _voxel_coords.toVoxel(object->bb_max);

//		std::cout << "Bounding box: (" << object->bb_min.x << ", " << object->bb_min.y << ", " << object->bb_min.z << ") (" << object->bb_max.x << ", " << object->bb_max.y << ", " << object->bb_max.z << ")" << std::endl;
//		std::cout << "Voxel coords: (" << vbb_min.x << ", " << vbb_min.y << ", " << vbb_min.z << ") (" << vbb_max.x << ", " << vbb_max.y << ", " << vbb_max.z << ")" << std::endl;

		for (int x = vbb_min.x; x <= vbb_max.x; ++x) {
			for (int y = vbb_min.y; y <= vbb_max.y; ++y) {
				for (int z = vbb_min.z; z <= vbb_max.z; ++z) {
					int3 voxel(x, y, z);
					tested_voxels ++;

					if (!_voxel_coords.check(voxel)) {
						invalid_voxels ++;
						continue;
					}

					// find all triangles that intersect with this voxel line
					float3 origin = _voxel_coords.toWorld(voxel);

					// direction of the ray used for intersection
					float3 direction(0.0f, 0.8f, 0.2f);

					int inside = 0;
					int start_offset = object->start_offset;
					int num_trigs = object->num_triangles;
					for (int i = 0; i < num_trigs; ++i) {

						float3 intersection;

						if (ray_triangle_intersect(origin, direction, triangles[start_offset + i].v0, triangles[start_offset + i].v1, triangles[start_offset + i].v2, intersection)) {
							inside++;
						}
					}
					if (inside % 2 == 1) {
						vs.setVoxel(voxel);
						hit_voxels ++;
					}
				}
			}
		}
//		std::cout << "Tested Object: " << tested_objects << ", Tested voxels: " << tested_voxels << ", hit voxels " << hit_voxels << ", invalid voxels: " << invalid_voxels << std::endl;

	}

}

void GenericVolumeEvaluator::compareVoxels(int& good, int& bad, int& overlap)
{

	good = 0;
	bad = 0;
	overlap = 0;

	for (int x = 0; x < _voxel_coords.voxelPerDimension().x; ++x) {
		for (int y = 0; y < _voxel_coords.voxelPerDimension().y; ++y) {
			for (int z = 0; z < _voxel_coords.voxelPerDimension().z; ++z) {
				int3 voxel(x, y, z);
				unsigned short v_t = _target_voxels.get(voxel);
				unsigned short v_o = _object_voxels.get(voxel);

				if (v_t > 0) {
					if (v_o > 0) {
						good ++;
					}
					else {
						bad ++;
					}
				}
				else {
					if (v_o > 0) {
						bad ++;
					}
				}
			}
		}
	}
}



void GenericVolumeEvaluator::calcTargetVoxels()
{
	/* clear the target voxels */
	_target_voxels.reset();

	/* create the geometry given the grammar string (usually a single object) */
	_target_genome = _genome->factory()->createGenomeFromString(*_target_grammar, _conf->getTargetString());
	_target_grammar->impl()->createGeometry(_target_genome);

	std::vector<Triangle>		triangles;
	std::vector<Object>			objects;

	/* create all triangles from the instanced geometry */
	createTriangles(*(_geometry->getInstancedTargetMeshBuffer()), triangles, objects);

	/* calculate voxels given the triangles */
	calcVoxels(triangles, objects, _target_voxels);

	std::cout << "calc_voxels for target done!" << std::endl;
}

void GenericVolumeEvaluator::calcObjectVoxels(Genome_IF* genome)
{

	auto t0 = std::chrono::high_resolution_clock::now();
	// clear the object voxels
	_object_voxels.reset();

	// generate the geometry for the genome
	_grammar->impl()->createGeometry(genome);

	std::vector<Triangle>	triangles;
	std::vector<Object>		objects;

	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);
	_prod_time = time_span.count();


	// create triangles from the instanced geometry
	createTriangles(*(_geometry->getInstancedObjectMeshBuffer()), triangles, objects);

	// calculate the voxels using ray tracing
	calcVoxels(triangles, objects, _object_voxels);

	// compare the calculated voxels
	compareVoxels(_good_voxels, _bad_voxels, _overlap_voxels);

	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span_2 = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	_calc_time = time_span_2.count();


}

void GenericVolumeEvaluator::generateTargetVoxels(std::vector<math::float4>& center)
{
	math::int3 voxels = _voxel_coords.voxelPerDimension();

	for (int x = 0; x < voxels.x; ++x) {
		for (int y = 0; y < voxels.y; ++y) {
			for (int z = 0; z < voxels.z; ++z) {
				math::int3 coords(x, y, z);
				if (_target_voxels.get(coords) != 0) {
					math::float4 world_coords(_voxel_coords.toWorld(coords), 1.0f);
					center.push_back(world_coords);
				}
			}
		}
	}
}
void GenericVolumeEvaluator::generateVoxels(const int index, std::vector<math::float4>& center)
{
	PopulationConf* population = _algo->get<PopulationConf*>("population");

	Genome_IF* genome = nullptr;
	if (index < 0) {
		genome = population->impl()->getBestGenome();
	}
	else {
		genome = population->impl()->getGenome(index);
	}

	calcObjectVoxels(genome);

	math::int3 voxels = _voxel_coords.voxelPerDimension();

	for (int x = 0; x < voxels.x; ++x) {
		for (int y = 0; y < voxels.y; ++y) {
			for (int z = 0; z < voxels.z; ++z) {
				math::int3 coords(x, y, z);
				unsigned short v = _object_voxels.get(coords);
				if (v != 0) {
					math::float4 world_coords(_voxel_coords.toWorld(coords), v*1.0f);
					center.push_back(world_coords);
				}
			}
		}
	}
}



void GenericVolumeEvaluator::init(ProceduralAlgorithm* base)
{
	_target_grammar = _algo->get<GrammarConf*>("target");
	_conf = _algo->get<VolumeEvaluationConf*>("evaluation");
	_grammar = _algo->get<GrammarConf*>("grammar");
	_genome = _algo->get<GenomeConf*>("genome");
	_geometry = _algo->get<GeometryConf*>("geometry");

	_voxel_coords.init(_conf->getBBmin(), _conf->getBBmax(), _conf->getVoxels());

	_target_voxels.init(_voxel_coords);

	calcTargetVoxels();

	_target_initialized = true;

	_object_voxels.init(_voxel_coords);

	if (_conf->getSeparateOverlap() == true) {
		std::cout << "Volume based Evaluation, separate overlap weight: " << std::endl;
		std::cout << "\tgood weight: " << _conf->getGoodWeight() << std::endl;
		std::cout << "\tbad weight: " << _conf->getBadWeight() << std::endl;
		std::cout << "\toverlap weight: " << _conf->getOverlapWeight() << std::endl;
		std::cout << "\tlength weight: " << _conf->getLengthWeight() << std::endl;
		std::cout << "\ttarget voxels: " << _target_voxels.getNumSetVoxels() << std::endl;
	}
	else {
		std::cout << "Volume based Evaluation: " << std::endl;
		std::cout << "\tgood weight: " << _conf->getGoodWeight() << std::endl;
		std::cout << "\tbad weight: " << _conf->getBadWeight() << std::endl;
		std::cout << "\tlength weight: " << _conf->getLengthWeight() << std::endl;
		std::cout << "\ttarget voxels: " << _target_voxels.getNumSetVoxels() << std::endl;
	}

}

void GenericVolumeEvaluator::createTriangles(GeometryBufferInstanced& buffer, std::vector<Triangle>& triangles, std::vector<Object>& objects)
{

	objects.clear();

	for (int s = 0; s < buffer.getNumShapes(); ++s) {

		std::vector<math::float3x4> mats;
		mats.resize(buffer.getMatrixBuffer(s).size());
		buffer.getMatrixBuffer(s).copy(&mats[0], 0, mats.size());

		ObjShape* shape = buffer.getShape(s);
		for (int instance = 0; instance < mats.size(); ++instance) {
			Object object;
			object.start_offset = triangles.size();
			object.num_triangles = 0;
			object.bb_min = math::float3(0.0f, 0.0f, 0.0f);
			object.bb_max = math::float3(0.0f, 0.0f, 0.0f);
			bool first = true;
			for (int i = 0; i < shape->vertices.size(); i+=3) {
				Triangle t;
				t.v0 = mats[instance] * math::float4(shape->vertices[i].p, 1.0f);
				t.v1 = mats[instance] * math::float4(shape->vertices[i+1].p, 1.0f);
				t.v2 = mats[instance] * math::float4(shape->vertices[i+2].p, 1.0f);
				if (first) {
					object.bb_min = t.v0;
					object.bb_max = t.v0;
					first = false;
				}
				object.bb_min.x = std::min(t.v0.x, object.bb_min.x);
				object.bb_min.x = std::min(t.v1.x, object.bb_min.x);
				object.bb_min.x = std::min(t.v2.x, object.bb_min.x);

				object.bb_min.y = std::min(t.v0.y, object.bb_min.y);
				object.bb_min.y = std::min(t.v1.y, object.bb_min.y);
				object.bb_min.y = std::min(t.v2.y, object.bb_min.y);

				object.bb_min.z = std::min(t.v0.z, object.bb_min.z);
				object.bb_min.z = std::min(t.v1.z, object.bb_min.z);
				object.bb_min.z = std::min(t.v2.z, object.bb_min.z);

				object.bb_max.x = std::max(t.v0.x, object.bb_max.x);
				object.bb_max.x = std::max(t.v1.x, object.bb_max.x);
				object.bb_max.x = std::max(t.v2.x, object.bb_max.x);

				object.bb_max.y = std::max(t.v0.y, object.bb_max.y);
				object.bb_max.y = std::max(t.v1.y, object.bb_max.y);
				object.bb_max.y = std::max(t.v2.y, object.bb_max.y);

				object.bb_max.z = std::max(t.v0.z, object.bb_max.z);
				object.bb_max.z = std::max(t.v1.z, object.bb_max.z);
				object.bb_max.z = std::max(t.v2.z, object.bb_max.z);

				triangles.push_back(t);
				object.num_triangles ++;
			}
			objects.push_back(object);
		}
	}
}


float GenericVolumeEvaluator::calcRating(Genome_IF* genome)
{

	calcObjectVoxels(genome);

	float rating = 0.0f;

	if (_conf->getSeparateOverlap() == true) {
		float good_rating = _conf->getGoodWeight() * smoothstep(0.0f, _target_voxels.getNumSetVoxels(), _good_voxels);
		float bad_rating = _conf->getBadWeight() * smoothstep(0.0f, _target_voxels.getNumSetVoxels() * 2, _bad_voxels + _overlap_voxels);
		float length_rating = _conf->getLengthWeight() * smoothstep(_genome->getMinimalLength(), _genome->getMaximalLength(), genome->length());
		rating = good_rating - bad_rating - length_rating;
	}
	else {
		float good_rating = _conf->getGoodWeight() * smoothstep(0.0f, _target_voxels.getNumSetVoxels(), _good_voxels);
		float bad_rating = _conf->getBadWeight() * smoothstep(0.0f, _target_voxels.getNumSetVoxels() * 2, _bad_voxels);
		float overlap_rating = _conf->getOverlapWeight() * smoothstep(0.0f, _target_voxels.getNumSetVoxels(), _overlap_voxels);
		float length_rating = _conf->getLengthWeight() * smoothstep(_genome->getMinimalLength(), _genome->getMaximalLength(), genome->length());
		rating = good_rating - bad_rating - overlap_rating - length_rating;
	}


//	std::cout << "Object rated" << std::endl;

	return rating;
}


		
} // namespace CPU
		
} // namespace PGA


