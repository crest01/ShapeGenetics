#ifndef INCLUDED_PARAMETERS_AXIS
#define INCLUDED_PARAMETERS_AXIS

#pragma once

#include <template_utils.h>
#include <math/vector.h>

namespace PGG
{
	namespace Parameters
	{
		enum class Axes
		{
			XAxis = 0,
			YAxis = 1,
			ZAxis = 2
		};

		template< Axes Axis, class VType>
		struct AccessVecClass;

		template<>
		struct AccessVecClass<Axes::XAxis, float>
		{
			DUAL_BUILD_FUNCTION
			static float access(const float& val)
			{
				return val;
			}

			DUAL_BUILD_FUNCTION
				static float& access(float& val)
			{
					return val;
			}
		};

		template<class VType>
		struct AccessVecClass<Axes::XAxis, VType>
		{
			DUAL_BUILD_FUNCTION
			static decltype(VType::x) access(const VType& vec)
			{
				return vec.x;
			}

			DUAL_BUILD_FUNCTION
			static decltype(VType::x)& access(VType& vec)
			{
				return vec.x;
			}
		};
		template<class VType>
		struct AccessVecClass<Axes::YAxis, VType>
		{
			DUAL_BUILD_FUNCTION
			static decltype(VType::y) access(const VType& vec)
			{
				return vec.y;
			}

			DUAL_BUILD_FUNCTION
			static decltype(VType::y)& access(VType& vec)
			{
				return vec.y;
			}
		};
		template<class VType>
		struct AccessVecClass<Axes::ZAxis, VType>
		{
			DUAL_BUILD_FUNCTION
			static decltype(VType::z) access(const VType& vec)
			{
				return vec.z;
			}

			DUAL_BUILD_FUNCTION
			static decltype(VType::z)& access(VType& vec)
			{
				return vec.z;
			}
		};

		template<Axes Axis, class VType>
		DUAL_BUILD_FUNCTION
		inline decltype(AccessVecClass<Axis, VType>::access(declval<VType>())) accessVec(const VType& vec)
		{
			return AccessVecClass<Axis, VType>::access(vec);
		}


		template<Axes Axis, class VType>
		DUAL_BUILD_FUNCTION
		inline decltype(AccessVecClass<Axis, VType>::access(declval<VType>()))& accessVec(VType& vec)
		{
			return AccessVecClass<Axis, VType>::access(vec);
		}

		DUAL_BUILD_FUNCTION
		inline float dynAccessVec(Axes axis, const float& val)
		{
			return axis == Axes::XAxis ? val : 0.0f;
		}

		DUAL_BUILD_FUNCTION
		inline float& dynAccessVec(Axes axis, float& val)
		{
			return val;
		}

		DUAL_BUILD_FUNCTION
		inline float dynAccessVec(Axes axis, const math::float2& vec)
		{
			return axis == Axes::XAxis ? vec.x :
				axis == Axes::YAxis ? vec.y : 0.0f;
		}

		DUAL_BUILD_FUNCTION
		inline float& dynAccessVec(Axes axis, math::float2& vec)
		{
			return axis == Axes::XAxis ? vec.x : vec.y;
		}

		DUAL_BUILD_FUNCTION
		inline float dynAccessVec(Axes axis, const math::float3& vec)
		{
			return axis == Axes::XAxis ? vec.x :
				axis == Axes::YAxis ? vec.y : vec.z;
		}

		DUAL_BUILD_FUNCTION
		inline float& dynAccessVec(Axes axis, math::float3& vec)
		{
			return axis == Axes::XAxis ? vec.x :
				axis == Axes::YAxis ? vec.y : vec.z;
		}


		template<Axes Axis, class VType>
		struct SubAccess;

		template<typename T>
		struct SubAccess<Axes::XAxis, math::vector<T, 3>>
		{
			typedef math::vector<T, 3> Vec;
			typedef math::vector<T, 2> ReducedVec;

			DUAL_BUILD_FUNCTION
			static ReducedVec extract(const Vec& vec)
			{
				return ReducedVec(vec.y, vec.z);
			}

			DUAL_BUILD_FUNCTION
			static Vec expand(const ReducedVec& redvec)
			{
				return Vec(T(0), redvec.x, redvec.y);
			}

			DUAL_BUILD_FUNCTION
			static Vec mask()
			{
				return Vec(T(1), T(0), T(0));
			}

