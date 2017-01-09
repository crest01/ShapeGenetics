/*
* CityGrammar.cpp
*
*  Created on: Aug 21, 2016
*      Author: Markus Steinberger
*/

#include <iostream>
#include <string>
#include "operators/Generator.impl"
#include "operators/ScopeOperators.impl"
#include "operators/Resize.impl"
#include "operators/Repeat.impl"
#include "operators/Subdivide.impl"
#include "operators/ComponentSplit.impl"
#include "operators/Extrude.impl"
#include "operators/RandomPath.impl"
#include "operators/Duplicate.impl"
#include "parameters/StaticParameter.h"
#include "parameters/StaticRandom.h"
#include "parameters/ParameterConversion.h"
#include "parameters/Random.h"
#include "parameters/ShapeParameter.h"
#include "parameters/ScopeParameter.h"
#include "parameters/ParameterOperations.h"
#include "modifiers/DirectCall.h"
#include "modifiers/RandomReseed.h"
#include "modifiers/Discard.h"
#include "modifiers/ScopeModifier.h"
#include "CPU/StaticCall.h"
#include "GeometryGeneratorInstanced.h"
#include "SpaceFillingCity.h"


using namespace PGG;
using namespace Shapes;
using namespace Parameters;
using namespace Scope;
using namespace Operators;
using namespace Modifiers;
using namespace CPU;

namespace PGA {

	namespace City {

		void SpaceFillingCityGrammar::initSymbols(SymbolManager& sm)
		{
			// start symbol, has no shape
			Start = sm.createStart("Start");
			LotN = sm.createNonTerminal("N"); sm.addPossibleChild(Start, LotN, 1, 1.0f);
			LotE = sm.createNonTerminal("E"); sm.addPossibleChild(Start, LotE, 1, 1.0f);
			LotS = sm.createNonTerminal("S"); sm.addPossibleChild(Start, LotS, 1, 1.0f);
			LotW = sm.createNonTerminal("W"); sm.addPossibleChild(Start, LotW, 1, 1.0f);
			
			LotDualChild = sm.createNonTerminal("LotDualChild");
			sm.addPossibleChild(LotN, LotDualChild, 1, RecursionProbability);
			sm.addPossibleChild(LotE, LotDualChild, 1, RecursionProbability);
			sm.addPossibleChild(LotS, LotDualChild, 1, RecursionProbability);
			sm.addPossibleChild(LotW, LotDualChild, 1, RecursionProbability);

			LotSingleChild = sm.createNonTerminal("LotSingleChild");
			sm.addPossibleChild(LotDualChild, LotDualChild, 1, RecursionProbability);
			sm.addPossibleChild(LotDualChild, LotSingleChild, 1, RecursionProbability);
			sm.addPossibleChild(LotSingleChild, LotSingleChild, 1, RecursionProbability);

			BuildingSymbol = sm.createTerminal("Building");

			sm.addPossibleChild(LotDualChild, BuildingSymbol, 1, BuildingProbability);
			sm.addPossibleChild(LotSingleChild, BuildingSymbol, 1, BuildingProbability);
			sm.addPossibleChild(Start, BuildingSymbol, 1, BuildingProbability);
			

			buildingGrammar.setupBuildingParamters(sm, BuildingSymbol);

			if (getNumPreparedShapes() != 2) {
				throw std::invalid_argument("Wrong number of shapes for the Grammar");
			}
		}


