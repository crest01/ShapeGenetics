/*
 * SpaceShipGrammar.h
 *
 *  Created on: Nov 2, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef SRC_GRAMMAR_RULES_SpaceShipGrammar_H_
#define SRC_GRAMMAR_RULES_SpaceShipGrammar_H_


#include "BaseGrammar.h"

namespace PGA {

namespace SpaceShip {

	class SpaceShipGrammar : public BaseGrammar {

	private:
		int			B;
		int			T_start;
		int			T_recursion;
		int			W_start;
		int			W_recursion;
		int 		S;

		int storeBodyParameter(Symbol* symbol, PGG::Parameters::ParameterTable& pt);
		int storeTopParameter(Symbol* symbol, PGG::Parameters::ParameterTable& pt);
		int storeWingParameter(Symbol* symbol, PGG::Parameters::ParameterTable& pt);


		int storeParameter(Symbol* symbol, PGG::Parameters::ParameterTable& pt);
		void initSymbols(SymbolManager& sm);
		void createAxiom(PGG::CPU::GrammarSystem& system, const int axiomId);

	public:
		SpaceShipGrammar(ProceduralAlgorithm* base, GrammarConf* grammar) :
		BaseGrammar(base, grammar)
		{}
	};

} // namespace SpaceShip
} // namespace PGA



#endif /* SRC_GRAMMAR_RULES_SpaceShipGrammar_H_ */
