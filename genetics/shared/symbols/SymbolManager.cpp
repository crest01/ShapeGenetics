/*
 * SymbolManager.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Karl Haubenwallner
 */

#include <iostream>

#include "SymbolManager.h"
#include "ParameterDescriptor.h"

namespace PGA {

SymbolManager::SymbolManager()
{
	_open_bracket = createTerminal("(");
	_close_bracket = createTerminal(")");
}

SymbolManager::~SymbolManager()
{
	for(int i = 0; i < _symbols.size(); ++i) {
		delete _symbols[i];
	}
	_symbols.clear();
}

int SymbolManager::createSymbolDescriptor(const std::string& name, const bool is_terminal, const bool is_start, const bool is_empty)
{
	int id = createSymbolId();
	SymbolDescriptor *s = new SymbolDescriptor(id, name, is_terminal, is_start, is_empty);
	_symbols.push_back(s);
	return id;
}

bool SymbolManager::isOpenBracket(int id) const
{
	return id == _open_bracket;
}

bool SymbolManager::isCloseBracket(int id) const
{
	return id == _close_bracket;
}

bool SymbolManager::isTerminal(int id) const
{
	SymbolDescriptor *s = symbol(id);
	if (s)
		return s->terminal();

	return false;
}
bool SymbolManager::isStart(int id) const
{
	SymbolDescriptor *s = symbol(id);
	if (s)
		return s->start();

	return false;
}
bool SymbolManager::isEmpty(int id) const
{
	SymbolDescriptor *s = symbol(id);
	if (s)
		return s->empty();

	return false;
}


int SymbolManager::SymbolId(const std::string& label) const
{
	for (int i = 0; i < _symbols.size(); ++i) {
		if (_symbols[i]->label().compare(label) == 0)
			return _symbols[i]->id();
	}
	std::cout << "SymblManager: Warning: symbol with label '" << label << "' doesn't exist!" << std::endl;
	throw(std::runtime_error("Symbol Label not found in SymbolManager"));
	return -1;
}

SymbolDescriptor* SymbolManager::symbol(const std::string& label) const
{
	return symbol(SymbolId(label));
}

SymbolDescriptor* SymbolManager::symbol(const int id) const
{
	if (_symbols.size() > id && id >= 0) {
		return _symbols[id];
	}
	std::cout << "SymbolManager: Warning: symbol '" << id << "' doesn't exist!" << std::endl;
	throw(std::runtime_error("Symbol not found in SymbolManager"));
	return nullptr;
}




void SymbolManager::addPossibleChild(const int id, const int child_id, const int max_num, const float probability)
{
	SymbolDescriptor *s = symbol(id);
	if (!s) return;

	if (child_id >= _symbols.size()) {
		std::cout << "SymbolManager: Warning: Child-symbol '" << child_id << "' doesn't exist!" << std::endl;
		throw(std::runtime_error("Symbol not found in SymbolManager"));
		return;
	}

	s->addPossibleChild(child_id, max_num, probability);

}

}; // namespace PGA
