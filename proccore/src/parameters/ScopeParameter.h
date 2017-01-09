#ifndef INCLUDED_PARAMETERS_SCOPE_PARAMETERS
#define INCLUDED_PARAMETERS_SCOPE_PARAMETERS

#pragma once

#include "DualBuild.h"
#include "scope/Scope.h"

#include <template_utils.h>

namespace PGG
{
	namespace Parameters
	{
		template <int ScopeLayer>
		struct ScopeParam
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static decltype(TScope:: template accessLayer<ScopeLayer>()) eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
			{ 
				return scopedShape.scope. template accessLayer<ScopeLayer>();
			}
		};

		template <int ScopeLayer, class Param>
		struct ScopeParamGT
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static bool eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				return scopedShape.scope. template accessLayer<ScopeLayer>() > Param::eval(scopedShape);
			}
		};
		template <int ScopeLayer, class Param>
		struct ScopeParamGET
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static bool eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				return scopedShape.scope. template accessLayer<ScopeLayer>() >= Param::eval(scopedShape);
			}
		};
		template <int ScopeLayer, class Param>
		struct ScopeParamLT
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static bool eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				return scopedShape.scope. template accessLayer<ScopeLayer>() < Param::eval(scopedShape);
			}
		};
		template <int ScopeLayer, class Param>
		struct ScopeParamLET
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static bool eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				return scopedShape.scope. template accessLayer<ScopeLayer>() <= Param::eval(scopedShape);
			}
		};
	}
}


#endif //INCLUDED_PARAMETERS_STATIC_PARAMETERS
