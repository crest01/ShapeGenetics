#ifndef INCLUDED_TARGETGEOMETRYGENERATOR_INSTANCED
#define INCLUDED_TARGETGEOMETRYGENERATOR_INSTANCED

#pragma once

#include <math/vector.h>
#include "scope/Scope.h"
#include "Evaluation_IF.h"
#include "GeneticAlgoConfig.h"
#include "GeometryBufferInstanced.h"
#include "GeometryConfigSingleton.h"

namespace PGA {

template<class Scope = PGG::Scope::CoordinateframeScope<>, const int model = 0, const bool applyScale = true>
class InstancedShapeGenerator
{
	GeometryBufferInstanced* _buffer;
public:
	InstancedShapeGenerator(GeometryBufferInstanced* buffer) :
		_buffer(buffer)
	{
	}

	template<class Shape>
	static InstancedShapeGenerator newShape(const PGG::Scope::ScopedShape<Shape, Scope>& s)
	{
		GeometryConfigSingleton* g_conf = GeometryConfigSingleton::getInstance();
		InstancedShapeGenerator g(g_conf->conf->getInstancedMeshBuffer());

		auto res = g._buffer->getMatrixBuffer(model).reserve(1);
		res[0] = s.scope.matrix;

		if (applyScale == true) {
			res[0]._11 *= s.shape.size.x; res[0]._21 *= s.shape.size.x; res[0]._31 *= s.shape.size.x;
			res[0]._12 *= s.shape.size.y; res[0]._22 *= s.shape.size.y; res[0]._32 *= s.shape.size.y;
			res[0]._13 *= s.shape.size.z; res[0]._23 *= s.shape.size.z; res[0]._33 *= s.shape.size.z;
		}
		return g;
	}
	static void allocVertices(InstancedShapeGenerator& g, int num)
	{
	}
	static void allocIndices(InstancedShapeGenerator& g, int num)
	{
	}
	static void addVertex(InstancedShapeGenerator& g, int index, const math::float3& v, const math::float3& n, const math::float2& t)
	{
	}
	static void addTriangle(InstancedShapeGenerator& g, unsigned int i0, unsigned int i1, unsigned int i2)
	{
	}
	static void finalize(InstancedShapeGenerator& g)
	{
	}
};
}  // namespace PGA


#endif //INCLUDED_TARGETGEOMETRYGENERATOR_INSTANCED
