
#include "ExperimentationScene.h"

//#include "CPUDynamic/BufferGenerator.h"
//#include "CPUDynamic/SupportedShapes.h"
//#include "CPUDynamic/SupportedOpertors.h"
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

#include "GeometryData.h"

#include "CPU/StaticCall.h"

#include <chrono>

using namespace PGG;
using namespace Shapes;
using namespace Parameters;
using namespace Scope;
using namespace Operators;
using namespace Modifiers;
using namespace CPU;

ExperimentationScene::ExperimentationScene() : system(4), seed(217197128u), pt(nullptr)
{

}
double ExperimentationScene::generate(std::vector<PGA::GeneratedVertex>& geometry, std::vector<unsigned>& indices)
{
	//CPU::setupDynamicGenerator<Shapes::SBox>(system, false, true, true, true);

	//Parameters::ParameterTable table;
	//int genOffset = table.storeParameters(0);
	//int offop2 = table.storeParameters(math::float3(0.f, 0.f, 0.f), genOffset, CPU::SupportedOperators::Generate, 0);
	//int aoffset = table.storeParameters(math::float3(2.f, 2.f, 2.f), offop2, CPU::SupportedOperators::Translate, 0);
	//system.loadRuntimeInfo(table);

	//Shapes::SBox box(Shapes::Box(math::float3(1, 1, 1)), DynamicScope(math::identity<math::float3x4>(), aoffset));
	//system.addAxiom(box, CPU::DynamicCallFunction(box, CPU::SupportedOperators::Scale));
	//system.run(4);

	//std::vector<CPU::VertexNormalTex> geom(CPU::buffer_generator_vertices_normals_textures[0]->size());
	//num_indices = CPU::buffer_generator_indices_vertices_normals_textures[0]->size();
	//std::vector<unsigned> ind(num_indices * 4);
	//CPU::buffer_generator_vertices_normals_textures[0]->copy(&geom[0]);
	//CPU::buffer_generator_indices_vertices_normals_textures[0]->copy(&ind[0]);

	//========================

	typedef CoordinateframeScope<XorShiftStar<32>, int, int> MyScope;
	typedef CPUGeometryGenerator<MyScope> MyGeometryGenerator;

	typedef Operators::Generator<MyGeometryGenerator> StaticGenerate;
	typedef Operators::Translate<Parameters::StaticFloat3<0, 0, 1_p>, StaticGenerate> TransGen;

	typedef Operators::Resize<Parameters::StaticFloat2<4_p, 2_p>, Modifiers::DirectCall<
		Operators::Repeat<Parameters::StaticAxes<Parameters::Axes::XAxis>, Parameters::StaticFloat<1.0_p>, Modifiers::DirectCall<
		StaticGenerate> > > > ScaleRepGen;
	//StaticRandFloat<1_p, 6_p> // VecEx<math::float3,StaticFloat<4_p>, StaticFloat<4_p>>

	typedef RandomPath<0,
		StaticFloat<0.5_p>, DirectCall<StaticGenerate>,
		StaticFloat<0.5_p>, DirectCall<Extrude<StaticFloat<-1.0_p>, DirectCall< Resize< VecEx<math::float3,StaticFloat<1.0_p>, StaticRandFloat<0_p, 1_p>, StaticFloat<1.0_p>>, DirectCall<StaticGenerate> > >
		> > > RandomExtrude;
	typedef Resize<VecEx<math::float3,StaticRandFloat<1_p, 6_p>, StaticFloat<4_p>>, DirectCall<
		Subdiv<StaticAxes<Axes::XAxis>,
			StaticFloat<1._r>, Reseed<0, StaticCall<RandomExtrude> >,
			StaticFloat<1._r>, DirectCall<StaticGenerate>,
			StaticFloat<1._r>, StaticCall<RandomExtrude>
		> > > ScaleSubdivGen;

	typedef Operators::CompSplit<Modifiers::DirectCall<StaticGenerate>, Modifiers::DirectCall<StaticGenerate>, Modifiers::DirectCall<StaticGenerate> > CompSplitGen;

	ScopedShape<Box, MyScope > initalBox(Box(math::float3(1.0f)), MyScope(math::identity<math::float3x4>(), seed, 0, 0));
	ScopedShape<Quad, MyScope > initalQuad(Quad(math::float2(1.0f)), MyScope(math::identity<math::float3x4>(), seed, 0, 0));
	//system.addAxiom<ScaleSubdivGen>(initalQuad);

	//system.addAxiom<StaticGenerate>(initalQuad);


	//forward declaratons for recursion
	class BodyRuleRecusion; 
	class TopRule;
	class TopRuleRecursion;
	class WingRule;
	class WingRuleRecursion;
	class BodyRule;
#if true

	//STATIC RANDOM VERSION
#if 0
	typedef CoordinateframeScope<XorShiftStar<32>, int> SpaceShipscope;
	typedef CPUGeometryGenerator<SpaceShipscope> SpaceShipGenerator;
	typedef Operators::Generator<SpaceShipGenerator> MyGenerate;

	ScopedShape<Box, SpaceShipscope > spaceShipAxiom(Box(math::float3(1.0f)), SpaceShipscope(math::identity<math::float3x4>(), seed, 0));

	class SpaceShip : public
		Resize<VecEx<math::float3,StaticRandFloat<0.8_p, 1.3_p>, StaticRandFloat<0.4_p, 0.8_p>, StaticRandFloat<0.8_p, 1.3_p>>, DirectCall<BodyRule> >
	{ };

	class BodyRule : public
		Translate<VecEx<math::float3,StaticFloat<0_p>, StaticFloat<0_p>, Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::ZAxis>> >, //0 is starting point, so move the box half the size to the front
			StaticCall <
				Duplicate <
					DirectCall<MyGenerate>, // body part is being generated
					DirectCall < RandomPath < 0,
						StaticFloat<1_p>, DirectCall<Discard>, // does not extend to the front
						StaticFloat<2_p>, DirectCall<Translate<VecEx<math::float3,StaticFloat<0_p>, StaticFloat<0_p>, Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::ZAxis>> >, StaticCall<BodyRuleRecusion> > > > // next body part
					>,
					DirectCall<RandomPath<0,
						StaticFloat<1_p>, DirectCall<Discard>, // does not extend to the top
						StaticFloat<1_p>, DirectCall<Translate<VecEx<math::float3,StaticFloat<0_p>, Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::YAxis>>, StaticFloat<0_p>>, StaticCall<TopRule> > > > // start top 
					>,  
					DirectCall<RandomPath<0,
						StaticFloat<1_p>, DirectCall<Discard>, // does not generate wings
						StaticFloat<1_p>, Duplicate < // mirror
							DirectCall<Translate<VecEx<math::float3, Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::XAxis> >, StaticFloat<0_p>, StaticFloat<0_p>>, StaticCall<WingRule> > >, // wing right
							DirectCall<Rotate<StaticAxes<Axes::ZAxis>, StaticFloat<3.14159265359_p>, DirectCall<Translate<VecEx<math::float3,Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::XAxis> >, StaticFloat<0_p>, StaticFloat<0_p>>, StaticCall<WingRule> > > > > >// wing left
						>
					>
				>
			>
		>
	{ };

	class BodyRuleRecusion : public
		ChoosePath<
			ScopeParamLT<1, StaticInt<5>>, DirectCall< //max body parts
				Resize<VecEx<math::float3,StaticRandFloat<0.5_p, 1.2_p>, StaticRandFloat<0.5_p, 1.2_p>, StaticRandFloat<0.5_p, 1.5_p>>,  //adjust body part size
					IncreaseScopeAttachment<1, StaticInt<1>, //increase recursion counter
						DirectCall<BodyRule> // execute the recursion
					>
				>
			>
		> { };


	class TopRule : public
		Resize<VecEx<math::float3,StaticRandFloat<0.3_p, 0.8_p>, StaticRandFloat<0.1_p, 0.2_p>, StaticRandFloat<0.3_p, 0.8_p>>, // initial top scale
			SetScopeAttachment<1, StaticInt<1>, // set recursion counter to 1
				DirectCall<TopRuleRecursion >
			>
		>
	{};

	class TopRuleRecursion : public
		Translate<VecEx<math::float3,StaticFloat<0_p>, Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::YAxis>>, StaticFloat<0_p> >, //0 is starting point, so move the box half the size up
			Duplicate <
				DirectCall<MyGenerate>, // top part is being generated
				DirectCall<ChoosePath<
						ScopeParamLT<1, StaticInt<4>>, //max top parts
							DirectCall< RandomPath<0,
								StaticFloat<1_p>, DirectCall<Discard>, // does not extend to the top
								StaticFloat<1_p>, DirectCall<
									Translate<VecEx<math::float3,StaticFloat<0_p>, Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::YAxis>>, StaticFloat<0_p> >,
										Resize<VecEx<math::float3,StaticRandFloat<0.6_p, 0.9_p>, StaticRandFloat<0.8_p, 1.1_p>, StaticRandFloat<0.6_p, 0.9_p>>,
											DirectCall<TopRuleRecursion>
										>
									>
								>
							> >
					>
				>
			>
		>
	{};

	class WingRule : public
		Resize<VecEx<math::float3,StaticRandFloat<0.3_p, 1.2_p>, StaticRandFloat<0.2_p, 0.5_p>, StaticRandFloat<0.2_p, 1.0_p>>, // initial wing scale
			SetScopeAttachment<1, StaticInt<1>, // set recursion counter to 1
				DirectCall<WingRuleRecursion >
			>
		>
	{};

	class WingRuleRecursion : public
		Translate<VecEx<math::float3,Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::XAxis>>, StaticFloat<0_p>, StaticFloat<0_p> >, //0 is starting point, so move the box half the size up
			Duplicate <
				DirectCall<MyGenerate>, // wing part is being generated
				DirectCall<ChoosePath<
						ScopeParamLT<1, StaticInt<10>>, //max wing parts
							DirectCall< RandomPath<0,
								StaticFloat<1_p>, DirectCall<Discard>, // does not extend further
								StaticFloat<5_p>, DirectCall<
									Translate<VecEx<math::float3,Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::XAxis>>, StaticFloat<0_p>, StaticFloat<0_p> >,
										Resize<VecEx<math::float3,StaticRandFloat<0.6_p, 1.2_p>, StaticRandFloat<0.9_p, 1.1_p>, StaticRandFloat<0.7_p, 1.2_p>>,
											DirectCall<Translate<VecEx<math::float3,StaticFloat<0_p>, StaticFloat<0_p>, Mul<ShapeSizeAxis<Axes::ZAxis>, StaticRandFloat<-0.5_p, 0.5_p>>>, // backward and forward move
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

#else
	typedef CoordinateframeScope<int> SpaceShipscope;
	typedef CPUGeometryGenerator<SpaceShipscope> SpaceShipGenerator;
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
		Translate<VecEx<math::float3,StaticFloat<0_p>, StaticFloat<0_p>, Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::ZAxis>> >, //0 is starting point, so move the box half the size to the front
			StaticCall <
				Duplicate <
					DirectCall<MyGenerate>, // body part is being generated
					DirectCall < 
						ChoosePath < DynamicInt<3, ParamLayer>, // extend body part
						DirectCall<Translate<VecEx<math::float3,StaticFloat<0_p>, StaticFloat<0_p>, Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::ZAxis>> >,
							SetScopeAttachment<ParamLayer, DynamicInt<4, ParamLayer>, StaticCall<BodyRuleRecusion> > > > > // next body part
					>,
					DirectCall<
						ChoosePath < DynamicInt<5, ParamLayer>, // build top part
						DirectCall<Translate<VecEx<math::float3,StaticFloat<0_p>, Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::YAxis>>, StaticFloat<0_p>>, 
							SetScopeAttachment<ParamLayer, DynamicInt<6, ParamLayer>, StaticCall<TopRule> > > > > // start top 
					>,
					DirectCall<
						ChoosePath < DynamicInt<7, ParamLayer>, //generate wings
							SetScopeAttachment<ParamLayer, DynamicInt<8, ParamLayer>,
							DirectCall< Duplicate < // mirror
								DirectCall<Translate<VecEx<math::float3, Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::XAxis> >, StaticFloat<0_p>, StaticFloat<0_p>>, StaticCall<WingRule> > >, // wing right
								DirectCall<Rotate<StaticAxes<Axes::ZAxis>, StaticFloat<3.14159265359_p>, DirectCall<Translate<VecEx<math::float3,Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::XAxis> >, StaticFloat<0_p>, StaticFloat<0_p>>, StaticCall<WingRule> > > > > >// wing left
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
		Translate<VecEx<math::float3,StaticFloat<0_p>, Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::YAxis>>, StaticFloat<0_p> >, //0 is starting point, so move the box half the size up
			Duplicate <
				DirectCall<MyGenerate>, // top part is being generated
				DirectCall<ChoosePath< 
					DynamicInt<3, ParamLayer>, 
						DirectCall< Translate<VecEx<math::float3,StaticFloat<0_p>, Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::YAxis>>, StaticFloat<0_p> >,
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
			DirectCall<Translate<VecEx<math::float3,StaticFloat<0_p>, StaticFloat<0_p>, Mul<ShapeSizeAxis<Axes::ZAxis>, DynamicFloat<3, ParamLayer>>>,
				DirectCall<WingRuleRecursion >
			> >
		>
	{};

	// offset 4: int 1/0 to enable next wing part
	// offset 5: int to change paramtable for next top part
	class WingRuleRecursion : public
		Translate<VecEx<math::float3,Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::XAxis>>, StaticFloat<0_p>, StaticFloat<0_p> >, //0 is starting point, so move the box half the size up
			Duplicate <
				DirectCall<MyGenerate>, // wing part is being generated
				DirectCall<ChoosePath<
						DynamicInt<4, ParamLayer>, 
							DirectCall< Translate<VecEx<math::float3,Mul<StaticFloat<0.5_p>, ShapeSizeAxis<Axes::XAxis>>, StaticFloat<0_p>, StaticFloat<0_p> >,
								SetScopeAttachment<ParamLayer, DynamicInt<5, ParamLayer>,
									DirectCall< Resize<DynamicFloat3<0, ParamLayer>, 
											DirectCall<Translate<VecEx<math::float3,StaticFloat<0_p>, StaticFloat<0_p>, Mul<ShapeSizeAxis<Axes::ZAxis>, DynamicFloat<3, ParamLayer>>>, // backward and forward move
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

	ParameterTable simplept;
	if (!pt)
	{
		int body3 = simplept.storeParameters(math::float3(0.8f, 0.8f, 1.8f), 0,0, 0,0, 0,0);

		int body2 = simplept.storeParameters(math::float3(0.8f, 0.8f, 0.8f), 1,body3, 0,0, 0,0);

		int top12 = simplept.storeParameters(math::float3(0.6f, 1.0f, 0.6f), 0, 0);
		int top11 = simplept.storeParameters(math::float3(0.6f, 1.0f, 0.6f), 1, top12);
		int top10 = simplept.storeParameters(math::float3(0.6f, 0.1f, 0.6f), 1, top11);

		int wing11 = simplept.storeParameters(math::float3(1.0f, 1.0f, 1.0f), -0.2f, 0, 0);
		int wing10 = simplept.storeParameters(math::float3(1.2f, 0.2f, 0.8f), 0.0f, 1, wing11);

		int body1 = simplept.storeParameters(math::float3(1.2f, 1.2f, 6.0f), 1,body2, 1,top10, 1,wing10);

		axiomId = simplept.storeParameters(math::float3(1, 0.6f, 0.2f), 1, body1, 0,0, 0,0);
		system.registerParameterTable(simplept);
	}
	else
	{
		system.registerParameterTable(*pt);
	}
	ScopedShape<Box, SpaceShipscope > spaceShipAxiom(Box(math::float3(1.0f)), SpaceShipscope(math::identity<math::float3x4>(), axiomId));
	
#endif

	system.addAxiom<SpaceShip>(spaceShipAxiom);
#endif
	//DirectCall< Resize<VecEx<math::float3,StaticRandFloat<1_p, 6_p>, StaticRandFloat<1_p, 6_p>, StaticRandFloat<1_p, 6_p>>
	
	auto t0 = std::chrono::high_resolution_clock::now();
	system.run(8);
	auto t1 = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);

	return time_span.count();
}

void ExperimentationScene::setParameterTable(const ParameterTable& parameterTable, int _axiomId)
{
	axiomId = _axiomId;
	pt = &parameterTable;
}