			DUAL_BUILD_FUNCTION
			static Vec othermask()
			{
				return Vec(T(0), T(1), T(1));
			}
		};

		template<typename T>
		struct SubAccess<Axes::YAxis, math::vector<T, 3>>
		{
			typedef math::vector<T, 3> Vec;
			typedef math::vector<T, 2> ReducedVec;

			DUAL_BUILD_FUNCTION
			static ReducedVec extract(const Vec& vec)
			{
				return ReducedVec(vec.x, vec.z);
			}

			DUAL_BUILD_FUNCTION
			static Vec expand(const ReducedVec& redvec)
			{
				return Vec(redvec.x, T(0), redvec.y);
			}

			DUAL_BUILD_FUNCTION
			static Vec mask()
			{
				return Vec(T(0), T(1), T(0));
			}

			DUAL_BUILD_FUNCTION
			static Vec othermask()
			{
				return Vec(T(1), T(0), T(1));
			}
		};

		template<typename T>
		struct SubAccess<Axes::ZAxis, math::vector<T, 3>>
		{
			typedef math::vector<T, 3> Vec;
			typedef math::vector<T, 2> ReducedVec;

			DUAL_BUILD_FUNCTION
			static ReducedVec extract(const Vec& vec)
			{
				return ReducedVec(vec.x, vec.y);
			}

			DUAL_BUILD_FUNCTION
			static Vec expand(const ReducedVec& redvec)
			{
				return Vec(redvec.x, redvec.y, T(0));
			}

			DUAL_BUILD_FUNCTION
			static Vec mask()
			{
				return Vec(T(0), T(0), T(1));
			}

			DUAL_BUILD_FUNCTION
			static Vec othermask()
			{
				return Vec(T(1), T(1), T(0));
			}
		};

		template<typename T>
		struct SubAccess<Axes::XAxis, math::vector<T, 2>>
		{
			typedef math::vector<T, 2> Vec;
			typedef T ReducedVec;

			DUAL_BUILD_FUNCTION
				static ReducedVec extract(const Vec& vec)
			{
					return vec.y;
				}

			DUAL_BUILD_FUNCTION
				static Vec expand(const ReducedVec& redvec)
			{
					return Vec(T(0), redvec);
				}

			DUAL_BUILD_FUNCTION
				static Vec mask()
			{
					return Vec(T(1), T(0));
				}

			DUAL_BUILD_FUNCTION
				static Vec othermask()
			{
					return Vec(T(0), T(1));
				}
		};

		template<typename T>
		struct SubAccess<Axes::YAxis, math::vector<T, 2>>
		{
			typedef math::vector<T, 2> Vec;
			typedef T ReducedVec;

			DUAL_BUILD_FUNCTION
			static ReducedVec extract(const Vec& vec)
			{
				return vec.x;
			}

			DUAL_BUILD_FUNCTION
			static Vec expand(const ReducedVec& redvec)
			{
				return Vec(T(0));
			}

			DUAL_BUILD_FUNCTION
			static Vec mask()
			{
				return Vec(T(0), T(1));
			}

			DUAL_BUILD_FUNCTION
			static Vec othermask()
			{
				return Vec(T(1), T(0));
			}
		};

		template<typename T>
		struct SubAccess<Axes::XAxis, T>
		{
			DUAL_BUILD_FUNCTION
			static T mask()
			{
				return T(1);
			}
			DUAL_BUILD_FUNCTION
				static T othermask()
			{
				return T(0);
			}
		};

		template<class V>
		struct DynSubAccess;

		template<typename T>
		struct DynSubAccess<math::vector<T, 3>>
		{
			typedef math::vector<T, 3> Vec;
			typedef math::vector<T, 2> ReducedVec;

			DUAL_BUILD_FUNCTION
			static ReducedVec extract(Axes axis, const Vec& vec)
			{
				//TODO: optimize for GPU with bit magic
				return axis == Axes::XAxis ? ReducedVec(vec.y, vec.z) :
					axis == Axes::YAxis ? ReducedVec(vec.x, vec.z) :
					ReducedVec(vec.x, vec.y);
			}

