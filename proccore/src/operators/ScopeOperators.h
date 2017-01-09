#ifndef INCLUDED_OPERATORS_SCOPE_OPERATORS
#define INCLUDED_OPERATORS_SCOPE_OPERATORS

#pragma once


#include "DualBuild.h"
#include "scope/Scope.h"
#include "parameters/Axis.h"
#include "shapes/ShapeTraits.h"

#include <template_utils.h>


namespace PGG
{
	namespace Operators
	{
		template<class Float3Param, class Next>
		struct Translate
		{
			template<class InShape, class TScope>
			DUAL_BUILD_FUNCTION static void apply(const Scope::ScopedShape<InShape, TScope> & scopedShape);
		};

		template<class FloatXParam, class Next>
		struct Scale
		{
			template<class InShape, class TScope>
			DUAL_BUILD_FUNCTION static void apply(const Scope::ScopedShape<InShape, TScope> & scopedShape);
		};

		//template<class Axis, class Angle, class Next>
		//template<class Float3x3, class Next>
		template<class ... AxisAndAngle_or_3x3Matrix_followed_by_Next>
		struct Rotate
		{
			template<class InShape, class TScope>
			DUAL_BUILD_FUNCTION static void apply(const Scope::ScopedShape<InShape, TScope> & scopedShape);
		};


		template<class Float4x4Param, class Next>
		struct TransformObject
		{
			template<class InShape, class TScope>
			DUAL_BUILD_FUNCTION static void apply(const Scope::ScopedShape<InShape, TScope> & scopedShape);
		};

		template<class Float3x4Param, class Next>
		struct SetCoordinateAxis
		{
			template<class InShape, class TScope>
			DUAL_BUILD_FUNCTION static void apply(const Scope::ScopedShape<InShape, TScope> & scopedShape);
		};
	}
}

#endif //INCLUDED_OPERATORS_CHANGE_SHAPE