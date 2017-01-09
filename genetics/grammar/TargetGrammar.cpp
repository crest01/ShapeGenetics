/*
 * TargetGrammar.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Karl Haubenwallner
 */

#include "TargetGrammar.h"

#include <iostream>
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


using namespace PGG;
using namespace Shapes;
using namespace Parameters;
using namespace Scope;
using namespace Operators;
using namespace Modifiers;
using namespace CPU;

namespace PGA {

namespace SpaceShip {

void TargetGrammar::initSymbols(SymbolManager& sm)
{

	// start symbol, has no shape
	S = sm.createStart("S");

	// basic element
	B = sm.createTerminal("B");
	sm.addParameter<math::float3>(B, "scale", math::float3(1.0f, 1.0f, 1.0f), math::float3(1.0f, 1.0f, 1.0f));
	sm.addParameter<math::float3>(B, "translation", math::float3(0.0f, 0.0f, 0.0f), math::float3(0.0f, 0.0f, 0.0f));
	sm.addPossibleChild(S, B, 1, 1.0f);

}

int TargetGrammar::storeParameter(Symbol* symbol, PGG::Parameters::ParameterTable& pt)
{

	if (symbol->id() == B) {
		math::float3 scale = symbol->getParameter()[0]->getValue<math::float3>();
		math::float3 translation = symbol->getParameter()[1]->getValue<math::float3>();

		int pt_offset = pt.storeParameters( scale, // size of model
											translation); // movement of model
		return pt_offset;
	}
	//std::cout << "no Parameters for Symbol id " << symbol->id() << std::endl;
	return -1;

}

void TargetGrammar::createAxiom(PGG::CPU::GrammarSystem& system, const int axiomId)
{

	//forward declaratons for recursion
	class BodyRule;

	typedef CoordinateframeScope<int> MyScope;

	typedef PGA::InstancedShapeGenerator<MyScope, 0, false> MyGenerator;
	typedef Operators::Generator<MyGenerator> MyGenerate;

	static const int ParamLayer = 0;
	class SingleShapeRule : public
		Resize<DynamicFloat3<0, ParamLayer>,
			DirectCall<Translate<DynamicFloat3<4, ParamLayer>,
						DirectCall<MyGenerate>
					 >
			>
		>
	{ };

	ScopedShape<Box, MyScope > BasicAxiom(Box(math::float3(1.0f)), MyScope(math::identity<math::float3x4>(), axiomId));

	system.addAxiom<SingleShapeRule>(BasicAxiom);

}


}; // namespace SpaceShip

}; // namespace PGA
