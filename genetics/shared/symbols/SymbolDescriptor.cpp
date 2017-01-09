/*
 * Symbol.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: Karl Haubenwallner
 */

#include "SymbolDescriptor.h"

namespace PGA {

SymbolDescriptor::SymbolDescriptor(const int unique_id, const std::string& label, const bool is_terminal, const bool is_start, const bool is_empty)
: 	_empty(is_empty),
	_start(is_start),
	_terminal(is_terminal),
	_id(unique_id),
	_label(label)
{}

SymbolDescriptor::~SymbolDescriptor()
{

	for( auto p: _parameters) {
		delete p;
	}
	_parameters.clear();
}


void SymbolDescriptor::addPossibleChild(const int id, const int max_num, const float probability)
{
	PossibleChild child;
	child.id = id;
	child.max_num = max_num;
	child.probability = probability;
	_possible_kids.push_back(child);
}

bool SymbolDescriptor::isPossibleChild(const int id)
{
	for (PossibleChild pc: _possible_kids) {
		if (pc.id == id) {
			return true;
		}
	}
	return false;
}


void SymbolDescriptor::addParameter(ParameterDescriptor_IF* param)
{
	_parameters.push_back(param);
}

}

