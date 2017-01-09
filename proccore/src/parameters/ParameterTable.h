#ifndef INCLUDED_PARAMETERS_PARAMETER_TABLE
#define INCLUDED_PARAMETERS_PARAMETER_TABLE

#include <vector>
#include <algorithm>
#include <math/vector.h>
#include "DynamicParameter.h"

namespace PGG
{
	namespace Parameters
	{
		class ParameterTable
		{
			std::vector<math::float4> paramList;

			template<class First, class ... Parameters>
			void insert(int soffset, int current_off, std::vector<size_t>& offsets, const First &  firstparam, const Parameters & ... parameters)
			{
				size_t size = sizeof(First);

				if (size <= 4)
				{
					offsets.push_back(current_off);
					float p = *reinterpret_cast<const float*>(&firstparam);
					if (current_off % 4 == 0)
						paramList.push_back(math::float4(p, 0, 0, 0));
					else if (current_off % 4 == 1)
						paramList.back().y = p;
					else if (current_off % 4 == 2)
						paramList.back().z = p;
					else
						paramList.back().w = p;
					current_off += 1;
				}
				else if (size <= 8)
				{
					current_off += current_off % 2;
					offsets.push_back(current_off);
					float p0 = *reinterpret_cast<const float*>(&firstparam);
					float p1 = reinterpret_cast<const float*>(&firstparam)[1];
					if (current_off % 4 == 0)
						paramList.push_back(math::float4(p0, p1, 0, 0));
					else
						paramList.back().z = p0, paramList.back().w = p1;
					current_off += 2;
				}
				else
				{
					int over = current_off % 4;
					if (over != 0)
						current_off += 4 - over;
					offsets.push_back(current_off);
					const float * pp = reinterpret_cast<const float*>(&firstparam);
					for (size_t remsize = size; remsize > 0; remsize -= std::min<size_t>(remsize, 16U))
					{
						math::float4 p(*pp++, 0, 0, 0);
						if (remsize > 4) p.y = *pp++;
						if (remsize > 8) p.z = *pp++;
						if (remsize > 12) p.w = *pp++;
						paramList.push_back(p);
					}
					current_off += static_cast<int>( (size + 3) / 4 );
				}
				insert(soffset, current_off, offsets, parameters...);
			}
			void insert(int soffset, int current_off, std::vector<size_t>& offsets)
			{ }

		public:
			template<class ... Parameters>
			int storeParameters(const Parameters & ... parameters)
			{
				//compute proper alignment for vectors and stuff
				std::vector<size_t> sizes = { sizeof(Parameters)... };
				std::vector<size_t> offsets;
				int soffset = static_cast<int>(paramList.size() * 4);
				insert(soffset, 0, offsets, parameters...);
				return soffset / 4;
			}
			std::vector<math::float4>& getParameters() { return paramList; }
			const std::vector<math::float4>& getParameters() const { return paramList; }
			void clearParameters() { paramList.clear(); }
		};

		namespace
		{ 
			template<int N, int Offset, int ScopeLayer, typename ... Types>
			class MakeDynamicParameterImpl;

			template<int N, int Offset, int ScopeLayer, typename FirstType, typename ... Types>
			class MakeDynamicParameterImpl<N, Offset, ScopeLayer, FirstType, Types...>
			{
				static const int Nextoffset = Offset + (
					sizeof(FirstType) <= 4 ? 1 :
					sizeof(FirstType) <= 8 ? ((Offset % 2) + 2) :
					(Offset % 4 == 0) ? ((sizeof(FirstType) + 3) / 4) :
					(Offset % 4 == 1) ? (((sizeof(FirstType) + 3) / 4) + 3) :
					(Offset % 4 == 2) ? (((sizeof(FirstType) + 3) / 4) + 2) :
					(((sizeof(FirstType) + 3) / 4) + 1)
					);
			public:
				typedef typename MakeDynamicParameterImpl<N - 1, Nextoffset, ScopeLayer, Types...>::Type Type;
			};

			template<int Offset, int ScopeLayer, typename FirstType, typename ... Types>
			class MakeDynamicParameterImpl<0, Offset, ScopeLayer, FirstType, Types...>
			{
				static const int ThisOffset = Offset + (
					sizeof(FirstType) <= 4 ? 0 :
					sizeof(FirstType) <= 8 ? (Offset % 2) :
					Offset % 4 == 0 ? 0 :
					Offset % 4 == 1 ? 3 :
					Offset % 4 == 2 ? 2 : 1
					);
			public:
				typedef DynamicTypeOf<FirstType, ThisOffset, ScopeLayer> Type;
			};
		}

		template<int N, int ScopeLayer, typename ... Types>
		using MakeDynamicParameter = typename MakeDynamicParameterImpl<N, 0, ScopeLayer, Types...>::Type;

		template<int ScopeLayer, typename ... Types>
		struct ParameterList
		{
			template<int N>
			class P : public MakeDynamicParameterImpl<N, 0, ScopeLayer, Types...>::Type {};
		};
		
	}
}

#endif
