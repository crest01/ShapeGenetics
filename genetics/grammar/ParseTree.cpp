/*
 * ParseTree.cpp
 *
 *  Created on: Nov 4, 2015
 *      Author: Karl Haubenwallner
 */

#include <stack>
#include <iostream>
#include "Genome_IF.h"
#include "Symbol.h"

#include "ParseTree.h"

namespace PGA {

ParseTree::~ParseTree()
{
	clearSymbols();
}

void ParseTree::clearSymbols()
{
	for (Symbol* s: _symbols) {
		delete s;
	}
	_symbols.clear();
}

void ParseTree::createSymbolsFromGene(Gene_IF* gene, Symbol* parent, Genome_IF* genome)
{
	for (int i= 0; i < genome->getNumChildren(gene->ref()); ++i) {
		Gene_IF* child = genome->getChildOfGene(gene->ref(), i);
		Symbol *s = new Symbol(child->type());
		for (int j= 0; j < child->numParameter(); ++j) {
			s->addParameter(child->getParameter(j));
		}
		_symbols.push_back(s);
		parent->addChild(s);
		createSymbolsFromGene(child, s, genome);
	}
}

bool ParseTree::createParseTreeFromGenome(Genome_IF* genome)
{

	clearSymbols();

	genome->resetIterator();

	Gene_IF* gene = genome->getStartGene(_sm);
	int symbol = gene->type();

	if (!_sm.isStart(symbol)) {
		std::cout << "Genome malformed!" << std::endl;
		return false;
	}

	Symbol *s = new Symbol(symbol);
	_symbols.push_back(s);

	createSymbolsFromGene(gene, s, genome);

	return true;

}


}  // namespace PGA
