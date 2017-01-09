/*
 * SpaceShipEvaluator.h
 *
 *  Created on: Dec 1, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_GRAMMAR_RULES_BOXESVOLUMEEVALUATOR_H_
#define GPUPROCGENETICS_SRC_GRAMMAR_RULES_BOXESVOLUMEEVALUATOR_H_

#include <iostream>
#include <mutex>

#include "Evaluation_IF.h"
#include "Genome_IF.h"
#include "Grammar_IF.h"
#include "Scope.h"
#include "SupportedShapes.h"
#include "ParameterTable.h"
#include "GeneticAlgoConfig.h"
#include "VoxelSpace.h"


namespace PGA {
namespace CPU {

class BoxesVolumeEvaluator: public Evaluation_IF
{
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
	PopulationConf*						_population;
	Genome_IF*							_target_genome;
	bool								_target_initialized;
	bool								_lazy_compare;
	VoxelSpace<unsigned short>			_target_space;
	VoxelSpace<unsigned short>			_object_space;

public:
	BoxesVolumeEvaluator(ProceduralAlgorithm* base) :
		_good_voxels(0),
		_bad_voxels(0),
		_overlap_voxels(0),
		_algo(base),
		_target_genome(nullptr),
		_target_initialized(false)
	{
	}


	void init(ProceduralAlgorithm* base)
	{
		_conf = _algo->get<VolumeEvaluationConf*>("evaluation");
		_target_grammar = _algo->get<GrammarConf*>("target");
		_grammar = _algo->get<GrammarConf*>("grammar");
		_genome = _algo->get<GenomeConf*>("genome");
		_geometry = _algo->get<GeometryConf*>("geometry");
		_population = _algo->get<PopulationConf*>("population");

		_target_space.init(_conf->getBBmin(), _conf->getBBmax(), _conf->getVoxels());
		_target_genome = _genome->factory()->createGenomeFromString(*_target_grammar, _conf->getTargetString());
		_geometry->setGenerate(false);
		_geometry->setEvaluator(this);
		_target_grammar->impl()->createGeometry(_target_genome);

		std::cout << "voxels for target set: " << _target_space.getNumSetVoxels() << std::endl;
		_target_initialized = true;
		_object_space.init(_conf->getBBmin(), _conf->getBBmax(), _conf->getVoxels());
	}

	~BoxesVolumeEvaluator()
	{
		delete _target_genome;
	}

	void addShape(const PGG::Shapes::SBox& s)
	{
		if (_target_initialized) {
				math::float3 size = s.shape.size;
				math::float3 center = math::float3(s.scope.matrix._14, s.scope.matrix._24, s.scope.matrix._34);
				_object_space.addAndCompareBox(size, center, _target_space, _good_voxels, _bad_voxels, _overlap_voxels);
		} else {
			math::float3 size = s.shape.size;
			math::float3 center = math::float3(s.scope.matrix._14, s.scope.matrix._24, s.scope.matrix._34);
			_target_space.addBox(size, center);
		}
	}

	void generateTargetGeometry()
	{
		_target_grammar->impl()->createGeometry(_target_genome);
		std::cerr << "Display Target" << std::endl;

	}

	void generateTargetVoxels(std::vector<math::float4>& center)
	{
		center.clear();
		_target_space.exportVoxels(center);
	}

	void generateVoxels(const int index, std::vector<math::float4>& center);

	void compareShape(const PGG::Shapes::SBox& s)
	{
		math::float3 size = s.shape.size;
		math::float3 center = math::float3(s.scope.matrix._14, s.scope.matrix._24, s.scope.matrix._34);

		int good;
		int bad;

		_target_space.compareBox(size, center, good, bad);

		_good_voxels += good;
		_bad_voxels += bad;

	}

	float calcRating(Genome_IF* genome);

	void finalize() {}

	float getLastCalculationTime()
	{
		// todo
		return 0.0f;
	}
	float getLastProductionTime()
	{
		// todo
		return 0.0f;
	}


};

}  // namespace CPU
}  // namespace PGA




#endif /* GPUPROCGENETICS_SRC_GRAMMAR_RULES_BoxesVolumeEvaluator_H_ */
