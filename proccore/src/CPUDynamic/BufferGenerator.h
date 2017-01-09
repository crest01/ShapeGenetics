#ifndef CPUDYNAMIC_BUFFER_GENERATOR
#define CPUDYNAMIC_BUFFER_GENERATOR

#pragma once

#include "CPU/SharedBuffer.h"
#include <math/vector.h>
#include <memory>
#include <vector>
#include "SupportedShapes.h"

namespace PGG
{
	namespace CPU
	{
		class GrammarSystem;

		struct VertexNormalTex
		{
			math::float3 pos;
			math::float3 normal;
			math::float2 texture;
		};
		struct VertexNormal
		{
			math::float3 pos;
			math::float3 normal;
		};

		extern std::vector<std::unique_ptr<SharedBuffer<VertexNormalTex>>> buffer_generator_vertices_normals_textures;
		extern std::vector<std::unique_ptr<SharedBuffer<VertexNormal>>> buffer_generator_vertices_normals;
		extern std::vector<std::unique_ptr<SharedBuffer<math::float3>>> buffer_generator_vertices;
		extern std::vector<std::unique_ptr<SharedBuffer<unsigned int>>> buffer_generator_indices_vertices_normals_textures;
		extern std::vector<std::unique_ptr<SharedBuffer<unsigned int>>> buffer_generator_indices_vertices_normals;
		extern std::vector<std::unique_ptr<SharedBuffer<unsigned int>>> buffer_generator_indices_vertices;

		extern std::vector<std::unique_ptr<SharedBuffer<Shapes::SBox>>> buffer_generator_box;
		extern std::vector<std::unique_ptr<SharedBuffer<Shapes::SQuad>>> buffer_generator_quad;
		extern std::vector<std::unique_ptr<SharedBuffer<Shapes::SPolygon>>> buffer_generator_polygon;
		extern std::vector<std::unique_ptr<SharedBuffer<Shapes::SPrism>>> buffer_generator_prism;
		extern std::vector<std::unique_ptr<SharedBuffer<Shapes::STriangle>>> buffer_generator_triangle;
		extern std::vector<std::unique_ptr<SharedBuffer<Shapes::STetrahedron>>> buffer_generator_tetrahedron;
		extern std::vector<std::unique_ptr<SharedBuffer<Shapes::SOctahedron>>> buffer_generator_octahedron;
		extern std::vector<std::unique_ptr<SharedBuffer<Shapes::SDodecahedron>>> buffer_generator_dodecahedron;
		extern std::vector<std::unique_ptr<SharedBuffer<Shapes::SIcosahedron>>> buffer_generator_icosahedron;

		template<class Shape>
		void setupDynamicGenerator(GrammarSystem& sys, bool storeTransformationMatrix = false, bool storeExplixitGeometry = true, bool storeNormals = true, bool StoreTexCoords = false, size_t NumBuffers = 1);
			
	}
}

#endif