#include "ObjShape.h"
#include <fstream>
#include <iostream>
namespace PGA {

void ObjShape::normalizeShape()
{

	if (v.size() == 0) {
		return;
	}
	math::float3 center;
	math::float3 min;
	math::float3 max;

	center = v[0];
	min = center;
	max = center;

	for (int i = 1; i < v.size(); ++i) {
		center += v[i];
		if (min.x > v[i].x) {
			min.x = v[i].x;
		}
		if (min.y > v[i].y) {
			min.y = v[i].y;
		}
		if (min.z > v[i].z) {
			min.z = v[i].z;
		}
		if (max.x < v[i].x) {
			max.x = v[i].x;
		}
		if (max.y < v[i].y) {
			max.y = v[i].y;
		}
		if (max.z < v[i].z) {
			max.z = v[i].z;
		}
	}

	center = center / v.size();
	math::float4x4 transform = math::identity<math::float4x4>();
	transform._14 = center.x * -1.0f;
	transform._24 = center.y * -1.0f;
	transform._34 = center.z * -1.0f;

	math::float3 size(max - min);
	math::float3 scale(1.0f/size.x, 1.0f/size.y, 1.0f/size.z);
	transform._11 = transform._11 * scale.x;
	transform._22 = transform._22 * scale.y;
	transform._33 = transform._33 * scale.z;

	for (int i = 0; i < v.size(); ++i) {
		math::float4 p(v[i].x, v[i].y, v[i].z, 1.0f);
		p = transform * p;
		v[i] = math::float3(p.x/p.w, p.y/p.w, p.z/p.w);
	}
	// todo: update the normals as well?

}

void ObjShape::applyMatrixToObj(const math::float3x4& m)
{
	for (int i = 0; i < v.size(); ++i) {
		v[i] = m*math::float4(v[i], 1.0f);
	}
	for (int i = 0; i < vn.size(); ++i) {
		vn[i] = normalize(m*math::float4(vn[i], 0.0f));
	}
}

int ObjShape::writeVertsToFile(std::ofstream& out)
{
	for (int i = 0; i < v.size(); ++i) {
		out << "v " << v[i].x << " " << v[i].y << " " << v[i].z << std::endl;
	}
	return v.size();
}

int ObjShape::writeNormToFile(std::ofstream& out)
{
	for (int i = 0; i < vn.size(); ++i) {
		out << "vn " << vn[i].x << " " << vn[i].y << " " << vn[i].z << std::endl;
	}
	return vn.size();
}

int ObjShape::writeTexToFile(std::ofstream& out)
{
	for (int i = 0; i < vt.size(); ++i) {
		out << "vt " << vt[i].x << " " << vt[i].y << std::endl;
	}
	return vt.size();
}

int ObjShape::writeFaceToFile(std::ofstream& out)
{
	for (int i = 0; i < f.size(); ++i) {
		out << "f ";
		for (int j = 0; j < 3; ++j) {
			out << f[i].v[j] + v_off << "/" << f[i].t[j] + t_off << "/" << f[i].n[j] + n_off << " ";
		}
		out << std::endl;
	}
	return f.size();
}


void ObjShape::createVerticesForOpenGL()
{
	for (int face = 0; face < f.size(); ++face) {
		GeneratedVertex verts[3];
		for (int i = 0; i < 3; ++i) {
			verts[i] = GeneratedVertex(v[f[face].v[i] - 1], vn[f[face].n[i] - 1], vt[f[face].t[i] - 1]);
		}

		math::float3 obj_n = (verts[0].n + verts[1].n + verts[2].n) / 3.0f;
		float direction = dot(cross(verts[0].p - verts[1].p, verts[0].p - verts[2].p),obj_n);
		if (direction > 0) {
			for (int i = 2; i >= 0; --i) {
				vertices.push_back(verts[i]);
			}
		}
		else {
			for (int i = 0; i <= 2; ++i) {
				vertices.push_back(verts[i]);
			}
		}
	}
}

bool ObjShape::readFromFile(const std::string& filename)
{
	std::ifstream file(filename);

	if (!file) {
		std::cout << "unable to open mesh file '" << filename << "'" <<  std::endl;
		return false;
	}
	std::cout << "reading from file " << filename << std::endl;

	std::string cmd;

	while (file >> cmd)
	{
		if (cmd[0] == '#')
		{
			while (file && file.get() != '\n');
		}
		else if (cmd == "v")
		{
			math::float3 p;
			file >> p.x >> p.y >> p.z;
			v.push_back(p);
		}
		else if (cmd == "vn")
		{
			math::float3 n;
			file >> n.x >> n.y >> n.z;
			vn.push_back(n);
		}
		else if (cmd == "vt")
		{
			math::float2 t;
			file >> t.x >> t.y;
			vt.push_back(t);
		}
		else if (cmd == "f")
		{
			Face face;

			for (int i = 0; i < 3; ++i)
			{
				int indices[3];

				// TODO: this only works if the file has verts/texture/norms for each face vertex
				for (int j = 0; j < 3; ++j)
				{
					file >> indices[j];
					file.get();
				}
				face.v[i] = indices[0];
				face.t[i] = indices[1];
				face.n[i] = indices[2];
			}

			f.push_back(face);
		}
	}

	return true;
}



} // namespace PGA
