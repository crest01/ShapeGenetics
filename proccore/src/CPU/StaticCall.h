#ifndef CPU_STATIC_ENQUEUE
#define CPU_STATIC_ENQUEUE

#pragma once

#include "scope/Scope.h"
#include "Evaluator.h"

namespace PGG
{
	namespace CPU
	{

		template<class Next, class PhaseParam = void>
		struct StaticCall
		{
			template<class TShape, class TScope>
			static void apply(const Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
				int phase = PhaseParam::eval(scopedShape);
				current_evalutator->enqueue(scopedShape, [](const Scope::ScopedShape<TShape, TScope>& s){ Next::apply(s); }, phase);
			}
		};

		template<class Next>
		struct StaticCall<Next, void>
		{
			template<class TShape, class TScope>
			static void apply(const Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
				current_evalutator->enqueue(scopedShape, 
					[](const Scope::ScopedShape<TShape, TScope>& s){ Next::apply(s); });
			}
		};
	}
}

#endif
