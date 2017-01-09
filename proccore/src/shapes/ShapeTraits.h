#ifndef INCLUDED_SHAPES_TRAITS
#define INCLUDED_SHAPES_TRAITS

#pragma once
namespace PGG
{
	namespace Shapes
	{
		namespace Traits
		{ 
			template<class TShape>
			struct Dimensions;

			template<class TShape>
			struct HasSizeVector;

			template<class TShape>
			struct ConstructByVector;

			template<class TShape>
			struct ConstructExplicit;
		}

		template<typename T>
		struct has_height
		{
			struct Fallback { int height; };
			struct Derived : T, Fallback { };

			template<typename C, C> struct ChT;

			template<typename C> static char(&f(ChT<int Fallback::*, &C::height>*))[1];
			template<typename C> static char(&f(...))[2];

			static const bool value = sizeof(f<Derived>(0)) == 2;
		};
	}
}


#endif //INCLUDED_SHAPES_TRAITS