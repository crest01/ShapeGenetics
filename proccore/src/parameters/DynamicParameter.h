#ifndef INCLUDED_PARAMETERS_DYNAMIC_PARAMETERS
#define INCLUDED_PARAMETERS_DYNAMIC_PARAMETERS

#pragma once

#include "DualBuild.h"
#include "scope/Scope.h"
#include "Axis.h"

#include <template_utils.h>
#include <math/vector.h>
#include <math/matrix.h>

namespace PGG
{
	namespace Parameters
	{

		DUAL_BUILD_FUNCTION math::float4 accessParamTable(int offset);

		template <int Offset, int ScopeLayer = 0>
		struct DynamicInt;
		template <int Offset, int ScopeLayer = 0>
		struct DynamicInt2;
		template <int Offset, int ScopeLayer = 0>
		struct DynamicInt3;
		template <int Offset, int ScopeLayer = 0>
		struct DynamicInt4;

		template <int Offset, int ScopeLayer = 0>
		struct DynamicUint;
		template <int Offset, int ScopeLayer = 0>
		struct DynamicUint2;
		template <int Offset, int ScopeLayer = 0>
		struct DynamicUint3;
		template <int Offset, int ScopeLayer = 0>
		struct DynamicUint4;

		template <int Offset, int ScopeLayer = 0>
		struct DynamicFloat;
		template <int Offset, int ScopeLayer = 0>
		struct DynamicFloat2;
		template <int Offset, int ScopeLayer = 0>
		struct DynamicFloat3;
		template <int Offset, int ScopeLayer = 0>
		struct DynamicFloat4;

		template <int Offset, int ScopeLayer = 0>
		struct DynamicFloat2x2;
		template <int Offset, int ScopeLayer = 0>
		struct DynamicFloat3x3;
		template <int Offset, int ScopeLayer = 0>
		struct DynamicFloat3x4;
		template <int Offset, int ScopeLayer = 0>
		struct DynamicFloat4x4;

		template <int Offset, int ScopeLayer = 0>
		struct DynamicAxes;
		
		namespace 
		{
			template< int Offset4, int ScopeLayer, class TShape, class TScope>
			math::float4 loadParam(const Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				int offset = scopedShape.scope. template accessLayer<ScopeLayer>() + Offset4;
				return accessParamTable(offset);
			}

