/*
 * Mutation.cpp
 *
 *  Created on: Nov 9, 2015
 *      Author: Karl Haubenwallner
 */

#include "SMCOperators.h"
#include "RandomValues.h"
#include "ParseTree.h"

namespace PGA {


void SMCGrowMutation::init(ProceduralAlgorithm* base)
{
	_conf = _algo->get<MutationConf*>("mutation");
	_genomeConf = _algo->get<GenomeConf*>("genome");
}


void SMCGrowMutation::mutate(GrammarConf& conf, Genome_IF* genome)
{
	// Find a random Symbol that can still have children
	SymbolManager& sm = conf.getSm();
	genome->resetIterator();
	std::vector<Gene_IF*> possible_genes;

	for (Gene_IF* gene = genome->nextGene(); gene != NULL; gene = genome->nextGene()) {
		if (gene->level() >= _genomeConf->getMaximalDepth()) {
			continue;
		}
		int num_children = gene->numChildren();
		int num_possible_children = sm.symbol(gene->type())->numPossibleChilds();
		if (num_children < num_possible_children) {
			possible_genes.push_back(gene);
		}
	}

	if (possible_genes.size() == 0) {
		return;
	}

	// the single genes were check in the order they were added to the genome, so the last
	// gene in this array was inserted last
	int semirandom_child = possible_genes.size() - 1;

	do {
		Gene_IF* gene = possible_genes[semirandom_child];

		if (possible_genes.size() > 1 && semirandom_child > 0 && (100.0f * getRandomValue01()) < _conf->getMutationProb()) {
			semirandom_child --;
			continue;
		}

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

		if (possible_child_types.size() == 0) {
			semirandom_child --;
			continue;
		}

		if (possible_child_types.size() == 1) {
			new_child_type = possible_child_types[0];
		}
		else {
			int random_child_type = getRandomValue(0, possible_child_types.size() -1);
			new_child_type = possible_child_types[random_child_type];
		}

		genome->createNewGene(gene, sm.symbol(new_child_type));
		break;
	} while (semirandom_child >= 0);
}

#ifdef old
void SMCGrowMutation::mutate(GrammarConf& conf, Genome_IF* genome)
{
	SymbolManager& sm = conf.getSm();
	unsigned int last_ref = genome->getLastRef();

	// find the last inserted symbol that still can have children

	Gene_IF* gene = nullptr;

	do {
		gene = genome->getGene(last_ref);
		int num_children = gene->numChildren();
		int num_possible_children = sm.symbol(gene->type())->numPossibleChilds();
		if (num_children < num_possible_children && _genomeConf->getMaximalDepth() > gene->level()) {
			break;
		}
		if (get)
		last_ref --;
	}while(last_ref > 0);

	if (gene == nullptr) {
		std::cout << "no valid gene found!" << std::endl;
		return;
	}

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
	else if (possible_child_types.size() == 0) {
		std::cout << "no valid sub-gene for gene " <<  gene->type() << " found!" << std::endl;
	}
	else {
		int random_child_type = getRandomValue(0, possible_child_types.size() -1);
		new_child_type = possible_child_types[random_child_type];
	}

	genome->createNewGene(gene, sm.symbol(new_child_type));

}

#endif /* old */

}  // namespace PGA


