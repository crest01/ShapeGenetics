#ifndef INCLUDED_OPERATORS_CHANGE_SHAPE
#define INCLUDED_OPERATORS_CHANGE_SHAPE

#pragma once


#include "DualBuild.h"
#include "Scope.h"

#include "ShapeTraits.h"

#include <template_utils.h>

namespace PGG
{
	namespace Operators
	{
		//template<class OutShape, class Next>
		//template<class OutShape, class CombinedParameters<ShapeCtorParams>, class Next>
		template<class Next,  class ... ShapeCtorParams_or_Next>
		struct ChangeShape
		{
			template <class InShape, class TScope>
			DUAL_BUILD_FUNCTION static void apply(const Scope::ScopedShape<InShape, TScope> & scopedShape);
		};
	}

}

#endif //INCLUDED_OPERATORS_CHANGE_SHAPE