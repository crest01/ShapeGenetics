#ifndef INCLUDED_MYSCENE
#define INCLUDED_MYSCENE

#pragma once

#include <vector>
#include "parameters/ParameterTable.h"
#include "GeometryBufferInstanced.h"
#include "Navigator.h"
class Scene
{
public:
	virtual double generate() = 0;
	virtual double generateOneGeneration() = 0;
	virtual void getGeometry(std::vector<PGA::GeneratedVertex>& geometry, std::vector<unsigned>& indices) = 0;
	virtual void getVoxels(std::vector<math::float4>& centers) = 0;
	virtual double recombine(bool restart) = 0;
	virtual bool moreGenerations() = 0;
	virtual void setSeed(unsigned int seed) = 0;
	virtual void setParameterTable(const PGG::Parameters::ParameterTable& parameterTable, int axiomId) = 0;
	virtual void showNext() = 0;
	virtual void showPrev() = 0;
	virtual void toggleDisplayTarget() = 0;
	virtual PGA::GeometryBufferInstanced& getGeometryBuffer() = 0;
	virtual std::string getTargetImageFilename() = 0;

	virtual void setCameraToEvalPoint(Navigator* navigator) = 0;

};
#endif