		int SpaceFillingCityGrammar::storeStartParameters(Symbol* symbol, PGG::Parameters::ParameterTable& pt)
		{

			//params 0: has N
			//params 1: has E
			//params 2: has S
			//params 3: has W
			//params 4: paramtable for N
			//params 5: paramtable for E
			//params 6: paramtable for S
			//params 7: paramtable for W
			//params 8: has building
			//params 9: paramtable for builing
			//params 10: stepdistance for lot

			int hasN = 0, hasE = 0, hasS = 0, hasW = 0, paramN = -1, paramE = -1, paramS = -1, paramW = -1, hasB = 0, paramB = -1;
			float  stepdist = 0.1f*BuildingBaseSize;

			for (auto& child : symbol->getChildren())
			{
				if (child->id() == LotN && child->hasChildren())
					hasN = 1, paramN = child->getChildren()[0]->getParamTableOffset();
				else if (child->id() == LotE && child->hasChildren())
					hasE = 1, paramE = child->getChildren()[0]->getParamTableOffset();
				else if (child->id() == LotS && child->hasChildren())
					hasS = 1, paramS = child->getChildren()[0]->getParamTableOffset();
				else if (child->id() == LotE && child->hasChildren())
					hasW = 1, paramW = child->getChildren()[0]->getParamTableOffset();
				else if (child->id() == BuildingSymbol)
					hasB = 1, paramB = child->getParamTableOffset();
			}
			return pt.storeParameters(hasN, hasE, hasS, hasW, paramN, paramE, paramS, paramW, hasB, paramB, stepdist);
		}

		int SpaceFillingCityGrammar::storeLotDualChildParameters(Symbol* symbol, PGG::Parameters::ParameterTable& pt)
		{
			// params 0: has x child
			// params 1: paramtable for x child
			// params 2: has y child
			// params 3: paramtable for y child
			// params 4: has building
			// params 5: paramtable for builing
			// params 6: stepdistance for lot

			int hasX = 0, paramX = -1, hasY = 0, paramY = -1, hasB = 0, paramB = -1;
			float  stepdist = 0.1f*BuildingBaseSize;

			for (auto& child : symbol->getChildren())
			{
				if (child->id() == LotDualChild)
					hasX = 1, paramX = child->getParamTableOffset();
				else if (child->id() == LotSingleChild)
					hasY = 1, paramY = child->getParamTableOffset();
				else if (child->id() == BuildingSymbol)
					hasB = 1, paramB = child->getParamTableOffset();
			}
			return pt.storeParameters(hasX, paramX, hasY, paramY, hasB, paramB, stepdist);
		}
		int SpaceFillingCityGrammar::storeLotSingleChildParameters(Symbol* symbol, PGG::Parameters::ParameterTable& pt)
		{
			int hasX = 0, paramX = 0, hasB = 0, paramB = -1;
			float  stepdist = 0.1f*BuildingBaseSize;

			for (auto& child : symbol->getChildren())
			{
				if (child->id() == LotSingleChild)
					hasX = 1, paramX = child->getParamTableOffset();
				else if (child->id() == BuildingSymbol)
					hasB = 1, paramB = child->getParamTableOffset();
			}
			return pt.storeParameters(hasX, paramX, hasB, paramB, stepdist);
		}

		int SpaceFillingCityGrammar::storeParameter(Symbol* symbol, PGG::Parameters::ParameterTable& pt)
		{
			if (symbol->id() == Start)
				return storeStartParameters(symbol, pt);
			if (symbol->id() == LotDualChild)
				return storeLotDualChildParameters(symbol, pt);
			if (symbol->id() == LotSingleChild)
				return storeLotSingleChildParameters(symbol, pt);
			if (symbol->id() == BuildingSymbol)
				return buildingGrammar.storeBuildingParameter(symbol, pt);

			if (symbol->id() == LotN || symbol->id() == LotE || symbol->id() == LotS || symbol->id() == LotW)
				return -1;

			//std::cout << "no Parameters for Symbol id " << symbol->id() << std::endl;
			return -1;
		}

		namespace SpaceFillingCityRules
		{
			typedef CoordinateframeScope<int, XorShiftStar<32>> CityScope;
			typedef PGA::InstancedShapeGenerator<CityScope, 0, true> WallGenerator;
			typedef PGA::InstancedShapeGenerator<CityScope, 1, true> WindowGenerator;
			typedef Operators::Generator<WallGenerator> MyWallGenerator;
			typedef Operators::Generator<WindowGenerator> MyWindowGenerator;

			static const int ParamLayer = 0;
			static const int RandLayer = 1;

			template<class Building>
			class DualLot;
			template<class Building>
			class SingleLot;


