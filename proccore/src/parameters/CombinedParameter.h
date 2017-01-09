#ifndef INCLUDED_PARAMETERS_COMBINED_PARAMETER
#define INCLUDED_PARAMETERS_COMBINED_PARAMETER

#pragma once

namespace PGG
{
	namespace Parameters
	{
		template<typename ... Parameters>
		struct CombinedParameter
		{
			template<class TShape, class TScope, typename F>
			DUAL_BUILD_FUNCTION static void eval(const Scope::ScopedShape<TShape, TScope> & scopedShape, F f)
			{
				f((Parameters::eval(scopedShape))...);
			}
		};
	}
}

#endif
