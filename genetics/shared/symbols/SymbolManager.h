/*
 * SymbolManager.h
 *
 *  Created on: Oct 31, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef SRC_GRAMMAR_SYMBOLMANAGER_H_
#define SRC_GRAMMAR_SYMBOLMANAGER_H_

#include "../utils/RandomValues.h"
#include "SymbolDescriptor.h"

namespace PGA {

class SymbolManager {
private:
	std::vector<SymbolDescriptor*> _symbols;

	int _open_bracket;
	int _close_bracket;

	int createSymbolId()
	{
		 return static_cast<int>(_symbols.size());
	}


public:

	SymbolManager();

	~SymbolManager();

	int createSymbolDescriptor(const std::string& name, const bool is_terminal, const bool is_start, const bool is_empty);

	int createStart(const std::string& name)		{ return createSymbolDescriptor(name, false, true,  false); }
	int createEmpty() 								{ return createSymbolDescriptor("-",  false, false, true); }
	int createTerminal(const std::string& name) 	{ return createSymbolDescriptor(name, true,  false, false); }
	int createNonTerminal(const std::string& name) 	{ return createSymbolDescriptor(name, false, false, false); }

	template <typename T>
	void addParameter(const int id, const std::string& param_name, const T& min, const T& max)
	{
		SymbolDescriptor *s = symbol(id);
		if (!s) return;

		ParameterDescriptor_IF *p = new ParameterDescriptor<T>(param_name, min, max);
		s->addParameter(p);
	}

	void addPossibleChild(const int id, const int child_id, const int max_num, const float probability);

	int SymbolId(const std::string& label) const;

	std::string getLabel(const int id) const
	{
		return symbol(id)->label();
	}

	bool isOpenBracket(int id) const;
	bool isCloseBracket(int id) const;
	bool isTerminal(int id) const;
	bool isStart(int id) const;
	bool isEmpty(int id) const;

	SymbolDescriptor* symbol(const std::string& label) const;

	SymbolDescriptor* symbol(const int id) const;

	SymbolDescriptor* getStartSymbol() const
	{

		for (SymbolDescriptor* sd : _symbols) {
			if (sd->start()) {
				return sd;
			}
		}
		return nullptr;

	}

};


} // namespace PGA




#endif /* SRC_GRAMMAR_SYMBOLMANAGER_H_ */
