#ifndef INCLUDED_SHAPES_PLATONICS
#define INCLUDED_SHAPES_PLATONICS

#pragma once

#include <math/vector.h>

#include "DualBuild.h"
#include "ShapeTraits.h"


namespace PGG
{
	namespace Shapes
	{
		struct Tetrahedron
		{
			float size;

			DUAL_BUILD_FUNCTION Tetrahedron() = default;
			DUAL_BUILD_FUNCTION Tetrahedron(float size) 
				: size(size)
			{ }
		};
		struct Box
		{
			math::float3 size;

			DUAL_BUILD_FUNCTION Box() = default;
			DUAL_BUILD_FUNCTION Box(math::float3 size)
				: size(size)
			{ }
		};
		struct Octahedron
		{
			math::float3 size;

			DUAL_BUILD_FUNCTION Octahedron() = default;
			DUAL_BUILD_FUNCTION Octahedron(math::float3 size)
				: size(size)
			{ }
		};
		struct Dodecahedron
		{
			float size;

			DUAL_BUILD_FUNCTION Dodecahedron() = default;
			DUAL_BUILD_FUNCTION Dodecahedron(float size)
				: size(size)
			{ }
		};
		struct Icosahedron
		{
			float size;

			DUAL_BUILD_FUNCTION Icosahedron() = default;
			DUAL_BUILD_FUNCTION Icosahedron(float size)
				: size(size)
			{ }
		};
		namespace Traits {

			template<> struct Dimensions < Tetrahedron > { static const int value = 3; };
			template<> struct Dimensions < Box > { static const int value = 3; };
			template<> struct Dimensions < Octahedron > { static const int value = 3; };
			template<> struct Dimensions < Dodecahedron > { static const int value = 3; };
			template<> struct Dimensions < Icosahedron > { static const int value = 3; };
			template<> struct ConstructByVector < Tetrahedron > { static const int value = 1; };
			template<> struct ConstructByVector < Box > { static const int value = 3; };
			template<> struct ConstructByVector < Octahedron > { static const int value = 3; };
			template<> struct ConstructByVector < Dodecahedron > { static const int value = 1; };
			template<> struct ConstructByVector < Icosahedron > { static const int value = 1; };
			template<> struct HasSizeVector < Tetrahedron > { static const int value = 1; };
			template<> struct HasSizeVector < Box > { static const int value = 3; };
			template<> struct HasSizeVector < Octahedron > { static const int value = 3; };
			template<> struct HasSizeVector < Dodecahedron > { static const int value = 1; };
			template<> struct HasSizeVector < Icosahedron > { static const int value = 1; };

		}
	}
}

#endif //INCLUDED_SHAPES_PLATONICS