			//N, E, S, W
			//params 0: has N
			//params 1: has E
			//params 2: has S
			//params 3: hasr W
			//params 4: paramtable for N
			//params 5: paramtable for E
			//params 6: paramtable for S
			//params 7: paramtable for W
			//params 8: has building
			//params 9: paramtable for builing
			//params 10: stepdistance for lot
			template<class Building>
			class City : public
				Duplicate<
				ReseedMulti<RandLayer,
				DirectCall<ChoosePath < DynamicInt<0, ParamLayer>, DirectCall<
				Rotate<StaticFloat3x3<0_p, 0_p, 1_p, 0_p, 1_p, 0_p, -1_p, 0_p, 0_p >,
				DirectCall<Translate<VecEx<math::float3, DynamicFloat<10, ParamLayer>, StaticFloat2<0_p, 0_p>>,
				SetScopeAttachment<ParamLayer, DynamicInt<4, ParamLayer>, StaticCall<DualLot<Building>> > > > > > > >
				>,
				ReseedMulti<RandLayer,
				DirectCall<ChoosePath <DynamicInt<1, ParamLayer>, DirectCall<
				Translate<VecEx<math::float3, DynamicFloat<10, ParamLayer>, StaticFloat2<0_p, 0_p>>,
				SetScopeAttachment<ParamLayer, DynamicInt<5, ParamLayer>, StaticCall<DualLot<Building>> > > > > >
				>,
				ReseedMulti<RandLayer,
				DirectCall<ChoosePath <DynamicInt<2, ParamLayer>, DirectCall<
				Rotate<StaticFloat3x3<0_p, 0_p, -1_p, 0_p, 1_p, 0_p, 1_p, 0_p, 0_p>,
				DirectCall<Translate<VecEx<math::float3, DynamicFloat<10, ParamLayer>, StaticFloat2<0_p, 0_p>>,
				SetScopeAttachment<ParamLayer, DynamicInt<6, ParamLayer>, StaticCall<DualLot<Building>> > > > > > > >
				>,
				ReseedMulti<RandLayer,
				DirectCall<ChoosePath <DynamicInt<3, ParamLayer>, DirectCall<
				Rotate<StaticFloat3x3<-1_p, 0_p, 0_p, 0_p, 1_p, 0_p, 0_p, 0_p, -1_p>,
				DirectCall<Translate<VecEx<math::float3, DynamicFloat<10, ParamLayer>, StaticFloat2<0_p, 0_p>>,
				SetScopeAttachment<ParamLayer, DynamicInt<7, ParamLayer>, StaticCall<DualLot<Building>> > > > > > > >
				>,
				ReseedMulti<RandLayer,
				DirectCall<ChoosePath < DynamicInt<8, ParamLayer>,
				SetScopeAttachment<ParamLayer, DynamicInt<9, ParamLayer>, StaticCall<Building> > > >
				>
				>
			{ };


			// params 0: has x child
			// params 1: paramtable for x child
			// params 2: has y child
			// params 3: paramtable for y child
			// params 4: has building
			// params 5: paramtable for builing
			// params 6: stepdistance for lot
			template<class Building>
			class DualLot : public
				Duplicate<
				ReseedMulti<RandLayer,
				DirectCall<ChoosePath < DynamicInt<0, ParamLayer>, DirectCall<
				Translate<VecEx<math::float3, DynamicFloat<6, ParamLayer>, StaticFloat2<0_p, 0_p>>,
				SetScopeAttachment<ParamLayer, DynamicInt<1, ParamLayer>, StaticCall<DualLot<Building>> > > > > >
				>,
				ReseedMulti<RandLayer,
				DirectCall<ChoosePath <DynamicInt<2, ParamLayer>, DirectCall<
				Rotate<StaticFloat3x3<0_p, 0_p, -1_p, 0_p, 1_p, 0_p, 1_p, 0_p, 0_p>,
				DirectCall<Translate<VecEx<math::float3, DynamicFloat<6, ParamLayer>, StaticFloat2<0_p, 0_p>>,
				SetScopeAttachment<ParamLayer, DynamicInt<3, ParamLayer>, StaticCall<SingleLot<Building>> > > > > > > >
				>,
				ReseedMulti<RandLayer,
				DirectCall<ChoosePath < DynamicInt<4, ParamLayer>,
				SetScopeAttachment<ParamLayer, DynamicInt<5, ParamLayer>, StaticCall<Building> > > >
				>
				>
			{ };


