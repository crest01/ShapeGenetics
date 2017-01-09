#ifndef INCLUDED_OPERATORS_RANDOM_PATH
#define INCLUDED_OPERATORS_RANDOM_PATH

#pragma once

#include "DualBuild.h"
#include "scope/Scope.h"

namespace PGG
{
	namespace Operators
	{
		template<int RandomScopeLayer, class ... Likelihood_Next_Pairs>
		struct RandomPath
		{
			template <class InShape, class TScope>
			DUAL_BUILD_FUNCTION static void apply(const Scope::ScopedShape<InShape, TScope> & scopedShape);
		};

		template<int RandomScopeLayer, class LikelihoodSum, class ... Likelihood_Next_Pairs>
		struct RandomPathPrecomputed
		{
			template <class InShape, class TScope>
			DUAL_BUILD_FUNCTION static void apply(const Scope::ScopedShape<InShape, TScope> & scopedShape);
		};

		template<class ... Active_Next_Pairs>
		struct ChoosePath
		{
			template <class InShape, class TScope>
			DUAL_BUILD_FUNCTION static void apply(const Scope::ScopedShape<InShape, TScope> & scopedShape);
		};
	}
}

#endif //INCLUDED_OPERATORS_GENERATOR