#ifndef CPUDYNAMIC_FUNCTION_TABLE
#define CPUDYNAMIC_FUNCTION_TABLE

#pragma once

#include "DynamicScope.h"
#include "DynamicCall.h"

#include "SupportedShapes.h"


namespace PGG
{
	namespace CPU
	{
		template<>
		void(*DynamicCallFunction(const PGG::Shapes::SBox& scopedShape, int id))(const PGG::Shapes::SBox&);
		template<>
		void(*DynamicCallFunction(const PGG::Shapes::SQuad& scopedShape, int id))(const PGG::Shapes::SQuad&);
		template<>
		void(*DynamicCallFunction(const PGG::Shapes::SPolygon& scopedShape, int id))(const PGG::Shapes::SPolygon&);
		template<>
		void(*DynamicCallFunction(const PGG::Shapes::SPrism& scopedShape, int id))(const PGG::Shapes::SPrism&);
		template<>
		void(*DynamicCallFunction(const PGG::Shapes::STetrahedron& scopedShape, int id))(const PGG::Shapes::STetrahedron&);
		template<>
		void(*DynamicCallFunction(const PGG::Shapes::SOctahedron& scopedShape, int id))(const PGG::Shapes::SOctahedron&);
		template<>
		void(*DynamicCallFunction(const PGG::Shapes::SDodecahedron& scopedShape, int id))(const PGG::Shapes::SDodecahedron&);
		template<>
		void(*DynamicCallFunction(const PGG::Shapes::SIcosahedron& scopedShape, int id))(const PGG::Shapes::SIcosahedron&);
	}
}

#endif