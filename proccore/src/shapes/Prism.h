#ifndef INCLUDED_SHAPES_PRISM
#define INCLUDED_SHAPES_PRISM

#pragma once

#include <math/vector.h>

#include "DualBuild.h"
#include "ShapeTraits.h"

namespace PGG
{
	namespace Shapes
	{
		template<int NUMVERTICES>
		struct StaticPrism
		{
			math::float2 basevertices[NUMVERTICES];
			float height;

			DUAL_BUILD_FUNCTION StaticPrism() = default;
			DUAL_BUILD_FUNCTION StaticPrism(math::float2 vertices[NUMVERTICES], float height) : height(height)
			{
				*basevertices = *vertices;
			}
		};

		template<int MAXNUMVERTICES>
		struct DynamicPrism
		{
			math::float2 basevertices[MAXNUMVERTICES];
			int num_vertices;
			float height;
		};

		namespace Traits {
			template<int NUMVERTICES> struct Dimensions < StaticPrism<NUMVERTICES> > { static const int Value = 3; };
			template<int NUMVERTICES> struct HasSizeVector < StaticPrism<NUMVERTICES>  > { static const int value = 1; };

			template<int MAXNUMVERTICES> struct Dimensions < DynamicPrism<MAXNUMVERTICES> > { static const int Value = 3; };
			template<int MAXNUMVERTICES> struct HasSizeVector < DynamicPrism<MAXNUMVERTICES>  > { static const int value = 1; };
		}
	}
}

#endif //INCLUDED_SHAPES_PLATONICS
