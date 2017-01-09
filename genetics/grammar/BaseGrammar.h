/*
 * BaseGrammar.h
 *
 *  Created on: Dec 5, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_GRAMMAR_RULES_BASEGRAMMAR_H_
#define GPUPROCGENETICS_SRC_GRAMMAR_RULES_BASEGRAMMAR_H_

#include "ParseTree.h"
#include "GeneticAlgoConfig.h"
#include "Grammar_IF.h"
#include "GeometryBufferInstanced.h"
#include "GeometryConfigSingleton.h"

namespace PGA {

class BaseGrammar : public Grammar_IF
{
protected:
	ProceduralAlgorithm* _algo;
	GeometryConf*	_geometry_conf;
	GrammarConf*	_conf;
	int				_num_prepared_shapes;

	virtual void initSymbols(SymbolManager& sm) = 0;
	virtual int storeParameter(Symbol* symbol, PGG::Parameters::ParameterTable& pt) = 0;
	virtual void createAxiom(PGG::CPU::GrammarSystem& system, const int axiomId) = 0;


public:
	BaseGrammar(ProceduralAlgorithm* algo, GrammarConf* grammar):
		_algo(algo),
		_conf(grammar)
	{}
	void init(ProceduralAlgorithm* base)
	{
		_geometry_conf = _algo->get<GeometryConf*>("geometry");
		prepareShapes();
		initSymbols(_conf->getSm());
		_conf->getPt().clearParameters();
		_conf->getSystem().registerParameterTable(_conf->getPt());
	}

	virtual int fillParameterTable(Genome_IF* genome)
	{
		ParseTree tree(_conf->getSm(), genome);

		_conf->getPt().clearParameters();

		int offset = 0;
		for (int i = tree.numSymbols() - 1; i >= 0; --i) {
			Symbol* s = tree.at(i);
			//if (_conf->getSm().isTerminal(s->id())) {
			int toffset = storeParameter(s, _conf->getPt());
			if (toffset >= 0)
			{
				offset = toffset;
				s->setParamTableOffset(toffset);
			}
				
			//}
		}
		return offset;
	}

	virtual void createGeometry(Genome_IF* genome)
	{
		// use the instanced mesh buffer for the object
		GeometryConfigSingleton::getInstance()->conf->setUsedMeshBuffer(GeometryConf::OBJECT);

		// clear the buffer
		GeometryConfigSingleton::getInstance()->conf->getInstancedMeshBuffer()->clearBuffers();

		// clear the parameter table
		_conf->getPt().clearParameters();

		// fill the parameter table with values from the genome
		int axiomId = fillParameterTable(genome);

		// create the grammar rules
		createAxiom(_conf->getSystem(), axiomId);

		// start generating the geometry
		_conf->getSystem().run(_conf->getNumThreads());
	}

	virtual void createDetailedGeometry(Genome_IF* genome)
	{
		createGeometry(genome);
	}

	virtual int getNumPreparedShapes()
	{
		return _num_prepared_shapes;
	}

	virtual void prepareShapes()
	{
		_num_prepared_shapes = _conf->getNumShapes();
		for (int i = 0; i < _num_prepared_shapes; ++i) {
			_geometry_conf->getInstancedObjectMeshBuffer()->prepareObjShape(_conf->getShapeFile(i), _conf->getShapeColor(i));
		}
	}


};


}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_GRAMMAR_RULES_BASEGRAMMAR_H_ */
