/*
* BuildingGrammar.h
*
*  Created on: Aug 24, 2016
*      Author: Markus Steinberger
*/

#pragma once

#include "operators/Generator.impl"
#include "operators/ScopeOperators.impl"
#include "operators/Resize.impl"
#include "operators/Repeat.impl"
#include "operators/Subdivide.impl"
#include "operators/ComponentSplit.impl"
#include "operators/Extrude.impl"
#include "operators/RandomPath.impl"
#include "operators/Duplicate.impl"
#include "operators/ChangeShape.impl"
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

#include "BaseGrammar.h"



namespace PGA 
{
	namespace City 
	{
		class BuildingGrammar
		{
			float BuildingBaseSize, BuildingMaxHeight, FloorHeight, OutputScale;
		public:

			BuildingGrammar(float BuildingBaseSize, float BuildingMaxHeight, float FloorHeight, float OutputScale) :
				BuildingBaseSize(BuildingBaseSize), BuildingMaxHeight(BuildingMaxHeight), FloorHeight(FloorHeight), OutputScale(OutputScale)
			{
			}
			void setupBuildingParamters(SymbolManager& sm, int symbol)
			{
				sm.addParameter<math::float2>(symbol, "size", math::float2(0.2f, 0.2f)*BuildingBaseSize, math::float2(0.9f, 0.9f)*BuildingBaseSize);
				sm.addParameter<float>(symbol, "height", 1.2f*FloorHeight, BuildingMaxHeight);
				sm.addParameter<float>(symbol, "floorheight", 0.8f*FloorHeight, 1.2f*FloorHeight);
				sm.addParameter<float>(symbol, "rotation", 0, math::constants<float>::pi());
			}
			int storeBuildingParameter(Symbol* symbol, PGG::Parameters::ParameterTable& pt)
			{
				math::float2 size = OutputScale*symbol->getParameter()[0]->getValue<math::float2>();
				float height = OutputScale*symbol->getParameter()[1]->getValue<float>();
				float floorheight = OutputScale*symbol->getParameter()[2]->getValue<float>();
				float rotation = symbol->getParameter()[3]->getValue<float>();
				return pt.storeParameters(size, height, floorheight, rotation);
			}

			static const int np05263 = -0.05263157894736842105_p;
			static const int p05263 = 0.05263157894736842105_p;
			static const int p05 = 0.05_p;
			static const int p1 = 0.1_p;
			static const int p2 = 0.2_p;
			static const int p5 = 0.5_p;
			static const int p95 = 0.95_p;
			static const int p99 = 0.99_p;
			static const int f1 = 1.0_p;
			static const int f2 = 2.0_p;
			static const int f4 = 4.0_p;
			static const int f20 = 20.0_p;

			template<class Next>
			class ExtrudeToWall : public
				PGG::Modifiers::DirectCall<PGG::Operators::Extrude<
					PGG::Parameters::Mul<PGG::Parameters::ShapeSizeAxis<PGG::Parameters::Axes::YAxis>, PGG::Parameters::StaticFloat<np05263>>,
					PGG::Modifiers::DirectCall<Next>
				> >
			{ };
			template<class WallGenerate, class WindowGenerate, int ParamLayer, int RandLayer>
			class FacadeElementInner : public
				PGG::Operators::Subdiv<PGG::Parameters::StaticAxes<PGG::Parameters::Axes::YAxis>,
					PGG::Parameters::MakeRel<PGG::Parameters::StaticFloat<f2>>, PGG::Modifiers::DirectCall<WallGenerate>,
					PGG::Parameters::Mul<PGG::Parameters::ShapeSizeAxis<PGG::Parameters::Axes::YAxis>, PGG::Parameters::StaticRandFloat<p5, p99, RandLayer>>, PGG::Modifiers::DirectCall<WindowGenerate>,
					PGG::Parameters::MakeRel<PGG::Parameters::StaticFloat<f1>>, PGG::Modifiers::DirectCall<WallGenerate>
				>
			{ };
			template<class WallGenerate, class WindowGenerate, int ParamLayer, int RandLayer>
			class FacadeElement : public
				PGG::Operators::Subdiv<PGG::Parameters::StaticAxes<PGG::Parameters::Axes::XAxis>,
					PGG::Parameters::MakeRel<PGG::Parameters::StaticFloat<f1>>, PGG::Modifiers::DirectCall<WallGenerate>,
					PGG::Parameters::MakeRel<PGG::Parameters::StaticRandFloat<f4, f20, RandLayer>>, PGG::CPU::StaticCall<FacadeElementInner<WallGenerate, WindowGenerate, ParamLayer, RandLayer>>,
					//PGG::Parameters::MakeRel<PGG::Parameters::StaticFloat<f20>>, PGG::CPU::StaticCall<FacadeElementInner<WallGenerate, WindowGenerate, ParamLayer, RandLayer>>,
					PGG::Parameters::MakeRel<PGG::Parameters::StaticFloat<f1>>, PGG::Modifiers::DirectCall<WallGenerate>
				>
			{ };

			template<class WallGenerate, class WindowGenerate, int ParamLayer, int RandLayer>
			class Facade : public
				PGG::Operators::Repeat<PGG::Parameters::StaticAxes<PGG::Parameters::Axes::XAxis>,
					PGG::Parameters::Mul<PGG::Parameters::ShapeSizeAxis<PGG::Parameters::Axes::YAxis>, PGG::Parameters::StaticRandFloat<p1, f1, RandLayer>>,
						PGG::CPU::StaticCall<FacadeElement<WallGenerate, WindowGenerate, ParamLayer, RandLayer>>
				>
			{ };

