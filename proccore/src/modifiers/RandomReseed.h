#ifndef INCLUDED_MODIFIERS_RANDOM_RESEED
#define INCLUDED_MODIFIERS_RANDOM_RESEED

#pragma once

#include "DualBuild.h"
#include "scope/Scope.h"

namespace PGG
{
	namespace Modifiers
	{
		template<int ScopeLayer, class NextModifier>
		struct Reseed
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION
				static void apply(const Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
				Scope::ScopedShape<TShape, TScope> nShape(scopedShape);
				nShape.scope. template accessLayer<ScopeLayer>().reinitialize();
				NextModifier::apply(nShape, outputId);
			}
		};

		template<int ScopeLayer, class NextModifier>
		struct ReseedMulti
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION
				static void apply(const Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
				Scope::ScopedShape<TShape, TScope> nShape(scopedShape);
				for (int i = 0; i <= outputId; ++i)
					nShape.scope. template accessLayer<ScopeLayer>().reinitialize();
				NextModifier::apply(nShape, outputId);
			}
		};

		template<int ScopeLayer, class NextModifier>
		struct ReseedBase
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION
				static void apply(Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
				scopedShape.scope. template accessLayer<ScopeLayer>().reinitialize();
				NextModifier::apply(scopedShape, outputId);
			}
		};


	}
}
#endif //INCLUDED_MODIFIERS_SCOPE_MODIFIER