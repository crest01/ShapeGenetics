#ifndef INCLUDED_MODIFIERS_SCOPE_MODIFIER
#define INCLUDED_MODIFIERS_SCOPE_MODIFIER

#pragma once

#include "DualBuild.h"
#include "scope/Scope.h"

namespace PGG
{
	namespace Modifiers
	{
		template<int ScopeLayer, class Parameter, class NextModifier>
		struct SetScopeAttachment
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION
			static void apply(Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
				auto val = Parameter::eval(scopedShape);
				Scope::ScopedShape<TShape, TScope>& nShape(scopedShape);
				nShape.scope. template accessLayer<ScopeLayer>() = val;
				NextModifier:: apply(nShape, outputId);
			}
		};
		template<int ScopeLayer, class Parameter, class NextModifier>
		struct IncreaseScopeAttachment
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION
				static void apply(Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
				auto val = Parameter::eval(scopedShape);
				Scope::ScopedShape<TShape, TScope>& nShape(scopedShape);
				nShape.scope. template accessLayer<ScopeLayer>() = nShape.scope. template accessLayer<ScopeLayer>() + val;
				NextModifier::apply(nShape, outputId);
			}
		};
		template<int ScopeLayer, class Parameter, class NextModifier>
		struct DecreaseScopeAttachment
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION
				static void apply(Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
				auto val = Parameter::eval(scopedShape);
				Scope::ScopedShape<TShape, TScope>& nShape(scopedShape);
				nShape.scope. template accessLayer<ScopeLayer>() = nShape.scope. template accessLayer<ScopeLayer>() - val;
				NextModifier::apply(nShape, outputId);
			}
		};
		template<int ScopeLayer, class Parameter, class NextModifier>
		struct MulScopeAttachment
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION
				static void apply(Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
				auto val = Parameter::eval(scopedShape);
				Scope::ScopedShape<TShape, TScope>& nShape(scopedShape);
				nShape.scope. template accessLayer<ScopeLayer>() = nShape.scope. template accessLayer<ScopeLayer>() * val;
				NextModifier::apply(nShape, outputId);
			}
		};
		template<int ScopeLayer, class Parameter, class NextModifier>
		struct DivScopeAttachment
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION
				static void apply(Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
				auto val = Parameter::eval(scopedShape);
				Scope::ScopedShape<TShape, TScope>& nShape(scopedShape);
				nShape.scope. template accessLayer<ScopeLayer>() = nShape.scope. template accessLayer<ScopeLayer>() * val;
				NextModifier::apply(nShape, outputId);
			}
		};

		template<int ScopeLayer, template<int > class Param, class OffsetParam, class MulParam, class NextModifier>
		struct SetScopeAttachmentParameterUsingOutputId
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION
				static void apply(Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
				int offset = OffsetParam::eval(scopedShape);
				int mul = MulParam::eval(scopedShape);
				//auto val = Param < offset + mul*outputId >::eval(scopedShape);
				Scope::ScopedShape<TShape, TScope>& nShape(scopedShape);
				//nShape.scope. template accessLayer<ScopeLayer>() = val;
				nShape.scope. template accessLayer<ScopeLayer>() = 0;
				NextModifier::apply(nShape, outputId);
			}
		};

		template<int ScopeLayer, class NextModifier>
		struct SetScopeAttachmentToOutputId
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION
				static void apply(Scope::ScopedShape<TShape, TScope>& scopedShape, int outputId = 0)
			{
				Scope::ScopedShape<TShape, TScope>& nShape(scopedShape);
				nShape.scope. template accessLayer<ScopeLayer>() = outputId;
				NextModifier::apply(nShape, outputId);
			}
		};
	}
}
#endif //INCLUDED_MODIFIERS_SCOPE_MODIFIER
