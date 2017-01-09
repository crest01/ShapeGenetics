/*
 * Symbol.h
 *
 *  Created on: Nov 3, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_GRAMMAR_SYMBOL_H_
#define GPUPROCGENETICS_SRC_GRAMMAR_SYMBOL_H_


#include <vector>
#include "ParameterTable.h"
#include "SymbolManager.h"

class Parameter;

namespace PGA {

class Symbol {
private:
	int						_id;
	int						_param_table;
	std::vector<Symbol*> 	_children;
	std::vector<Parameter*>	_params;
public:
	Symbol(const int id): _id(id), _param_table(-1) {};
	int id() { return _id; };
	void addChild(Symbol* child) { _children.push_back(child); }
	void addParameter(Parameter* param) { _params.push_back(param); }

	int setParamTableOffset(const int paramTable)
	{
		_param_table = paramTable;
		return paramTable;
	}
	int	getParamTableOffset() { return _param_table; }

	bool hasChildren() { return _children.size() != 0; }

	bool canHaveChildren();

	int numParams() { return static_cast<int>(_params.size()); }

	const std::vector<Symbol*>& getChildren() { return _children; }
	const std::vector<Parameter*>& getParameter() { return _params; }

};

}  // namespace PGA




#endif /* GPUPROCGENETICS_SRC_GRAMMAR_SYMBOL_H_ */
