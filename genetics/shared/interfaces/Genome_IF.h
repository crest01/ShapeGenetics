/*
 * Genome.h
 *
 *  Created on: Oct 31, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef SRC_GENETICS_INTERFACES_GENOME_IF_H_
#define SRC_GENETICS_INTERFACES_GENOME_IF_H_

#include <vector>
#include <string>

#include "Parameter.h"
#include "SymbolManager.h"
#include "SymbolDescriptor.h"

namespace PGA {

class ProceduralAlgorithm;
class GrammarConf;

/*
 * a single gene, contains the symbol and parameters for the production system,
 * and references to it's parents and children
 */
class Gene_IF {
public:
	virtual int	ref() = 0;
	virtual int	type() = 0;

	virtual int parentRef() = 0;
	virtual Gene_IF* getParent() const = 0;

	virtual int level() = 0;

	virtual unsigned int numParameter() const = 0;
	virtual Parameter* getParameter(const unsigned int index) = 0;

	virtual unsigned int numChildren() const = 0;
	virtual Gene_IF* getChild(const unsigned int index) = 0;

	virtual void setParameterOffset(const unsigned int) = 0;
	virtual unsigned int getParameterOffset()= 0;

};

/*
 * a single individual, basicly a collection of genes.
 * In the paper i call it an Individual, or a Chromosome, because fuck consistency
 */
class Genome_IF {
public:

	virtual ~Genome_IF(){}

	// create and initialise the genes from a string. The tree-structure is given by '(' and ')'
	virtual void createFromString(const SymbolManager& sm, const std::string& string) = 0;

	// copy the gene into the genome and link to the parent (which has to exist)
	virtual Gene_IF* insertGene(Gene_IF *parent, Gene_IF* gene) = 0;

	// create, initialize and add a new gene and link it to the parent
	virtual Gene_IF* createNewGene(Gene_IF *parent, SymbolDescriptor* sd) = 0;

	// duplicate all genes from the given genome
	virtual void clone(const SymbolManager& sm, Genome_IF* other) = 0;

	// remove a gene and all it's children from the array
	virtual void deleteGene(const unsigned int ref) = 0;

	// return gene with the given reference
	virtual Gene_IF* getGene(const unsigned int ref) = 0;

	// return the highest reference (not guaranteed to be continuous)
	virtual unsigned int getLastRef() = 0;

	// return gene with the given reference
	virtual Gene_IF* getStartGene(const SymbolManager& sm) = 0;

	// return a random gene from the genome
	virtual Gene_IF* getRandomGene() = 0;

	// select a random gene dependend on the depth: lower elements are more likely
	virtual Gene_IF* selectSemiRandomGene() = 0;

	// calc the probability that a specific gene from that level is selected
	virtual float getSelectionLogProb(int level) = 0;

	// return a random gene with the given type from the genome
	virtual Gene_IF* getRandomGeneWithType(const int type) = 0;

	// return the number of direct children for a gene
	virtual unsigned int getNumChildren(const unsigned int parent_ref) = 0;

	// return the n'th child of a gene
	virtual Gene_IF* getChildOfGene(const unsigned int parent_ref, const unsigned int child_index) = 0;

	// number of genes
	virtual int length() const = 0;

	// returns the maximum depth of the genome-tree
	virtual int maxLevel() const = 0;

	// reset the internal iterator. Used in conjunction with nextGene()
	virtual void resetIterator() = 0;

	// returns the gene at the current iterator-position and increases the iterator.
	// returns nullptr if the end is reached
	virtual Gene_IF* nextGene() = 0;

	// print the genome on the commandline. The format is somewhat undefined...
	virtual void print() = 0;

	// set and get the fitness value (Eval points)
	virtual void setEvalPoints(const float points) = 0;
	virtual float getEvalPoints() const = 0;

	// set and get the temperature of the genome (for MH)
	virtual void setTemperature(const float temperature) = 0;
	virtual float getTemperature() = 0;

	// set and get the log likelihood of the previous modification of the genome (for MH)
	virtual void setLogLikelihood(const float temperature) = 0;
	virtual float getLogLikelihood() = 0;

	// set and get the log likelihood of the previous modification of the genome (for MH)
	virtual void setLogPrior(const float temperature) = 0;
	virtual float getLogPrior() = 0;

};

/*
 * a collection of various methods to create a new genome
 */
class Genome_Factory_IF {
public:
	virtual void init(ProceduralAlgorithm* base) = 0;

	// create an empty genome (on the CPU: call 'new Genome_CPU()' )
	virtual Genome_IF* createEmptyGenome() = 0;

	// create an empty genome and call the mutation->grow() operator length times
	virtual Genome_IF* createRandomGenome(GrammarConf& grammar, const int length) = 0;

	// create the genome given from the string. The parameters are initialized at random (according to the SymbolDescriptor)
	virtual Genome_IF* createGenomeFromString(GrammarConf& grammar, const std::string& string) = 0;

	// create an empty genome and clone the given genome into it
	virtual Genome_IF* createGenome(GrammarConf& grammar, Genome_IF* other) = 0;
};


} // Namespace PGA

#endif /* SRC_GENETICS_GENOME_H_ */
