#include "BufferGenerator.h"

#include "GrammarSystem.h"
#include "DynamicParameter.h"
#include "Generate.h"

namespace PGG
{
	namespace CPU
	{

		std::vector<std::unique_ptr<SharedBuffer<VertexNormalTex>>> buffer_generator_vertices_normals_textures;
		std::vector<std::unique_ptr<SharedBuffer<VertexNormal>>> buffer_generator_vertices_normals;
		std::vector<std::unique_ptr<SharedBuffer<math::float3>>> buffer_generator_vertices;
		std::vector<std::unique_ptr<SharedBuffer<unsigned int>>> buffer_generator_indices_vertices_normals_textures;
		std::vector<std::unique_ptr<SharedBuffer<unsigned int>>> buffer_generator_indices_vertices_normals;
		std::vector<std::unique_ptr<SharedBuffer<unsigned int>>> buffer_generator_indices_vertices;

		std::vector<std::unique_ptr<SharedBuffer<Shapes::SBox>>> buffer_generator_box;
		std::vector<std::unique_ptr<SharedBuffer<Shapes::SQuad>>> buffer_generator_quad;
		std::vector<std::unique_ptr<SharedBuffer<Shapes::SPolygon>>> buffer_generator_polygon;
		std::vector<std::unique_ptr<SharedBuffer<Shapes::SPrism>>> buffer_generator_prism;
		std::vector<std::unique_ptr<SharedBuffer<Shapes::STriangle>>> buffer_generator_triangle;
		std::vector<std::unique_ptr<SharedBuffer<Shapes::STetrahedron>>> buffer_generator_tetrahedron;
		std::vector<std::unique_ptr<SharedBuffer<Shapes::SOctahedron>>> buffer_generator_octahedron;
		std::vector<std::unique_ptr<SharedBuffer<Shapes::SDodecahedron>>> buffer_generator_dodecahedron;
		std::vector<std::unique_ptr<SharedBuffer<Shapes::SIcosahedron>>> buffer_generator_icosahedron;

		namespace
		{
			template<class Shape>
			std::vector<std::unique_ptr<SharedBuffer<Shape>>>& getBufferGeneratorBuffers();

			template<> std::vector<std::unique_ptr<SharedBuffer<Shapes::SBox>>>& getBufferGeneratorBuffers() { return buffer_generator_box; }
			template<> std::vector<std::unique_ptr<SharedBuffer<Shapes::SQuad>>>& getBufferGeneratorBuffers() { return buffer_generator_quad; }
			template<> std::vector<std::unique_ptr<SharedBuffer<Shapes::SPolygon>>>& getBufferGeneratorBuffers() { return buffer_generator_polygon; }
			template<> std::vector<std::unique_ptr<SharedBuffer<Shapes::SPrism>>>& getBufferGeneratorBuffers() { return buffer_generator_prism; }
			template<> std::vector<std::unique_ptr<SharedBuffer<Shapes::STriangle>>>& getBufferGeneratorBuffers() { return buffer_generator_triangle; }
			template<> std::vector<std::unique_ptr<SharedBuffer<Shapes::STetrahedron>>>& getBufferGeneratorBuffers() { return buffer_generator_tetrahedron; }
			template<> std::vector<std::unique_ptr<SharedBuffer<Shapes::SOctahedron>>>& getBufferGeneratorBuffers() { return buffer_generator_octahedron; }
			template<> std::vector<std::unique_ptr<SharedBuffer<Shapes::SDodecahedron>>>& getBufferGeneratorBuffers() { return buffer_generator_dodecahedron; }
			template<> std::vector<std::unique_ptr<SharedBuffer<Shapes::SIcosahedron>>>& getBufferGeneratorBuffers() { return buffer_generator_icosahedron; }

			struct EmptyGenerator : public Generator
			{
				void reserveVertices(int num) { }
				void reserveIndices(int num) { }
				void addVertex(int index, const math::float3& v, const math::float3& n, const math::float2& t) { }
				void addTriangle(unsigned int i0, unsigned int i1, unsigned int i2) { }
				void finishObject() { delete this; }
			};


