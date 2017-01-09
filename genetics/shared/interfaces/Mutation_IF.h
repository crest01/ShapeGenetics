/*
 * Mutation.h
 *
 *  Created on: Dec 4, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_GENETICS_INTERFACES_MUTATION_IF_H_
#define GPUPROCGENETICS_SRC_GENETICS_INTERFACES_MUTATION_IF_H_

#include "Genome_IF.h"

namespace PGA {

class Grammar_conf;
class ProceduralAlgorithm;

/*
 * Mutation operator for a single genome
 */
class Mutation_IF
{
public:
	virtual void init(ProceduralAlgorithm* base) = 0;

	// check if the probability-values say we should mutate or not
	virtual bool doMutation() = 0;

	// do a random mutation
	virtual void mutate(GrammarConf& conf, Genome_IF* genome) = 0;

	// do a grow-mutation
	virtual void grow(GrammarConf& conf, Genome_IF* genome) = 0;

	virtual void do_move(GrammarConf& conf, Genome_IF* genome, Gene_IF* gene) = 0;
};

}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_GENETICS_INTERFACES_MUTATION_IF_H_ */
