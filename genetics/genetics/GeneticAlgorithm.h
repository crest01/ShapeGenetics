/*
 * GeneticAlgorithm.h
 *
 *  Created on: Dec 5, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_GENETICS_GENETICALGORITHM_H_
#define GPUPROCGENETICS_SRC_GENETICS_GENETICALGORITHM_H_

#include "Allocator.h"

namespace PGA {

class GeneticAlgorithm : public GeneticAlgorithm_IF
{
	ProceduralAlgorithm*	_algo;
	AlgorithmConf*	 		_conf;
	StatisticsConf*	 		_stats;
	PopulationConf*			_population;
	GenomeConf* 			_genome;
	GrammarConf*			_grammar;
	MutationConf*			_mutation;
	RecombinationConf* 		_recombination;
	SelectionConf* 			_selection;
	EvaluationConf*			_evaluation;
	GeometryConf*			_geometry;
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
	GeneticAlgorithm(ProceduralAlgorithm* base):
		_algo(base),
		_generation(0),
		_total_time(0.0f)
	{}

	void init(ProceduralAlgorithm* base)
	{
		_conf = _algo->get<AlgorithmConf*>("base");
		_population = _algo->get<PopulationConf*>("population");
		_genome = _algo->get<GenomeConf*>("genome");
		_grammar = _algo->get<GrammarConf*>("grammar");
		_mutation = _algo->get<MutationConf*>("mutation");
		_recombination = _algo->get<RecombinationConf*>("recombination");
		_selection = _algo->get<SelectionConf*>("selection");
		_evaluation = _algo->get<EvaluationConf*>("evaluation");
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
			Genome_IF* new_genome = _genome->factory()->createRandomGenome(*_grammar, _conf->getInitGenomeLength());
			_population->impl()->insertIntoActiveGeneration(new_genome);
		}

		auto t1 = std::chrono::high_resolution_clock::now();

		if (_stats->recordStats()) {
			std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);
			float t = time_span.count();
			_stats->impl()->setReproductionTime(t);
		}

		evaluatePopulation();

		if (_stats->recordStats()) {
			_stats->impl()->storeCurrentStats(_generation);
		}

	}

	void evaluatePopulation()
	{
		bool record_stats = _stats->recordStats();

		float sum_prod_time = 0.0f;
		float sum_calc_time = 0.0f;
		for (int i = 0; i < _population->impl()->activeGenerationSize(); ++i) {
			Genome_IF* genome = _population->impl()->getGenome(i);
			float old_rating = genome->getEvalPoints();
			float rating = _evaluation->impl()->calcRating(genome);
			//	std::cout << "evaluate individual " << i <<": old fitness: " << old_rating <<", new: " << rating << std::endl;
			sum_calc_time += _evaluation->impl()->getLastCalculationTime();
			sum_prod_time += _evaluation->impl()->getLastProductionTime();
			genome->setEvalPoints(rating);
			if (record_stats) {
				_stats->impl()->addFitness(genome->getEvalPoints());
				_stats->impl()->addLength(static_cast<float>(genome->length()));

			}
		}

		_population->impl()->sortActiveGeneration();

		if (record_stats) {
			_stats->impl()->setEvalTime(sum_prod_time, sum_calc_time);
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
		// store the current stats
		// delete the old generation
		_population->impl()->clearNextGeneration();

		_selection->first()->prepareForSelection();
		_selection->second()->prepareForSelection();

		// keep the best population of the current generation
		int elite = _conf->getElitism();
		if (elite > 0) {
			for (int i = 0; i < elite; ++i) {
				Genome_IF* source = _population->impl()->getGenome(i);
			//	std::cout << "Elitism: keep individual with fitness " << source->getEvalPoints() << std::endl;
				Genome_IF* genome = _genome->factory()->createGenome(*_grammar, source);
				genome->setEvalPoints(source->getEvalPoints());
				_population->impl()->insertIntoNextGeneration(genome);
			}
		}
		int accepted_children = elite;

		do {

			// prepare space for the new genomes
			Genome_IF* child1 = _genome->factory()->createEmptyGenome();
			Genome_IF* child2 = _genome->factory()->createEmptyGenome();

			// select the parents from the current generation
			Genome_IF* parent1 = _selection->first()->selectGenome();
			Genome_IF* parent2 = _selection->second()->selectGenome();

			// clone the parents and mutate them
			if (_mutation->impl()->doMutation()) {
				child1->clone(_grammar->getSm(), parent1);
				_mutation->impl()->mutate(*_grammar, child1);

				child2->clone(_grammar->getSm(), parent2);
				_mutation->impl()->mutate(*_grammar, child2);
			}
			// or create children
			else {
				int retries = _recombination->impl()->recombine(parent1, parent2, child1, child2);
				if (record_stats) _stats->impl()->addRetries(retries);
			}

			// avoid creating deformed children
			if (child1->length() >= _genome->getMinimalLength() && child1->length() < _genome->getMaximalLength()) {
				// insert into new generation
				_population->impl()->insertIntoNextGeneration(child1);
				accepted_children ++;

			}
			else {
				delete child1;
			}
			if (child2->length() >= _genome->getMinimalLength() && child2->length() < _genome->getMaximalLength()) {
				_population->impl()->insertIntoNextGeneration(child2);
				accepted_children ++;
			}
			else {
				delete child2;
			}
		} while(accepted_children < _conf->getPopulationSize());


		if (record_stats) {
			auto t1 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);
			float t = time_span.count();

			_stats->impl()->setReproductionTime(t);
		}

		// make the new generation the current
		_population->impl()->switchGeneration();

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

};



}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_GENETICS_GENETICALGORITHM_H_ */
