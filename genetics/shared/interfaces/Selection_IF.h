/*
 * Selection.h
 *
 *  Created on: Dec 4, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_GENETICS_INTERFACES_SELECTION_H_
#define GPUPROCGENETICS_SRC_GENETICS_INTERFACES_SELECTION_H_

namespace PGA {

class ProceduralAlgorithm;
class Genome_IF;

/*
 * select a single individual from the current population
 */
class Selection_IF {
public:
	virtual void init(ProceduralAlgorithm* base) = 0;

	virtual const std::string getName() = 0;
	// called once for the whole generation, e.g. to sum the fitness values
	virtual void prepareForSelection() = 0;

	// select a single genome
	virtual Genome_IF* selectGenome() = 0;
};

}  // namespace PGA


#endif /* GPUPROCGENETICS_SRC_GENETICS_INTERFACES_SELECTION_H_ */
