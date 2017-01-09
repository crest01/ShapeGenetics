/*
 * CrossoverCPU.h
 *
 *  Created on: Nov 25, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_GENETICS_CROSSOVERCPU_H_
#define GPUPROCGENETICS_SRC_GENETICS_CROSSOVERCPU_H_

#include "Recombination_IF.h"
#include "SymbolManager.h"
#include "GenomeCPU.h"
#include "Genome_IF.h"

namespace PGA {


class Crossover_CPU : public Recombination_IF {

private:

	ProceduralAlgorithm*	_algo;
	RecombinationConf*	 	_conf;
	GrammarConf*			_grammar;

	struct GeneConnection {
		Gene_IF* start;
		Gene_IF* end;
	};


	Gene_IF* insertGenesWithoutCutPoint(Genome_IF* source, Gene_IF* source_gene, Genome_IF* target, Gene_IF* target_gene, GeneConnection cut_point)
	{

		Gene_IF* new_gene = nullptr;

		if (_grammar->getSm().isStart(source_gene->type())) {
			new_gene = target->createNewGene(nullptr, _grammar->getSm().getStartSymbol());
		}
		else {
			new_gene = target->insertGene(target_gene, source_gene);
		}

		Gene_IF* rv = nullptr;

		if (source_gene == cut_point.start)
			rv = new_gene;

		for (int i = 0; i < source_gene->numChildren(); ++i) {
			Gene_IF * next_source_gene = source_gene->getChild(i);

			if (source_gene == cut_point.start && next_source_gene == cut_point.end) {
				continue;
			}
			Gene_IF* tmp_rv = insertGenesWithoutCutPoint(source, next_source_gene, target, new_gene, cut_point);
			if (tmp_rv)
				rv = tmp_rv;
		}
		return rv;
	}

	Gene_IF* cloneWithoutCutPoint(Genome_IF* source, Genome_IF* target, GeneConnection cut_point)
	{
		return insertGenesWithoutCutPoint(source, source->getStartGene(_grammar->getSm()), target, nullptr, cut_point);
	}

	void insertGenesAt(Genome_IF* source, Gene_IF* source_gene, Genome_IF* target, Gene_IF* target_gene)
	{

		Gene_IF * new_gene = target->insertGene(target_gene, source_gene);

		for (int i = 0; i < source_gene->numChildren(); ++i) {
			Gene_IF * next_source_gene = source_gene->getChild(i);
			insertGenesAt(source, next_source_gene, target, new_gene);
		}
	}

	GeneConnection getRandomGeneConnection(Genome_IF* genome)
	{
		GeneConnection c;
		// todo: check if this gene is the start symbol?
		c.end = genome->getRandomGene();
		c.start = c.end->getParent();
		return c;
	}

	GeneConnection getRandomFittingConnection(Genome_IF* genome, GeneConnection& other_c)
	{
		GeneConnection c;

		c.start = nullptr;
		c.end = nullptr;

		std::vector<Gene_IF*> possible_genes;

		genome->resetIterator();
		for (Gene_IF* gene = genome->nextGene(); gene != nullptr; gene = genome->nextGene()) {
			if (_grammar->getSm().symbol(gene->type())->isPossibleChild(other_c.end->type())) {
				possible_genes.push_back(gene);
			}
		}
		if (possible_genes.size() > 0) {
			int index = getRandomValue(0, possible_genes.size() -1);
			c.start = possible_genes[index];

			for (int i = 0; i < c.start->numChildren(); ++i) {
				Gene_IF* gene = c.start->getChild(i);

				// TODO: some randomness whether to replace a child or create a new one (if possible)

				if (gene->type() == other_c.end->type()) {
					c.end = gene;
					break;
				}
			}

		}
		return c;
	}



public:

	Crossover_CPU(ProceduralAlgorithm* base):
		_algo(base)
	{}

	virtual void init(ProceduralAlgorithm* base)
	{
		_conf = _algo->get<RecombinationConf*>("recombination");
		_grammar = _algo->get<GrammarConf*>("grammar");
	}

	int recombine(Genome_IF* parent1, Genome_IF* parent2, Genome_IF* child1, Genome_IF* child2)
	{

		int tries = 1;

		GeneConnection	cut_point_parent1;
		GeneConnection  cut_point_parent2;

		do {
			cut_point_parent1.start = nullptr;
			cut_point_parent1.end = nullptr;
			cut_point_parent2.start = nullptr;
			cut_point_parent2.end = nullptr;

			cut_point_parent1 = getRandomGeneConnection(parent1);
			cut_point_parent2 = getRandomFittingConnection(parent2, cut_point_parent1);
			if (cut_point_parent2.start != nullptr)
				break;
			tries ++;
		} while(tries <= 3);


		if (cut_point_parent2.start) {

			// copy all genes and their children from the second Parent Genome to the new Genome, except at the cut-point
			Gene_IF* crossover_point = cloneWithoutCutPoint(parent2, child2, cut_point_parent2);

			// insert the gene-snippet from the first parent at the cut-point
			insertGenesAt(parent1, cut_point_parent1.end, child2, crossover_point);

			// copy all genes and their children from the first Parent Genome to the new Genome, except at the cut-point
			crossover_point = cloneWithoutCutPoint(parent1, child1, cut_point_parent1);

			// this can be a nullptr: the gene-snippet was added as a new child
			if (cut_point_parent2.end)
				insertGenesAt(parent2, cut_point_parent2.end, child1, crossover_point);
		}
		return tries;
	}
};


}  // namespace PGA


#endif /* GPUPROCGENETICS_SRC_GENETICS_CROSSOVERCPU_H_ */
