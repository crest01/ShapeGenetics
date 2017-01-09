/*
 * GeneticAlgoScene.h
 *
 *  Created on: Nov 2, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef SRC_GENETICALGOSCENE_H_
#define SRC_GENETICALGOSCENE_H_


#include "Scene.h"
#include "CPU/GrammarSystem.h"
#include "ProceduralAlgorithm.h"
#include <sstream>
#include "OrbitalNavigator.h"

class GeneticAlgoScene : public Scene
{
	unsigned int seed;
	PGA::ProceduralAlgorithm_impl* genetic_algo;
	int _current_generation;
public:
	GeneticAlgoScene(const std::string& filename);

	double generate();
	double recombine(bool restart);

	void getGeometry(std::vector<PGA::GeneratedVertex>& geometry, std::vector<unsigned>& indices);
	void getVoxels(std::vector<math::float4>& centers);
	void setSeed(unsigned int s)
	{
		seed = s;
	}
	void setParameterTable(const PGG::Parameters::ParameterTable& parameterTable, int axiomId) {}
	void showNext()
	{
		genetic_algo->displayNext();
	}

	void showPrev()
	{
		genetic_algo->displayPrev();
	}

	double generateOneGeneration()
	{
		genetic_algo->doOneGeneration();
		return 0.0f;
	}


	void toggleDisplayTarget()
	{
		genetic_algo->toggleDisplayTarget();
	}

	void setCameraToEvalPoint(Navigator* navigator)
	{
		if (OrbitalNavigator* o = dynamic_cast<OrbitalNavigator*>(navigator)) {
			genetic_algo->getCameraParameters(o->phi, o->theta, o->radius, o->lookat);
			o->update();
		}
	}

	std::string getTargetImageFilename()
	{
		std::stringstream filename;
		filename << genetic_algo->getName() << ".png";
		return filename.str();
	}

	bool moreGenerations() {
		return (_current_generation < genetic_algo->getMaxNumGenerations());
	}

	void init()
	{
		genetic_algo->init();
	}

	PGA::GeometryBufferInstanced& getGeometryBuffer();
};




#endif /* SRC_GENETICALGOSCENE_H_ */
