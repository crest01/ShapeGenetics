/*
 * GeometryBuffer.h
 *
 *  Created on: Dec 5, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_GRAMMAR_GEOMETRYBUFFER_INSTANCED_H_
#define GPUPROCGENETICS_SRC_GRAMMAR_GEOMETRYBUFFER_INSTANCED_H_

#include "CPU/SharedBuffer.h"
#include "GeneratedVertex.h"
#include "ObjShape.h"
#include <fstream>
#include <iostream>
#include <string>

namespace PGA {

class GeometryBufferInstanced {
private:

	std::vector<ObjShape*> shapes;
	std::vector<math::float4>	colors;
	std::vector<PGG::CPU::SharedBuffer<math::float3x4>* >	matrices;

public:

	GeometryBufferInstanced() {};
	~GeometryBufferInstanced()
	{
		unloadShapes();
	}

	int prepareObjShape(const std::string& filename, const math::float4& color, const bool normalize = true)
	{
		int index = static_cast<int>(shapes.size());

		ObjShape* shape = new ObjShape();
		if (shape->readFromFile(filename) == false) {
			return -1;
		}
		if (normalize) {
			shape->normalizeShape();
		}
		shape->createVerticesForOpenGL();

		shapes.push_back(shape);
		matrices.push_back(new PGG::CPU::SharedBuffer<math::float3x4>);
		colors.push_back(color);
		return index;
	}

	void saveToObj(const std::string& filename)
	{
		std::ofstream file(filename);

		int v_offset = 0;
		int t_offset = 0;
		int n_offset = 0;

		for (int s = 0; s < shapes.size(); ++s) {
			if (matrices[s]->size() == 0) {
				continue;
			}
			std::vector<math::float3x4> mats;
			mats.resize(matrices[s]->size());
			matrices[s]->copy(&mats[0], 0, matrices[s]->size());
			ObjShape* base = shapes[s];
			file << "o shape_" << s << std::endl;
			for (int i = 0; i < mats.size(); ++i) {
//				file << "g shape_" << s << "_instance_" << i << std::endl;
				ObjShape instanced(*base);
				instanced.applyMatrixToObj(mats[i]);

				int v_off_new = instanced.writeVertsToFile(file);
				int n_off_new = instanced.writeNormToFile(file);
				int t_off_new = instanced.writeTexToFile(file);

				instanced.set_n_offset(n_offset);
				instanced.set_t_offset(t_offset);
				instanced.set_v_offset(v_offset);

				instanced.writeFaceToFile(file);
				v_offset += v_off_new;
				n_offset += n_off_new;
				t_offset += t_off_new;
			}
		}
	}

	void unloadShapes()
	{
		for (int i = 0; i < shapes.size(); ++i) {
			delete shapes[i];
			delete matrices[i];
		}
		shapes.clear();
		matrices.clear();
	}

	ObjShape* getShape(int index) {
		return shapes[index];
	}

	math::float4& getColor(int index) {
		return colors[index];
	}

	PGG::CPU::SharedBuffer<math::float3x4>& getMatrixBuffer(int index) {
		return *matrices[index];
	}

	int getNumShapes()
	{
		return static_cast<int>(shapes.size());
	}

	void clearBuffers()
	{
		for (int i = 0; i < shapes.size(); ++i) {
			matrices[i]->clear();
		}
	}

};

}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_GRAMMAR_GEOMETRYBUFFER_INSTANCED_H_ */
