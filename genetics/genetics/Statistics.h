/*
 * Statistics.h
 *
 *  Created on: Dec 9, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_GENETICS_STATISTICS_H_
#define GPUPROCGENETICS_SRC_GENETICS_STATISTICS_H_

#include <sstream>
#include <map>
#include <algorithm>
#include "Statistics_IF.h"
#include "GeneticAlgoConfig.h"


namespace PGA {

class ProceduralGeneticAlgorithm;
class PopulationConf;
class GrammarConf;
class GenomeConf;

class Statistics: public Statistics_IF {
private:

	ProceduralAlgorithm* _algo;
	AlgorithmConf*		_base;
	GeometryConf*		_geometry;
	StatisticsConf*		_conf;
	PopulationConf*		_population;
	GrammarConf*		_grammar;

	class StatisticValue {
	private:
		bool 	_init = false;
		float 	_min;
		float 	_max;
		float 	_avg;
	public:
		StatisticValue() :
		_init(false),
		_min(0.0f),
		_max(0.0f),
		_avg(0.0f)
		{}

		void reset() {
			_init = false;
		}

		float min() { return _min; }
		float max() { return _max; }
		float avg() { return _avg; }

		void addValue(float val) {
			if (!_init) {
				_min = val;
				_max = val;
				_avg = val;
				_init = true;
			}
			else {
				_min = std::min(val, _min);
				_max = std::max(val, _max);
				_avg = (_avg + val) / 2.0f;
			}
		}
	};

	struct GenerationStats {
		StatisticValue min_fitness;
		StatisticValue max_fitness;
		StatisticValue avg_fitness;
		StatisticValue eval_production_time;
		StatisticValue eval_calculation_time;
		StatisticValue reproduction_time;
		StatisticValue retries;
		StatisticValue genome_length;
		int			   generation;
	};

	::std::map<int, GenerationStats> 	_stats;
	StatisticValue						_current_fitness;
	StatisticValue 						_retries;
	float								_eval_production_time;
	float								_eval_calculation_time;
	float								_reproduction_time;
	int									_current_generation;
	int									_runs;
	::std::ostream* 					_out;


	GenerationStats						_current;

	void writeSimpleStatistics(::std::ostream& out);

public:
	Statistics(ProceduralAlgorithm* base);
	~Statistics();

	void init(ProceduralAlgorithm* base);
	void finishRun();
	void addRetries(const int& retries);
	void addFitness(const float& fitness);
	void addLength(const float& length);
	void setEvalTime(const float& production_time, const float& calc_time);
	void setReproductionTime(const float& time);

	void clearStats();
	void setCurrentGeneration(const int& generation);
	void storeCurrentStats(const int& generation);
	void writeToFile(const int& run);

	void setOutStream(::std::ostream& out);
	void writeHeader(bool cvs, bool include_config);
	void writeStatistics(bool cvs);

	void saveBestObj(const int& generation, const int& run, const bool& final_generation);

};


}  // namespace PGA




#endif /* GPUPROCGENETICS_SRC_GENETICS_STATISTICS_H_ */
