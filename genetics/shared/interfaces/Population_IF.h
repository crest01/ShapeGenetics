/*
 * Population.h
 *
 *  Created on: Dec 4, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_GENETICS_INTERFACES_POPULATION_IF_H_
#define GPUPROCGENETICS_SRC_GENETICS_INTERFACES_POPULATION_IF_H_

#include "Genome_IF.h"

namespace PGA {

class ProceduralAlgorithm;

/*
 * Population: a collection of individuals, or genomes.
 * Contains two sets: the current generation, and the next one, and those switch places. Usually ordered by the fitness value
 */
class Population_IF
{
public:
	virtual void init(ProceduralAlgorithm* base) = 0;

	// return the specific genome from the current generation
	virtual Genome_IF* getGenome(const int index) = 0;

	// start the production system for the genome in the current generation
	virtual void generateGeometry(const int index) = 0;

	// add a genome to the next generation
	virtual void insertIntoNextGeneration(Genome_IF* genome) = 0;

	// add a genome to the current generation
	virtual void insertIntoActiveGeneration(Genome_IF* genome) = 0;

	// switch current and next generation
	virtual void switchGeneration() = 0;

	// sort the current generation by the fitness value
	virtual void sortActiveGeneration() = 0;

	// number of individuals in the current generation
	virtual int activeGenerationSize() = 0;

	// number of individuals in the next generation
	virtual int nextGenerationSize() = 0;

	// delete all individuals from the current generation
	virtual void clearCurrentGeneration() = 0;

	// delete all individuals from the next generation
	virtual void clearNextGeneration() = 0;

	// return the sum of the fitness points. No calculation going on here (see add/resetSumEvalPoints)
	virtual float getSumEvalPoints() = 0;

	// add the fitness value to the sum
	virtual void addEvalPoints(const float points) = 0;

	// set the sum of fitness points to zero
	virtual void resetSumEvalPoints() = 0;

	// get the best genome. For sorted populations, this is equal to getGenome(0)
	virtual Genome_IF* getBestGenome() = 0;

	// get the absolute best genome of all runs
	virtual Genome_IF* getAbsoluteBestGenome() = 0;

	virtual void replaceAt(Genome_IF* genome, int index) = 0;

	virtual void swap(int index1, int index2) = 0;

};

}  // namespace PGA


#endif /* GPUPROCGENETICS_SRC_GENETICS_INTERFACES_POPULATION_IF_H_ */
