#include "ParameterStorage.h"
#include "Evaluator.h"
#include <math/vector.h>

namespace PGG
{
	namespace CPU
	{
		namespace
		{
			thread_local const Parameters::ParameterTable* thread_parameterTable;
		}
		void setDynParameterTable(Evaluator& evaluator, const Parameters::ParameterTable& paramTable)
		{
			evaluator.addContextInitializer([&paramTable](){ thread_parameterTable = &paramTable; });
		}
	}
	namespace Parameters
	{
		math::float4 accessParamTable(int offset)
		{
			return CPU::thread_parameterTable->getParameters()[offset];
		}
	}
}