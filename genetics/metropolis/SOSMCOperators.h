/*
 * Mutation.h
 *
 *  Created on: Nov 9, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_SOSMC_OPERATORS_H_
#define GPUPROCGENETICS_SRC_SOSMC_OPERATORS_H_

#include <iostream>

#include "GeneticAlgoConfig.h"
#include "Genome_IF.h"
#include "Mutation_IF.h"
#include "Population_IF.h"
#include "Selection_IF.h"
#include "SymbolManager.h"

namespace PGA {

class SoSmcSelection: public Selection_IF
{

private:
	ProceduralAlgorithm* _algo;
	PopulationConf* _population;
	SelectionConf*  _conf;
	float 			_fitness_offset;
	float			_sum_fitness;
public:
	SoSmcSelection(ProceduralAlgorithm* base):
		_algo(base)
	{}

	const std::string getName() { return "Roulette"; }

	void prepareForSelection()
	{
		// get the negative fitness of the worst individual
		_fitness_offset = _population->impl()->getGenome(_population->impl()->activeGenerationSize() -1)->getEvalPoints() * -1;

		// i only need this offset if the worst fitness is negative
		if (_fitness_offset > 0.0) {
			_fitness_offset = 0.0f;
		}

		// normalize the fitness so they sum to 1
		_sum_fitness = 0.0f;
		for (int i = 0; i < _population->impl()->activeGenerationSize(); ++i) {
			_sum_fitness += (_fitness_offset + _population->impl()->getGenome(i)->getEvalPoints());
		}
	}

	void init(ProceduralAlgorithm* base)
	{
		_population = _algo->get<PopulationConf*>("population");
		_conf = _algo->get<SelectionConf*>("resampling");
	}

	Genome_IF* selectGenome()
	{
		float random_val = getRandomValue(0.0f, 1.0f);
		float relative_fitness = 0.0f;

		Genome_IF* candidate = nullptr;

		// start summing the fitness values from the lowest until we reach the random value
		for (int i = 0; i < _population->impl()->activeGenerationSize(); ++i) {
			candidate = _population->impl()->getGenome(i);
			float current_fitness = (_fitness_offset + candidate->getEvalPoints()) * 1.0f/_sum_fitness;
			relative_fitness += current_fitness;
			if (random_val <= relative_fitness) {
				break;
			}
		}
		if (candidate == nullptr) {
			std::cout << "nothing selected! " << std::endl;
		}
		return candidate;
	}


};


class SoSmcGrowMutation: public Mutation_IF {
private:
	ProceduralAlgorithm* _algo;
	MutationConf*  _conf;

public:
	SoSmcGrowMutation(ProceduralAlgorithm* base):
		_algo(base)
	{ }
	~SoSmcGrowMutation() {}

	void init(ProceduralAlgorithm* base);

	bool doMutation() {
		return true;
	}

	// Add a single element
	void mutate(GrammarConf& grammar, Genome_IF* genome);

	// do a grow-mutation
	void grow(GrammarConf& conf, Genome_IF* genome)
	{
		return mutate(conf, genome);
	}

	void do_move(GrammarConf& grammar, Genome_IF* genome, Gene_IF* gene)
	{
		throw(std::runtime_error("do_move not implemented for this Mutation-Operator!"));
	}

};


}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_SOSMC_OPERATORS_H_ */
