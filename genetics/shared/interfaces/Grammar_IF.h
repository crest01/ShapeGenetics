/*
 * Rules.h
 *
 *  Created on: Nov 2, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef SRC_GENETICS_INTERFACES_GRAMMAR_IF_H_
#define SRC_GENETICS_INTERFACES_GRAMMAR_IF_H_

#include "GrammarSystem.h"
#include "ParameterTable.h"
#include "Genome_IF.h"

namespace PGA {

class ProceduralAlgorithm;

/*
 * the production system, contains an implementation of the grammar
 */
class Grammar_IF {
public:
	virtual void init(ProceduralAlgorithm* base) = 0;

	// start the production system
	virtual void createGeometry(Genome_IF* genome) = 0;
	virtual void createDetailedGeometry(Genome_IF* genome) = 0;
};

}



#endif /* SRC_GENETICS_INTERFACES_GRAMMAR_IF_H_ */
