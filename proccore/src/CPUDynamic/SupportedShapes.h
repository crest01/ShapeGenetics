#ifndef CPUDYNAMIC_SUPPORTED_SHAPES
#define CPUDYNAMIC_SUPPORTED_SHAPES

#include "DynamicScope.h"

#include "shapes/Platonics.h"
#include "shapes/Polygon.h"
#include "shapes/Prism.h"
#include "shapes/Quad.h"


namespace PGG
{
	namespace Shapes
	{
		
		typedef Scope::ScopedShape<PGG::Shapes::Box, DynamicScope > SBox;
		typedef Scope::ScopedShape<PGG::Shapes::Quad, DynamicScope > SQuad;
		typedef Scope::ScopedShape<PGG::Shapes::DynamicPolygon<24>, DynamicScope > SPolygon;
		typedef Scope::ScopedShape<PGG::Shapes::DynamicPrism<24>, DynamicScope > SPrism;
		typedef Scope::ScopedShape<PGG::Shapes::Triangle, DynamicScope > STriangle;
		typedef Scope::ScopedShape<PGG::Shapes::Tetrahedron, DynamicScope > STetrahedron;
		typedef Scope::ScopedShape<PGG::Shapes::Octahedron, DynamicScope > SOctahedron;
		typedef Scope::ScopedShape<PGG::Shapes::Dodecahedron, DynamicScope > SDodecahedron;
		typedef Scope::ScopedShape<PGG::Shapes::Icosahedron, DynamicScope > SIcosahedron;
	}
}


#endif //CPUDYNAMIC_SUPPORTED_SHAPES
