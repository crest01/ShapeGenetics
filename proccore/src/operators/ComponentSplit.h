#ifndef INCLUDED_OPERATORS_COMPONENT_SPLIT
#define INCLUDED_OPERATORS_COMPONENT_SPLIT

#pragma once


#include "DualBuild.h"
#include "scope/Scope.h"

namespace PGG
{
	namespace Operators
	{
		template<class ... PartsAndNext>
		struct CompSplit
		{
			template <class InShape, class TScope>
			DUAL_BUILD_FUNCTION static void apply(const Scope::ScopedShape<InShape, TScope> & scopedShape);
		};
	}
}

#endif //INCLUDED_OPERATORS_CHANGE_SHAPE