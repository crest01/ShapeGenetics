/*
 * ThreeBranchTreeGrammar_v2.h
 *
 *  Created on: Nov 2, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef SRC_GRAMMAR_RULES_ThreeBranchTreeGrammar_v2_H_
#define SRC_GRAMMAR_RULES_ThreeBranchTreeGrammar_v2_H_


#include "BaseGrammar.h"

namespace PGA {

namespace Tree {

	class ThreeBranchTreeGrammar_v2 : public BaseGrammar {

	private:
		int 		S;
		int			Trunk;
		int			Branch1;
		int			Branch2;
		int			Branch3;

		int storeTrunkParameter(Symbol* symbol, PGG::Parameters::ParameterTable& pt);
		int storeBranchParameter(Symbol* symbol, PGG::Parameters::ParameterTable& pt);

		int storeParameter(Symbol* symbol, PGG::Parameters::ParameterTable& pt);
		void initSymbols(SymbolManager& sm);
		void createAxiom(PGG::CPU::GrammarSystem& system, const int axiomId);

	public:
		ThreeBranchTreeGrammar_v2(ProceduralAlgorithm* base, GrammarConf* grammar_conf) :
		BaseGrammar(base, grammar_conf)
		{}

	};

} // namespace Tree
} // namespace PGA



#endif /* SRC_GRAMMAR_RULES_ThreeBranchTreeGrammar_v2_H_ */
