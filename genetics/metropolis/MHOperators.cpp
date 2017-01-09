/*
 * Mutation.cpp
 *
 *  Created on: Nov 9, 2015
 *      Author: Karl Haubenwallner
 */

#include "MHOperators.h"
#include "RandomValues.h"
#include "ParseTree.h"

namespace PGA {


void DiffusionMove::init(ProceduralAlgorithm* algo)
{
	_conf = _algo->get<MutationConf*>("diffusion");
}

void DiffusionMove::do_move(GrammarConf& grammar, Genome_IF* genome, Gene_IF* gene)
{

	// uniform selection probability for each gene, except the start symbol

	SymbolManager& sm = grammar.getSm();

	for (int i = 0; i < gene->numParameter(); ++i) {
		SymbolDescriptor* sd = sm.symbol(gene->type());
		ParameterDescriptor_IF* pd = sd->getParam(i);
		pd->generateRandomValue(gene->getParameter(i));
	}
}


void JumpMove::init(ProceduralAlgorithm* base)
{
	_conf = _algo->get<MutationConf*>("jump");
	_genome = _algo->get<GenomeConf*>("genome");
}


void JumpMove::do_move(GrammarConf& grammar, Genome_IF* genome, Gene_IF* gene)
{
	// remove all child genes
	deleteChildGenes(gene, genome);

	// add anywhere between 1 and (max_length - length) genes to the tree
	addRandomSymbols(grammar.getSm(), gene, genome);
}

void JumpMove::deleteChildGenes(Gene_IF* gene, Genome_IF* genome)
{
	// nothing to do
	if (genome->length() <= 1) {
		return;
	}
	for (int i = 0; i < gene->numChildren(); ++i) {
		Gene_IF* child = gene->getChild(i);
		genome->deleteGene(child->ref());
	}
	return;
}

int	JumpMove::countChildrenWithType(Gene_IF* gene, int type)
{
	int ret = 0;
	for (int i = 0; i < gene->numChildren(); ++i) {
		if (gene->getChild(i)->type() == type) {
			ret ++;
		}
	}

	return ret;
}

int	JumpMove::maxLevel(Genome_IF* genome, const SymbolManager& sm)
{
	int maxLevel = 0;
	int ret = 0;
	Gene_IF * root = genome->getStartGene(sm);
	std::vector<Gene_IF*> stack;
	stack.push_back(root);
	int index = 0;
	do {
		Gene_IF* gene = stack[index];
		if (maxLevel < gene->level()) {
			maxLevel = gene->level();
		}
		for (int i = 0; i < gene->numChildren(); ++i) {
			Gene_IF* child = gene->getChild(i);
			stack.push_back(child);
		}
		index ++;
	}while (index < stack.size());

	return maxLevel;
}

bool JumpMove::can_have_children(Gene_IF* gene, const SymbolManager& sm)
{
	SymbolDescriptor* sd = sm.symbol(gene->type());
	for (int i = 0; i < sd->numPossibleChilds(); ++i) {
		SymbolDescriptor::PossibleChild pc = sd->getPossibleChild(i);
		if (pc.max_num > countChildrenWithType(gene, pc.id)) {
			return true;
		}
	}
	return false;
}

Gene_IF* JumpMove::selectRandomValidSubGene(Genome_IF* genome, Gene_IF* gene, const SymbolManager& sm)
{
	if (genome->length() == 1) {
		return gene;
	}
	std::vector<Gene_IF*> possible_genes;
	std::vector<Gene_IF*> check_genes;
	check_genes.push_back(gene);
	if (can_have_children(gene, sm)) {
		possible_genes.push_back(gene);
	}
	int index = 0;
	do {
		Gene_IF* current_gene = check_genes[index];
		for (int i = 0; i < current_gene->numChildren(); ++i) {
			Gene_IF* child = current_gene->getChild(i);
			if (can_have_children(child, sm)) {
				possible_genes.push_back(child);
			}
			check_genes.push_back(child);
		}
		index ++;
	} while (index < check_genes.size());

	if (possible_genes.size() == 0) {
		return nullptr;
	}
	if (possible_genes.size() == 1) {
		return possible_genes[0];
	}
	int random_index = getRandomValue(0, possible_genes.size()-1);
	return possible_genes[random_index];
}


float JumpMove::addRandomSymbols(const SymbolManager& sm, Gene_IF* root, Genome_IF* genome)
{

	int current_max_level = maxLevel(genome, sm);

	int total_max_level = 25;
	int added_childs = 0;


	do {
		Gene_IF* gene = selectRandomValidSubGene(genome, root, sm);
		if (gene == nullptr) {
//			std::cout << "no valid subGene found for Gene type " << root->type() << std::endl;
			break;
		}

		SymbolDescriptor* sd = sm.symbol(gene->type());

		// select a random possible child type
		std::vector<int> possible_child_types;

		// TODO: pay attention to percentage values of possible childs

		int num_children = gene->numChildren();
		for (int j = 0; j < sd->numPossibleChilds(); ++j) {
			SymbolDescriptor::PossibleChild child_d = sd->getPossibleChild(j);
			int current_children = countChildrenWithType(gene, child_d.id);
			if (child_d.max_num > current_children) {
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

		Gene_IF* new_gene = genome->createNewGene(gene, sm.symbol(new_child_type));

		int num_children_new = gene->numChildren();

		if (num_children == num_children_new) {
			std::cout << "didn't add new children!" << std::endl;
		}
		sd = sm.symbol(new_gene->type());

		// reinitalize the paramters so we get the probability values for the selected values
		for (int i = 0; i < new_gene->numParameter(); ++i) {
			ParameterDescriptor_IF* pd = sd->getParam(i);
			pd->generateRandomValue(new_gene->getParameter(i));
			// prob += math::log(pd->probability()); this is zero for the uniform distribution
		}
		added_childs ++;
		float stop = getRandomValue01();
		current_max_level = maxLevel(genome, sm);
		if (current_max_level >= total_max_level) {
			break;
		}
		float stop_prob = 0.02f;

		if (stop < stop_prob) {
			break;
		}
	}while(1);
	//std::cout << "max level " << current_max_level << ", added " << added_childs << " elements " << std::endl;
	return 0.0f;
}



void GrowMutation::init(ProceduralAlgorithm* base)
{
	_conf = _algo->get<MutationConf*>("init_mutation");
}

void GrowMutation::grow(GrammarConf& conf, Genome_IF* genome)
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


