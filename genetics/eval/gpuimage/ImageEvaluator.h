/*
 * SpaceShipEvaluator.h
 *
 *  Created on: Dec 1, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_GRAMMAR_RULES_IMAGEEVALUATOR_H_
#define GPUPROCGENETICS_SRC_GRAMMAR_RULES_IMAGEEVALUATOR_H_

#include "Evaluation_IF.h"
#include "Genome_IF.h"
#include "Scope.h"
#include "GeneticAlgoConfig.h"


namespace PGA {
namespace GPU {

class ToTextureRendererInstanced;

class ImageEvaluator: public Evaluation_IF
{
private:
	int									_good_pixels;
	int									_bad_pixels;
	ProceduralAlgorithm*				_algo;
	ImageEvaluationConf*				_conf;
	AlgorithmConf*						_base;
	GrammarConf*						_target_grammar;
	GrammarConf*						_grammar;
	GenomeConf*							_genome;
	GeometryConf*						_geometry;
	PopulationConf*						_population;
	bool								_target_initialized;
	ToTextureRendererInstanced*			_renderer;
	float 								_calc_time;
	float 								_prod_time;

public:
	ImageEvaluator(ProceduralAlgorithm* base);

	~ImageEvaluator();

	void init(ProceduralAlgorithm* base);

	void addShape(const PGG::Shapes::SBox& s) {}

	void finalize() {}

	void generateTargetGeometry() {};
	void generateTargetVoxels(std::vector<math::float4>& voxels) {}
	void generateVoxels(const int index, std::vector<math::float4>& voxels) {}

	float getLastCalculationTime()
	{
		return _calc_time;
	}
	float getLastProductionTime()
	{
		return _prod_time;
	}

	float calcRating(Genome_IF* genome);

};

}  // namespace GPU
}  // namespace PGA




#endif /* GPUPROCGENETICS_SRC_GRAMMAR_RULES_IMAGEEVALUATOR_H_ */
