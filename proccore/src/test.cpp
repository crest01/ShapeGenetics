#include <iostream>

#include "Platonics.h"
#include "Quad.h"
#include "Prism.h"

#include "Scope.h"
#include "StaticParameter.h"
#include "DynamicParameter.h"
#include "Discard.h"
#include "DirectCall.h"
#include "StaticCall.h"

#include "ChangeShape.impl"
#include "ScopeOperators.impl"
#include "Resize.impl"
#include "Repeat.impl"
#include "Subdivide.impl"

#include "Print.h"

#include "GrammarSystem.h"
#include "DynamicCall.h"
#include "ParameterTable.h"


#include "CPUDynamic/BufferGenerator.h"
#include "CPUDynamic/SupportedShapes.h"
#include "CPUDynamic/SupportedOpertors.h"

void compiletest()
{
/*
	using namespace PGG;
	using namespace Scope;
	using namespace Shapes;
	using namespace Operators;
	using namespace Parameters;
	using namespace Modifiers;
	using namespace CPU;

	typedef math::float2 f2;

	ScopedShape<Box, CoordinateframeScope<> > initalBox(Box(math::float3(1.0f)), CoordinateframeScope<>(math::identity<math::float3x4>()));
	ScopedShape<Quad, CoordinateframeScope<> > initalQuad(Quad(math::float2(5.0f)), CoordinateframeScope<>(math::identity<math::float3x4>()));
	ScopedShape<DynamicPrism<8>, CoordinateframeScope<> > initalPrism;
	initalPrism.shape.height = 5.0f;
	initalPrism.shape.num_vertices = 5;
	initalPrism.scope = CoordinateframeScope<>(math::identity<math::float3x4>());
*/
	////Repeat<StaticAxes<Axes::YAxis>, StaticFloat<0x3f000000>, PrintPosition< Discard > >::apply(initalPrism);
	//Subdiv<StaticAxes<Axes::YAxis>,
	//		StaticFloat<0x3f000000>, PrintPosition< Discard >,
	//		StaticFloat<0x3f800000>, PrintPosition< Discard >,
	//		MakeRel< StaticFloat<0x3f000000> >, PrintPosition< Discard > 
	//>::apply(initalPrism);

	//Subdiv<StaticAxes<Axes::XAxis>,
	//		StaticFloat<0x3f000000>, PrintShapeInfo< Discard >,
	//		StaticFloat<0x3f800000>, PrintShapeInfo< Discard >,
	//		MakeRel< StaticFloat<0x3f000000> >, PrintShapeInfo< Discard >
	//>::apply(initalBox);


	//Translate<StaticFloat3<0x3f800000, 0x40000000, 0x40400000 >, PrintShapeInfo< Discard > >::apply(initalBox);
	//Scale<StaticFloat3<0x3f800000, 0x40000000, 0x40400000 >, PrintShapeInfo< Discard > >::apply(initalBox);
	//Scale<StaticFloat<0x40000000 >, PrintShapeInfo< Discard > >::apply(initalBox);
	//Rotate<StaticAxes<Axes::XAxis>, StaticFloat<0x40000000 >, PrintShapeInfo< Discard > >::apply(initalBox);
	//Rotate<StaticAxes<Axes::YAxis>, StaticFloat<0x40000000 >, PrintShapeInfo< Discard > >::apply(initalBox);
	//Rotate<StaticAxes<Axes::ZAxis>, StaticFloat<0x40000000 >, PrintShapeInfo< Discard > >::apply(initalBox);
	//Rotate<StaticFloat3x3<0x3f800000, 0x40000000, 0x40400000, 0x3f800000, 0x40000000, 0x40400000, 0x3f800000, 0x40000000, 0x40400000 >, PrintShapeInfo< Discard > >::apply(initalBox);
	//TransformObject<StaticFloat4x4<0x3f800000, 0x40000000, 0x40400000, 0x3f800000, 0x40000000, 0x40400000, 0x3f800000, 0x40000000, 0x40400000, 0x3f800000, 0x40000000, 0x40400000, 0x3f800000, 0x40000000, 0x40400000, 0x40400000 >, PrintShapeInfo< Discard > >::apply(initalBox);
	//SetSize<StaticFloat3<0x3f800000, 0x40000000, 0x40400000>, PrintShapeInfo< Discard> > ::apply(initalBox);
	//Repeat<StaticAxes<Axes::XAxis>, StaticFloat<0x3f000000>, PrintShapeInfo< Discard> >:: apply(initalQuad);



	//ChangeShape<Tetrahedron, PrintShapeInfo< Discard > > ::apply(initalBox);


	//Subdiv<StaticAxes<Axes::XAxis>, 
	//	StaticFloat<0x3f000000>, PrintShapeInfo< Discard >,
	//	StaticFloat<0x3f800000>, PrintShapeInfo< Discard >,
	//	MakeRel< StaticFloat<0x3f000000> >,

	//	DirectCall<
	//	SubdivPrecomputed<
	//		StaticAxes<Axes::YAxis>,
	//		StaticFloat<0x3fc00000>, StaticFloat<0x40000000>,
	//		MakeRel<StaticFloat<0x3f800000> >, PrintShapeInfo< Discard >,
	//		StaticFloat<0x3f000000>, PrintShapeInfo< Discard >,
	//		StaticFloat<0x3f800000>, PrintShapeInfo< Discard >,
	//		MakeRel<StaticFloat<0x3f800000> >, PrintShapeInfo< Discard > 
	//	> >

	//	> ::apply(initalBox);

	//typedef Translate<StaticFloat3<0x3f800000, 0x40000000, 0x40400000 >, PrintShapeInfo< Discard > > Op2;
	//typedef Rotate<StaticAxes<Axes::XAxis>, StaticFloat<0x40000000 >, PrintShapeInfo< StaticCall<Op2> > > Op1;
	//typedef Resize<StaticFloat3<0x3f800000, 0x40000000, 0x40400000 >, PrintShapeInfo< StaticCall<Op1> > > InitalOp;

//	CPU::GrammarSystem system(4);

	//CPU::setupDynamicGenerator<Shapes::SBox>(system);

	//ParameterTable table;
	//int genOffset = table.storeParameters(0);
	//int offop2 = table.storeParameters(math::float3(1.f, 0.5f, 0.f), genOffset, SupportedOperators::Generate, 0);
	//int aoffset = table.storeParameters(math::float3(2.f, 2.f, 2.f), offop2, SupportedOperators::Translate, 0);
	//system.loadRuntimeInfo(table);

	//SBox box(Box(math::float3(1, 1, 1)), DynamicScope(math::identity<math::float3x4>(), aoffset));
	//system.addAxiom(box, DynamicCallFunction(box, SupportedOperators::Scale));
	//system.run(4);

	//CPU::buffer_generator_vertices_normals[0]->size();
	//CPU::buffer_generator_indices_vertices_normals[0]->size();
}