			DUAL_BUILD_FUNCTION
			static Vec expand(Axes axis, const ReducedVec& redvec)
			{
				//TODO: optimize for GPU with bit magic
				return axis == Axes::XAxis ? Vec(T(0), redvec.x, redvec.y) :
					axis == Axes::YAxis ? Vec(redvec.x, T(0), redvec.y) :
					Vec(redvec.x, redvec.y, T(0));
			}

			DUAL_BUILD_FUNCTION
			static Vec mask(Axes axis)
			{
				//TODO: optimize for GPU with bit magic
				return axis == Axes::XAxis ? Vec(T(1), T(0), T(0)) :
					axis == Axes::YAxis ? Vec(T(0), T(1), T(0)) :
					Vec(T(0), T(0), T(1));
			}

			DUAL_BUILD_FUNCTION
			static Vec othermask(Axes axis)
			{
				//TODO: optimize for GPU with bit magic
				return axis == Axes::XAxis ? Vec(T(0), T(1), T(1)) :
					axis == Axes::YAxis ? Vec(T(1), T(0), T(1)) :
					Vec(T(1), T(1), T(0));
			}
		};

		template<typename T>
		struct DynSubAccess<math::vector<T, 2>>
		{
			typedef math::vector<T, 2> Vec;
			typedef T ReducedVec;

			DUAL_BUILD_FUNCTION
			static ReducedVec extract(Axes axis, const Vec& vec)
			{
				//TODO: optimize for GPU with bit magic
				return axis == Axes::XAxis ? vec.y :
					axis == Axes::YAxis ? vec.x :
					T(0);
			}

			DUAL_BUILD_FUNCTION
			static Vec expand(Axes axis, const ReducedVec& redvec)
			{
				//TODO: optimize for GPU with bit magic
				return axis == Axes::XAxis ? Vec(T(0), redvec) :
					axis == Axes::YAxis ? Vec(redvec, T(0)) :
					Vec(T(0), T(0));
			}

			DUAL_BUILD_FUNCTION
			static Vec mask(Axes axis)
			{
				//TODO: optimize for GPU with bit magic
				return axis == Axes::XAxis ? Vec(T(1), T(0)) :
					axis == Axes::YAxis ? Vec(T(0), T(1)) :
					Vec(T(0), T(0));
			}

			DUAL_BUILD_FUNCTION
			static Vec othermask(Axes axis)
			{
				//TODO: optimize for GPU with bit magic
				return axis == Axes::XAxis ? Vec(T(0), T(1)) :
					axis == Axes::YAxis ? Vec(T(1), T(0)) :
					Vec(T(1), T(1));
			}
		};

		template<>
		struct DynSubAccess<float>
		{
			typedef float Vec;
			typedef float ReducedVec;

			DUAL_BUILD_FUNCTION
			static ReducedVec extract(Axes axis, const Vec& vec)
			{
				//TODO: optimize for GPU with bit magic
				return 0.0f;
			}

			DUAL_BUILD_FUNCTION
			static Vec expand(Axes axis, const ReducedVec& redvec)
			{
				//TODO: optimize for GPU with bit magic
				return 0.0f;
			}

			DUAL_BUILD_FUNCTION
			static Vec mask(Axes axis)
			{
				//TODO: optimize for GPU with bit magic
				return axis == Axes::XAxis ? 1.0f : 0.0f;
			}

			DUAL_BUILD_FUNCTION
			static Vec othermask(Axes axis)
			{
				//TODO: optimize for GPU with bit magic
				return 0.0f;
			}
		};

		/* should work but doesnt with MSVC
		template <typename T, typename = void>
		struct has_static_axis : std::false_type { };

		template <typename T>
		struct has_static_axis <T, decltype(T::Axis, void())> : std::true_type{};*/

		template<typename T> 
		struct has_static_axis 
		{
			struct Fallback { int Axis; };
			struct Derived : T, Fallback { };

			template<typename C, C> struct ChT;

			template<typename C> static char(&f(ChT<int Fallback::*, &C::Axis>*))[1];
			template<typename C> static char(&f(...))[2];

			static const bool value = sizeof(f<Derived>(0)) == 2;
		};


		template<Axes axis>
		struct toInt;

		template<> struct toInt<Axes::XAxis> { static const int value = 0; };
		template<> struct toInt<Axes::YAxis> { static const int value = 1; };
		template<> struct toInt<Axes::ZAxis> { static const int value = 3; };
	}
}

#endif
