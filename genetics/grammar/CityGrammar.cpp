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
#include "CityGrammar.h"


using namespace PGG;
using namespace Shapes;
using namespace Parameters;
using namespace Scope;
using namespace Operators;
using namespace Modifiers;
using namespace CPU;

namespace PGA {

namespace City {

	void CityGrammar::initSymbols(SymbolManager& sm)
	{
		// start symbol, has no shape
		Start = sm.createStart("Start");

		// Buildings
		if (!SeparateBuildings)
		{
			SeparateBuildingSymbol = sm.createTerminal("Builing");
			buildingGrammar.setupBuildingParamters(sm, SeparateBuildingSymbol);
		}
		else
			SeparateBuildingSymbol = -1;

		
		for (int j = 0; j < GridY; ++j)
			for (int i = 0; i < GridX; ++i)
			{
				int symbol;

				if (SeparateBuildings)
				{
					symbol = sm.createTerminal((std::string("Builing_") + std::to_string(i) + "_" + std::to_string(j)).c_str());
					sm.addPossibleChild(Start, symbol, 1, BuildingProbability);
					buildingGrammar.setupBuildingParamters(sm, symbol);
				}
				else
				{
					symbol = sm.createNonTerminal((std::string("Lot_") + std::to_string(i) + "_" + std::to_string(j)).c_str());
					sm.addPossibleChild(Start, symbol, 1, BuildingProbability);
					sm.addPossibleChild(symbol, SeparateBuildingSymbol, 1, 1.0f);
				}
				if (j == 0 && i == 0)
					LotSymbolStart = symbol;

			}
		
			

		if (getNumPreparedShapes() != 2) {
			throw std::invalid_argument("Wrong number of shapes for the Grammar");
		}
	}


	int CityGrammar::storeBoardParameters(Symbol* symbol, PGG::Parameters::ParameterTable& pt)
	{
		int buildings = static_cast<int>(symbol->getChildren().size());
		int nextBuilding = -1;
		for (int i = buildings-1; i >= 0; --i)
		{
			PGA::Symbol* building = symbol->getChildren()[i];
			//even if prob is 1 the child might not exist (yet..)
			if (!SeparateBuildings && building->getChildren().size() == 0)
				continue;
			int bid = building->id();
			math::int2 bcoords(bid % GridX, bid / GridX);
			math::float2 offset(bcoords.x*BuildingBaseSize, bcoords.y*BuildingBaseSize);
			int buildingParams;
			if(SeparateBuildings)
				buildingParams = building->getParamTableOffset();
			else
				buildingParams = building->getChildren()[0]->getParamTableOffset();
			int hasnext = nextBuilding != -1 ? 1 : 0;
			nextBuilding = pt.storeParameters(0.1f*offset, buildingParams, hasnext, nextBuilding);
		}
		math::float2 startoffset((1.0f - GridX)*0.5f*BuildingBaseSize, (1.0f - GridY)*0.5f*BuildingBaseSize);
		int hasnext = nextBuilding != -1 ? 1 : 0;
		return pt.storeParameters(0.1f*startoffset, hasnext, nextBuilding);
	}

	int CityGrammar::storeParameter(Symbol* symbol, PGG::Parameters::ParameterTable& pt)
	{
		if (symbol->id() == Start)
			return storeBoardParameters(symbol, pt);
		int bid = symbol->id() - LotSymbolStart;
		if (bid >= 0 && bid < GridX * GridY)
		{
			if (SeparateBuildings)
				return buildingGrammar.storeBuildingParameter(symbol, pt);
			else
				return -1;
		}
			
		if(symbol->id() == SeparateBuildingSymbol)
			return buildingGrammar.storeBuildingParameter(symbol, pt);

		//std::cout << "no Parameters for Symbol id " << symbol->id() << std::endl;
		return -1;
	}

	namespace CityGrammarRuleset
	{
		typedef CoordinateframeScope<int, XorShiftStar<32>> CityScope;
		typedef PGA::InstancedShapeGenerator<CityScope, 0, true> WallGenerator;
		typedef PGA::InstancedShapeGenerator<CityScope, 1, true> WindowGenerator;
		typedef Operators::Generator<WallGenerator> MyWallGenerator;
		typedef Operators::Generator<WindowGenerator> MyWindowGenerator;

		const int ParamLayer = 0;
		const int RandLayer = 1;

		template<class Building>
		class Lot;

		// offset 0: float2 translate
		// offset 2: has a lot
		// offset 3: lot params offset
		template<class Building>
		class City : public
			Translate<VecEx<math::float3, DynamicFloat<0, ParamLayer>, StaticFloat<0>, DynamicFloat<1, ParamLayer>>,
			DirectCall<ChoosePath < DynamicInt<2, ParamLayer>,
			SetScopeAttachment<ParamLayer, DynamicInt<3, ParamLayer>, StaticCall<Lot<Building>>  > > >  >
		{ };

		// offset 0: float2 lot offset
		// offset 2: params for building to generate
		// offset 3: generate another lot
		// offset 4: params for next lot
		template<class Building>
		class Lot : public
			Reseed<RandLayer, DirectCall<
			Duplicate<
			DirectCall< Translate<VecEx<math::float3, DynamicFloat<0, ParamLayer>, StaticFloat<0>, DynamicFloat<1, ParamLayer>>,
			SetScopeAttachment<ParamLayer, DynamicInt<2, ParamLayer>, StaticCall<Building> > > >,
			DirectCall<ChoosePath < DynamicInt<3, ParamLayer>,
			SetScopeAttachment<ParamLayer, DynamicInt<4, ParamLayer>, StaticCall<Lot<Building>> > > > >
			> >
		{ };

	}

	void CityGrammar::createAxiom(PGG::CPU::GrammarSystem& system, const int axiomId)
	{
		using namespace CityGrammarRuleset;

		typedef BuildingGrammar::SimpleBuilding<MyWallGenerator, ParamLayer> Building;

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

	void CityGrammar::createDetailedAxiom(PGG::CPU::GrammarSystem & system, const int axiomId)
	{
		using namespace CityGrammarRuleset;

		typedef BuildingGrammar::Building<MyWallGenerator, MyWindowGenerator, ParamLayer, RandLayer> Building;

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

	void CityGrammar::createDetailedGeometry(Genome_IF * genome)
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
