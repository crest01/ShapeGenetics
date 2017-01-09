#ifndef CPU_GRAMMAR_SYSTEM
#define CPU_GRAMMAR_SYSTEM

#pragma once

#include "Evaluator.h"
#include "parameters/ParameterTable.h"
#include "ParameterStorage.h"

namespace PGG
{
	namespace CPU
	{
		class GrammarSystem
		{
			Evaluator evaluator;
		public:
			GrammarSystem(int MaxPhases = 128) : evaluator(MaxPhases) {}

			void registerParameterTable( const Parameters::ParameterTable& table )
			{
				setDynParameterTable(evaluator, table);
			}
			template<class InitalOperator, class ScopedShape>
			void addAxiom(const ScopedShape& axiom, int phase = 0)
			{
				evaluator.enqueue(axiom, [](const ScopedShape& s){ InitalOperator::apply(s); }, phase);
			}
			template<class F, class ScopedShape>
			void addAxiom(const ScopedShape& axiom, F f, int phase = 0)
			{
				evaluator.enqueue(axiom, f, phase);
			}
			template<class F>
			void addContextInitializer(F f)
			{
				evaluator.addContextInitializer(f);
			}
			void run(int numThreads = 4)
			{
				while (evaluator.run(4));
			}
		};
	}
}


#endif
