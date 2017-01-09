/*
 * ObjShape.h
 *
 *  Created on: Aug 18, 2016
 *      Author: toe
 */

#ifndef OBJSHAPE_H_
#define OBJSHAPE_H_

#include <string>
#include "GeneratedVertex.h"
#include "math/matrix.h"

namespace PGA {

class ObjShape {
private:
	// the data from the obj-file
	struct Face {
		int v[3];
		int t[3];
		int n[3];
	};
	std::vector<math::float3> v;
	std::vector<math::float3> vn;
	std::vector<math::float2> vt;
	std::vector<Face> f;
	int v_off;
	int t_off;
	int n_off;

public:

	// the data for openGL, constructed from the obj-file
	std::vector<GeneratedVertex> vertices;

	ObjShape():
		v_off(0),
		t_off(0),
		n_off(0)
	{}

	ObjShape(const ObjShape& other):
		v(other.v),
		vn(other.vn),
		vt(other.vt),
		f(other.f),
		v_off(0),
		t_off(0),
		n_off(0)
	{}

	void set_v_offset(const int& offset)
	{
		v_off = offset;
	}

	void set_n_offset(const int& offset)
	{
		n_off = offset;
	}

	void set_t_offset(const int& offset)
	{
		t_off = offset;
	}

	~ObjShape() {};


	bool readFromFile(const std::string& filename);

	void normalizeShape();

	void createVerticesForOpenGL();

	void applyMatrixToObj(const math::float3x4& m);

	int writeVertsToFile(std::ofstream& out);
	int writeNormToFile(std::ofstream& out);
	int writeTexToFile(std::ofstream& out);
	int writeFaceToFile(std::ofstream& out);
};

} // namespace PGA



#endif /* OBJSHAPE_H_ */
