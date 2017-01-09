/*
 * GeneticAlgorithm.h
 *
 *  Created on: Dec 5, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_SOSMC_H_
#define GPUPROCGENETICS_SRC_SOSMC_H_

#include "Allocator.h"



namespace PGA {

class SOSMCAlgorithm : public GeneticAlgorithm_IF
{
	ProceduralAlgorithm*	_algo;
	AlgorithmConf*	 		_conf;
	PopulationConf*			_population;
	GenomeConf* 			_genome;
	GrammarConf*			_grammar;
	MutationConf*			_grow_mutation;
	SelectionConf*			_resampling;
	EvaluationConf*			_evaluation;
	GeometryConf*			_geometry;
	StatisticsConf*			_stats;

	int _generation;
	float	_total_time;

	void addTimePerGeneration(const float& time)
	{
		_total_time += time;
	}

	void resetTime()
	{
		_total_time = 0.0f;
	}

	bool do_more_generations()
	{
		bool ret = false;

		if (_conf->getUseMaxRuntime()) {
			if (_total_time < _conf->getMaxRuntime()) {
				ret = true;
			}
		}
		else if (_conf->getMaxGenerations() > _generation) {
			ret = true;
		}

		return ret;
	}


public:
	SOSMCAlgorithm(ProceduralAlgorithm* base):
		_algo(base),
		_total_time(0.0),
		_generation(0)
	{}

	void init(ProceduralAlgorithm* base)
	{
		_conf = _algo->get<AlgorithmConf*>("base");
		_population = _algo->get<PopulationConf*>("population");
		_genome = _algo->get<GenomeConf*>("genome");
		_grammar = _algo->get<GrammarConf*>("grammar");
		_grow_mutation = _algo->get<MutationConf*>("mutation");
		_evaluation = _algo->get<EvaluationConf*>("evaluation");
		_resampling = _algo->get<SelectionConf*>("resampling");
		_geometry = _algo->get<GeometryConf*>("geometry");
		_stats = _algo->get<StatisticsConf*>("statistics");

		createInitialPopulation();
	}

	void createInitialPopulation()
	{
		auto t0 = std::chrono::high_resolution_clock::now();

		_generation = 0;

		_conf->setCurrentGeneration(_generation);

		if (_stats->recordStats()) {
			_stats->impl()->setCurrentGeneration(_generation);
		}

		for (int i = 0; i < _conf->getPopulationSize(); ++i) {
			// start with genomes of length 1
			Genome_IF* new_genome = _genome->factory()->createRandomGenome(*_grammar, 2);
			_population->impl()->insertIntoActiveGeneration(new_genome);
		}


		evaluateInitialPopulation();

		auto t1 = std::chrono::high_resolution_clock::now();
		if (_stats->recordStats()) {
			std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);
			float t = time_span.count();
			_stats->impl()->setReproductionTime(t);
			_stats->impl()->storeCurrentStats(_generation);
		}
	}

	void evaluateInitialPopulation()
	{
		bool record_stats = _stats->recordStats();

		for (int i = 0; i < _population->impl()->activeGenerationSize(); ++i) {
			Genome_IF* genome = _population->impl()->getGenome(i);
			float old_rating = genome->getEvalPoints();
			float rating = _evaluation->impl()->calcRating(genome);
			//	std::cout << "evaluate individual " << i <<": old fitness: " << old_rating <<", new: " << rating << std::endl;
			genome->setEvalPoints(rating);
			if (record_stats) {
				_stats->impl()->addFitness(genome->getEvalPoints());
				_stats->impl()->addLength(static_cast<float>(genome->length()));
			}
		}
	}

	void createNewGeneration()
	{

		bool record_stats = _stats->recordStats();

		_conf->setCurrentGeneration(_generation);

		auto t0 = std::chrono::high_resolution_clock::now();

		if (record_stats) {
			_stats->impl()->setCurrentGeneration(_generation);
		}

		float sum_rating = 0.0f;

		// store the current stats
		// delete the old generation
		_population->impl()->clearNextGeneration();

		for (int i = 0; i < _conf->getPopulationSize(); ++i) {

			// prepare space for the next genome
			Genome_IF* next_step = _genome->factory()->createEmptyGenome();

			// get the current genome
			Genome_IF* current_step = _population->impl()->getGenome(i);
			// clone the current genome
			next_step->clone(_grammar->getSm(), current_step);

			_grow_mutation->impl()->mutate(*_grammar, next_step);
			float rating = _evaluation->impl()->calcRating(next_step);
			next_step->setEvalPoints(rating);
			_population->impl()->insertIntoNextGeneration(next_step);

			if (record_stats) {
				_stats->impl()->addFitness(rating);
				_stats->impl()->addLength(static_cast<float>(next_step->length()));
			}

		}

		// switch populations
		_population->impl()->switchGeneration();
		// delete the old population
		_population->impl()->clearNextGeneration();
		// sort the new generation
		_population->impl()->sortActiveGeneration();
		// normalize the fitness values
		_resampling->first()->prepareForSelection();

		// resample based on the fitness values
		for (int i = 0; i < _conf->getPopulationSize(); ++i) {
			Genome_IF* genome = _resampling->first()->selectGenome();
			// prepare space for the next genome
			Genome_IF* new_genome = _genome->factory()->createGenome(*_grammar, genome);
			new_genome->setEvalPoints(genome->getEvalPoints());
			_population->impl()->insertIntoNextGeneration(new_genome);
		}

		if (record_stats) {
			auto t1 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);
			float t = time_span.count();

			_stats->impl()->setReproductionTime(t);
			_stats->impl()->storeCurrentStats(_generation);
		}


		// make the new generation the current.
		_population->impl()->switchGeneration();

		// sort the generation so the best genome is the first
		_population->impl()->sortActiveGeneration();

		reinitMtGenerator();

	}

	void reset() {
		_population->impl()->clearCurrentGeneration();
		_population->impl()->clearNextGeneration();
		createInitialPopulation();
	}

	void run()
	{
		for (int run = 1; run <= _stats->getReruns(); ++run) {
			std::cerr << "start run " << run << " of " << _stats->getReruns() << " for " << _conf->getName() <<  std::endl;
			bool cont = false;
			do {
				auto t0 = std::chrono::high_resolution_clock::now();
				doOneGeneration();
				auto t1 = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);
				float t = time_span.count();
				addTimePerGeneration(t);
				cont = do_more_generations();
				if (_generation % 10 == 0) {
					std::cerr << "Generation " << _generation << ", best points " << _population->impl()->getBestGenome()->getEvalPoints() << " time: " << _total_time << std::endl;
				}
				if (cont == false){
					std::cerr << "Final Generation " << _generation << ", best points " << _population->impl()->getBestGenome()->getEvalPoints() << " time: " << _total_time << std::endl;
				}
				_stats->impl()->saveBestObj(_generation, run, !cont);
			}while(cont);
			_stats->impl()->finishRun();
			resetTime();
			if (run != _stats->getReruns()) {
				reset();
			}
		}
	}

	void doOneGeneration()
	{
		_generation++;
		createNewGeneration();
		evaluatePopulation();
		if (_stats->recordStats()) _stats->impl()->storeCurrentStats(_generation);
	}

	void evaluatePopulation() {}


};



}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_SOSMC_H_ */
