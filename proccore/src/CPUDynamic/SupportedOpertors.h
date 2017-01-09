#ifndef CPUDYNAMIC_SUPPORTED_OPERATORS
#define CPUDYNAMIC_SUPPORTED_OPERATORS


#include "DynamicScope.h"
#include "CPU/DynamicCall.h"

#include "SupportedShapes.h"

#include <map>
#include <unordered_map>

namespace PGG
{
	namespace CPU
	{
		enum class SupportedOperators
		{
			Discard = -1,

			Translate = 101, //float3, (int, int, int)
			ScaleUniform = 102, //float, (int, int, int)
			Scale = 103, //float3, (int, int, int)
			RotateAxis = 104, // Axes, float, (int, int, int)
			RotateMatrix = 105, // float3x3, (int, int, int)

			ChangeShapeTetrahedron = 901, //ChangeShape<STetrahedron>(int, int, int)

			Generate = 1000, //ChangeShape<STetrahedron>(int, int, int)
		};

		class EnumClassHash
		{
			std::hash<size_t> hasher;
		public:
			template <typename T>
			std::size_t operator()(T t) const
			{
				return hasher(static_cast<std::size_t>(t));
			}
		};
	}

	namespace Operators
	{
		// Scope Operators
		template<class Shape>
		void ScopeOperators_Translate(const Scope::ScopedShape<Shape, DynamicScope >& sshape);
		template<class Shape>
		void ScopeOperators_ScaleUniform(const Scope::ScopedShape<Shape, DynamicScope >& sshape);
		template<class Shape>
		void ScopeOperators_Scale3(const Scope::ScopedShape<Shape, DynamicScope >& sshape);
		template<class Shape>
		void ScopeOperators_RotateAxis(const Scope::ScopedShape<Shape, DynamicScope >& sshape);
		template<class Shape>
		void ScopeOperators_RotateMatrix(const Scope::ScopedShape<Shape, DynamicScope >& sshape);

		// Change Shape Operators
		void ChangeShape_Box_Tetrahedron(const Shapes::SBox& sbox);

		// Generators
		template<class SShape>
		void DynamicGenerate(const SShape&);
	}
	namespace CPU
	{
		// Quad Operators
		const std::unordered_map<SupportedOperators, void(*)(const Shapes::SBox&), EnumClassHash> BoxOperators = {
			{ SupportedOperators::Discard, nullptr},

			{ SupportedOperators::Translate, Operators::ScopeOperators_Translate<Shapes::Box> },
			{ SupportedOperators::ScaleUniform, Operators::ScopeOperators_ScaleUniform<Shapes::Box> },
			{ SupportedOperators::Scale, Operators::ScopeOperators_Scale3<Shapes::Box> },
			{ SupportedOperators::RotateAxis, Operators::ScopeOperators_RotateAxis<Shapes::Box> },
			{ SupportedOperators::RotateMatrix, Operators::ScopeOperators_RotateMatrix<Shapes::Box> },

			{ SupportedOperators::ChangeShapeTetrahedron, Operators::ChangeShape_Box_Tetrahedron },

			{ SupportedOperators::Generate, Operators::DynamicGenerate<Shapes::SBox> }
		};

		const std::unordered_map<SupportedOperators, void(*)(const Shapes::SQuad&), EnumClassHash> QuadOperators = {
			{ SupportedOperators::Discard, nullptr },

			{ SupportedOperators::Translate, Operators::ScopeOperators_Translate<Shapes::Quad> },
			{ SupportedOperators::ScaleUniform, Operators::ScopeOperators_ScaleUniform<Shapes::Quad> },
			{ SupportedOperators::Scale, Operators::ScopeOperators_Scale3<Shapes::Quad> },
			{ SupportedOperators::RotateAxis, Operators::ScopeOperators_RotateAxis<Shapes::Quad> },
			{ SupportedOperators::RotateMatrix, Operators::ScopeOperators_RotateMatrix<Shapes::Quad> },

			{ SupportedOperators::Generate, Operators::DynamicGenerate<Shapes::SQuad> },
		};

		// Polygon Operators
		const std::unordered_map<SupportedOperators, void(*)(const Shapes::SPolygon&), EnumClassHash> PolygonOperators = {
			{ SupportedOperators::Discard, nullptr },
		};

		// PrismOperators Operators
		const std::unordered_map<SupportedOperators, void(*)(const Shapes::SPrism&), EnumClassHash> PrismOperators = {
			{ SupportedOperators::Discard, nullptr },
		};

		// Tetrahedron Operators
		const std::unordered_map<SupportedOperators, void(*)(const Shapes::STetrahedron&), EnumClassHash> TetrahedronOperators = {
			{ SupportedOperators::Discard, nullptr },
		};

		// Octahedron Operators
		const std::unordered_map<SupportedOperators, void(*)(const Shapes::SOctahedron&), EnumClassHash> OctahedronOperators = {
			{ SupportedOperators::Discard, nullptr },
		};

		// Dodecahedron Operators
		const std::unordered_map<SupportedOperators, void(*)(const Shapes::SDodecahedron&), EnumClassHash> DodecahedronOperators = {
			{ SupportedOperators::Discard, nullptr },
		};

		// Icosahedron Operators
		const std::unordered_map<SupportedOperators, void(*)(const Shapes::SIcosahedron&), EnumClassHash> IcosahedronOperators = {
			{ SupportedOperators::Discard, nullptr },
		};

	}
}


#endif //CPUDYNAMIC_SUPPORTED_OPERATORS
