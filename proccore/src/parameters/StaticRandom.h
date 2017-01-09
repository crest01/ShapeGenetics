#ifndef INCLUDED_PARAMETERS_STATIC_RANDOM
#define INCLUDED_PARAMETERS_STATIC_RANDOM

#pragma once

#include "DualBuild.h"
#include "scope/Scope.h"
#include "parameters/Axis.h"
#include "parameters/VectorParameters.h"

#include <template_utils.h>
#include <math/vector.h>
#include <math/matrix.h>


namespace PGG
{
	namespace Parameters
	{
		// Note: you can construct vector/matrix parameters with different ranges using the Vec
		// and VecEx template you can even combine rand params with other static params

		template <int MinVal, int MaxVal, int ScopeLayer = 0>
		struct StaticRandInt;
		template <int MinVal, int MaxVal, int ScopeLayer = 0>
		struct StaticRandInt2;
		template <int MinVal, int MaxVal, int ScopeLayer = 0>
		struct StaticRandInt3;
		template <int MinVal, int MaxVal, int ScopeLayer = 0>
		struct StaticRandInt4;

		template <int MinVal, int MaxVal, int ScopeLayer = 0>
		struct StaticRandUint;
		template <int MinVal, int MaxVal, int ScopeLayer = 0>
		struct StaticRandUint2;
		template <int MinVal, int MaxVal, int ScopeLayer = 0>
		struct StaticRandUint3;
		template <int MinVal, int MaxVal, int ScopeLayer = 0>
		struct StaticRandUint4;

		template <int MinVal, int MaxVal, int ScopeLayer = 0>
		struct StaticRandFloat;
		template <int MinVal, int MaxVal, int ScopeLayer = 0>
		struct StaticRandFloat2;
		template <int MinVal, int MaxVal, int ScopeLayer = 0>
		struct StaticRandFloat3;
		template <int MinVal, int MaxVal, int ScopeLayer = 0>
		struct StaticRandFloat4;

		template <int MinVal, int MaxVal, int ScopeLayer = 0>
		struct StaticRandFloat2x2;
		template <int MinVal, int MaxVal, int ScopeLayer = 0>
		struct StaticRandFloat3x3;
		template <int MinVal, int MaxVal, int ScopeLayer = 0>
		struct StaticRandFloat3x4;
		template <int MinVal, int MaxVal, int ScopeLayer = 0>
		struct StaticRandFloat4x4;


		template<int ScopeLayer>
		struct StaticRandomAxes;

