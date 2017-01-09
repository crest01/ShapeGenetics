/*
 * Mutation.h
 *
 *  Created on: Nov 9, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_GENETICS_MUTATIONOPERATOR_H_
#define GPUPROCGENETICS_SRC_GENETICS_MUTATIONOPERATOR_H_

#include <iostream>

#include "Genome_IF.h"
#include "Mutation_IF.h"
#include "GeneticAlgoConfig.h"
#include "SymbolManager.h"

namespace PGA {

class MutationOperator: public Mutation_IF {
private:
	ProceduralAlgorithm* _algo;
	MutationConf*  _conf;

	// select a random parameter and re-initialize it
	void do_parameter_mutation(const SymbolManager& sm, Genome_IF* genome);

	// select a random symbol, and remove all child symbols
	void do_symbol_cut(const SymbolManager& sm, Genome_IF* genome);

	// select a random symbol, and add a child symbol
	void do_symbol_grow(const SymbolManager& sm, Genome_IF* genome);

public:
	MutationOperator(ProceduralAlgorithm* base):
		_algo(base)
	{ }

	void init(ProceduralAlgorithm* base)
	{
		_conf = _algo->get<MutationConf*>("mutation");

		if (static_cast<int>((_conf->getParameterMutationProb() + _conf->getSymbolCutProb() + _conf->getSymbolGrowProb())) != 100) {
			std::cout << "MutationOperator(): Warning: Mutation probabilities not properly distributed!" << std::endl;
			throw(std::runtime_error("MutationOperator Probability mismatch"));
		}
	}

	bool doMutation() {
		float random_value = getRandomValue(0.0f, 100.0f);
		if (random_value > _conf->getMutationProb()) {
			return false;
		}
		return true;
	}

	void mutate(GrammarConf& grammar, Genome_IF* genome);
	void grow(GrammarConf& grammar, Genome_IF* genome) {
		do_symbol_grow(grammar.getSm(), genome);
	}

	void do_move(GrammarConf& grammar, Genome_IF* genome, Gene_IF* gene)
	{
			throw(std::runtime_error("getLogProbability not implemented for this Mutation-Operator!"));
	}

};


}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_GENETICS_MUTATIONOPERATOR_H_ */
