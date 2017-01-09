/*
 * Mutation.cpp
 *
 *  Created on: Nov 9, 2015
 *      Author: Karl Haubenwallner
 */

#include "SOSMCOperators.h"
#include "RandomValues.h"
#include "ParseTree.h"

namespace PGA {


void SoSmcGrowMutation::init(ProceduralAlgorithm* base)
{
	_conf = _algo->get<MutationConf*>("init_mutation");
}

void SoSmcGrowMutation::mutate(GrammarConf& conf, Genome_IF* genome)
{
	// Find a random Symbol that can still have children
	SymbolManager& sm = conf.getSm();
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


