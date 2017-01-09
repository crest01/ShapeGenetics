#ifndef INCLUDED_SHAPES_VERTEX
#define INCLUDED_SHAPES_VERTEX

#pragma once

#include <math/vector.h>
#include <math/matrix.h>

#include "DualBuild.h"
#include "ShapeTraits.h"


namespace PGG
{
	namespace Shapes
	{
		namespace Traits
		{
			struct Vertex
			{
				math::float3 pos;
			};

			template<> struct ConstructByVector < Vertex > { static const int value = 3; };
		}
	}
}

#endif //INCLUDED_SHAPES_VERTEX
