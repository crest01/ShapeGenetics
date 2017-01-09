#ifndef INCLUDED_PARAMETERS_RANDOM
#define INCLUDED_PARAMETERS_RANDOM

#pragma once

#include "DualBuild.h"
#include <template_utils.h>
#include <cstdint>

namespace PGG
{
	namespace Parameters
	{
		namespace
		{
			template<int Shift>
			struct ShiftRight
			{
				template<class T>
				DUAL_BUILD_FUNCTION
				static typename enable_if< Shift>= 0, T>::type shift(T& t)
				{
					return t >> Shift;
				}
				template<class T>
				DUAL_BUILD_FUNCTION
				static typename enable_if< Shift < 0, T>::type shift(T& t)
				{
					return t << -Shift;
				}
			};

			template<int Shift>
			struct ShiftLeft
			{
				template<class T>
				DUAL_BUILD_FUNCTION
				static typename enable_if< Shift >= 0, T>::type shift(T& t)
				{
					return t << Shift;
				}
				template<class T>
				DUAL_BUILD_FUNCTION
				static typename enable_if < Shift < 0, T>::type shift(T& t)
				{
					return t >> -Shift;
				}
			};

			template<unsigned Bits>
			struct XorShiftStarConcepts;

			template<>
			struct XorShiftStarConcepts<32>
			{
				typedef uint32_t Type;
				static const uint32_t M = 1419247029U;
			};

			template<>
			struct XorShiftStarConcepts<64>
			{
				typedef uint64_t Type;
				static const uint64_t M = 2685821657736338717ULL;
			};
		}


		template<unsigned Bits, int ShiftAPercent = 19, int ShiftBPercent = -40, int ShiftCPercent = 43, unsigned ShiftRangePercentLow = 10, unsigned ShiftRangePercentHigh = 50>
		class XorShiftStar
		{
			static const int ShiftA = static_cast<int>(Bits) * ShiftAPercent / 100;
			static const int ShiftB = static_cast<int>(Bits) * ShiftBPercent / 100;
			static const int ShiftC = static_cast<int>(Bits) * ShiftCPercent / 100;
			static const unsigned ShiftRangeLow = Bits * ShiftRangePercentLow / 100;
			static const unsigned ShiftRangeHigh = Bits * ShiftRangePercentHigh / 100;
			static const unsigned ShiftRangeDiff = ShiftRangeHigh - ShiftRangeLow;

			typename XorShiftStarConcepts<Bits>::Type state;

			DUAL_BUILD_FUNCTION void moveStateInternal()
			{
				state ^= ShiftRight<ShiftA>::shift(state);
				state ^= ShiftRight<ShiftB>::shift(state);
				state ^= ShiftRight<ShiftC>::shift(state);
				state = state * XorShiftStarConcepts<Bits>::M;
			}

			template<int UBITS>
			DUAL_BUILD_FUNCTION typename enable_if<UBITS == 32, uint32_t>::type generateUintInternal()
			{
				moveStateInternal();
				return state;
			}

			template<int UBITS>
			DUAL_BUILD_FUNCTION typename enable_if<(UBITS > 32), uint32_t>::type generateUintInternal()
			{
				moveStateInternal();
				return state & 0xFFFFFFFF;
			}

			template<int UBITS>
			DUAL_BUILD_FUNCTION typename enable_if<UBITS == 32, uint64_t>::type generateUint64Internal()
			{
				moveStateInternal();
				uint64_t res = static_cast<uint64_t>(state) << 32U;
				moveStateInternal();
				return res | state;
			}

			template<int UBITS>
			DUAL_BUILD_FUNCTION typename enable_if<UBITS == 64, uint64_t>::type generateUint64Internal()
			{
				moveStateInternal();
				return state;
			}

			template<int UBITS>
			DUAL_BUILD_FUNCTION typename enable_if<(UBITS > 64), uint64_t>::type generateUint64Internal()
			{
				moveStateInternal();
				return state & 0xFFFFFFFFFFFFFFFFULL;
			}

		public:
			DUAL_BUILD_FUNCTION XorShiftStar(typename XorShiftStarConcepts<Bits>::Type state = XorShiftStarConcepts<Bits>::M) : state(state) { }

			DUAL_BUILD_FUNCTION void moveState()
			{
				moveStateInternal();
			}

			DUAL_BUILD_FUNCTION uint32_t generateUint()
			{
				return generateUintInternal<Bits>();
			}

			DUAL_BUILD_FUNCTION uint64_t generateUint64()
			{
				return generateUint64Internal<Bits>();
			}

			// generate a float (0.0 <= x < 1.0) using multiplication
			DUAL_BUILD_FUNCTION float generateFloat()
			{
				return (generateUint() >> 8) * (1.0f / 16777216.0f);
			}

			// generate a float (0.0 <= x < 1.0) using bit magic
			DUAL_BUILD_FUNCTION float generateFloat01()
			{
				return __uint_as_float((generateUint() >> 9) | 0x3f800000U) - 1.0f;
			}

			// change internal state based on the current state
			DUAL_BUILD_FUNCTION void reinitialize()
			{
				unsigned int v = generateUint();
				unsigned int  s0 = ShiftRangeLow + (v & 0xFF) * ShiftRangeDiff / 255;
				bool leftShift0 = (v & 0x100) != 0;
				unsigned int  s1 = ShiftRangeLow + ((v >> 9) & 0xFF) * ShiftRangeDiff / 255;
				unsigned int  s2 = ShiftRangeLow + ((v >> 17) & 0xFF) * ShiftRangeDiff / 255;
				bool leftShift2 = (v & 0x2000000) != 0;
				
				if(leftShift0)
				{
					state ^= state << s0;
					state ^= state >> s1;
				}
				else
				{ 
					state ^= state >> s0;
					state ^= state << s1;
				}
				state ^= leftShift2 ? state << s2 : state >> s2;
				state = state * 2685821657736338717ULL;
			}
		};
	}
}

#endif