/*
 * ParseTree.h
 *
 *  Created on: Nov 4, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_GRAMMAR_PARSETREE_H_
#define GPUPROCGENETICS_SRC_GRAMMAR_PARSETREE_H_

#include <vector>
#include "Symbol.h"
#include "SymbolManager.h"
#include "Genome_IF.h"


namespace PGA {

class Genome_IF;

class ParseTree {
private:
	const SymbolManager&	_sm;
	std::vector<Symbol*> 	_symbols;

	void clearSymbols();

	void createSymbolsFromGene(Gene_IF* gene, Symbol* parent, Genome_IF* genome);

public:
	ParseTree(const SymbolManager& sm) : _sm(sm) {}
	ParseTree(const SymbolManager& sm, Genome_IF* genome):
		_sm(sm)
	{
		createParseTreeFromGenome(genome);
	}
	~ParseTree();
	int numSymbols() const {
		return static_cast<int>(_symbols.size());
	}

	Symbol* at(int index) const {
		return _symbols.at(index);
	}
	bool createParseTreeFromGenome(Genome_IF* genome);
};

}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_GRAMMAR_PARSETREE_H_ */
