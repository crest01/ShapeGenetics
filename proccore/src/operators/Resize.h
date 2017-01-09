#ifndef INCLUDED_OPERATORS_RESIZE
#define INCLUDED_OPERATORS_RESIZE

#pragma once

#include <math/vector.h>

#include "DualBuild.h"
#include "scope/Scope.h"
#include "shapes/ShapeTraits.h"

#include <template_utils.h>


namespace PGG
{
	namespace Operators
	{
		template<class Param, class Next>
		struct Resize
		{
			template <class InShape, class TScope>
			DUAL_BUILD_FUNCTION static void apply(const Scope::ScopedShape<InShape, TScope> & scopedShape);
		};

		template<class Param, class Next>
		struct SetSize
		{
			template <class InShape, class TScope>
			DUAL_BUILD_FUNCTION static void apply(const Scope::ScopedShape<InShape, TScope> & scopedShape);
		};
	}
}

#endif //INCLUDED_OPERATORS_RESIZE