			template <int Offset4, int Offset1, int ScopeLayer = 0>
			struct DynamicIntImpl;
			template <int Offset4, int ScopeLayer>
			struct DynamicIntImpl<Offset4, 0, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static int eval(const Scope::ScopedShape<TShape, TScope> & scopedShape){ return __float_as_int(loadParam<Offset4, ScopeLayer>(scopedShape).x); }
			};
			template <int Offset4, int ScopeLayer>
			struct DynamicIntImpl<Offset4, 1, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static int eval(const Scope::ScopedShape<TShape, TScope> & scopedShape){ return __float_as_int(loadParam<Offset4, ScopeLayer>(scopedShape).y); }
			};
			template <int Offset4, int ScopeLayer>
			struct DynamicIntImpl<Offset4, 2, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static int eval(const Scope::ScopedShape<TShape, TScope> & scopedShape){ return __float_as_int(loadParam<Offset4, ScopeLayer>(scopedShape).z); }
			};
			template <int Offset4, int ScopeLayer>
			struct DynamicIntImpl<Offset4, 3, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static int eval(const Scope::ScopedShape<TShape, TScope> & scopedShape){ return __float_as_int(loadParam<Offset4, ScopeLayer>(scopedShape).w); }
			};

			template <int Offset4, int Offset1, int ScopeLayer = 0>
			struct DynamicInt2Impl;
			template <int Offset4, int ScopeLayer>
			struct DynamicInt2Impl<Offset4, 0, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static math::int2 eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
				{ 
					math::float4 loaded = loadParam<Offset4, ScopeLayer>(scopedShape);
					return math::int2(__float_as_int(loaded.x), __float_as_int(loaded.y));
				}
			};
			template <int Offset4, int ScopeLayer>
			struct DynamicInt2Impl<Offset4, 2, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static math::int2 eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
				{
					math::float4 loaded = loadParam<Offset4, ScopeLayer>(scopedShape);
					return math::int2(__float_as_int(loaded.z), __float_as_int(loaded.w));
				}
			};

			template <int Offset4, int Offset1, int ScopeLayer = 0>
			struct DynamicInt3Impl;
			template <int Offset4, int ScopeLayer>
			struct DynamicInt3Impl<Offset4, 0, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static math::int3 eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
				{
					math::float4 loaded = loadParam<Offset4, ScopeLayer>(scopedShape);
					return math::int3(__float_as_int(loaded.x), __float_as_int(loaded.y), __float_as_int(loaded.z));
				}
			};

			template <int Offset4, int Offset1, int ScopeLayer = 0>
			struct DynamicInt4Impl;
			template <int Offset4, int ScopeLayer>
			struct DynamicInt4Impl<Offset4, 0, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static math::int4 eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
				{
					math::float4 loaded = loadParam<Offset4, ScopeLayer>(scopedShape);
					return math::int4(__float_as_int(loaded.x), __float_as_int(loaded.y), __float_as_int(loaded.z), __float_as_int(loaded.w));
				}
			};
		}


		template <int Offset, int ScopeLayer>
		struct DynamicInt : DynamicIntImpl<Offset / 4, Offset % 4, ScopeLayer> { };
		template <int Offset, int ScopeLayer>
		struct DynamicInt2 : DynamicInt2Impl<Offset / 4, Offset % 4, ScopeLayer> { };
		template <int Offset, int ScopeLayer>
		struct DynamicInt3 : DynamicInt3Impl<Offset / 4, Offset % 4, ScopeLayer> { };
		template <int Offset, int ScopeLayer>
		struct DynamicInt4 : DynamicInt4Impl<Offset / 4, Offset % 4, ScopeLayer> { };

		namespace
		{
			template <int Offset4, int Offset1, int ScopeLayer = 0>
			struct DynamicUintImpl;
			template <int Offset4, int ScopeLayer>
			struct DynamicUintImpl<Offset4, 0, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static unsigned int eval(const Scope::ScopedShape<TShape, TScope> & scopedShape){ return __float_as_uint(loadParam<Offset4, ScopeLayer>(scopedShape).x); }
			};
			template <int Offset4, int ScopeLayer>
			struct DynamicUintImpl<Offset4, 1, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static unsigned int eval(const Scope::ScopedShape<TShape, TScope> & scopedShape){ return __float_as_uint(loadParam<Offset4, ScopeLayer>(scopedShape).y); }
			};
			template <int Offset4, int ScopeLayer>
			struct DynamicUintImpl<Offset4, 2, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static unsigned int eval(const Scope::ScopedShape<TShape, TScope> & scopedShape){ return __float_as_uint(loadParam<Offset4, ScopeLayer>(scopedShape).z); }
			};
			template <int Offset4, int ScopeLayer>
			struct DynamicUintImpl<Offset4, 3, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static unsigned int eval(const Scope::ScopedShape<TShape, TScope> & scopedShape){ return __float_as_uint(loadParam<Offset4, ScopeLayer>(scopedShape).w); }
			};

			template <int Offset4, int Offset1, int ScopeLayer = 0>
			struct DynamicUint2Impl;
			template <int Offset4, int ScopeLayer>
			struct DynamicUint2Impl<Offset4, 0, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static math::uint2 eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
				{
					math::float4 loaded = loadParam<Offset4, ScopeLayer>(scopedShape);
					return math::uint2(__float_as_uint(loaded.x), __float_as_uint(loaded.y));
				}
			};
			template <int Offset4, int ScopeLayer>
			struct DynamicUint2Impl<Offset4, 2, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static math::uint2 eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
				{
					math::float4 loaded = loadParam<Offset4, ScopeLayer>(scopedShape);
					return math::uint2(__float_as_uint(loaded.z), __float_as_uint(loaded.w));
				}
			};

			template <int Offset4, int Offset1, int ScopeLayer>
			struct DynamicUint3Impl;
			template <int Offset4, int ScopeLayer>
			struct DynamicUint3Impl<Offset4, 0, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static math::uint3 eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
				{
					math::float4 loaded = loadParam<Offset4, ScopeLayer>(scopedShape);
					return math::uint3(__float_as_uint(loaded.x), __float_as_uint(loaded.y), __float_as_uint(loaded.z));
				}
			};

			template <int Offset4, int Offset1, int ScopeLayer>
			struct DynamicUint4Impl;
			template <int Offset4, int ScopeLayer>
			struct DynamicUint4Impl<Offset4, 0, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static math::uint4 eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
				{
					math::float4 loaded = loadParam<Offset4, ScopeLayer>(scopedShape);
					return math::uint4(__float_as_uint(loaded.x), __float_as_uint(loaded.y), __float_as_uint(loaded.z), __float_as_uint(loaded.w));
				}
			};
		}

		template <int Offset, int ScopeLayer>
		struct DynamicUint : DynamicUintImpl<Offset / 4, Offset % 4, ScopeLayer> { };
		template <int Offset, int ScopeLayer>
		struct DynamicUint2 : DynamicUint2Impl<Offset / 4, Offset % 4, ScopeLayer> { };
		template <int Offset, int ScopeLayer>
		struct DynamicUint3 : DynamicUint3Impl<Offset / 4, Offset % 4, ScopeLayer> { };
		template <int Offset, int ScopeLayer>
		struct DynamicUint4 : DynamicUint4Impl<Offset / 4, Offset % 4, ScopeLayer> { };

		namespace
		{

			template <int Offset4, int Offset1, int ScopeLayer = 0>
			struct DynamicFloatImpl;
			template <int Offset4, int ScopeLayer>
			struct DynamicFloatImpl<Offset4, 0, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static float eval(const Scope::ScopedShape<TShape, TScope> & scopedShape){ return loadParam<Offset4, ScopeLayer>(scopedShape).x; }
			};
			template <int Offset4, int ScopeLayer>
			struct DynamicFloatImpl<Offset4, 1, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static float eval(const Scope::ScopedShape<TShape, TScope> & scopedShape){ return loadParam<Offset4, ScopeLayer>(scopedShape).y; }
			};
			template <int Offset4, int ScopeLayer>
			struct DynamicFloatImpl<Offset4, 2, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static float eval(const Scope::ScopedShape<TShape, TScope> & scopedShape){ return loadParam<Offset4, ScopeLayer>(scopedShape).z; }
			};
			template <int Offset4, int ScopeLayer>
			struct DynamicFloatImpl<Offset4, 3, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static float eval(const Scope::ScopedShape<TShape, TScope> & scopedShape){ return loadParam<Offset4, ScopeLayer>(scopedShape).w; }
			};

			template <int Offset4, int Offset1, int ScopeLayer = 0>
			struct DynamicFloat2Impl;
			template <int Offset4, int ScopeLayer>
			struct DynamicFloat2Impl<Offset4, 0, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static math::float2 eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
				{
					math::float4 loaded = loadParam<Offset4, ScopeLayer>(scopedShape);
					return math::float2(loaded.x, loaded.y);
				}
			};
			template <int Offset4, int ScopeLayer>
			struct DynamicFloat2Impl<Offset4, 2, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static math::float2 eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
				{
					math::float4 loaded = loadParam<Offset4, ScopeLayer>(scopedShape);
					return math::float2(loaded.z, loaded.w);
				}
			};

			template <int Offset4, int Offset1, int ScopeLayer = 0>
			struct DynamicFloat3Impl;
			template <int Offset4, int ScopeLayer>
			struct DynamicFloat3Impl<Offset4, 0, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static math::float3 eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
				{
					math::float4 loaded = loadParam<Offset4, ScopeLayer>(scopedShape);
					return math::float3(loaded.x, loaded.y, loaded.z);
				}
			};

			template <int Offset4, int Offset1, int ScopeLayer = 0>
			struct DynamicFloat4Impl;
			template <int Offset4, int ScopeLayer>
			struct DynamicFloat4Impl<Offset4, 0, ScopeLayer>
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static math::float4 eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
				{
					math::float4 loaded = loadParam<Offset4, ScopeLayer>(scopedShape);
					return math::float4(loaded.x, loaded.y, loaded.z, loaded.w);
				}
			};
		}

		template <int Offset, int ScopeLayer>
		struct DynamicFloat : DynamicFloatImpl<Offset / 4, Offset % 4, ScopeLayer> { };
		template <int Offset, int ScopeLayer>
		struct DynamicFloat2 : DynamicFloat2Impl<Offset / 4, Offset % 4, ScopeLayer> { };
		template <int Offset, int ScopeLayer>
		struct DynamicFloat3 : DynamicFloat3Impl<Offset / 4, Offset % 4, ScopeLayer> { };
		template <int Offset, int ScopeLayer>
		struct DynamicFloat4 : DynamicFloat4Impl<Offset / 4, Offset % 4, ScopeLayer> { };


		namespace
		{

			template <int Offset4, int Offset1, int ScopeLayer = 0>
			struct DynamicFloat2x2Impl;
			template <int Offset4, int ScopeLayer>
			struct DynamicFloat2x2Impl < Offset4, 0, ScopeLayer >
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static math::float2x2 eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
				{ 
					math::float4 loaded = loadParam<Offset4, ScopeLayer>(scopedShape);
					return  math::float2x2(loaded.x, loaded.y,
						loaded.y, loaded.w);
				}
			};

			template <int Offset4, int Offset1, int ScopeLayer = 0>
			struct DynamicFloat3x3Impl;
			template <int Offset4, int ScopeLayer>
			struct DynamicFloat3x3Impl < Offset4, 0, ScopeLayer >
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static math::float3x3 eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
				{
					math::float4 loaded0 = loadParam<Offset4, ScopeLayer>(scopedShape);
					math::float4 loaded1 = loadParam<Offset4+1, ScopeLayer>(scopedShape);
					math::float4 loaded2 = loadParam<Offset4+2, ScopeLayer>(scopedShape);
					return  math::float3x3(loaded0.x, loaded0.y, loaded0.z,
						loaded0.w, loaded1.x, loaded1.y,
						loaded1.z, loaded1.w, loaded2.x);
				}
			};

			template <int Offset4, int Offset1, int ScopeLayer = 0>
			struct DynamicFloat3x4Impl;
			template <int Offset4, int ScopeLayer>
			struct DynamicFloat3x4Impl < Offset4, 0, ScopeLayer >
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static math::float3x4 eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
				{
					math::float4 loaded0 = loadParam<Offset4, ScopeLayer>(scopedShape);
					math::float4 loaded1 = loadParam<Offset4 + 1, ScopeLayer>(scopedShape);
					math::float4 loaded2 = loadParam<Offset4 + 2, ScopeLayer>(scopedShape);
					return  math::float3x4(loaded0.x, loaded0.y, loaded0.z, loaded0.w,
						loaded1.x, loaded1.y, loaded1.z, loaded1.w, 
						loaded2.x, loaded2.y, loaded2.z, loaded2.w );
				}
			};
			template <int Offset4, int Offset1, int ScopeLayer = 0>
			struct DynamicFloat4x4Impl;
			template <int Offset4, int ScopeLayer>
			struct DynamicFloat4x4Impl < Offset4, 0, ScopeLayer >
			{
				template<class TShape, class TScope>
				DUAL_BUILD_FUNCTION static math::float4x4 eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
				{
					math::float4 loaded0 = loadParam<Offset4, ScopeLayer>(scopedShape);
					math::float4 loaded1 = loadParam<Offset4 + 1, ScopeLayer>(scopedShape);
					math::float4 loaded2 = loadParam<Offset4 + 2, ScopeLayer>(scopedShape);
					math::float4 loaded3 = loadParam<Offset4 + 3, ScopeLayer>(scopedShape);

					return  math::float4x4(loaded0.x, loaded0.y, loaded0.z, loaded0.w,
						loaded1.x, loaded1.y, loaded1.z, loaded1.w,
						loaded2.x, loaded2.y, loaded2.z, loaded2.w,
						loaded3.x, loaded3.y, loaded3.z, loaded3.w);
				}
			};
		}

		template <int Offset, int ScopeLayer>
		struct DynamicFloat2x2 : DynamicFloat2x2Impl<Offset / 4, Offset % 4, ScopeLayer> { };
		template <int Offset, int ScopeLayer>
		struct DynamicFloat3x3 : DynamicFloat3x3Impl<Offset / 4, Offset % 4, ScopeLayer> { };
		template <int Offset, int ScopeLayer>
		struct DynamicFloat3x4 : DynamicFloat3x4Impl<Offset / 4, Offset % 4, ScopeLayer> { };
		template <int Offset, int ScopeLayer>
		struct DynamicFloat4x4 : DynamicFloat4x4Impl<Offset / 4, Offset % 4, ScopeLayer> { };


		template <int Offset, int ScopeLayer>
		struct DynamicAxes
		{
			template<class TShape, class TScope>
			DUAL_BUILD_FUNCTION static Axes eval(const Scope::ScopedShape<TShape, TScope> & scopedShape)
			{
				return static_cast<Axes>(DynamicIntImpl < Offset / 4, Offset % 4, ScopeLayer > ::eval(scopedShape));
			}
		};


		namespace
		{ 
			template<typename T, int Offset, int ScopeLayer>
			struct DynamicTypeOfImpl;

			template<int Offset, int ScopeLayer>
			struct DynamicTypeOfImpl < int, Offset, ScopeLayer> { typedef DynamicInt<Offset, ScopeLayer> Type; };
			template<int Offset, int ScopeLayer>
			struct DynamicTypeOfImpl < math::int2, Offset, ScopeLayer> { typedef DynamicInt2<Offset, ScopeLayer> Type; };
			template<int Offset, int ScopeLayer>
			struct DynamicTypeOfImpl < math::int3, Offset, ScopeLayer> { typedef DynamicInt3<Offset, ScopeLayer> Type; };
			template<int Offset, int ScopeLayer>
			struct DynamicTypeOfImpl < math::int4, Offset, ScopeLayer> { typedef DynamicInt4<Offset, ScopeLayer> Type; };

			template<int Offset, int ScopeLayer>
			struct DynamicTypeOfImpl < unsigned int, Offset, ScopeLayer> { typedef DynamicUint<Offset, ScopeLayer> Type; };
			template<int Offset, int ScopeLayer>
			struct DynamicTypeOfImpl < math::uint2, Offset, ScopeLayer> { typedef DynamicUint2<Offset, ScopeLayer> Type; };
			template<int Offset, int ScopeLayer>
			struct DynamicTypeOfImpl < math::uint3, Offset, ScopeLayer> { typedef DynamicUint3<Offset, ScopeLayer> Type; };
			template<int Offset, int ScopeLayer>
			struct DynamicTypeOfImpl < math::uint4, Offset, ScopeLayer> { typedef DynamicUint4<Offset, ScopeLayer> Type; };

			template<int Offset, int ScopeLayer>
			struct DynamicTypeOfImpl < float, Offset, ScopeLayer> { typedef DynamicFloat<Offset, ScopeLayer> Type; };
			template<int Offset, int ScopeLayer>
			struct DynamicTypeOfImpl < math::float2, Offset, ScopeLayer> { typedef DynamicFloat2<Offset, ScopeLayer> Type; };
			template<int Offset, int ScopeLayer>
			struct DynamicTypeOfImpl < math::float3, Offset, ScopeLayer> { typedef DynamicFloat3<Offset, ScopeLayer> Type; };
			template<int Offset, int ScopeLayer>
			struct DynamicTypeOfImpl < math::float4, Offset, ScopeLayer> { typedef DynamicFloat4<Offset, ScopeLayer> Type; };

			template<int Offset, int ScopeLayer>
			struct DynamicTypeOfImpl < math::float2x2, Offset, ScopeLayer> { typedef DynamicFloat2x2<Offset, ScopeLayer> Type; };
			template<int Offset, int ScopeLayer>
			struct DynamicTypeOfImpl < math::float3x3, Offset, ScopeLayer> { typedef DynamicFloat3x3<Offset, ScopeLayer> Type; };
			template<int Offset, int ScopeLayer>
			struct DynamicTypeOfImpl < math::float3x4, Offset, ScopeLayer> { typedef DynamicFloat3x4<Offset, ScopeLayer> Type; };
			template<int Offset, int ScopeLayer>
			struct DynamicTypeOfImpl < math::float4x4, Offset, ScopeLayer> { typedef DynamicFloat4x4<Offset, ScopeLayer> Type; };

			template<int Offset, int ScopeLayer>
			struct DynamicTypeOfImpl < Axes, Offset, ScopeLayer> { typedef DynamicAxes<Offset, ScopeLayer> Type; };
		}

		template<typename T, int Offset, int ScopeLayer>
		using DynamicTypeOf = typename DynamicTypeOfImpl<T, Offset, ScopeLayer>::Type;
	}
}


#endif //INCLUDED_PARAMETERS_STATIC_PARAMETERS