			template<bool Normals = false, bool Texture = false>
			struct StorageBuffer;
			template<>
			struct StorageBuffer<false, false>
			{
				typedef math::float3 Type;
				static SharedBuffer<math::float3>& access(size_t bufferId) { return *buffer_generator_vertices[bufferId].get(); }
				static SharedBuffer<unsigned int>& accessIndices(size_t bufferId) { return *buffer_generator_indices_vertices[bufferId].get(); }
				static Type makeEntry(const math::float3& v, const math::float3& n, const math::float2& t)
				{
					return v;
				}
			};
			template<>
			struct StorageBuffer<true, false>
			{
				typedef VertexNormal Type;
				static SharedBuffer<VertexNormal>& access(size_t bufferId) { return *buffer_generator_vertices_normals[bufferId]; }
				static SharedBuffer<unsigned int>& accessIndices(size_t bufferId) { return *buffer_generator_indices_vertices_normals[bufferId]; }
				static Type makeEntry(const math::float3& v, const math::float3& n, const math::float2& t)
				{
					Type e;
					e.pos = v;
					e.normal = n;
					return e;
				}
			};
			template<>
			struct StorageBuffer<true, true>
			{
				typedef VertexNormalTex Type;
				static SharedBuffer<VertexNormalTex>& access(size_t bufferId) { return *buffer_generator_vertices_normals_textures[bufferId]; }
				static SharedBuffer<unsigned int>& accessIndices(size_t bufferId) { return *buffer_generator_indices_vertices_normals_textures[bufferId]; }
				static Type makeEntry(const math::float3& v, const math::float3& n, const math::float2& t)
				{
					Type e;
					e.pos = v;
					e.normal = n;
					e.texture = t;
					return e;
				}
			};

			template<bool Normals, bool Texture>
			class BufferExplicitGeometryGenerator : public Generator
			{
				const DynamicScope & scope;
				size_t bufferId;
				size_t indexOffset;
				unsigned int vertexOffset;
				typename StorageBuffer<Normals, Texture>::Type* myBufferOffset;
				unsigned int* myIndicesOffset;

			public:
				template<class Shape>
				BufferExplicitGeometryGenerator(const Scope::ScopedShape<Shape, DynamicScope>& s, size_t bufferId) : scope(s.scope), bufferId(bufferId), indexOffset(0), vertexOffset(0), myBufferOffset(nullptr), myIndicesOffset(nullptr)
				{
				}
				void reserveVertices(int num)
				{
					auto spot = StorageBuffer<Normals, Texture>::access(bufferId).reserve(num);
					vertexOffset = static_cast<unsigned>(spot.offset);
					myBufferOffset = spot;
				}
				void reserveIndices(int num)
				{
					myIndicesOffset = StorageBuffer<Normals, Texture>::accessIndices(bufferId).reserve(num);
					indexOffset = 0;
				}
				void addVertex(int index, const math::float3& v, const math::float3& n, const math::float2& t)
				{
					myBufferOffset[index] = StorageBuffer<Normals, Texture>::makeEntry(scope.applyToVertex(v), scope.applyToNormal(n), t);
				}
				void addTriangle(unsigned int i0, unsigned int i1, unsigned int i2)
				{
					myIndicesOffset[indexOffset++] = vertexOffset + i0;
					myIndicesOffset[indexOffset++] = vertexOffset + i1;
					myIndicesOffset[indexOffset++] = vertexOffset + i2;
				}
				void finishObject()
				{
					delete this;
				}
			};

			template<class Shape, bool Scope, bool Vertices, bool Normals, bool TextureCoords>
			Generator* BufferGeneratorFunction(const Shape& shape)
			{
				size_t bufferId = Parameters::DynamicUint<0, TableEntryScopeOffset>::eval(shape);
				if (Scope)
				{
					getBufferGeneratorBuffers<Shape>()[bufferId]->reserve(1)[0] = shape;
				}
				if (Vertices)
				{
					return new BufferExplicitGeometryGenerator<Normals, TextureCoords>(shape, bufferId);
				}
				else
					return new EmptyGenerator();
			}
		}



