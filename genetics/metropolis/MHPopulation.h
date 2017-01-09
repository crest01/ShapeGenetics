/*
 * Population.h
 *
 *  Created on: Dec 4, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_MHPOPULATION_H_
#define GPUPROCGENETICS_SRC_MHPOPULATION_H_

namespace PGA {

class MHPopulation : public Population_IF
{
private:
	std::vector<Genome_IF*>		_population;
	ProceduralAlgorithm*		_algo;
	PopulationConf* 			_conf;
	GrammarConf*				_grammar;
	GeometryConf*				_geometry;
	GenomeConf*					_genome;
	Genome_IF*					_best;
	Genome_IF*					_absolute_best;
	float						_sum_eval_points;

	std::vector<Genome_IF*>& getActivePopulation() {
		return _population;
	}

	std::vector<Genome_IF*>& getInactivePopulation() {
		throw std::runtime_error("No inactive Population");
	}


public:
	MHPopulation(ProceduralAlgorithm* base):
		_algo(base),
		_sum_eval_points(0.0f),
		_best(nullptr),
		_absolute_best(nullptr)
	{}

	~MHPopulation()
	{
		for (int i = 0; i < _population.size(); ++i) {
			delete _population[i];
		}
		_population.clear();
	}

	void generateGeometry(const int index)
	{
		if (getActivePopulation().empty()) {
			return;
		}

		_grammar->impl()->createGeometry(getActivePopulation()[index]);

		std::cerr << "Display Genome " << index << " (" << getActivePopulation()[index]->getEvalPoints() << " Points) of " << getActivePopulation().size() << std::endl;
	}


	void init(ProceduralAlgorithm* base)
	{
		_conf = _algo->get<PopulationConf*>("population");
		_grammar = _algo->get<GrammarConf*>("grammar");
		_geometry = _algo->get<GeometryConf*>("geometry");
		_genome = _algo->get<GenomeConf*>("genome");
	}

	Genome_IF* getGenome(const int index)
	{
		return getActivePopulation()[index];
	}

	Genome_IF* getBestGenome()
	{
		return _best;
	}


	void check_best(Genome_IF* genome)
	{
		if (_best != nullptr && _best->getEvalPoints() < genome->getEvalPoints()) {
			delete _best;
			_best = _genome->factory()->createEmptyGenome();
			_best->clone(_grammar->getSm(), genome);
			_best->setEvalPoints(genome->getEvalPoints());
		}
		else if (_best == nullptr) {
			_best = _genome->factory()->createEmptyGenome();
			_best->clone(_grammar->getSm(), genome);
			_best->setEvalPoints(genome->getEvalPoints());
		}
		check_absolute_best(genome);
	}

	void check_absolute_best(Genome_IF* genome)
	{
		if (_absolute_best != nullptr && _absolute_best->getEvalPoints() < genome->getEvalPoints()) {
			delete _absolute_best;
			_absolute_best = _genome->factory()->createEmptyGenome();
			_absolute_best->clone(_grammar->getSm(), genome);
			_absolute_best->setEvalPoints(genome->getEvalPoints());
		}
		else if (_absolute_best == nullptr) {
			_absolute_best = _genome->factory()->createEmptyGenome();
			_absolute_best->clone(_grammar->getSm(), genome);
			_absolute_best->setEvalPoints(genome->getEvalPoints());
		}
	}

	// get the absolute best genome of all runs
	Genome_IF* getAbsoluteBestGenome()
	{
		return _absolute_best;
	}


	void replaceAt(Genome_IF* genome, int index)
	{

		check_best(genome);

		Genome_IF* old = _population[index];
		if (old != nullptr) {
			delete old;
		}
		_population[index] = genome;

	}

	void swap(int index1, int index2)
	{
		Genome_IF* tmp = _population[index1];
		_population[index1] = _population[index2];
		_population[index2] = tmp;
	}

	void insertIntoNextGeneration(Genome_IF* genome)
	{}

	void insertIntoActiveGeneration(Genome_IF* genome)
	{
		check_best(genome);
		_population.push_back(genome);
	}

	void switchGeneration()
	{}

	int activeGenerationSize() {
		return _population.size();
	}

	int nextGenerationSize() {
		return 0;
	}

	void clearCurrentGeneration()
	{
		for (Genome_IF* g : getActivePopulation()) {
			delete g;
		}
		getActivePopulation().clear();
		delete _best;
		_best = nullptr;
	}

	void clearNextGeneration()
	{}

	void sortActiveGeneration()
	{
	    // sort using a custom function object
	    struct {
	        bool operator()(Genome_IF* a, Genome_IF* b)
	        {
	            return a->getEvalPoints() > b->getEvalPoints();
	        }
	    } customLess;

		std::sort(getActivePopulation().begin(), getActivePopulation().end(), customLess);

	}

	float getSumEvalPoints() {
		return _sum_eval_points;
	}

	void addEvalPoints(const float points) {
		_sum_eval_points += points;
	}

	void resetSumEvalPoints() {
		_sum_eval_points = 0.0f;
	}


};



}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_MHPOPULATION_H_ */
