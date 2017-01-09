/*
 * Population.h
 *
 *  Created on: Dec 4, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_GENETICS_POPULATION_H_
#define GPUPROCGENETICS_SRC_GENETICS_POPULATION_H_

namespace PGA {

class Population : public Population_IF
{
private:
	bool						_use_population1;
	std::vector<Genome_IF*>		_population1;
	std::vector<Genome_IF*>		_population2;
	float						_sum_eval_points;
	ProceduralAlgorithm*		_algo;
	GenomeConf*					_genome;
	PopulationConf* 			_conf;
	GrammarConf*				_grammar;
	GeometryConf*				_geometry;
	Genome_IF*					_absolute_best;

	std::vector<Genome_IF*>& getActivePopulation() {
		if (_use_population1) {
			return _population1;
		}
		else {
			return _population2;
		}
	}

	std::vector<Genome_IF*>& getInactivePopulation() {
		if (_use_population1) {
			return _population2;
		}
		else {
			return _population1;
		}
	}

	void update_absolute_best(Genome_IF* genome)
	{
		if (_absolute_best == nullptr) {
			_absolute_best = _genome->factory()->createGenome(*_grammar, genome);
			_absolute_best->setEvalPoints(genome->getEvalPoints());
		}
		if (_absolute_best->getEvalPoints() < genome->getEvalPoints()) {
			delete _absolute_best;
			_absolute_best = _genome->factory()->createGenome(*_grammar, genome);
			_absolute_best->setEvalPoints(genome->getEvalPoints());
		}
		return;
	}

public:
	Population(ProceduralAlgorithm* base):
		_use_population1(true),
		_sum_eval_points(0.0f),
		_algo(base),
		_absolute_best(nullptr)
	{}

	~Population() {
		clearCurrentGeneration();
		clearNextGeneration();
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


	void insertIntoNextGeneration(Genome_IF* genome)
	{
		getInactivePopulation().push_back(genome);
	}

	void insertIntoActiveGeneration(Genome_IF* genome)
	{
		getActivePopulation().push_back(genome);
	}

	void switchGeneration()
	{
		_use_population1 = !_use_population1;
	}
	int activeGenerationSize() {
		return getActivePopulation().size();
	}

	int nextGenerationSize() {
		return getInactivePopulation().size();
	}

	void clearCurrentGeneration()
	{
		for (Genome_IF* g : getActivePopulation()) {
			delete g;
		}
		getActivePopulation().clear();
	}

	void clearNextGeneration()
	{
		for (Genome_IF* g : getInactivePopulation()) {
			delete g;
		}
		getInactivePopulation().clear();
	}

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

		update_absolute_best(getActivePopulation()[0]);
	}

	// get the absolute best genome of all runs
	Genome_IF* getAbsoluteBestGenome()
	{
		return _absolute_best;
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

	void replaceAt(Genome_IF* genome, int index)
	{

		Genome_IF* old = getActivePopulation()[index];
		if (old != nullptr) {
			delete old;
		}
		getActivePopulation()[index] = genome;

	}

	Genome_IF* getBestGenome()
	{
		return getActivePopulation()[0];
	}


	void swap(int index1, int index2)
	{
		Genome_IF* tmp = getActivePopulation()[index1];
		getActivePopulation()[index1] = getActivePopulation()[index2];
		getActivePopulation()[index2] = tmp;
	}


};



}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_GENETICS_POPULATION_H_ */
