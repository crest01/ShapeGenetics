#ifndef CPU_PARAMETER_STORAGE_INCLUDED
#define CPU_PARAMETER_STORAGE_INCLUDED

#pragma once

#if defined(_MSC_VER) && _MSC_VER < 1900
#define thread_local __declspec( thread )
#endif

#include "parameters/ParameterTable.h"

namespace PGG
{
	namespace CPU
	{
		class Evaluator;
		void setDynParameterTable(Evaluator& evaluator, const Parameters::ParameterTable& paramTable);
	}
}

#endif