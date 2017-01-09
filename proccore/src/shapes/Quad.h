#ifndef INCLUDED_SHAPES_QUAD
#define INCLUDED_SHAPES_QUAD

#pragma once



#include <math/vector.h>
#include <math/matrix.h>

#include "DualBuild.h"
#include "ShapeTraits.h"

namespace PGG
{
	namespace Shapes
	{
		struct Quad
		{
			math::float2 size;

			DUAL_BUILD_FUNCTION Quad() = default;
			DUAL_BUILD_FUNCTION Quad(math::float2 size)
				: size(size)
			{ }
			DUAL_BUILD_FUNCTION Quad(float xSize, float ySize)
				: size(xSize, ySize)
			{ }
		};

		namespace Traits {
			template<> struct Dimensions < Quad > { static const int Value = 2; };
			template<> struct ConstructByVector< Quad > { static const int Value = 2; };
			template<> struct HasSizeVector < Quad > { static const int value = 2; };
		}
	}
}

#endif //INCLUDED_SHAPES_PLATONICS
