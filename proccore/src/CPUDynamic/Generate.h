#ifndef CPUDYNAMIC_GENERATE
#define CPUDYNAMIC_GENERATE

#include "DynamicScope.h"
#include "SupportedShapes.h"

namespace PGG
{
	namespace CPU
	{
		class GrammarSystem;

		struct Generator
		{
			virtual void reserveVertices(int num) = 0;
			virtual void reserveIndices(int num) = 0;
			virtual void addVertex(int index, const math::float3& v, const math::float3& n, const math::float2& t) = 0;
			virtual void addTriangle(unsigned int i0, unsigned int i1, unsigned int i2) = 0;
			virtual void finishObject() = 0;
		};

		typedef Generator*(*GenerateBox)(const Shapes::SBox& box);
		typedef Generator*(*GenerateQuad)(const Shapes::SQuad& quad);
		typedef Generator*(*GeneratePolygon)(const Shapes::SPolygon& polygon);
		typedef Generator*(*GeneratePrism)(const Shapes::SPrism& prism);
		typedef Generator*(*GenerateTriangle)(const Shapes::STriangle& triangle);
		typedef Generator*(*GenerateTetrahedron)(const Shapes::STetrahedron& tetrahedron);

		template<class Shape>
		void setDynamicGenerator(GrammarSystem& sys, Generator*(*GenerateFunc)(const Scope::ScopedShape<Shape, DynamicScope>&));
	}
}
#endif