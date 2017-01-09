/*
 * Selection.h
 *
 *  Created on: Dec 5, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_GENETICS_SELECTION_H_
#define GPUPROCGENETICS_SRC_GENETICS_SELECTION_H_

namespace PGA {

class RandomSelector : public Selection_IF
{
private:
	ProceduralAlgorithm* _algo;
	PopulationConf* _population;
public:
	RandomSelector(ProceduralAlgorithm* base):
		_algo(base)
	{}

	void init(ProceduralAlgorithm* base)
	{
		_population = _algo->get<PopulationConf*>("population");
	}

	const std::string getName() { return "Random"; }

	virtual void prepareForSelection() {}

	Genome_IF* selectGenome()
	{
		int index = getRandomValue(0, _population->impl()->activeGenerationSize());
		return _population->impl()->getGenome(index);
	}

};

class TournamentSelector : public Selection_IF
{
private:
	ProceduralAlgorithm* _algo;
	PopulationConf* _population;
	SelectionConf*  _conf;
public:
	TournamentSelector(ProceduralAlgorithm* base):
	_algo(base)
	{}

	const std::string getName() { return "Tournament"; }

	void prepareForSelection() {}

	void init(ProceduralAlgorithm* base)
	{
		_population = _algo->get<PopulationConf*>("population");
		_conf = _algo->get<SelectionConf*>("selection");
	}

	Genome_IF* selectGenome()
	{
		int index = getRandomValue(0, _population->impl()->activeGenerationSize() - 1);

		Genome_IF* candidate = _population->impl()->getGenome(index);

		for (int i = 1; i < _conf->getTournamentSize(); ++i) {
			index = getRandomValue(0, _population->impl()->activeGenerationSize() - 1);
			Genome_IF* other = _population->impl()->getGenome(index);
			if (other->getEvalPoints() > candidate->getEvalPoints()) {
				candidate = other;
			}
		}
		return candidate;
	}

};


class RouletteWheelSelector: public Selection_IF
{

private:
	ProceduralAlgorithm* _algo;
	PopulationConf* _population;
	SelectionConf*  _conf;
	float			_fitness_offset;
	float			_sum_fitness;
public:
	RouletteWheelSelector(ProceduralAlgorithm* base):
		_algo(base)
	{}

	const std::string getName() { return "Roulette"; }

	void prepareForSelection()
	{
		// get the negative fitness of the worst individual
		_fitness_offset = _population->impl()->getGenome(_population->impl()->activeGenerationSize() -1)->getEvalPoints() * -1;

		_sum_fitness = 0.0f;
		for (int i = 0; i < _population->impl()->activeGenerationSize(); ++i) {
			_sum_fitness += (_fitness_offset + _population->impl()->getGenome(i)->getEvalPoints());
		}
	}

	void init(ProceduralAlgorithm* base)
	{
		_population = _algo->get<PopulationConf*>("population");
		_conf = _algo->get<SelectionConf*>("selection");
	}

	Genome_IF* selectGenome()
	{
		float random_val = getRandomValue(0.0f, 1.0f);
		float relative_fitness = 0.0f;

		for (int i = 0; i < _population->impl()->activeGenerationSize(); ++i) {
			Genome_IF* candidate = _population->impl()->getGenome(i);
			float current_fitness = (candidate->getEvalPoints() + _fitness_offset);
			if (current_fitness > 0.0f) {
				relative_fitness += current_fitness/_sum_fitness;
				if (random_val <= relative_fitness) {
					return candidate;
				}
			}
		}
		// the best is always a save choice
		return _population->impl()->getBestGenome();
	}


};



}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_GENETICS_SELECTION_H_ */
