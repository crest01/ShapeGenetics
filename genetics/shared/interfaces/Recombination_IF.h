/*
 * Crossover.h
 *
 *  Created on: Nov 25, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_GENETICS_RECOMBINATION_H_
#define GPUPROCGENETICS_SRC_GENETICS_RECOMBINATION_H_

#include "Genome_IF.h"

namespace PGA {

class ProceduralAlgorithm;

/*
 * Recombination: Generate two new individuals given two parents
 */
class Recombination_IF {
public:
	virtual void init(ProceduralAlgorithm* base) = 0;
	virtual int recombine(Genome_IF* parent1, Genome_IF* parent2, Genome_IF* child1, Genome_IF* child2) = 0;
};





}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_GENETICS_RECOMBINATION_H_ */
