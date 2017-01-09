#ifndef INCLUDED_PARAMETERS_STATIC_PARAMETERS
#define INCLUDED_PARAMETERS_STATIC_PARAMETERS

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
		template <int Value>
		struct StaticInt
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static int eval(const Scope::ScopedShape<TShape, TScope> & scopedShape){ return Value; }
		};

		template <unsigned int Value>
		struct StaticUint
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static unsigned int eval(const Scope::ScopedShape<TShape, TScope> & scopedShape) { return Value; }
		};

		template <int Value>
		struct StaticFloat
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static float eval(const Scope::ScopedShape<TShape, TScope> & scopedShape) { return __int_as_float(Value); }
		};

		template<class StaticParameter>
		struct MakeRel
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static
				decltype(StaticParameter::eval(declval<Scope::ScopedShape<TShape, TScope>&>())) eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				return -StaticParameter::eval(scopedShape);
			}

			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static
				decltype(StaticParameter::eval(declval<Scope::ScopedShape<TShape, TScope>&>())) eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				return -StaticParameter::eval(scopedShape);
			}
		};

		template<Axes TAxis>
		struct StaticAxes
		{
			static const Axes Axis = TAxis;
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static Axes eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				return Axis;
			}
		};

		template <class StaticResult, int ... Vs>
		struct StaticVec; 
		
		template <int ... Vs>
		struct StaticVec<int, Vs...> : public VecEx < typename ChooseVecC<int, sizeof...(Vs)>::Type, StaticInt<Vs>...> { };
		template <int ... Vs>
		struct StaticVec<unsigned int, Vs...> : public VecEx < typename ChooseVecC<unsigned int, sizeof...(Vs)>::Type, StaticUint<Vs>...> { };
		template <int ... Vs>
		struct StaticVec<float, Vs...> : public VecEx < typename ChooseVecC<float, sizeof...(Vs)>::Type, StaticFloat<Vs>...> { };


		template <int V0, int V1> struct StaticInt2 : public StaticVec < int, V0, V1 > {};
		template <int V0, int V1, int V2> struct StaticInt3 : public StaticVec < int, V0, V1, V2 > {};
		template <int V0, int V1, int V2, int V3> struct StaticInt4 : public StaticVec < int, V0, V1, V2, V3 > {};

		template <int V0, int V1> struct StaticUint2 : public StaticVec < unsigned int, V0, V1 > {};
		template <int V0, int V1, int V2> struct StaticUint3 : public StaticVec < unsigned int, V0, V1, V2 > {};
		template <int V0, int V1, int V2, unsigned V3> struct StaticUint4 : public StaticVec < unsigned int, V0, V1, V2, V3 > {};

		template <int V0, int V1> struct StaticFloat2 : public StaticVec < float, V0, V1 > {};
		template <int V0, int V1, int V2> struct StaticFloat3 : public StaticVec < float, V0, V1, V2 > {};
		template <int V0, int V1, int V2, int V3> struct StaticFloat4 : public StaticVec < float, V0, V1, V2, V3 > {};


		template <int V00, int V01, 
		          int V10, int V11> 
		using StaticFloat2x2 = MatEx < math::float2x2, StaticFloat<V00>, StaticFloat<V01>, StaticFloat<V10>, StaticFloat<V11> >;
		template <int V00, int V01, int V02,
		          int V10, int V11, int V12,
		          int V20, int V21, int V22>
		using StaticFloat3x3 = MatEx < math::float3x3, StaticFloat<V00>, StaticFloat<V01>, StaticFloat<V02>, StaticFloat<V10>, StaticFloat<V11>, StaticFloat<V12>, StaticFloat<V20>, StaticFloat<V21>, StaticFloat<V22> >;


		template <int V00, int V01, int V02, int V03,
		          int V10, int V11, int V12, int V13,
		          int V20, int V21, int V22, int V23>
		using StaticFloat3x4 = MatEx < math::float3x3, StaticFloat<V00>, StaticFloat<V01>, StaticFloat<V02>, StaticFloat<V03>, StaticFloat<V10>, StaticFloat<V11>, StaticFloat<V12>, StaticFloat<V13>, StaticFloat<V20>, StaticFloat<V21>, StaticFloat<V22>, StaticFloat<V23> >;
		
		template <int V00, int V01, int V02, int V03,
		          int V10, int V11, int V12, int V13,
		          int V20, int V21, int V22, int V23,
		          int V30, int V31, int V32, int V33>
		using StaticFloat4x4 = MatEx < math::float4x4, StaticFloat<V00>, StaticFloat<V01>, StaticFloat<V02>, StaticFloat<V03>, StaticFloat<V10>, StaticFloat<V11>, StaticFloat<V12>, StaticFloat<V13>, StaticFloat<V20>, StaticFloat<V21>, StaticFloat<V22>, StaticFloat<V23>, StaticFloat<V30>, StaticFloat<V31>, StaticFloat<V32>, StaticFloat<V33> >;
		

	}
}


#endif //INCLUDED_PARAMETERS_STATIC_PARAMETERS
