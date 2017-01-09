#include "DynamicScope.h"

#include "ScopeOperators.impl"
#include "SupportedShapes.h"

#include "ParameterTable.h"

#include "DynamicCall.h"
#include "ScopeModifier.h"
#include "DynamicParameter.h"



namespace PGG
{
	namespace Operators
	{
		using namespace Scope;
		using namespace Parameters;
		using namespace Shapes;
		using namespace Modifiers;
		using namespace CPU;

		template<class Shape>
		void ScopeOperators_Translate(const Scope::ScopedShape<Shape, DynamicScope >& sshape)
		{
			
			typedef ParameterList<TableEntryScopeOffset, math::float3, int, int, int> Ps;
			Translate <Ps::P<0>,
				DynamicCallAndChangeContext<TableEntryScopeOffset, Ps::P<1>, Ps::P<2>, Ps::P<3> > > ::apply(sshape);
		}

		template<class Shape>
		void ScopeOperators_ScaleUniform(const Scope::ScopedShape<Shape, DynamicScope >& sshape)
		{
			typedef ParameterList<TableEntryScopeOffset, float, int, int, int> Ps;
			Scale <Ps::P<0>,
				DynamicCallAndChangeContext<TableEntryScopeOffset, Ps::P<1>, Ps::P<2>, Ps::P<3> > > ::apply(sshape);
		}

		template<class Shape>
		void ScopeOperators_Scale3(const Scope::ScopedShape<Shape, DynamicScope >& sshape)
		{
			typedef ParameterList<TableEntryScopeOffset, math::float3, int, int, int> Ps;
			Scale <Ps::P<0>,
				DynamicCallAndChangeContext<TableEntryScopeOffset, Ps::P<1>, Ps::P<2>, Ps::P<3> > > ::apply(sshape);
		}

		template<class Shape>
		void ScopeOperators_RotateAxis(const Scope::ScopedShape<Shape, DynamicScope >& sshape)
		{
			typedef ParameterList<TableEntryScopeOffset, Axes, float, int, int, int> Ps;
			Rotate <Ps::P<0>, Ps::P<1>,
				DynamicCallAndChangeContext<TableEntryScopeOffset, Ps::P<2>, Ps::P<3>, Ps::P<4> > > ::apply(sshape);
		}

		template<class Shape>
		void ScopeOperators_RotateMatrix(const Scope::ScopedShape<Shape, DynamicScope >& sshape)
		{
			typedef ParameterList<TableEntryScopeOffset, math::float3x3, int, int, int> Ps;
			Rotate <Ps::P<0>,
				DynamicCallAndChangeContext<TableEntryScopeOffset, Ps::P<1>, Ps::P<2>, Ps::P<3> > >  ::apply(sshape);
		}

		template void ScopeOperators_Translate(const Scope::ScopedShape<Box, DynamicScope >&);
		template void ScopeOperators_ScaleUniform(const Scope::ScopedShape<Box, DynamicScope >&);
		template void ScopeOperators_Scale3(const Scope::ScopedShape<Box, DynamicScope >&);
		template void ScopeOperators_RotateAxis(const Scope::ScopedShape<Box, DynamicScope >&);
		template void ScopeOperators_RotateMatrix(const Scope::ScopedShape<Box, DynamicScope >&);

		template void ScopeOperators_Translate(const Scope::ScopedShape<Quad, DynamicScope >&);
		template void ScopeOperators_ScaleUniform(const Scope::ScopedShape<Quad, DynamicScope >&);
		template void ScopeOperators_Scale3(const Scope::ScopedShape<Quad, DynamicScope >&);
		template void ScopeOperators_RotateAxis(const Scope::ScopedShape<Quad, DynamicScope >&);
		template void ScopeOperators_RotateMatrix(const Scope::ScopedShape<Quad, DynamicScope >&);

	}
}