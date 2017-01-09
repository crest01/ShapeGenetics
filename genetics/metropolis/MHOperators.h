/*
 * Mutation.h
 *
 *  Created on: Nov 9, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_METROPOLIS_MH_OPERATORS_H_
#define GPUPROCGENETICS_SRC_METROPOLIS_MH_OPERATORS_H_

#include <iostream>

#include "GeneticAlgoConfig.h"
#include "Genome_IF.h"
#include "Mutation_IF.h"
#include "SymbolManager.h"

namespace PGA {

class DiffusionMove: public Mutation_IF {
private:
	ProceduralAlgorithm* _algo;
	MutationConf*  	_conf;
	float 			_logProb;

public:
	DiffusionMove(ProceduralAlgorithm* base):
		_algo(base),
		_logProb(0.0f)
	{}

	DiffusionMove() {}
	~DiffusionMove() {}

	void init(ProceduralAlgorithm* base);

	bool doMutation() {
		float random_value = getRandomValue(0.0f, 100.0f);
		if (random_value > _conf->getMutationProb()) {
			return false;
		}
		return true;
	}

	float getLogProbability(Genome_IF* genome)
	{
		return _logProb;
	}


	// change all values of the parameters
	void mutate(GrammarConf& grammar, Genome_IF* genome) {}

	// do a grow-mutation
	void grow(GrammarConf& conf, Genome_IF* genome) {};

	void do_move(GrammarConf& grammar, Genome_IF* genome, Gene_IF* gene);


};

class JumpMove: public Mutation_IF {
private:
	ProceduralAlgorithm* 	_algo;
	MutationConf*  			_conf;
	GenomeConf*				_genome;
	int						_mutated_depth;

	bool can_have_children(Gene_IF* gene, const SymbolManager& sm);
	int	countChildrenWithType(Gene_IF* gene, int type);
	Gene_IF* selectRandomValidSubGene(Genome_IF* genome, Gene_IF* gene, const SymbolManager& sm);
	float addRandomSymbols(const SymbolManager& sm, Gene_IF* gene, Genome_IF* genome);
	void deleteChildGenes(Gene_IF* gene, Genome_IF* genome);
	int	maxLevel(Genome_IF* genome, const SymbolManager& sm);

	JumpMove() {}

public:
	JumpMove(ProceduralAlgorithm* base):
		_algo(base),
		_mutated_depth(0)
	{ }
	~JumpMove() {}

	void init(ProceduralAlgorithm* base);

	bool doMutation() {
		float random_value = getRandomValue(0.0f, 100.0f);
		if (random_value > _conf->getMutationProb()) {
			return false;
		}
		return true;
	}

	// change all values of the parameters
	void mutate(GrammarConf& grammar, Genome_IF* genome) {}

	// do a grow-mutation
	void grow(GrammarConf& conf, Genome_IF* genome) {};

	void do_move(GrammarConf& grammar, Genome_IF* genome, Gene_IF* gene);


};

class GrowMutation: public Mutation_IF {
private:
	ProceduralAlgorithm* _algo;
	MutationConf*  _conf;

public:
	GrowMutation(ProceduralAlgorithm* base):
		_algo(base)
	{ }
	~GrowMutation() {}

	void init(ProceduralAlgorithm* base);

	bool doMutation() {
		return false;
	}

	// change all values of the parameters
	void mutate(GrammarConf& grammar, Genome_IF* genome) {}

	// do a grow-mutation
	void grow(GrammarConf& conf, Genome_IF* genome);

	void do_move(GrammarConf& grammar, Genome_IF* genome, Gene_IF* gene)
	{
		throw(std::runtime_error("do_move not implemented for this Mutation-Operator!"));
	}

};


}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_METROPOLIS_MH_OPERATORS_H_ */
