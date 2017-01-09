#ifndef INCLUDED_SHAPES_POLYGON
#define INCLUDED_SHAPES_POLYGON

#pragma once


#include <math/vector.h>

#include "DualBuild.h"
#include "ShapeTraits.h"

namespace PGG
{
	namespace Shapes
	{
		template<int NUMVERTICES>
		struct StaticPolygon
		{
			math::float2 vertices[NUMVERTICES];

			DUAL_BUILD_FUNCTION StaticPolygon() = default;
			DUAL_BUILD_FUNCTION StaticPolygon(math::float2 vertices[NUMVERTICES])
			{
				for (int i = 0; i < NUMVERTICES; ++i)
					this->vertices[i] = vertices[i];
			}
		};

		template<int MAXNUMVERTICES>
		struct DynamicPolygon
		{
			math::float2 vertices[MAXNUMVERTICES];
			int num_vertices;

			DUAL_BUILD_FUNCTION DynamicPolygon() = default;
			DUAL_BUILD_FUNCTION DynamicPolygon(int num_vertices, math::float2* vertices) 
				: num_vertices(num_vertices)
			{
				for (int i = 0; i < num_vertices; ++i)
					this->vertices[i] = vertices[i];
			}
		};

		typedef StaticPolygon<3> Triangle;
		namespace Traits {
			template<int NUMVERTICES> struct Dimensions < StaticPolygon<NUMVERTICES> > { static const int value = 2; };
			template<int NUMVERTICES>  struct ConstructExplicit< StaticPolygon<NUMVERTICES> > { static const bool value = true; };

			template<int MAXNUMVERTICES> struct Dimensions < DynamicPolygon<MAXNUMVERTICES> > { static const int value = 2; };
			template<int MAXNUMVERTICES>  struct ConstructExplicit< DynamicPolygon<MAXNUMVERTICES> > { static const bool value = true; };
		}
	}
}

#endif //INCLUDED_SHAPES_POLYGON
