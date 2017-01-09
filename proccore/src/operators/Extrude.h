#ifndef INCLUDED_OPERATORS_EXTRUDE
#define INCLUDED_OPERATORS_EXTRUDE

#pragma once


#include "DualBuild.h"
#include "scope/Scope.h"

namespace PGG
{
	namespace Operators
	{
		template<class FloatParam, class Next>
		struct Extrude
		{
			template <class InShape, class TScope>
			DUAL_BUILD_FUNCTION static void apply(const Scope::ScopedShape<InShape, TScope> & scopedShape);
		};
	}
}

#endif //INCLUDED_OPERATORS_CHANGE_SHAPE