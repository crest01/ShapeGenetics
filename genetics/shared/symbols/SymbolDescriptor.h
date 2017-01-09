/*
 * Symbol.h
 *
 *  Created on: Oct 30, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef SRC_GRAMMAR_SYMBOLDESCRIPTOR_H_
#define SRC_GRAMMAR_SYMBOLDESCRIPTOR_H_

#include <string>
#include <vector>
#include <map>
#include <typeinfo>

#include "ParameterDescriptor.h"

namespace PGA {   // Procedural Genetic Algorithms


class SymbolDescriptor {

public:
	struct PossibleChild {
		int 	id;
		int		max_num;
		float	probability;
	};

private:
	bool									_empty;
	bool									_start;
	bool									_terminal;
	int			 							_id;
	std::string								_label;
	std::vector<ParameterDescriptor_IF*>	_parameters;
	std::vector<PossibleChild>				_possible_kids;
	std::vector<int>						_possible_parents;

public:
	SymbolDescriptor(const int unique_id, const std::string& label, const bool is_terminal, const bool is_start, const bool is_empty);

	virtual ~SymbolDescriptor();

	virtual int id() { return _id; }
	virtual const std::string& label() { return _label; };

	virtual void addParameter(ParameterDescriptor_IF* param);
	virtual int numParams() { return static_cast<int>(_parameters.size()); }
	virtual ParameterDescriptor_IF* getParam(int index) { return _parameters[index]; }

	virtual void addPossibleChild(const int id, const int max_num, const float probability);
	virtual int numPossibleChilds() { return static_cast<int>(_possible_kids.size()); }
	virtual PossibleChild& getPossibleChild(const int index) { return _possible_kids[index]; }
	virtual bool isPossibleChild(const int id);

	virtual bool empty() { return _empty; };
	virtual bool start() { return _start; };
	virtual bool terminal() { return _terminal; };


};

}  // namespace PGA



#endif /* SRC_GRAMMAR_SYMBOLDESCRIPTOR_H_ */
