#ifndef INCLUDED_MODIFIERS_DISCARD
#define INCLUDED_MODIFIERS_DISCARD

#pragma once

#include "DualBuild.h"
#include "scope/Scope.h"

namespace PGG
{
	namespace Modifiers
	{
		struct Discard
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION
			static void apply(const Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0) { }
		};
	}
}
#endif //INCLUDED_MODIFIERS_DIRECT_CALL