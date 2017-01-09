/*
 * GeneratedVertex.h
 *
 *  Created on: Mar 3, 2016
 *      Author: toe
 */

#ifndef GENERATEDVERTEX_H_
#define GENERATEDVERTEX_H_

#include <vector>
#include "math/vector.h"

namespace PGA {

struct GeneratedVertex
{
	GeneratedVertex() = default;
	GeneratedVertex(const math::float3& p, const math::float3& n, const math::float2& tex) : p(p), n(n), tex(tex) {}
	math::float3 p;
	math::float3 n;
	math::float2 tex;
	GeneratedVertex(const GeneratedVertex& other)
	{
		this->p = other.p;
		this->n = other.n;
		this->tex = other.tex;
	}

};

struct GeneratedObject {
	unsigned int vertex_offset;
	unsigned int num_vertex;
	unsigned int index_offset;
	unsigned int num_index;
};

}  // namespace PGA




#endif /* GENERATEDVERTEX_H_ */
