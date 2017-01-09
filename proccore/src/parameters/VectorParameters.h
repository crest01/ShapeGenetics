#ifndef INCLUDED_PARAMETERS_VECTOR_PARAMETERS
#define INCLUDED_PARAMETERS_VECTOR_PARAMETERS

#pragma once

#include "DualBuild.h"
#include "scope/Scope.h"

#include <template_utils.h>
#include <math/vector.h>
#include <math/matrix.h>

namespace PGG
{
	namespace Parameters
	{
		namespace {
			template<class ... Ts>
			class ChooseVecT;

			template<> struct ChooseVecT<int, int> { typedef math::int2 Type; };
			template<> struct ChooseVecT<unsigned int, unsigned int> { typedef math::uint2 Type; };
			template<> struct ChooseVecT<float, float> { typedef math::float2 Type; };
			template<> struct ChooseVecT<int, int, int> { typedef math::int3 Type; };
			template<> struct ChooseVecT<unsigned int, unsigned int, unsigned int> { typedef math::uint3 Type; };
			template<> struct ChooseVecT<float, float, float> { typedef math::float3 Type; };
			template<> struct ChooseVecT<int, int, int, int> { typedef math::int4 Type; };
			template<> struct ChooseVecT<unsigned int, unsigned int, unsigned int, unsigned int> { typedef math::uint4 Type; };
			template<> struct ChooseVecT<float, float, float, float> { typedef math::float4 Type; };

			template<class T, int Count>
			class ChooseVecC;

			template<> struct ChooseVecC<int, 2> { typedef math::int2 Type; };
			template<> struct ChooseVecC<unsigned int, 2> { typedef math::uint2 Type; };
			template<> struct ChooseVecC<float, 2> { typedef math::float2 Type; };
			template<> struct ChooseVecC<int, 3> { typedef math::int3 Type; };
			template<> struct ChooseVecC<unsigned int, 3> { typedef math::uint3 Type; };
			template<> struct ChooseVecC<float, 3> { typedef math::float3 Type; };
			template<> struct ChooseVecC<int, 4> { typedef math::int4 Type; };
			template<> struct ChooseVecC<unsigned int, 4> { typedef math::uint4 Type; };
			template<> struct ChooseVecC<float, 4> { typedef math::float4 Type; };

			template<class TShape, class TScope, class ... Ts>
//			using ChooseVec = ChooseVecT<decltype(Ts::eval(declval<Scope::ScopedShape<TShape, TScope>>())) ...>;
			using ChooseVec = ChooseVecT<decltype(Ts::eval(declval<Scope::ScopedShape<TShape, TScope>&>()))...>;

			template<class ... Ts>
			class ChooseMatT;

			template<> struct ChooseMatT<float, float, 
										float, float> { typedef math::float2x2 Type; };
			template<> struct ChooseMatT<float, float, float, 
										float, float, float, 
										float, float, float> { typedef math::float3x3 Type; };
			template<> struct ChooseMatT<float, float, float, float, 
										float, float, float, float,
										float, float, float, float> { typedef math::float3x4 Type; };
			template<> struct ChooseMatT<float, float, float, float, 
										float, float, float, float,
										float, float, float, float,
										float, float, float, float> { typedef math::float4x4 Type; };

			template<class T, int R, int C>
			class ChooseMatC;

			template<> struct ChooseMatC<float, 2, 2> { typedef math::float2x2 Type; };
			template<> struct ChooseMatC<float, 3, 3> { typedef math::float3x3 Type; };
			template<> struct ChooseMatC<float, 3, 4> { typedef math::float3x4 Type; };
			template<> struct ChooseMatC<float, 4, 4> { typedef math::float4x4 Type; };

			template<class TShape, class TScope, class ... Ts>
			using ChooseMat = ChooseMatT<decltype(Ts::eval(declval<Scope::ScopedShape<TShape, TScope>&>()))...>;
		}

		template<class TVec, class ... Ts>
		struct VecEx
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static TVec eval(const Scope::ScopedShape<TShape, TScope> & scopedShape) { return TVec(Ts::eval(scopedShape)...); }
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static TVec eval(Scope::ScopedShape<TShape, TScope> & scopedShape) { return TVec(Ts::eval(scopedShape)...); }
		};

		template<class ... Ts>
		struct Vec
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static typename ChooseVec<TShape, TScope, Ts...>::Type eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				//return typename ChooseVec<TShape, TScope, Ts...>::Type( 0.0f );
				return typename ChooseVec<TShape, TScope, Ts...>::Type(Ts::eval(scopedShape)...);
			}
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static typename ChooseVec<TShape, TScope, Ts...>::Type eval(Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				//return typename ChooseVec<TShape, TScope, Ts...>::Type( 0.0f );
				return typename ChooseVec<TShape, TScope, Ts...>::Type(Ts::eval(scopedShape)...);
			}

		};

		template<class TMat, class ... Ts>
		struct MatEx
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static TMat eval(const Scope::ScopedShape<TShape, TScope> & scopedShape) { return TMat(Ts::eval(scopedShape)...); }

			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static TMat eval(Scope::ScopedShape<TShape, TScope> & scopedShape) { return TMat(Ts::eval(scopedShape)...); }
		};

		template<class ... Ts>
		struct Mat
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static typename ChooseMat<TShape, TScope, Ts...>::Type eval(const Scope::ScopedShape<TShape, TScope> & scopedShape) { return typename ChooseMat<TShape, TScope, Ts...>::Type(Ts::eval(scopedShape)...); }

			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static typename ChooseMat<TShape, TScope, Ts...>::Type eval(Scope::ScopedShape<TShape, TScope> & scopedShape) { return typename ChooseMat<TShape, TScope, Ts...>::Type(Ts::eval(scopedShape)...); }

		};
	}
}

#endif
