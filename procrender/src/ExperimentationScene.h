#ifndef INCLUDED_EXPERIMENTSCENE
#define INCLUDED_EXPERIMENTSCENE

#pragma once

#include "Scene.h"
#include "CPU/GrammarSystem.h"

class ExperimentationScene : public Scene
{
	unsigned int seed;
	PGG::CPU::GrammarSystem system;

	const PGG::Parameters::ParameterTable* pt;
	int axiomId;
public:
	ExperimentationScene();
	double generate(std::vector<PGA::GeneratedVertex>& geometry, std::vector<unsigned>& indices);
	void setSeed(unsigned int s)
	{
		seed = s;
	}
	void setParameterTable(const PGG::Parameters::ParameterTable& parameterTable, int axiomId);
	void showNext() {};
	void showPrev() {};
	void toggleDisplayTarget() {};
	double generateOneGeneration() { return 0.0f; }
	double recombine(bool restart) { return 0.0f;  };
	bool moreGenerations() { return false; }
	std::string getTargetImageFilename() { return "out.png"; }

	void setCameraToEvalPoint(Navigator* navigator) {}
};
#endif
