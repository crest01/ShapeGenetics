#ifndef INCLUDED_OPERATORS_GENERATOR
#define INCLUDED_OPERATORS_GENERATOR

#pragma once

#include "DualBuild.h"
#include "Scope.h"

namespace PGG
{
	namespace Operators
	{
		template<class CustomGenerator>
		struct Generator
		{
			template <class InShape, class TScope>
			DUAL_BUILD_FUNCTION static void apply(const Scope::ScopedShape<InShape, TScope> & scopedShape);
		};
	}
}

#endif //INCLUDED_OPERATORS_GENERATOR