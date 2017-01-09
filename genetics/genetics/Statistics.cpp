/*
 * Statistics.cpp
 *
 *  Created on: Dec 19, 2015
 *      Author: Karl Haubenwallner
 */

#include <sstream>
#include "Statistics.h"
#include "GeneticAlgoConfig.h"
#include "Population_IF.h"
#include "Grammar_IF.h"
#include "GeometryBufferInstanced.h"

namespace PGA {


Statistics::Statistics(ProceduralAlgorithm* base):
	_algo(base),
	_runs(0)
{}

Statistics::~Statistics() {
	clearStats();
}

void Statistics::init(ProceduralAlgorithm* base)
{
	_base = base->get<AlgorithmConf*>("base");
	_geometry = base->get<GeometryConf*>("geometry");
	_conf = base->get<StatisticsConf*>("statistics");
	_population = base->get<PopulationConf*>("population");
	_grammar = base->get<GrammarConf*>("grammar");
	clearStats();


}

void Statistics::storeCurrentStats(const int& generation)
{
	_current.min_fitness.addValue(_current_fitness.min());
	_current.max_fitness.addValue(_current_fitness.max());
	_current.avg_fitness.addValue(_current_fitness.avg());
	_current.generation = generation;
	_current.retries.addValue(_retries.max());
	_stats[generation] = _current;
}

void Statistics::addRetries(const int& retries)
{
	_retries.addValue(retries * 1.0f);
}

void Statistics::addFitness(const float& fitness)
{
	_current_fitness.addValue(fitness);
}

void Statistics::addLength(const float& length)
{
	_current.genome_length.addValue(length);
}

void Statistics::setEvalTime(const float& production_time, const float& calculation_time)
{
	_current.eval_production_time.addValue(production_time);
	_current.eval_calculation_time.addValue(calculation_time);
}

void Statistics::setReproductionTime(const float& time)
{
	_current.reproduction_time.addValue(time);
}

void Statistics::setCurrentGeneration(const int& generation)
{
	_current = _stats[generation];
	_current.generation = generation;
	_current_fitness.reset();
	_retries.reset();
}

void Statistics::clearStats()
{
	_current_fitness.reset();
	_retries.reset();
	_eval_production_time = 0.0f;
	_eval_calculation_time = 0.0f;
	_reproduction_time = 0.0f;
	_runs = 0;
	_stats.clear();
}

void Statistics::setOutStream(std::ostream& out)
{
	_out = &out;
}


void Statistics::writeHeader(bool cvs, bool include_config)
{
	if (include_config) {
		std::map<std::string, ConfigBase*>& cfgs = _algo->getAllConfigs();
		for (std::map<std::string, ConfigBase*>::iterator it = cfgs.begin(); it != cfgs.end(); ++it) {
			it->second->toString(*_out);
		}
	}
	if (cvs) {
		(*_out) << "Generation,Max Fitness,Avg Fitness,Min Fitness, Eval production time, Eval calculation time, Reproduction time, Retries,Min length, Avg length, Max length" << std::endl;
	}
	else {
		(*_out) << "Generation \t Fitness: Max/Avg/Min\tEval production Time \t Eval calculation Time \t Reproduction Time "<< std::endl;
	}
}

void Statistics::writeStatistics(bool cvs)
{
	float total_time = 0.0f;

	for (int i = 0; i < _stats.size(); ++i) {
		GenerationStats s = _stats[i];
		total_time += s.eval_production_time.avg();
		total_time += s.eval_calculation_time.avg();
		total_time += s.reproduction_time.avg();
		if (cvs && _runs > 1) {
			(*_out) << s.generation
								<< "," << s.max_fitness.min() << ";" << s.max_fitness.avg() << ";" << s.max_fitness.max()
								<< "," << s.avg_fitness.min() << ";" << s.avg_fitness.avg() << ";" << s.avg_fitness.max()
								<< "," << s.min_fitness.min() << ";" << s.min_fitness.avg() << ";" << s.min_fitness.max()
								<< "," << s.eval_production_time.min() << ";" << s.eval_production_time.avg() << ";" << s.eval_production_time.max()
								<< "," << s.eval_calculation_time.min() << ";" << s.eval_calculation_time.avg() << ";" << s.eval_calculation_time.max()
								<< "," << s.reproduction_time.min() << ";" << s.reproduction_time.avg() << ";" << s.reproduction_time.max()
								<< "," << s.retries.min() << ";" << s.retries.avg() << ";" << s.retries.max()
								<< std::endl;
		}
		else if (cvs && _runs == 1) {
			(*_out) << s.generation
								<< "," << s.max_fitness.avg()
								<< "," << s.avg_fitness.avg()
								<< "," << s.min_fitness.avg()
								<< "," << s.eval_production_time.avg()
								<< "," << s.eval_calculation_time.avg()
								<< "," << s.reproduction_time.avg()
								<< "," << s.retries.max()
								<< "," << s.genome_length.min() << "," << s.genome_length.avg() << ";" << s.genome_length.max()
								<< std::endl;
		}
		else
			(*_out) << s.generation << "\t" << s.max_fitness.avg() << " / " << s.avg_fitness.avg() << " / " << s.min_fitness.avg() << "\t" << s.eval_production_time.avg() <<  "ms \t" << s.eval_calculation_time.avg() <<  "ms \t" << s.reproduction_time.avg() << "ms" << std::endl;
	}

	if (!cvs) {
		(*_out) << "Total Time: " << total_time << "ms" << std::endl;
	}
}


void Statistics::writeSimpleStatistics(::std::ostream& out)
{
	out << "Generation,max Fitness,Time" << std::endl;
	for (int i = 0; i < _stats.size(); ++i) {
		GenerationStats s = _stats[i];
		float total_time = s.eval_production_time.avg();
		total_time += s.eval_calculation_time.avg();
		total_time += s.reproduction_time.avg();
		out << s.generation
				<< "," << s.max_fitness.avg()
				<< "," << total_time
				<< std::endl;
	}
}


void Statistics::finishRun()
{
	int run = _runs;
	run ++;
	writeToFile(run);
	clearStats();
	_runs = run;
}

void Statistics::writeToFile(const int& run)
{
	std::string filename = _base->getPrefix() + "run_" + std::to_string(run)+ "_stats.csv";
	std::cout << "Statistics will be written to " << filename << std::endl;

	std::ofstream stats_file;
    stats_file.open(filename, std::ofstream::out);
    writeSimpleStatistics(stats_file);
    stats_file.close();
}


void Statistics::saveBestObj(const int& generation, const int& run, const bool& final_generation)
{
	bool save = false;
	bool detailed = false;
	bool save_best = false;

	if (final_generation && _conf->saveFinalObj()) {
		save = true;
		save_best = true;
		detailed = _conf->saveFinalObj_detailed();
	}
	if (!final_generation && _conf->saveEveryObj()) {
		save |= true;
		detailed |= _conf->saveEveryObj_detailed();
	}

	if (save) {
		if (detailed) {
			std::string filename = _base->getPrefix() + "run_" + std::to_string(run)+ "_" + _conf->getobjPrefix() + "detailed_" + std::to_string(generation)+".obj";
			_grammar->impl()->createDetailedGeometry(_population->impl()->getBestGenome());
			_geometry->getInstancedMeshBuffer()->saveToObj(filename);
		}
		else {
			std::string filename = _base->getPrefix() + "run_" + std::to_string(run)+ "_" + _conf->getobjPrefix() + std::to_string(generation) + ".obj";
			_grammar->impl()->createGeometry(_population->impl()->getBestGenome());
			_geometry->getInstancedMeshBuffer()->saveToObj(filename);
		}
	}
	if (save_best) {
		std::string filename = _base->getPrefix() + _conf->getobjPrefix() + "best.obj";
		Genome_IF* best = _population->impl()->getAbsoluteBestGenome();
		if (best != nullptr) {
			_grammar->impl()->createDetailedGeometry(best);
			std::cout << "save absolute best genome (fitness: " << best->getEvalPoints() <<") to " << filename << std::endl;
			_geometry->getInstancedMeshBuffer()->saveToObj(filename);
		}
	}
	return;
}

}  // namespace PGA



