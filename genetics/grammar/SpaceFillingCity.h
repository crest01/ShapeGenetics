/*
* CityGrammar.h
*
*  Created on: Aug 21, 2016
*      Author: Markus Steinberger
*/

#ifndef SRC_GRAMMAR_RULES_SpaceFillingCity_H_
#define SRC_GRAMMAR_RULES_SpaceFillingCity_H_


#include "BaseGrammar.h"
#include <vector>
#include "BuildingGrammar.h"

namespace PGA {

	namespace City {

		class SpaceFillingCityGrammar : public BaseGrammar {

		private:
			int Start;
			int BuildingSymbol;
			int LotN, LotE, LotS, LotW;
			int LotDualChild, LotSingleChild;
			float BuildingProbability;
			float RecursionProbability;
			float BuildingBaseSize;

			int storeStartParameters(Symbol* symbol, PGG::Parameters::ParameterTable& pt);
			int storeLotDualChildParameters(Symbol* symbol, PGG::Parameters::ParameterTable& pt);
			int storeLotSingleChildParameters(Symbol* symbol, PGG::Parameters::ParameterTable& pt);

			int storeParameter(Symbol* symbol, PGG::Parameters::ParameterTable& pt);
			void initSymbols(SymbolManager& sm);
			void createAxiom(PGG::CPU::GrammarSystem& system, const int axiomId);
			void createDetailedAxiom(PGG::CPU::GrammarSystem& system, const int axiomId);

			BuildingGrammar buildingGrammar;

		public:
			SpaceFillingCityGrammar(ProceduralAlgorithm* base, GrammarConf* grammar, float RecursionProbability = 0.9f, float BuildingProp = 0.33f,
				float BuildingBaseSize = 22.0f, float BuildingMaxHeight = 100.0f, float FloorHeight = 3.0f) :
				BaseGrammar(base, grammar), RecursionProbability(RecursionProbability), BuildingProbability(BuildingProp), BuildingBaseSize(BuildingBaseSize),
				buildingGrammar(BuildingBaseSize, BuildingMaxHeight, FloorHeight, 0.1f)

			{}

			virtual void createDetailedGeometry(Genome_IF* genome) override;

			virtual ~SpaceFillingCityGrammar() {}

		};

	} // namespace City
} // namespace PGA



#endif /* SRC_GRAMMAR_RULES_CityGrammar_H_ */