			template<class WallGenerate, class WindowGenerate, int ParamLayer, int RandLayer>
			class FacadeWall : public
				PGG::Operators::Subdiv<PGG::Parameters::StaticAxes<PGG::Parameters::Axes::XAxis>,
					PGG::Parameters::Mul<PGG::Parameters::ShapeSizeAxis<PGG::Parameters::Axes::YAxis>, PGG::Parameters::StaticFloat<p05263>>, PGG::CPU::StaticCall<WallGenerate>,
					PGG::Parameters::MakeRel<PGG::Parameters::StaticFloat<f1>>, PGG::Modifiers::DirectCall<Facade<WallGenerate, WindowGenerate, ParamLayer, RandLayer>>,
					PGG::Parameters::Mul<PGG::Parameters::ShapeSizeAxis<PGG::Parameters::Axes::YAxis>, PGG::Parameters::StaticFloat<p05263>>, PGG::Modifiers::Discard
				>
			{ };



			template<class WallGenerate, class WindowGenerate, int ParamLayer, int RandLayer>
			class Walls : public
				PGG::Operators::CompSplit<
					PGG::Modifiers::Discard,
					PGG::Modifiers::Discard,
					PGG::CPU::StaticCall<ExtrudeToWall<FacadeWall<WallGenerate, WindowGenerate, ParamLayer, RandLayer>>>
				>
			{ };

			template<class WallGenerate, int ParamLayer>
			class Ceiling : public
				WallGenerate
			{ };



			template<class WallGenerate, class WindowGenerate, int ParamLayer, int RandLayer>
			class Floor : public
				PGG::Operators::Subdiv<PGG::Parameters::StaticAxes<PGG::Parameters::Axes::YAxis>, 
					PGG::Parameters::MakeRel<PGG::Parameters::StaticFloat<0.95_p>>, PGG::Modifiers::DirectCall<Walls< WallGenerate, WindowGenerate, ParamLayer, RandLayer>>,
					PGG::Parameters::MakeRel<PGG::Parameters::StaticFloat<0.05_p>>, PGG::CPU::StaticCall<Ceiling<WallGenerate,ParamLayer>>
				>
			{ };

			// offset 0: float2 building base size
			// offset 2: building height
			// offset 3: floor height
			// offset 4: building rotation
			template<class WallGenerate, class WindowGenerate, int ParamLayer, int RandLayer>
			class Building : public
				PGG::Operators::Rotate<PGG::Parameters::StaticAxes<PGG::Parameters::Axes::YAxis>, PGG::Parameters::DynamicFloat<4>, 
					//PGG::Operators::Resize<PGG::Parameters::DynamicFloat2<0, ParamLayer>, 
						PGG::Modifiers::DirectCall<PGG::Operators::ChangeShape<PGG::Shapes::Box, PGG::Parameters::VecEx<math::float3, PGG::Parameters::DynamicFloat<0, ParamLayer>, PGG::Parameters::DynamicFloat<2, ParamLayer>, PGG::Parameters::DynamicFloat<1, ParamLayer>>,
							PGG::Operators::Translate< PGG::Parameters::VecEx<math::float3, PGG::Parameters::StaticFloat<0>, PGG::Parameters::Mul<PGG::Parameters::ShapeSizeAxis<PGG::Parameters::Axes::YAxis>, PGG::Parameters::StaticFloat<p5>>, PGG::Parameters::StaticFloat<0>>,
						//PGG::Modifiers::DirectCall<PGG::Operators::Extrude<PGG::Parameters::DynamicFloat<2, ParamLayer>,
								PGG::Operators::Repeat<PGG::Parameters::StaticAxes<PGG::Parameters::Axes::YAxis>, PGG::Parameters::DynamicFloat<3, ParamLayer>, PGG::CPU::StaticCall<Floor<WallGenerate, WindowGenerate, ParamLayer, RandLayer>> > >
							> 
						>
					//> 
				>
			{ };

			// offset 0: float2 building base size
			// offset 2: building height
			// offset 3: floor height
			// offset 4: building rotation
			template<class BoxGenerate, int ParamLayer>
			class SimpleBuilding : public
				PGG::Operators::Rotate<PGG::Parameters::StaticAxes<PGG::Parameters::Axes::YAxis>, PGG::Parameters::DynamicFloat<4>,
					PGG::Modifiers::DirectCall<PGG::Operators::ChangeShape<PGG::Shapes::Box, PGG::Parameters::VecEx<math::float3, PGG::Parameters::DynamicFloat<0, ParamLayer>, PGG::Parameters::DynamicFloat<2, ParamLayer>, PGG::Parameters::DynamicFloat<1, ParamLayer>>,
						PGG::Operators::Translate< PGG::Parameters::VecEx<math::float3, PGG::Parameters::StaticFloat<0>, PGG::Parameters::Mul<PGG::Parameters::ShapeSizeAxis<PGG::Parameters::Axes::YAxis>, PGG::Parameters::StaticFloat<p5>>, PGG::Parameters::StaticFloat<0>>,
							PGG::CPU::StaticCall<BoxGenerate> >
						>
					>
				>
			{ };


		};

	}
}