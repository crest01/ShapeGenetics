#ifndef INCLUDED_OPERATORS_DUPLICATE
#define INCLUDED_OPERATORS_DUPLICATE

#pragma once

#include <math/vector.h>

#include "DualBuild.h"
#include "scope/Scope.h"
#include "shapes/ShapeTraits.h"
#include "parameters/Axis.h"

#include <template_utils.h>


namespace PGG
{
	namespace Operators
	{
		template<class ... AllNext>
		struct Duplicate
		{
			template <class InShape, class TScope>
			DUAL_BUILD_FUNCTION static void apply(const Scope::ScopedShape<InShape, TScope> & scopedShape);
		};
	}
}

#endif //INCLUDED_OPERATORS_REPEAT