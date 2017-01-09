#ifndef INCLUDED_PARAMETERS_PARAMETER_OPERATIONS
#define INCLUDED_PARAMETERS_PARAMETER_OPERATIONS

#pragma once

#include "DualBuild.h"
#include "scope/Scope.h"
#include "parameters/Axis.h"

#include <template_utils.h>

namespace PGG
{
	namespace Parameters
	{
		template<class ParamA, class ParamB>
		struct LogicalAnd
		{
		   template<class TShape, class TScope>
		   DUAL_BUILD_FUNCTION static decltype(ParamA::eval(declval<Scope::ScopedShape<TShape, TScope>>()) && ParamB::eval(declval<Scope::ScopedShape<TShape, TScope>&>()))
			eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
		   {
			   return ParamA::eval(scopedShape) && ParamB::eval(scopedShape);
		   }
		};

		template<class ParamA, class ParamB>
		struct LogicalOr
		{
		   template<class TShape, class TScope>
		   DUAL_BUILD_FUNCTION static decltype(ParamA::eval(declval<Scope::ScopedShape<TShape, TScope>>()) || ParamB::eval(declval<Scope::ScopedShape<TShape, TScope>&>()))
			eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
		   {
			   return ParamA::eval(scopedShape) || ParamB::eval(scopedShape);
		   }
		};

		template<class Param>
		  struct LogicalNot
		  {
		   template<class TShape, class TScope>
		   DUAL_BUILD_FUNCTION static decltype(Param::eval(declval<Scope::ScopedShape<TShape, TScope>&>()))
		    eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
		   {
		    return !Param::eval(scopedShape);
		   }
		  };

		template<class ParamA, class ParamB>
		struct Mul
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static decltype(ParamA::eval(declval<Scope::ScopedShape<TShape, TScope>>())*ParamB::eval(declval<Scope::ScopedShape<TShape, TScope>&>()))
				eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				return ParamA::eval(scopedShape) * ParamB::eval(scopedShape);
			}
		};
		template<class ParamA, class ParamB>
		struct Div
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static decltype(ParamA::eval(declval<Scope::ScopedShape<TShape, TScope>>())/ParamB::eval(declval<Scope::ScopedShape<TShape, TScope>&>()))
				eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				return ParamA::eval(scopedShape) / ParamB::eval(scopedShape);
			}
		};
		template<class ParamA, class ParamB>
		struct Add
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static decltype(ParamA::eval(declval<Scope::ScopedShape<TShape, TScope>>())+ParamB::eval(declval<Scope::ScopedShape<TShape, TScope>&>()))
				eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				return ParamA::eval(scopedShape) + ParamB::eval(scopedShape);
			}
		};
		template<class ParamA, class ParamB>
		struct Sub
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static decltype(ParamA::eval(declval<Scope::ScopedShape<TShape, TScope>>()) - ParamB::eval(declval<Scope::ScopedShape<TShape, TScope>&>()))
				eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				return ParamA::eval(scopedShape) - ParamB::eval(scopedShape);
			}
		};
	}
}


#endif