		template <int MinVal, int MaxVal, int ScopeLayer>
		struct StaticRandInt
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static int eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				auto & rng = scopedShape.scope. template accessLayer<ScopeLayer>();
				int diff = MaxVal - MinVal;
				return MinVal + (diff & rng.generateUint());
			}
		};
		template <int MinVal, int MaxVal, int ScopeLayer>
		struct StaticRandInt2
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static math::int2 eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				auto & rng = scopedShape.scope. template accessLayer<ScopeLayer>();
				int diff = MaxVal - MinVal;
				return math::int2(MinVal + (diff & rng.generateUint()), MinVal + (diff & rng.generateUint()));
			}
		};
		template <int MinVal, int MaxVal, int ScopeLayer>
		struct StaticRandInt3
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static math::int3 eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				auto & rng = scopedShape.scope. template accessLayer<ScopeLayer>();
				int diff = MaxVal - MinVal;
				return math::int3(MinVal + (diff & rng.generateUint()), MinVal + (diff & rng.generateUint()), MinVal + (diff & rng.generateUint()));
			}
		};
		template <int MinVal, int MaxVal, int ScopeLayer>
		struct StaticRandInt4
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static math::int4 eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				auto & rng = scopedShape.scope. template accessLayer<ScopeLayer>();
				int diff = MaxVal - MinVal;
				return math::int4(MinVal + (diff & rng.generateUint()), MinVal + (diff & rng.generateUint()), MinVal + (diff & rng.generateUint()), MinVal + (diff & rng.generateUint()));
			}
		};


		template <int MinVal, int MaxVal, int ScopeLayer>
		struct StaticRandUint
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static unsigned int eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				auto & rng = scopedShape.scope. template accessLayer<ScopeLayer>();
				int diff = MaxVal - MinVal;
				return MinVal + (diff & rng.generateUint());
			}
		};
		template <int MinVal, int MaxVal, int ScopeLayer>
		struct StaticRandUint2
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static math::uint2 eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				auto & rng = scopedShape.scope. template accessLayer<ScopeLayer>();
				int diff = MaxVal - MinVal;
				return math::int2(MinVal + (diff & rng.generateUint()), MinVal + (diff & rng.generateUint()));
			}
		};
		template <int MinVal, int MaxVal, int ScopeLayer>
		struct StaticRandUint3
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static math::uint3 eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				auto & rng = scopedShape.scope. template accessLayer<ScopeLayer>();
				int diff = MaxVal - MinVal;
				return math::int3(MinVal + (diff & rng.generateUint()), MinVal + (diff & rng.generateUint()), MinVal + (diff & rng.generateUint()));
			}
		};
		template <int MinVal, int MaxVal, int ScopeLayer>
		struct StaticRandUint4
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static math::uint4 eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				auto & rng = scopedShape.scope. template accessLayer<ScopeLayer>();
				int diff = MaxVal - MinVal;
				return math::int4(MinVal + (diff & rng.generateUint()), MinVal + (diff & rng.generateUint()), MinVal + (diff & rng.generateUint()), MinVal + (diff & rng.generateUint()));
			}
		};

		template <int MinVal, int MaxVal, int ScopeLayer>
		struct StaticRandFloat
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static float eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				auto & rng = scopedShape.scope. template accessLayer<ScopeLayer>();
				float diff = __int_as_float(MaxVal) - __int_as_float(MinVal);
				return __int_as_float(MinVal) + diff * rng.generateFloat();
			}
		};
		template <int MinVal, int MaxVal, int ScopeLayer>
		struct StaticRandFloat2
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static math::float2 eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				auto & rng = scopedShape.scope. template accessLayer<ScopeLayer>();
				float diff = __int_as_float(MaxVal) - __int_as_float(MinVal);
				return __int_as_float(MinVal) +  math::float2(diff * rng.generateFloat(), diff * rng.generateFloat());
			}
		};
		template <int MinVal, int MaxVal, int ScopeLayer>
		struct StaticRandFloat3
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static math::float3 eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				auto & rng = scopedShape.scope. template accessLayer<ScopeLayer>();
				float diff = __int_as_float(MaxVal) - __int_as_float(MinVal);
				return __int_as_float(MinVal) + math::float3(diff * rng.generateFloat(), diff * rng.generateFloat(), diff * rng.generateFloat());
			}
		};
		template <int MinVal, int MaxVal, int ScopeLayer>
		struct StaticRandFloat4
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static math::float4 eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				auto & rng = scopedShape.scope. template accessLayer<ScopeLayer>();
				float diff = __int_as_float(MaxVal) - __int_as_float(MinVal);
				return __int_as_float(MinVal) + math::float4(diff * rng.generateFloat(), diff * rng.generateFloat(), diff * rng.generateFloat(), diff * rng.generateFloat());
			}
		};

		


		template <int MinVal, int MaxVal, int ScopeLayer>
		struct StaticRandFloat2x2
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static math::float2x2 eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
			{ 
				auto & rng = scopedShape.scope. template accessLayer<ScopeLayer>();
				float diff = __int_as_float(MaxVal) - __int_as_float(MinVal);
				return math::float2x2(__int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(),
				                      __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat());
			}
		};

		template <int MinVal, int MaxVal, int ScopeLayer>
		struct StaticRandFloat3x3
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static math::float3x3 eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
			{ 
				auto & rng = scopedShape.scope. template accessLayer<ScopeLayer>();
				float diff = __int_as_float(MaxVal) - __int_as_float(MinVal);
				return math::float3x3(__int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(),
				                      __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(), 
				                      __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat());
			}
		};

		template <int MinVal, int MaxVal, int ScopeLayer>
		struct StaticRandFloat3x4
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static math::float3x4 eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				auto & rng = scopedShape.scope. template accessLayer<ScopeLayer>();
				float diff = __int_as_float(MaxVal) - __int_as_float(MinVal);
				return math::float3x4(__int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(),
				                      __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(),
				                      __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat());
			}
		};

		template <int MinVal, int MaxVal, int ScopeLayer>
		struct StaticRandFloat4x4
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static math::float4x4 eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				auto & rng = scopedShape.scope. template accessLayer<ScopeLayer>();
				float diff = __int_as_float(MaxVal) - __int_as_float(MinVal);
				return math::float4x4(__int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(),
				                      __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(),
				                      __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(),
				                      __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat(), __int_as_float(MinVal) + diff * rng.generateFloat());
			}
		};

		template<int ScopeLayer>
		struct StaticRandomAxes
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static Axes eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				auto & rng = scopedShape.scope. template accessLayer<ScopeLayer>();
				return static_cast<Axes>(rng.generateUint() % 3);
			}
		};
	}
}


#endif //INCLUDED_PARAMETERS_STATIC_PARAMETERS