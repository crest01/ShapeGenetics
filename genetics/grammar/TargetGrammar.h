/*
 * TargetGrammar.h
 *
 *  Created on: Nov 2, 2015
 *      Author: Karl Haubenwallner
 */

#ifndef SRC_GRAMMAR_RULES_TargetGrammar_H_
#define SRC_GRAMMAR_RULES_TargetGrammar_H_

#include "BaseGrammar.h"

namespace PGA {

namespace SpaceShip {

	class TargetGrammar : public BaseGrammar {

	private:

		int			B;
		int 		S;

		int storeParameter(Symbol* symbol, PGG::Parameters::ParameterTable& pt);

		void initSymbols(SymbolManager& sm);
		void createAxiom(PGG::CPU::GrammarSystem& system, const int axiomId);

	public:
		TargetGrammar(ProceduralAlgorithm* base, GrammarConf* grammar_conf) :
		BaseGrammar(base, grammar_conf)
		{}

		virtual void createGeometry(Genome_IF* genome)
		{

			// use the instanced mesh buffer for the target
			GeometryConfigSingleton::getInstance()->conf->setUsedMeshBuffer(GeometryConf::TARGET);

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

		void prepareShapes() override
		{
			_num_prepared_shapes = _conf->getNumShapes();
			for (int i = 0; i < _num_prepared_shapes; ++i) {
				_geometry_conf->getInstancedTargetMeshBuffer()->prepareObjShape(_conf->getShapeFile(i), _conf->getShapeColor(i), false);
			}
		}



	};

} // namespace SpaceShip
} // namespace PGA



#endif /* SRC_GRAMMAR_RULES_TargetGrammar_H_ */
