/*
 * SpaceShipGrammar.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: Karl Haubenwallner
 */

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
#include "SpaceShipGrammar.h"


using namespace PGG;
using namespace Shapes;
using namespace Parameters;
using namespace Scope;
using namespace Operators;
using namespace Modifiers;
using namespace CPU;

namespace PGA {

namespace SpaceShip {

void SpaceShipGrammar::initSymbols(SymbolManager& sm)
{
	// start symbol, has no shape
	S = sm.createStart("S");

	// Body element
	B = sm.createTerminal("B");
	sm.addParameter<math::float3>(B, "size", math::float3(0.6, 0.6f, 0.6f), math::float3(1.5f, 1.5f, 1.5f));

	// Top (pyramid) element on top of body
	T_start = sm.createTerminal("T");
	sm.addParameter<math::float3>(T_start, "size", math::float3(0.3, 0.1f, 0.3f), math::float3(0.8f, 0.2f, 0.8f));

	// Top (pyramid) element on top of body
	T_recursion = sm.createTerminal("t");
	sm.addParameter<math::float3>(T_recursion, "size", math::float3(0.6, 0.8f, 0.6f), math::float3(1.1f, 1.1f, 1.1f));

	// Wing element
	W_start = sm.createTerminal("W");
	sm.addParameter<math::float3>(W_start, "size", math::float3(0.3f, 0.2f, 0.2f), math::float3(1.2f, 0.5f, 1.0f));
	sm.addParameter<float>(W_start, "forward/backward movement", -0.5f, 0.5f);

	W_recursion = sm.createTerminal("w");
	sm.addParameter<math::float3>(W_recursion, "size", math::float3(0.6f, 0.9f, 0.7f), math::float3(1.2f, 1.0f, 1.2f));
	sm.addParameter<float>(W_recursion, "forward/backward movement", -0.5f, 0.5f);

	sm.addPossibleChild(S, B, 1, 1.0f);

	sm.addPossibleChild(B, B, 1, 1.0f/3.0f);
	sm.addPossibleChild(B, W_start, 1, 1.0f/3.0f);
	sm.addPossibleChild(B, T_start, 1, 1.0f/3.0f);

	sm.addPossibleChild(W_start, W_recursion, 1, 1.0f);
	sm.addPossibleChild(W_recursion, W_recursion, 1, 0.9f);


	sm.addPossibleChild(T_start, T_recursion, 1, 1.0f);
	sm.addPossibleChild(T_recursion, T_recursion, 1, 1.0f);

	if (getNumPreparedShapes() != 1) {
		throw std::invalid_argument("Wrong number of shapes for the Grammar");
	}

}

int SpaceShipGrammar::storeParameter(Symbol* symbol, PGG::Parameters::ParameterTable& pt)
{
	if (symbol->id() == S)
		return -1;

	if (symbol->id() == B) {
		return storeBodyParameter(symbol, pt);
	}

	if (symbol->id() == T_start || symbol->id() == T_recursion) {
		return storeTopParameter(symbol, pt);
	}

	if (symbol->id() == W_start || symbol->id() == W_recursion) {
		return storeWingParameter(symbol, pt);
	}

	//std::cout << "no Parameters for Symbol id " << symbol->id() << std::endl;
	return -1;

}

int SpaceShipGrammar::storeBodyParameter(Symbol* s, PGG::Parameters::ParameterTable& pt)
{
	int body_child = 0;
	int body_child_offset = 0;

	int top_child = 0;
	int top_child_offset = 0;

	int wing_child = 0;
	int wing_child_offset = 0;

	for (int i = 0; i < s->getChildren().size(); ++i) {
		Symbol* c = s->getChildren().at(i);
		if (c->id() == B) {
			body_child = 1;
			body_child_offset = c->getParamTableOffset();
			continue;
		}
		if (c->id() == T_start) {
			top_child = 1;
			top_child_offset = c->getParamTableOffset();
			continue;
		}
		if (c->id() == W_start) {
			wing_child = 1;
			wing_child_offset = c->getParamTableOffset();
			continue;
		}
	}

	math::float3 size = s->getParameter()[0]->getValue<math::float3>();

	int pt_offset = pt.storeParameters(	size, // size of body part
										body_child,				// attach another body part
										body_child_offset,		// Param Layer of new body part
										top_child,		 		// attach a top part
										top_child_offset,		// Param Layer of top part
										wing_child,		 		// attach a top part
										wing_child_offset);		// Param Layer of top part

	return pt_offset;

}

int SpaceShipGrammar::storeTopParameter(Symbol* s, PGG::Parameters::ParameterTable& pt)
{
	int top_child = 0;
	int top_child_offset = 0;

	for (int i = 0; i < s->getChildren().size(); ++i) {
		Symbol* c = s->getChildren().at(i);
		if (c->id() == T_recursion) {
			top_child = 1;
			top_child_offset = c->getParamTableOffset();
			break;
		}
	}

	math::float3 size = s->getParameter()[0]->getValue<math::float3>();

	int pt_offset = pt.storeParameters(	size, // size of top part
										top_child,			 	// attach a top part
										top_child_offset);		// Param Layer of top part
	return pt_offset;

}

int SpaceShipGrammar::storeWingParameter(Symbol* s, PGG::Parameters::ParameterTable& pt)
{
	int wing_child = 0;
	int wing_child_offset = 0;

	for (int i = 0; i < s->getChildren().size(); ++i) {
		Symbol* c = s->getChildren().at(i);
		if (c->id() == W_recursion) {
			wing_child = 1;
			wing_child_offset = c->getParamTableOffset();
			break;
		}
	}

	math::float3 size = s->getParameter()[0]->getValue<math::float3>();
	float movement = s->getParameter()[1]->getValue<float>();

	int pt_offset = pt.storeParameters(	size, // size of wing part
										movement, // movement of wing
										wing_child,			 	// attach a top part
										wing_child_offset);		// Param Layer of top part
	return pt_offset;

}

void SpaceShipGrammar::createAxiom(PGG::CPU::GrammarSystem& system, const int axiomId)
{

	//forward declaratons for recursion
	class BodyRuleRecusion;
	class TopRule;
	class TopRuleRecursion;
	class WingRule;
	class WingRuleRecursion;
	class BodyRule;

	typedef CoordinateframeScope<int> SpaceShipscope;
	typedef PGA::InstancedShapeGenerator<SpaceShipscope, 0, true> SpaceShipGenerator;
	typedef Operators::Generator<SpaceShipGenerator> MyGenerate;

	static const int ParamLayer = 0;

	// offset 0: float3 size
	class SpaceShip : public
		Resize<DynamicFloat3<0, ParamLayer>, DirectCall<BodyRule> >
	{ };

	// offset 3: int 1/0 to enable next body part
	// offset 4: int to change paramtable for next body part
	// offset 5: int 1/0 to enable top part
	// offset 6: int to change paramtable for top part
	// offset 7: int 1/0 to build wings
	// offset 8: int to change paramtable for wings (equal for both)

	class BodyRule : public
		Translate<Vec<StaticFloat<0_p>, StaticFloat<0_p>, Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::ZAxis>> >, //0 is starting point, so move the box half the size to the front
			StaticCall <
				Duplicate <
					DirectCall<MyGenerate>, // body part is being generated
					DirectCall <
						ChoosePath < DynamicInt<3, ParamLayer>, // extend body part
						DirectCall<Translate<Vec<StaticFloat<0_p>, StaticFloat<0_p>, Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::ZAxis>> >,
							SetScopeAttachment<ParamLayer, DynamicInt<4, ParamLayer>, StaticCall<BodyRuleRecusion> > > > > // next body part
					>,
					DirectCall<
						ChoosePath < DynamicInt<5, ParamLayer>, // build top part
						DirectCall<Translate<Vec<StaticFloat<0_p>, Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::YAxis>>, StaticFloat<0_p>>,
							SetScopeAttachment<ParamLayer, DynamicInt<6, ParamLayer>, StaticCall<TopRule> > > > > // start top
					>,
					DirectCall<
						ChoosePath < DynamicInt<7, ParamLayer>, //generate wings
							SetScopeAttachment<ParamLayer, DynamicInt<8, ParamLayer>,
							DirectCall< Duplicate < // mirror
								DirectCall<Translate<Vec< Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::XAxis> >, StaticFloat<0_p>, StaticFloat<0_p>>, StaticCall<WingRule> > >, // wing right
								DirectCall<Rotate<StaticAxes<Axes::ZAxis>, StaticFloat<3.14159265359_p>, DirectCall<Translate<Vec<Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::XAxis> >, StaticFloat<0_p>, StaticFloat<0_p>>, StaticCall<WingRule> > > > > >// wing left
						> > >
					>
				>
			>
		>
	{ };


	// offset 0: float3 size
	class BodyRuleRecusion : public
		Resize<DynamicFloat3<0, ParamLayer>,  //adjust body part size
			DirectCall<BodyRule> // execute the recursion
	> { };

	// offset 0: float3 size
	class TopRule : public
		Resize<DynamicFloat3<0, ParamLayer>, // initial top scale
			DirectCall<TopRuleRecursion >
		>
	{};

	// offset 3: int 1/0 to enable next top part
	// offset 4: int to change paramtable for next top part
	class TopRuleRecursion : public
		Translate<Vec<StaticFloat<0_p>, Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::YAxis>>, StaticFloat<0_p> >, //0 is starting point, so move the box half the size up
			Duplicate <
				DirectCall<MyGenerate>, // top part is being generated
				DirectCall<ChoosePath<
					DynamicInt<3, ParamLayer>,
						DirectCall< Translate<Vec<StaticFloat<0_p>, Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::YAxis>>, StaticFloat<0_p> >,
							SetScopeAttachment<ParamLayer, DynamicInt<4, ParamLayer>,
								DirectCall< Resize<DynamicFloat3<0, ParamLayer>, DirectCall<TopRuleRecursion>
										>
									>
								>
							>
						>
					>
				>
			>
		>
	{};

	// offset 0: float3 size
	// offset 3: float forward backward move
	class WingRule : public
		Resize<DynamicFloat3<0, ParamLayer>, // initial wing scale
			DirectCall<Translate<Vec<StaticFloat<0_p>, StaticFloat<0_p>, Mul<ShapeSizeAxis<Axes::ZAxis>, DynamicFloat<3, ParamLayer>>>,
				DirectCall<WingRuleRecursion >
			> >
		>
	{};

	// offset 4: int 1/0 to enable next wing part
	// offset 5: int to change paramtable for next top part
	class WingRuleRecursion : public
		Translate<Vec<Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::XAxis>>, StaticFloat<0_p>, StaticFloat<0_p> >, //0 is starting point, so move the box half the size up
			Duplicate <
				DirectCall<MyGenerate>, // wing part is being generated
				DirectCall<ChoosePath<
						DynamicInt<4, ParamLayer>,
							DirectCall< Translate<Vec<Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::XAxis>>, StaticFloat<0_p>, StaticFloat<0_p> >,
								SetScopeAttachment<ParamLayer, DynamicInt<5, ParamLayer>,
									DirectCall< Resize<DynamicFloat3<0, ParamLayer>,
											DirectCall<Translate<Vec<StaticFloat<0_p>, StaticFloat<0_p>, Mul<ShapeSizeAxis<Axes::ZAxis>, DynamicFloat<3, ParamLayer>>>, // backward and forward move
												DirectCall<WingRuleRecursion>
											> >
										>
									>
								>
							> >
					>
				>
			>
		>
	{};

	ScopedShape<Box, SpaceShipscope > spaceShipAxiom(Box(math::float3(1.0f)), SpaceShipscope(math::identity<math::float3x4>(), axiomId));

	system.addAxiom<SpaceShip>(spaceShipAxiom);

}


}; // namespace SpaceShip

}; // namespace PGA