			// params 0: has x child
			// params 1: paramtable for x child
			// params 2: has building
			// params 3: paramtable for builing
			// params 4: stepdistance for lot
			template<class Building>
			class SingleLot : public
				Duplicate<
				ReseedMulti<RandLayer,
				DirectCall<ChoosePath < DynamicInt<0, ParamLayer>, DirectCall<
				Translate<VecEx<math::float3, DynamicFloat<4, ParamLayer>, StaticFloat2<0_p, 0_p>>,
				SetScopeAttachment<ParamLayer, DynamicInt<1, ParamLayer>, StaticCall<SingleLot<Building>> > > > > >
				>,
				ReseedMulti<RandLayer,
				DirectCall<ChoosePath < DynamicInt<2, ParamLayer>,
				SetScopeAttachment<ParamLayer, DynamicInt<3, ParamLayer>, StaticCall<Building> > > >
				>
				>
			{ };

		}
		void SpaceFillingCityGrammar::createAxiom(PGG::CPU::GrammarSystem& system, const int axiomId)
		{
			using namespace SpaceFillingCityRules;

			using Building = BuildingGrammar::SimpleBuilding<MyWallGenerator, ParamLayer>;

			int seed = 12345679;
			//math::float3x4 rot( 1.f, 0.f, 0.f, 0.f,
			//                    0.f, 0.f, 1.f, 0.f,
			//                    0.f,-1.f, 0.f, 0.f);
			math::float3x4 rot( 1.f, 0.f, 0.f, 0.f,
								0.f, 1.f, 0.f, 0.f,
								0.f, 0.f, 1.f, 0.f);
			ScopedShape<Quad, CityScope > cityAxiom(Quad(math::float2(1.0f)), CityScope(rot, axiomId, seed));
			system.addAxiom<City<Building>>(cityAxiom);
		}

		void SpaceFillingCityGrammar::createDetailedAxiom(PGG::CPU::GrammarSystem & system, const int axiomId)
		{
			using namespace SpaceFillingCityRules;

			using Building = BuildingGrammar::Building<MyWallGenerator, MyWindowGenerator, ParamLayer, RandLayer>;

			int seed = 12345679;
			//math::float3x4 rot( 1.f, 0.f, 0.f, 0.f,
			//                    0.f, 0.f, 1.f, 0.f,
			//                    0.f,-1.f, 0.f, 0.f);
			math::float3x4 rot( 1.f, 0.f, 0.f, 0.f,
								0.f, 1.f, 0.f, 0.f,
								0.f, 0.f, 1.f, 0.f);
			ScopedShape<Quad, CityScope > cityAxiom(Quad(math::float2(1.0f)), CityScope(rot, axiomId, seed));
			system.addAxiom<City<Building>>(cityAxiom);
		}

		void SpaceFillingCityGrammar::createDetailedGeometry(Genome_IF * genome)
		{
			// use the instanced mesh buffer for the object
			GeometryConfigSingleton::getInstance()->conf->setUsedMeshBuffer(GeometryConf::OBJECT);

			// clear the buffer
			GeometryConfigSingleton::getInstance()->conf->getInstancedMeshBuffer()->clearBuffers();

			// clear the parameter table
			_conf->getPt().clearParameters();

			// fill the parameter table with values from the genome
			int axiomId = fillParameterTable(genome);

			// create the grammar rules
			createDetailedAxiom(_conf->getSystem(), axiomId);

			// start generating the geometry
			_conf->getSystem().run(_conf->getNumThreads());
		}

	}; // namespace SpaceShip

}; // namespace PGA
