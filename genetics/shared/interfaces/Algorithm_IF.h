/*
 * Algorithm_IF.h
 *
 *  Created on: Dec 5, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_INTERFACES_ALGORITHM_IF_H_
#define GPUPROCGENETICS_SRC_INTERFACES_ALGORITHM_IF_H_

namespace PGA {

class GeneticAlgorithm_IF
{
public:
	virtual void init(ProceduralAlgorithm* base) = 0;

	// create the initial population
	virtual void createInitialPopulation() = 0;

	// do the GA for one generation (selection + recombination)
	virtual void createNewGeneration() = 0;

	// calculate fitness values for the current active population
	virtual void evaluatePopulation() = 0;

	// reset everything (delete population)
	virtual void reset() = 0;

	// do the GA for as many generations as configured
	virtual void run() = 0;

	// recombine and evaluate one generation, do not delete anything
	virtual void doOneGeneration() = 0;

};



}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_INTERFACES_ALGORITHM_IF_H_ */
