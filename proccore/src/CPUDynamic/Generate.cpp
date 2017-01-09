#include "DynamicScope.h"

#include "ScopeOperators.impl"
#include "SupportedShapes.h"

#include "ParameterTable.h"

#include "DynamicCall.h"
#include "ScopeModifier.h"
#include "DynamicParameter.h"

#include "Generate.h"
#include "GrammarSystem.h"

#include "Generator.impl"

namespace PGG
{
	namespace CPU
	{
		namespace
		{
			Generator* emptyGenerateBox(const Shapes::SBox& ) { return nullptr; }
			Generator* emptyGenerateQuad(const Shapes::SQuad&) { return nullptr; }
			Generator* emptyGeneratePolygon(const Shapes::SPolygon& ) { return nullptr; }
			Generator* emptyGeneratePrism(const Shapes::SPrism& ) { return nullptr; }
			Generator* emptyGenerateTriangle(const Shapes::STriangle& ) { return nullptr; }
			Generator* emptyGenerateTetrahedron(const Shapes::STetrahedron& ) { return nullptr; }


			extern thread_local GenerateBox boxGenerator = emptyGenerateBox;
			extern thread_local GenerateQuad quadGenerator = emptyGenerateQuad;
			extern thread_local GeneratePolygon polygonGenerator = emptyGeneratePolygon;
			extern thread_local GeneratePrism prismGenerator = emptyGeneratePrism;
			extern thread_local GenerateTriangle triangleGenerator = emptyGenerateTriangle;
			extern thread_local GenerateTetrahedron tetrahedronGenerator = emptyGenerateTetrahedron;
		}

		template<>
		void setDynamicGenerator(GrammarSystem& sys, Generator*(*generateFunc)(const Shapes::SBox&))
		{
			sys.addContextInitializer([&generateFunc]() { boxGenerator = generateFunc; });
		}
		template<>
		void setDynamicGenerator(GrammarSystem& sys, Generator*(*generateFunc)(const Shapes::SQuad&))
		{
			sys.addContextInitializer([generateFunc]() { quadGenerator = generateFunc; });
		}
		template<>
		void setDynamicGenerator(GrammarSystem& sys, Generator*(*generateFunc)(const Shapes::SPolygon&))
		{
			sys.addContextInitializer([generateFunc]() { polygonGenerator = generateFunc; });
		}
		template<>
		void setDynamicGenerator(GrammarSystem& sys, Generator*(*generateFunc)(const Shapes::SPrism&))
		{
			sys.addContextInitializer([generateFunc]() { prismGenerator = generateFunc; });
		}
		template<>
		void setDynamicGenerator(GrammarSystem& sys, Generator*(*generateFunc)(const Shapes::STriangle&))
		{
			sys.addContextInitializer([generateFunc]() { triangleGenerator = generateFunc; });
		}
		template<>
		void setDynamicGenerator(GrammarSystem& sys, Generator*(*generateFunc)(const Shapes::STetrahedron&))
		{
			sys.addContextInitializer([generateFunc]() { tetrahedronGenerator = generateFunc; });
		}

		namespace
		{
			struct DynamicGenerator
			{
				template<class SShape>
				static Generator* newShape(const SShape& sshape);

				static void allocVertices(Generator* g, int n) { g->reserveVertices(n); }
				static void allocIndices(Generator* g, int n) { g->reserveIndices(n); }
				static void addVertex(Generator* g, int i, const math::float3& pos, const math::float3& normal, const math::float2& tex) { g->addVertex(i, pos, normal, tex); }
				static void addTriangle(Generator* g, unsigned int i0, unsigned int i1, unsigned int i2) { g->addTriangle(i0, i1, i2); }
				static void finalize(Generator* g) { g->finishObject(); }
			};

			template<> Generator* DynamicGenerator::newShape<Shapes::SBox>(const Shapes::SBox& sshape) { return boxGenerator(sshape); }
			template<> Generator* DynamicGenerator::newShape<Shapes::SQuad>(const Shapes::SQuad& sshape) { return quadGenerator(sshape); }
			template<> Generator* DynamicGenerator::newShape<Shapes::SPolygon>(const Shapes::SPolygon& sshape) { return polygonGenerator(sshape); }
			template<> Generator* DynamicGenerator::newShape<Shapes::SPrism>(const Shapes::SPrism& sshape) { return prismGenerator(sshape); }
			template<> Generator* DynamicGenerator::newShape<Shapes::STriangle>(const Shapes::STriangle& sshape) { return triangleGenerator(sshape); }
			template<> Generator* DynamicGenerator::newShape<Shapes::STetrahedron>(const Shapes::STetrahedron& sshape) { return tetrahedronGenerator(sshape); }
		}

	}
	namespace Operators
	{
		template<class SShape>
		void DynamicGenerate(const SShape& sshape)
		{
			Operators::Generator<CPU::DynamicGenerator>::apply(sshape);
		}
		template void DynamicGenerate(const Shapes::SBox&);
		template void DynamicGenerate(const Shapes::SQuad&);
		//TODO implement and activate
		//template void DynamicGenerate(const Shapes::SPolygon&);
		//template void DynamicGenerate(const Shapes::SPrism&);
		//template void DynamicGenerate(const Shapes::STriangle&);
		//template void DynamicGenerate(const Shapes::STetrahedron&);
	}

}