		template<class Shape>
		void setupDynamicGenerator(GrammarSystem& sys, bool storeTransformationMatrix, bool storeExplixitGeometry, bool storeNormals, bool storeTexCoords, size_t numBuffers)
		{
			while (storeTransformationMatrix && getBufferGeneratorBuffers<Shape>().size() < numBuffers)
				getBufferGeneratorBuffers<Shape>().emplace_back(new SharedBuffer<Shape>());
			while (storeExplixitGeometry && storeNormals && storeTexCoords && buffer_generator_vertices_normals_textures.size() < numBuffers)
				buffer_generator_vertices_normals_textures.emplace_back(new SharedBuffer<VertexNormalTex>()),
				buffer_generator_indices_vertices_normals_textures.emplace_back(new SharedBuffer<unsigned int>());
			while (storeExplixitGeometry && storeNormals && !storeTexCoords && buffer_generator_vertices_normals.size() < numBuffers)
				buffer_generator_vertices_normals.emplace_back(new SharedBuffer<VertexNormal>()),
				buffer_generator_indices_vertices_normals.emplace_back(new SharedBuffer<unsigned int>());
			while (storeExplixitGeometry && !storeNormals && !storeTexCoords && buffer_generator_vertices.size() < numBuffers)
				buffer_generator_vertices.emplace_back(new SharedBuffer<math::float3>()),
				buffer_generator_indices_vertices.emplace_back(new SharedBuffer<unsigned int>());

			if (storeTransformationMatrix && storeExplixitGeometry && storeNormals && storeTexCoords)
				setDynamicGenerator(sys, BufferGeneratorFunction<Shape, true, true, true, true>);
			else if (storeTransformationMatrix && storeExplixitGeometry && storeNormals && !storeTexCoords)
				setDynamicGenerator(sys, BufferGeneratorFunction<Shape, true, true, true, false>);
			else if (storeTransformationMatrix && storeExplixitGeometry && !storeNormals && !storeTexCoords)
				setDynamicGenerator(sys, BufferGeneratorFunction<Shape, true, true, false, false>);
			else if (storeTransformationMatrix && !storeExplixitGeometry && !storeNormals && !storeTexCoords)
				setDynamicGenerator(sys, BufferGeneratorFunction<Shape, true, false, false , false>);
			else if (!storeTransformationMatrix && storeExplixitGeometry && storeNormals && storeTexCoords)
				setDynamicGenerator(sys, BufferGeneratorFunction<Shape, false, true, true, true>);
			else if (!storeTransformationMatrix && storeExplixitGeometry && storeNormals && !storeTexCoords)
				setDynamicGenerator(sys, BufferGeneratorFunction<Shape, false, true, true, false>);
			else if (!storeTransformationMatrix && storeExplixitGeometry && !storeNormals && !storeTexCoords)
				setDynamicGenerator(sys, BufferGeneratorFunction<Shape, false, true, false, false>);
		}

		template void setupDynamicGenerator<Shapes::SBox>(GrammarSystem&, bool, bool, bool, bool, size_t);
		template void setupDynamicGenerator<Shapes::SQuad>(GrammarSystem&, bool, bool, bool, bool, size_t);
		//template void setupDynamicGenerator<Shapes::SPolygon>(GrammarSystem&, bool, bool, bool, bool, size_t);
		//template void setupDynamicGenerator<Shapes::SPrism>(GrammarSystem&, bool, bool, bool, bool, size_t);
		//template void setupDynamicGenerator<Shapes::STriangle>(GrammarSystem&, bool, bool, bool, bool, size_t);
		//template void setupDynamicGenerator<Shapes::STetrahedron>(GrammarSystem&, bool, bool, bool, bool, size_t);
		//template void setupDynamicGenerator<Shapes::SOctahedron>(GrammarSystem&, bool, bool, bool, bool, size_t);
		//template void setupDynamicGenerator<Shapes::SDodecahedron>(GrammarSystem&, bool, bool, bool, bool, size_t);
		//template void setupDynamicGenerator<Shapes::SIcosahedron>(GrammarSystem&, bool, bool, bool, bool, size_t);
	}
}