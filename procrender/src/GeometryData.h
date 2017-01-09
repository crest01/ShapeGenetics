#ifndef INCLUDED_GEOMETRY_DATA
#define INCLUDED_GEOMETRY_DATA

#pragma once

#include <math/vector.h>
#include "CPU/SharedBuffer.h"
#include "scope/Scope.h"

struct Vertex
{
	Vertex() = default;
	Vertex(const math::float3& p, const math::float3& n, const math::float2& tex) : p(p), n(n), tex(tex) {}
	math::float3 p;
	math::float3 n;
	math::float2 tex;
};

extern PGG::CPU::SharedBuffer<Vertex> vertices;
extern PGG::CPU::SharedBuffer<unsigned> indices;


template<class Scope = PGG::Scope::CoordinateframeScope<>>
class CPUGeometryGenerator
{
	const Scope & scope;
	size_t indexOffset;
	unsigned int vertexOffset;
	Vertex* myVertexOffset;
	unsigned int* myIndicesOffset;

public:
	template<class Shape>
	CPUGeometryGenerator(const PGG::Scope::ScopedShape<Shape, Scope>& s) : scope(s.scope), indexOffset(0), myVertexOffset(nullptr), myIndicesOffset(nullptr)
	{
	}
	template<class Shape>
	static CPUGeometryGenerator newShape(const PGG::Scope::ScopedShape<Shape, Scope>& s)
	{
		return CPUGeometryGenerator(s);
	}
	static void allocVertices(CPUGeometryGenerator& g, int num)
	{
		auto res = vertices.reserve(num);
		g.vertexOffset = static_cast<unsigned int>(res.offset);
		g.myVertexOffset = res;
	}
	static void allocIndices(CPUGeometryGenerator& g, int num)
	{
		g.myIndicesOffset = indices.reserve(num);
		g.indexOffset = 0;
	}
	static void addVertex(CPUGeometryGenerator& g, int index, const math::float3& v, const math::float3& n, const math::float2& t)
	{
		g.myVertexOffset[index] = Vertex(g.scope.applyToVertex(v), g.scope.applyToNormal(n), t);
	}
	static void addTriangle(CPUGeometryGenerator& g, unsigned int i0, unsigned int i1, unsigned int i2)
	{
		g.myIndicesOffset[g.indexOffset++] = g.vertexOffset + i0;
		g.myIndicesOffset[g.indexOffset++] = g.vertexOffset + i1;
		g.myIndicesOffset[g.indexOffset++] = g.vertexOffset + i2;
	}
	static void finalize(CPUGeometryGenerator& g)
	{
	}
};

#endif