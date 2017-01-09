/*
 * TwoBranchTreeGrammar.h
 *
 *  Created on: Nov 2, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef SRC_GRAMMAR_RULES_TwoBranchTreeGrammar_H_
#define SRC_GRAMMAR_RULES_TwoBranchTreeGrammar_H_


#include "BaseGrammar.h"

namespace PGA {

namespace Tree {

	class TwoBranchTreeGrammar : public BaseGrammar {

	private:
		int 		S;
		int			Trunk;
		int			Branch1;
		int			Branch2;

		int storeTrunkParameter(Symbol* symbol, PGG::Parameters::ParameterTable& pt);
		int storeBranchParameter(Symbol* symbol, PGG::Parameters::ParameterTable& pt);

		int storeParameter(Symbol* symbol, PGG::Parameters::ParameterTable& pt);
		void initSymbols(SymbolManager& sm);
		void createAxiom(PGG::CPU::GrammarSystem& system, const int axiomId);

	public:
		TwoBranchTreeGrammar(ProceduralAlgorithm* base, GrammarConf* grammar_conf) :
		BaseGrammar(base, grammar_conf)
		{}

	};

} // namespace Tree
} // namespace PGA



#endif /* SRC_GRAMMAR_RULES_TwoBranchTreeGrammar_H_ */
