#ifndef INCLUDED_MODIFIERS_DIRECT_CALL
#define INCLUDED_MODIFIERS_DIRECT_CALL

#pragma once

#include "DualBuild.h"
#include "scope/Scope.h"

namespace PGG
{
	namespace Modifiers
	{
		template<class Next>
		struct DirectCall
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION
			static void apply(const Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
				Next :: apply(scopedShape);
			}
		};
	}
}
#endif //INCLUDED_MODIFIERS_DIRECT_CALL