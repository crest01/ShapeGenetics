#ifndef INCLUDED_PARAMETERS_SHAPE_PARAMETERS
#define INCLUDED_PARAMETERS_SHAPE_PARAMETERS

#pragma once

#include "DualBuild.h"
#include "scope/Scope.h"
#include "parameters/Axis.h"

#include <template_utils.h>

namespace PGG
{
	namespace Parameters
	{
		struct ShapeSize
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static decltype(TShape::size) eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				return scopedShape.shape. size;
			}
		};
		template<Axes Coord>
		struct ShapeSizeAxis
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static float eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				return AccessVecClass<Coord, decltype(TShape::size) >::access(scopedShape.shape.size);
			}
		};
	}
}


#endif //INCLUDED_PARAMETERS_STATIC_PARAMETERS
