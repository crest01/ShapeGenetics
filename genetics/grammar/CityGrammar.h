/*
 * CityGrammar.h
 *
 *  Created on: Aug 21, 2016
 *      Author: Markus Steinberger
 */

#ifndef SRC_GRAMMAR_RULES_CityGrammar_H_
#define SRC_GRAMMAR_RULES_CityGrammar_H_


#include "BaseGrammar.h"
#include <vector>
#include "BuildingGrammar.h"

namespace PGA {

namespace City {

	class CityGrammar : public BaseGrammar {

	private:
		int GridX, GridY;
		int Start;
		int SeparateBuildingSymbol;
		int LotSymbolStart;
		float BuildingProbability;
		float BuildingBaseSize;
		bool SeparateBuildings;

		int storeBoardParameters(Symbol* symbol, PGG::Parameters::ParameterTable& pt);

		int storeParameter(Symbol* symbol, PGG::Parameters::ParameterTable& pt);
		void initSymbols(SymbolManager& sm);
		void createAxiom(PGG::CPU::GrammarSystem& system, const int axiomId);
		void createDetailedAxiom(PGG::CPU::GrammarSystem& system, const int axiomId);

		BuildingGrammar buildingGrammar;

	public:
		CityGrammar(ProceduralAlgorithm* base, GrammarConf* grammar, bool SeparateBuildings = false, float BuildingProp = 0.33f, int GridX = 10, int GridY = 10,
			float BuildingBaseSize = 22.0f, float BuildingMaxHeight = 100.0f, float FloorHeight = 3.0f) :
		BaseGrammar(base, grammar), GridX(GridX), GridY(GridY), BuildingProbability(BuildingProp), BuildingBaseSize(BuildingBaseSize), 
			SeparateBuildings(SeparateBuildings), buildingGrammar(BuildingBaseSize, BuildingMaxHeight, FloorHeight, 0.1f)

		{}

		virtual void createDetailedGeometry(Genome_IF* genome) override;

		virtual ~CityGrammar() {}
	};

} // namespace City
} // namespace PGA



#endif /* SRC_GRAMMAR_RULES_CityGrammar_H_ */
