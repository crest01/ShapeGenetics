/*
 * ProceduralAlgorithm.h
 *
 *  Created on: Jul 15, 2016
 *      Author: toe
 */

#ifndef PROCEDURALALGORITHM_H_
#define PROCEDURALALGORITHM_H_

#include <fstream>

#include "Evaluation_IF.h"
#include "Genome_IF.h"
#include "Grammar_IF.h"
#include "Mutation_IF.h"
#include "Population_IF.h"
#include "Recombination_IF.h"
#include "Selection_IF.h"
#include "Algorithm_IF.h"
#include "Statistics_IF.h"
#include "GeometryBufferInstanced.h"
#include "GeneratedVertex.h"
#include "Config.h"

#include "SymbolManager.h"

#include "GeneticAlgoConfig.h"

namespace PGA {

class ProceduralAlgorithm_impl {
private:
	ProceduralAlgorithm* _algo;
	std::string			_type;
	ProceduralAlgorithm_impl() :
		_current_index(-1)
	{
		_algo = new ProceduralAlgorithm;
	}


	~ProceduralAlgorithm_impl()
	{
//		stats_file.close();
		delete _algo;
	};

	ProceduralAlgorithm_impl(ProceduralAlgorithm_impl const&) = delete;
    void operator=(ProceduralAlgorithm_impl const&) = delete;

    int _current_index;

    std::ofstream stats_file;

    bool _display_target;


	static void createBasicAlgoFromConfig(ProceduralAlgorithm* algo, Config& config);
	void initCommonAlgo();

	static ProceduralAlgorithm_impl* createGeneticAlgoFromConfig(Config& config);
	void initGeneticAlgo();
	static ProceduralAlgorithm_impl* createMHAlgoFromConfig(Config& config);
	void initMHAlgo();
	static ProceduralAlgorithm_impl* createSMCAlgoFromConfig(Config& config);
	void initSMCAlgo();
	static ProceduralAlgorithm_impl* createSOSMCAlgoFromConfig(Config& config);
	void initSOSMCAlgo();


public:

	static ProceduralAlgorithm_impl* createFromConfig(const std::string& filename);

	void init();

	void findSolution();

	void toggleDisplayTarget() {
		_display_target = ! _display_target;
	}

	void displayNext()
	{
		_current_index ++;
		PopulationConf* pop = _algo->get<PopulationConf*>("population");
		if (_current_index >= pop->impl()->activeGenerationSize()) {
			_current_index = -1;
		}
	}
	void displayPrev()
	{
		_current_index --;
		PopulationConf* pop = _algo->get<PopulationConf*>("population");
		if (_current_index < -1) {
			_current_index = pop->impl()->activeGenerationSize() - 1;
		}
	}

	GeometryBufferInstanced& getGeometryBufferInstanced()
	{
		GeometryConf* geometry_conf = _algo->get<GeometryConf*>("geometry");
		if (_display_target) {
			return *geometry_conf->getInstancedTargetMeshBuffer();
		}
		else {
			return *geometry_conf->getInstancedObjectMeshBuffer();
		}
	}
	const std::string& getName()
	{
		AlgorithmConf* conf = _algo->get<AlgorithmConf*>("base");
		return conf->getName();
	}


	void getVoxels(std::vector<math::float4>& center)
	{
		EvaluationConf* conf = _algo->get<EvaluationConf*>("evaluation");
		if (_display_target) {
			conf->impl()->generateTargetVoxels(center);
		}
		else {
			conf->impl()->generateVoxels(_current_index, center);
		}
	}

	void generateGeometry()
	{
		if (_display_target) {
			std::cerr << "Display target" << std::endl;
			return;
		}
		else {
			GeometryConf* geometry = _algo->get<GeometryConf*>("geometry");

			PopulationConf* pop = _algo->get<PopulationConf*>("population");
			Genome_IF* genome = nullptr;
			if (_current_index == -1) {
				genome = pop->impl()->getBestGenome();
				}
			else {
				genome = pop->impl()->getGenome(_current_index);
			}

			if (genome == nullptr) {
				return;
			}

			GrammarConf* grammar = _algo->get<GrammarConf*>("grammar");
			grammar->impl()->createGeometry(genome);

			if (_current_index == -1) {
				std::cerr << "Display best Genome (" << genome->getEvalPoints() << " Points, " << genome->length() << " symbols) of " << pop->impl()->activeGenerationSize() << std::endl;
			}
			else {
				std::cerr << "Display Genome " << _current_index << " (" << genome->getEvalPoints() << " Points, " << genome->length() << " symbols) of " << pop->impl()->activeGenerationSize() << std::endl;
			}
		}

	}

	void getCameraParameters(float& phi, float& theta, float& radius, math::float3& lookat)
	{
		EvaluationConf* eval = _algo->get<EvaluationConf*>("evaluation");
		if(ImageEvaluationConf* v = dynamic_cast<ImageEvaluationConf*>(eval)) {
			float fov, znear, zfar;
			v->getCameraParameters(fov, znear, zfar, phi, theta, radius, lookat);
		}
	}


	void create_next_generation()
	{
		AlgorithmConf* conf = _algo->get<AlgorithmConf*>("base");
		conf->impl()->createNewGeneration();
		conf->impl()->evaluatePopulation();
		//_current_index = 0;
	}

	int getMaxNumGenerations() {
		AlgorithmConf* conf = _algo->get<AlgorithmConf*>("base");
		return conf->getMaxGenerations();
	}

	void doOneGeneration()
	{
		AlgorithmConf* conf = _algo->get<AlgorithmConf*>("base");
		conf->impl()->doOneGeneration();
		//_current_index = 0;
	}

	void writeStatistics() {
		StatisticsConf* conf = _algo->get<StatisticsConf*>("statistics");

		conf->impl()->writeStatistics(false);
	}

	void reset() {
		AlgorithmConf* conf = _algo->get<AlgorithmConf*>("base");
		StatisticsConf* stats = _algo->get<StatisticsConf*>("statistics");
		stats->impl()->clearStats();
		conf->impl()->reset();
	}

	void run()
	{
		AlgorithmConf* conf = _algo->get<AlgorithmConf*>("base");
		StatisticsConf* stats = _algo->get<StatisticsConf*>("statistics");
		conf->impl()->run();
		_current_index = -1;
		stats->impl()->writeStatistics(true);
	}

	void saveObj()
	{
		GeometryConf* geometry = _algo->get<GeometryConf*>("geometry");
		geometry->getInstancedMeshBuffer()->saveToObj("result.obj");
	}

};

} // namespace PGA



#endif /* PROCEDURALALGORITHM_H_ */
