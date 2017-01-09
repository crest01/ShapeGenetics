#ifndef INCLUDED_SHAPES_NAME
#define INCLUDED_SHAPES_NAME

#pragma once

#include "DualBuild.h"

#if IS_CUDE_CODE
namespace PGG {
	namespace Shapes {
		typedef const char* String;
	}
}
#else
#include <string>
namespace PGG {
	namespace Shapes {
		typedef std::string String;
	}
}
#endif

namespace PGG
{
	namespace Shapes
	{
		struct Tetrahedron;
		struct Box;
		struct Octahedron;
		struct Dodecahedron;
		struct Icosahedron;
		struct Quad;
		template<int NUMVERTICES>
		struct StaticPolygon;
		template<int MAXNUMVERTICES>
		struct DynamicPolygon;
		template<int NUMVERTICES>
		struct StaticPrism;
		template<int MAXNUMVERTICES>
		struct DynamicPrism;

		DUAL_BUILD_FUNCTION
		template<class Shape>
		String name();

		template<class Shape>
		class ComplexShapeName
		{
			DUAL_BUILD_FUNCTION String name() { return "UnknownShape"; }
		};


		DUAL_BUILD_FUNCTION	template<> String name<Tetrahedron>() { return "Tetrahedron"; }
		DUAL_BUILD_FUNCTION	template<> String name<Box>() { return "Box"; }
		DUAL_BUILD_FUNCTION	template<> String name<Octahedron>() { return "Octahedron"; }
		DUAL_BUILD_FUNCTION	template<> String name<Dodecahedron>() { return "Dodecahedron"; }
		DUAL_BUILD_FUNCTION	template<> String name<Icosahedron>() { return "Icosahedron"; }
		DUAL_BUILD_FUNCTION	template<> String name<Quad>() { return "Quad"; }
		DUAL_BUILD_FUNCTION	template<> String name<StaticPolygon<3>>() { return "Triangle"; }
		template<int NUMVERTICES> struct ComplexShapeName< StaticPolygon<NUMVERTICES> > { DUAL_BUILD_FUNCTION static String name() { return "StaticPolygon"; } };
		template<int MAXNUMVERTICES> struct ComplexShapeName< DynamicPolygon<MAXNUMVERTICES> > { DUAL_BUILD_FUNCTION static String name() { return "DynamicPolygon"; } };
		template<int NUMVERTICES> struct ComplexShapeName< StaticPrism<NUMVERTICES> > { DUAL_BUILD_FUNCTION static String name() { return "StaticPrism"; } };
		template<int MAXNUMVERTICES> struct ComplexShapeName< DynamicPrism<MAXNUMVERTICES> > { DUAL_BUILD_FUNCTION static String name() { return "DynamicPrism"; } };

		DUAL_BUILD_FUNCTION	template<class Shape> String name() { return ComplexShapeName<Shape>::name(); }
	}
}


#endif //INCLUDED_SHAPES_NAME