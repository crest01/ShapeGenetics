/*
 * GenomeCPU.h
 *
 *  Created on: Nov 12, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_GENETICS_GENOMECPU_H_
#define GPUPROCGENETICS_SRC_GENETICS_GENOMECPU_H_

#include <stack>

#include "Genome_IF.h"

namespace PGA {

class SymbolManager;

struct Gene_CPU: public Gene_IF {
private:
	Genome_IF*				_genome;
	int						_parent_ref;
	int						_level;
	int 					_type;
	int						_ref;
	int						_paramtable_offset;
	std::vector<int>		_children;
	std::vector<Parameter>	_params;

public:
	Gene_CPU(Genome_IF* genome, const int type, const int ref): _genome(genome), _parent_ref(-1), _level(0), _type(type), _ref(ref), _paramtable_offset(0)
	{}

	~Gene_CPU()
	{
		for (Parameter p: _params) {
			delete[] p.data;
		}
	}

	void setParent(const int ref, const int parent_level) {
		_parent_ref = ref;
		_level = parent_level + 1;
	}

	unsigned int numParameter() const {
		return _params.size();
	}

	Parameter* createParameter(const unsigned int size)
	{
		Parameter param;
		param.size = size;
		param.data = new unsigned char[size];
		_params.push_back(param);
		return &_params.back();
	}

	void addChild(const int ref)
	{
		_children.push_back(ref);
	}

	unsigned int numChildren() const
	{
		return _children.size();
	}
	Gene_IF* getChild(const unsigned int index)
	{
		return _genome->getGene(getChildRef(index));
	}

	const int getChildRef(const unsigned int index)
	{
		return _children[index];
	}

	void deleteChildRef(const unsigned int ref) {
		_children.erase(std::remove(_children.begin(), _children.end(), ref), _children.end());
	}

	// Gene_IF - hooks
	unsigned int numParameter() { return _params.size(); };
	Parameter* getParameter(const unsigned int index)	{ return &_params[index]; }
	int ref() { return _ref; }
	int	type() { return _type; }
	int parentRef() { return _parent_ref; }
	Gene_IF* getParent() const { return _genome->getGene(_parent_ref); }
	int level() { return _level; };
	void setParameterOffset( const unsigned int offset) { _paramtable_offset = offset; }
	unsigned int getParameterOffset() { return _paramtable_offset; }


};

class Genome_CPU : public Genome_IF {
private:
	std::map<int, Gene_CPU*>			_data;
	std::map<int, Gene_CPU*>::iterator	_it;
	unsigned int						_last_ref;
	int									_max_level;

	float								_eval_points;
	float								_temperature;
	float								_logLikelihood;
	float								_logPrior;

	void clearGenome()
	{
		_data.clear();
	}

public:

	Genome_CPU():_last_ref(0), _max_level(0), _eval_points(0), _temperature(0) {}

	~Genome_CPU()
	{
		std::map<int, Gene_CPU*>::const_iterator it = _data.begin();
		for (; it != _data.end(); ++it) {
			delete it->second;
		}
		_data.clear();
	}

	void setTemperature(const float temperature)
	{
		_temperature = temperature;
	}
	float getTemperature()
	{
		return _temperature;
	}

	void setLogLikelihood(const float logLikelihood)
	{
		_logLikelihood = logLikelihood;
	}
	float getLogLikelihood()
	{
		return _logLikelihood;
	}


	// set and get the log likelihood of the previous modification of the genome (for MH)
	void setLogPrior(const float logPrior)
	{
		_logPrior = logPrior;
	}

	float getLogPrior()
	{
		return _logPrior;
	}


	void createFromString(const SymbolManager& sm, const std::string& string)
	{
		std::stack<Gene_IF*> parents;

		Gene_IF* gene;

		for (int idx = 0; idx < string.length(); ++idx) {
			SymbolDescriptor* sd = sm.symbol(string.substr(idx, 1));
			if (sm.isOpenBracket(sd->id())) {
				parents.push(gene);
				continue;
			}
			if (sm.isCloseBracket(sd->id())) {
				parents.pop();
				continue;
			}

			if (sm.isStart(sd->id())) {
				gene = createNewGene(nullptr, sd);
			}
			else {
				gene = createNewGene(parents.top(), sd);
			}
		}
	}

	void insertGeneRecursive(Gene_IF* parent, Gene_IF* other_gene)
	{
		// insert the gene and copy all parameters
		Gene_IF* next_parent = insertGene(parent, other_gene);

		for (int i = 0; i < other_gene->numChildren(); ++i) {
			Gene_IF* next_gene = other_gene->getChild(i);
			insertGeneRecursive(next_parent, next_gene);
		}
	}

	void clone(const SymbolManager& sm, Genome_IF* other)
	{
		Gene_IF* other_start_gene = other->getStartGene(sm);

		Gene_IF* start_gene = createNewGene(nullptr, sm.getStartSymbol());

		for (int i = 0; i < other_start_gene->numChildren(); ++i) {
			Gene_IF* other_gene = other_start_gene->getChild(i);
			insertGeneRecursive(start_gene, other_gene);
		}

		if (other->length() != this->length()) {
			std::cout << "cloning failed? from " << other->length() << " genes to " << this->length() << std::endl;
		}

	}

	void deleteGene(const unsigned int ref, bool firstCall)
	{
		if (_data.find(ref) == _data.end()) {
			std::cout << "Genome: Gene with ref " << ref << "doesn't exist" << std::endl;
			throw(std::runtime_error("Genome.deleteGene()"));
			return;
		}
		Gene_CPU* gene = _data[ref];
		for (int i = 0; i < gene->numChildren(); ++i) {
			// delete gene, but don't touch the parent (i.e. the current gene)
			deleteGene(gene->getChildRef(i), false);
		}

		// also remove gene from Parent-Gene
		if (firstCall)  {
			if (gene->parentRef() >= 0) {
				Gene_CPU* parent = _data[gene->parentRef()];
				parent->deleteChildRef(gene->ref());
			}
		}
		delete gene;
		_data.erase(ref);
	}

	// remove a gene and all it's children from the array
	void deleteGene(const unsigned int ref)
	{
		deleteGene(ref, true);
	}

	unsigned int getLastRef()
	{
		return _last_ref;
	}

	// return gene with the given reference
	Gene_IF* getGene(const unsigned int ref) {
		return _data[ref];
	}

	Gene_IF* getStartGene(const SymbolManager& sm) {
		std::map<int, Gene_CPU*>::const_iterator it = _data.begin();
		for (; it != _data.end(); ++it) {
			if(sm.isStart(it->second->type())) {
				return it->second;
			}
		}
		return nullptr;
	}

	Gene_IF* getRandomGene()
	{
		int value = getRandomValue(1, length());
		std::map<int, Gene_CPU*>::const_iterator it = _data.begin();
		for (; value > 0; --value, ++it) {}
		return it->second;
	}

	Gene_IF* getRandomGeneWithType(const int type)
	{
		std::vector<int> ref_list;
		std::map<int, Gene_CPU*>::const_iterator it = _data.begin();
		for (; it != _data.end(); ++it) {
			if(it->second->type() == type) {
				ref_list.push_back(it->second->ref());
			}
		}
		int value = getRandomValue(0, ref_list.size()-1);
		return _data[ref_list[value]];
	}


	Gene_IF* selectSemiRandomGene()
	{
		int level = getRandomValue(1, _max_level);

		std::vector<Gene_CPU*> genes_in_level;

		std::map<int, Gene_CPU*>::iterator it_gene = _data.begin();
		for (; it_gene != _data.end(); it_gene ++) {
			if ((it_gene->second)->level() == level) {
				genes_in_level.push_back(it_gene->second);
			}
		}
		if (genes_in_level.size() == 1) {
			return genes_in_level[0];
		}
		else {
			int index = getRandomValue(0, genes_in_level.size());
			return genes_in_level[index];
		}
	}

	float getSelectionLogProb(int level)
	{

		int genes_in_level = 0;

		std::map<int, Gene_CPU*>::iterator it_gene = _data.begin();
		for (; it_gene != _data.end(); it_gene ++) {
			if ((it_gene->second)->level() == level) {
				genes_in_level ++;
			}
		}
		// this can happen if the genome got smaller
		if (genes_in_level == 0) {
			return math::log((float)level/(float)_max_level);
		}

		return math::log(((float)level/(float)_max_level)) + math::log(1.0f/(float)genes_in_level);

	}


	unsigned int getNumChildren(const unsigned int parent_ref) {
		return _data[parent_ref]->numChildren();
	}

	// return the n'th child of a gene
	Gene_IF* getChildOfGene(const unsigned int parent_ref, const unsigned int child_index) {

		int child_ref = _data[parent_ref]->getChildRef(child_index);
		std::map<int, Gene_CPU*>::iterator	it = _data.find(child_ref);
		if (it != _data.end()) {
			return it->second;
		}
		else {
			std::cout << "Genome: parent (ref " << parent_ref << ") has no child at index "<< child_index <<" (ref " << child_ref << ")" << std::endl;
			throw(std::runtime_error("Genome.getChildOfGene()"));
			return nullptr;
		}
	}


	int maxLevel() const { return _max_level; }

	int length() const { return _data.size(); }

	Gene_IF* insertGene(Gene_IF *parent, Gene_IF* orig)
	{
		Gene_CPU* gene = new Gene_CPU(this, orig->type(), ++_last_ref);
		if (parent) {
			gene->setParent(parent->ref(), parent->level());
		}

		for (int i = 0; i < orig->numParameter(); ++i) {
			Parameter* p = gene->createParameter(orig->getParameter(i)->size);
			memcpy(p->data, orig->getParameter(i)->data, orig->getParameter(i)->size);
		}
		_data[gene->ref()] = gene;

		Gene_CPU* cpu_parent = dynamic_cast<Gene_CPU*>(parent);
		cpu_parent->addChild(gene->ref());

		if (gene->level() > _max_level) {
			_max_level = gene->level();
		}

		return gene;
	}

	Gene_IF* createNewGene(Gene_IF *parent, SymbolDescriptor* sd)
	{
		Gene_CPU* gene = new Gene_CPU(this, sd->id(), ++_last_ref);
		if (parent) {
			gene->setParent(parent->ref(), parent->level());
			Gene_CPU* cpu_parent = dynamic_cast<Gene_CPU*>(parent);
			cpu_parent->addChild(gene->ref());
		}

		for (int i = 0; i < sd->numParams(); ++i) {
			ParameterDescriptor_IF *pd = sd->getParam(i);
			Parameter* p = gene->createParameter(pd->typeSize());
			pd->generateRandomValue(p);
		}
		_data[gene->ref()] = gene;

		if (gene->level() > _max_level) {
			_max_level = gene->level();
		}

		return gene;
	}

	virtual void resetIterator()
	{
		_it = _data.begin();
	}

	virtual Gene_IF* nextGene()
	{
		if (_it != _data.end()) {
			Gene_CPU* gene = _it->second;
			_it ++;
			return gene;
		}
		else {
			return nullptr;
		}
	}

	void setEvalPoints(const float points) {
		_eval_points = points;
	}

	float getEvalPoints() const  {
		return _eval_points;
	}

	void print() {
		std::map<int, Gene_CPU*>::const_iterator it = _data.begin();
		for (; it != _data.end(); ++it) {
			std::cout << "Ref " << it->second->ref() << ":" << std::endl;
			std::cout << "   Type  :" << it->second->type() << std::endl;
			std::cout << "   Childs:" << it->second->numChildren() << std::endl;
			std::cout << "   Parent:" << it->second->parentRef() << std::endl;
		}
	}


};

class Genome_Factory: public Genome_Factory_IF {
private:
	ProceduralAlgorithm* _algo;
	MutationConf* _mutation;
public:
	Genome_Factory(ProceduralAlgorithm* base):
		_algo(base)
	{};
	void init(ProceduralAlgorithm* base)
	{
		_mutation = _algo->get<MutationConf*>("init_mutation");
	}
	Genome_IF* createEmptyGenome()
	{
		return new Genome_CPU();
	}
	Genome_IF* createRandomGenome(GrammarConf& grammar, const int length)
	{
		Genome_CPU* genome = new Genome_CPU();
		genome->createNewGene(nullptr, grammar.getSm().getStartSymbol());
		for (int i = 1; i < length; ++i) {
			_mutation->impl()->grow(grammar, genome);
		}
		return genome;
	}

	Genome_IF* createGenomeFromString(GrammarConf& grammar, const std::string& string)
	{
		Genome_CPU* genome = new Genome_CPU();
		genome->createFromString(grammar.getSm(), string);
		return genome;
	}

	Genome_IF* createGenome(GrammarConf& grammar, Genome_IF* other)
	{
		Genome_CPU* genome = new Genome_CPU();
		genome->clone(grammar.getSm(), other);
		return genome;
	}


};




} // namespace PGA




#endif /* GPUPROCGENETICS_SRC_GENETICS_GENOMECPU_H_ */
