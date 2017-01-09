/*
 * Statistics_IF.h
 *
 *  Created on: Dec 12, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_INTERFACES_STATISTICS_IF_H_
#define GPUPROCGENETICS_SRC_INTERFACES_STATISTICS_IF_H_

namespace PGA {

/*
 * collect some statistics and write them somewhere after each generation.
 * Collects min/max/average fitness, reproduction time and evaluation time for each generation, and
 * also collects min/max/average of all these values for multiple runs
 */

class ProceduralAlgorithm;

class Statistics_IF {
public:

	virtual void finishRun() = 0;

	virtual void addRetries(const int& retries) = 0;


	// add a fitness value for a single individual and calc the min/max/average of all given values of the current generation
	virtual void addFitness(const float& rating) = 0;

	virtual void addLength(const float& length) = 0;

	// out-stream for the output
	virtual void setOutStream(std::ostream& out) = 0;

	// write the configuration header to the out-stream
	virtual void writeHeader(bool cvs, bool include_config) = 0;

	// write the collected statistics for all generations
	virtual void writeStatistics(bool cvs) = 0;

	// add the fitness calculation time for the current generation
	virtual void setEvalTime(const float& production_time, const float& calculation_time) = 0;

	// add the reproduction time for the current generation
	virtual void setReproductionTime(const float& time) = 0;

	// delete all collected values
	virtual void clearStats() = 0;

	// set the current generation, init / reuse the values from previous runs
	virtual void setCurrentGeneration(const int& generation) = 0;

	// save the min/avg/max fitness values for the current generation and store all statistics
	virtual void storeCurrentStats(const int& generation) = 0;

	virtual void saveBestObj(const int& generation, const int& run, const bool& final_generation) = 0;

	virtual void init(ProceduralAlgorithm* base) = 0;

	virtual void writeToFile(const int& run) = 0;

};



}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_INTERFACES_STATISTICS_IF_H_ */
