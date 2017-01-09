/*
 * GeneticAlgorithm.h
 *
 *  Created on: Dec 5, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_METROPOLISHASTINGS_H_
#define GPUPROCGENETICS_SRC_METROPOLISHASTINGS_H_

#include "Allocator.h"

namespace PGA {

class MHAlgorithm : public GeneticAlgorithm_IF
{
	ProceduralAlgorithm*	_algo;
	AlgorithmConf*	 		_conf;
	PopulationConf*			_population;
	GenomeConf* 			_genome;
	GrammarConf*			_grammar;
	MutationConf*			_jump_move;
	MutationConf*			_diffusion_move;
	MutationConf*			_grow_operator;
	EvaluationConf*			_evaluation;
	GeometryConf*			_geometry;
	StatisticsConf*			_stats;
	std::vector<float>		_temperature;

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
	MHAlgorithm(ProceduralAlgorithm* base):
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
		_jump_move = _algo->get<MutationConf*>("jump");
		_diffusion_move = _algo->get<MutationConf*>("diffusion");
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

		float temperature_exp = 1.03f;

		for (int i = 0; i < _conf->getPopulationSize(); ++i) {
			Genome_IF* new_genome = _genome->factory()->createRandomGenome(*_grammar, _conf->getInitGenomeLength());
			new_genome->setTemperature(1.0f/math::pow(1.0f * (i+1), temperature_exp));

			float rating = _evaluation->impl()->calcRating(new_genome);

			new_genome->setEvalPoints(rating);
			new_genome->setLogLikelihood(rating <= 0.0f ? math::log(0.001f) : math::log(rating));
			new_genome->setLogPrior(1.0f);

			_population->impl()->insertIntoActiveGeneration(new_genome);
			if (_stats->recordStats()) {
				_stats->impl()->addFitness(new_genome->getEvalPoints());
				_stats->impl()->addLength(static_cast<float>(new_genome->length()));

			}
		}

		auto t1 = std::chrono::high_resolution_clock::now();

		if (_stats->recordStats()) {
			std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);
			float t = time_span.count();
			_stats->impl()->setReproductionTime(t);
			_stats->impl()->storeCurrentStats(_generation);
		}



	}

	void createNewGeneration()
	{
		// re-initialize the random generator. Doing this every generation might be to often though
		// since the internet told me the mersenne twister might perform poorly in the beginning if initialized with too small values
		reinitMtGenerator();

		bool record_stats = _stats->recordStats();

		_conf->setCurrentGeneration(_generation);

		auto t0 = std::chrono::high_resolution_clock::now();

		if (record_stats) {
			_stats->impl()->setCurrentGeneration(_generation);
		}
		// store the current stats
		// delete the old generation
		_population->impl()->clearNextGeneration();


		for (int i = 0; i < _conf->getPopulationSize(); ++i) {
			bool print = false;

			// prepare space for the next genome
			Genome_IF* next_step = _genome->factory()->createEmptyGenome();

			// get the current genome
			Genome_IF* current_step = _population->impl()->getGenome(i);

			// clone the current genome
			next_step->clone(_grammar->getSm(), current_step);

			// copy the temperature
			next_step->setTemperature(current_step->getTemperature());


			float acceptance_prob = 0.0f;
			// do a jump move: change the structure of the derivation tree
			if (_jump_move->impl()->doMutation()) {

				// select a gene to modify, based on the depth of the tree
				Gene_IF* gene = next_step->selectSemiRandomGene();

				// calc the log-prob for selecting that gene
				float old_prior = next_step->getSelectionLogProb(gene->level());

				// create a new tree structure starting at the selected gene
				_jump_move->impl()->do_move(*_grammar, next_step, gene);

//				if(next_step->length() < 15)
//					print = true;

				// calc the log-prob for selecting the same gene in the new tree
				float new_prior = next_step->getSelectionLogProb(gene->level());

				float new_rating = _evaluation->impl()->calcRating(next_step);
				float old_rating = current_step->getEvalPoints();

				next_step->setEvalPoints(new_rating);
				next_step->setLogPrior(new_prior - old_prior);
				if (new_rating <= 0 || old_rating <= 0)
				{
					float mr = -std::min(old_rating, new_rating);
					new_rating += mr + 0.01f;
					old_rating += mr + 0.01f;
				}
				acceptance_prob = math::exp(current_step->getTemperature() * ((new_prior - old_prior) + (math::log(new_rating) - math::log(old_rating))));
				if (print)
					std::cout << "["<< i << "]: jump: T = " << current_step->getTemperature() <<", old prior = " << old_prior << ", new prior = " << new_prior << ", old rating = " << old_rating << ", new = " << new_rating << std::endl;
			}
			else {
				// do a diffusion move: reinitalize the parameter values of a single symbol.

				// select a gene to modify, based on the depth of the tree
				Gene_IF* gene = next_step->selectSemiRandomGene();

				// calc the log-prob for selecting that gene (not used for calculation, but required for parallel tempering
				float prior = next_step->getSelectionLogProb(gene->level());
				next_step->setLogPrior(prior - current_step->getLogPrior());

				_diffusion_move->impl()->do_move(*_grammar, next_step, gene);

//				if(next_step->length() < 15)
//					print = true;

				float new_rating = _evaluation->impl()->calcRating(next_step);
				float old_rating = current_step->getEvalPoints();
				// TODO: do i have to calculate this differently?
				next_step->setEvalPoints(new_rating);

				if (new_rating <= 0 || old_rating <= 0)
				{
					float mr = -std::min(old_rating, new_rating);
					new_rating += mr + 0.01f;
					old_rating += mr + 0.01f;
				}

				acceptance_prob = math::exp(current_step->getTemperature() * (math::log(new_rating) - math::log(old_rating)));
				if (print)
					std::cout << "[" << i << "]: diff: T = " << current_step->getTemperature() <<", new rating = " << new_rating << ", old = " << old_rating << std::endl;
			}

			if (acceptance_prob >= getRandomValue01()) {
				if (print)
					std::cout << "accept with prob " << acceptance_prob << std::endl;
				_population->impl()->replaceAt(next_step, i);
				_stats->impl()->addFitness(next_step->getEvalPoints());

				// the temperature is used as exponent, and the acceptance probability is (usually) between 0 and 1,
				// so i have to increase the exponent to reduce the acceptance prob.
				float annealing = 1.1f;
				next_step->setTemperature(next_step->getTemperature() * annealing);
			}
			else {
				_stats->impl()->addFitness(current_step->getEvalPoints());
				if (print)
					std::cout << "reject with prob " << acceptance_prob << std::endl;
				// todo: initiate a diffusion move for the modified child and try again
				delete next_step;

				float annealing = 1.1f;
				current_step->setTemperature(current_step->getTemperature() * annealing);

			}

		}

		// do some parallel tempering
		int index_1 = getRandomValue(0, _conf->getPopulationSize());
		int index_2 = getRandomValue(0, _conf->getPopulationSize());
		if (index_1 == index_2) {
			index_1 = getRandomValue(0, _conf->getPopulationSize());
		}

		if (index_1 != index_2) {

			Genome_IF* genome_1 = _population->impl()->getGenome(index_1);
			Genome_IF* genome_2 = _population->impl()->getGenome(index_2);

			// TODO: i don't think this works properly at all
			float g1_prior = genome_1->getLogPrior();
			float g2_prior = genome_2->getLogPrior();

			float rating1 = genome_1->getEvalPoints();
			float rating2 = genome_2->getEvalPoints();

			if (rating1 <= 0 || rating2 <= 0)
			{
				float mr = -std::min(rating1, rating2);
				rating1 += mr + 0.01f;
				rating2 += mr + 0.01f;
			}

			float p_11 = math::exp(genome_1->getTemperature() * (math::log(rating1) + g1_prior));
			float p_22 = math::exp(genome_2->getTemperature() * (math::log(rating2) + g2_prior));
			float p_12 = math::exp(genome_1->getTemperature() * (math::log(rating2) + g2_prior));
			float p_21 = math::exp(genome_2->getTemperature() * (math::log(rating1) + g1_prior));


			float accept_swap_prob = ((p_12 * p_21) / (p_11 * p_22));
			if (accept_swap_prob >= getRandomValue01()) {
//				if (index_1 == 0 || index_2 == 0)
//					std::cout << "swap index " << index_1 << " with " << index_2 << " with prob " << accept_swap_prob << std::endl;

				float temp1 = genome_1->getTemperature();
				float temp2 = genome_2->getTemperature();
				genome_1->setTemperature(temp2);
				genome_2->setTemperature(temp1);
				_population->impl()->swap(index_1, index_2);

			}

		}
		if (record_stats) {
			auto t1 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);
			float t = time_span.count();

			Genome_IF* best = _population->impl()->getBestGenome();
			_stats->impl()->addFitness(best->getEvalPoints());

			_stats->impl()->setReproductionTime(t);
			_stats->impl()->storeCurrentStats(_generation);
		}
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
					std::cerr << "Generation " << _generation << ", best points " << _population->impl()->getBestGenome()->getEvalPoints() << " time: " << _total_time <<  std::endl;
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



#endif /* GPUPROCGENETICS_SRC_METROPOLISHASTINGS_H_ */
