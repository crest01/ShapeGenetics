
#include "DynamicFunctiontable.h"
#include "SupportedOpertors.h"

using namespace PGG;
using namespace Scope;
using namespace Shapes;

namespace PGG
{
	namespace CPU
	{

		template<>
		void(*DynamicCallFunction(const PGG::Shapes::SBox& scopedShape, int id))(const PGG::Shapes::SBox&)
		{
			return BoxOperators.at(static_cast<SupportedOperators>(id));
		}

		template<>
		void(*DynamicCallFunction(const PGG::Shapes::SQuad& scopedShape, int id))(const PGG::Shapes::SQuad&)
		{
			return QuadOperators.at(static_cast<SupportedOperators>(id));
		}

		template<>
		void(*DynamicCallFunction(const PGG::Shapes::SPolygon& scopedShape, int id))(const PGG::Shapes::SPolygon&)
		{
			return PolygonOperators.at(static_cast<SupportedOperators>(id));

		}


		template<>
		void(*DynamicCallFunction(const PGG::Shapes::SPrism& scopedShape, int id))(const PGG::Shapes::SPrism&)
		{
			return PrismOperators.at(static_cast<SupportedOperators>(id));
		}



		template<>
		void(*DynamicCallFunction(const PGG::Shapes::STetrahedron& scopedShape, int id))(const PGG::Shapes::STetrahedron&)
		{
			return TetrahedronOperators.at(static_cast<SupportedOperators>(id));
		}


		template<>
		void(*DynamicCallFunction(const PGG::Shapes::SOctahedron& scopedShape, int id))(const PGG::Shapes::SOctahedron&)
		{
			return OctahedronOperators.at(static_cast<SupportedOperators>(id));
		}



		template<>
		void(*DynamicCallFunction(const PGG::Shapes::SDodecahedron& scopedShape, int id))(const PGG::Shapes::SDodecahedron&)
		{
			return DodecahedronOperators.at(static_cast<SupportedOperators>(id));
		}


		template<>
		void(*DynamicCallFunction(const PGG::Shapes::SIcosahedron& scopedShape, int id))(const PGG::Shapes::SIcosahedron&)
		{
			return IcosahedronOperators.at(static_cast<SupportedOperators>(id));
		}

	}
}
