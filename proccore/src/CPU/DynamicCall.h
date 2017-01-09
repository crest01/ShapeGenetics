#ifndef CPU_DYNAMIC_ENQUEUE
#define CPU_DYNAMIC_ENQUEUE

#pragma once

#include "scope/Scope.h"
#include "Evaluator.h"
#include <tuple>
#include <functional>

namespace PGG
{
	namespace CPU
	{

		template<class ScopedShape>
		void (*DynamicCallFunction(const ScopedShape& scopedShape, int id))(const ScopedShape&);

		enum class SupportedOperators;
		template<class ScopedShape>
		void(*DynamicCallFunction(const ScopedShape& scopedShape, SupportedOperators id))(const ScopedShape&) { return DynamicCallFunction(scopedShape, static_cast<int>(id)); }

		template<class OperatorIdParam, class PhaseParam = void>
		struct DynamicCall
		{
			template<class TShape, class TScope>
			static void apply(const Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
				int phase = PhaseParam::eval(scopedShape);
				int id = OperatorIdParam::eval(scopedShape);

				if (id == -1)
					return;

				auto f = DynamicCallFunction(scopedShape, id);
				current_evalutator->enqueue(scopedShape, f, phase);
			}
		};

		template<class OperatorIdParam>
		struct DynamicCall<OperatorIdParam, void>
		{
			template<class TShape, class TScope>
			static void apply(const Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
				int id = OperatorIdParam::eval(scopedShape);

				if (id == -1)
					return;

				auto f = DynamicCallFunction(scopedShape, id);
				current_evalutator->enqueue(scopedShape, f, 0);
			}
		};


		template<int ScopeLayer, class LayerParam, class OperatorIdParam, class PhaseParam = void>
		struct DynamicCallAndChangeContext
		{
			template<class TShape, class TScope>
			static void apply(const Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
				int id = OperatorIdParam::eval(scopedShape);
				if (id == -1)
					return;
				int phase = PhaseParam::eval(scopedShape);
				Scope::ScopedShape<TShape, TScope> nShape(scopedShape);
				nShape.scope. template accessLayer<ScopeLayer>() = LayerParam::eval(scopedShape);

				auto f = DynamicCallFunction(nShape, id);
				current_evalutator->enqueue(nShape, f, phase);
			}
		};

		template<int ScopeLayer, class LayerParam, class OperatorIdParam>
		struct DynamicCallAndChangeContext<ScopeLayer, LayerParam,OperatorIdParam, void>
		{
			template<class TShape, class TScope>
			static void apply(const Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
				int id = OperatorIdParam::eval(scopedShape);
				if (id == -1)
					return;
				Scope::ScopedShape<TShape, TScope> nShape(scopedShape);
				nShape.scope. template accessLayer<ScopeLayer>() = LayerParam::eval(scopedShape);

				auto f = DynamicCallFunction(nShape, id);
				current_evalutator->enqueue(nShape, f);
			}
		};
	}
}

#endif
