#include "DynamicScope.h"

#include "ChangeShape.impl"
#include "SupportedShapes.h"

#include "DynamicCall.h"
#include "ScopeModifier.h"
#include "DynamicParameter.h"

#include "ParameterTable.h"

namespace PGG
{
	namespace Operators
	{ 
		using namespace Scope;
		using namespace Parameters;
		using namespace Shapes;
		using namespace Modifiers;
		using namespace CPU;

		void ChangeShape_Box_Tetrahedron(const Shapes::SBox& sbox)
		{
			typedef ParameterList<TableEntryScopeOffset, int, int, int> Ps;
			ChangeShape < Tetrahedron,
				DynamicCallAndChangeContext<TableEntryScopeOffset, Ps::P<0>, Ps::P<1>, Ps::P<2> > > :: apply (sbox);
		}
	}
}