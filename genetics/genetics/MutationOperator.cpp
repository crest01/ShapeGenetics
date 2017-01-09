/*
 * Mutation.cpp
 *
 *  Created on: Nov 9, 2015
 *      Author: Karl Haubenwallner
 */

#include "MutationOperator.h"
#include "RandomValues.h"
#include "ParseTree.h"
namespace PGA {

void MutationOperator::mutate(GrammarConf& grammar, Genome_IF* genome)
{
	float random_value = getRandomValue(0.0f, 100.0f);

	if (random_value < _conf->getParameterMutationProb()) {
		do_parameter_mutation(grammar.getSm(), genome);
	} else if (random_value < _conf->getParameterMutationProb() + _conf->getSymbolGrowProb()) {
		do_symbol_grow(grammar.getSm(), genome);
	} else if (random_value < _conf->getParameterMutationProb() + _conf->getSymbolGrowProb() + _conf->getSymbolCutProb()) {
		do_symbol_cut(grammar.getSm(), genome);
	}
	return;
}

void MutationOperator::do_parameter_mutation(const SymbolManager& sm, Genome_IF* genome)
{

	Gene_IF* gene = genome->getRandomGene();
	SymbolDescriptor* sd = sm.symbol(gene->type());

	for (int i = 0; i < gene->numParameter(); ++i) {
		sd->getParam(i)->generateRandomValue(gene->getParameter(i));
	}
}

void MutationOperator::do_symbol_cut(const SymbolManager& sm, Genome_IF* genome)
{
	if (genome->length() <= 1) {
		return;
	}
	do {
		// select a random gene and delete it and all it's children
		Gene_IF* gene = genome->getRandomGene();
		if (sm.isStart(gene->type())) {
			continue;
		}
		genome->deleteGene(gene->ref());
		break;
	} while (1);

	// there has to be at least a single object next to the start symbol
	if (genome->length() == 1) {
		do_symbol_grow(sm, genome);
	}

	return;
}


void MutationOperator::do_symbol_grow(const SymbolManager& sm, Genome_IF* genome)
{
	// Find a random Symbol that can still have children

	genome->resetIterator();
	std::vector<Gene_IF*> possible_genes;

	for (Gene_IF* gene = genome->nextGene(); gene != NULL; gene = genome->nextGene()) {
		int num_children = gene->numChildren();
		int num_possible_children = sm.symbol(gene->type())->numPossibleChilds();
		if (num_children < num_possible_children) {
			possible_genes.push_back(gene);
		}
	}

	if (possible_genes.size() == 0) {
		return;
	}
	int random_child = getRandomValue(0, possible_genes.size() -1);

	Gene_IF* gene = possible_genes[random_child];

	SymbolDescriptor* sd = sm.symbol(gene->type());

	// select a random possible child type
	std::vector<int> possible_child_types;

	// TODO: pay attention to percentage values of possible childs

	int num_children = gene->numChildren();
	for (int j = 0; j < sd->numPossibleChilds(); ++j) {
		SymbolDescriptor::PossibleChild child_d = sd->getPossibleChild(j);
		if (child_d.max_num < 0) {
			possible_child_types.push_back(child_d.id);
			continue;
		}
		bool possible = true;
		int num = 0;
		for (int i = 0; i < num_children; ++i) {
			Gene_IF* child = gene->getChild(i);
			if (child->type() == child_d.id) {
				num++;
				if (num >= child_d.max_num) {
					possible = false;
					break;
				}
			}
		}
		if (possible) {
			possible_child_types.push_back(child_d.id);
		}
	}

	int new_child_type = 0;

	if (possible_child_types.size() == 1) {
		new_child_type = possible_child_types[0];
	}
	else {
		int random_child_type = getRandomValue(0, possible_child_types.size() -1);
		new_child_type = possible_child_types[random_child_type];
	}

	genome->createNewGene(gene, sm.symbol(new_child_type));

}



}  // namespace PGA


