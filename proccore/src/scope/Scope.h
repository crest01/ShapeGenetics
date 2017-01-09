#ifndef INCLUDED_SHAPES_SCOPE
#define INCLUDED_SHAPES_SCOPE

#pragma once

#include <math/vector.h>
#include <math/matrix.h>
#include "DualBuild.h"
#include "ScopeAugmentation.h"

namespace PGG
{
	namespace Scope
	{

		struct CoordinateFrame
		{
			math::float3x4 matrix;

			DUAL_BUILD_FUNCTION void translateShape(const math::float3& translation)
			{
				matrix._14 += matrix._11 * translation.x + matrix._12 * translation.y + matrix._13 * translation.z;
				matrix._24 += matrix._21 * translation.x + matrix._22 * translation.y + matrix._23 * translation.z;
				matrix._34 += matrix._31 * translation.x + matrix._32 * translation.y + matrix._33 * translation.z;
			}
			DUAL_BUILD_FUNCTION void scaleShape(const math::float3& scaling)
			{
				matrix._11 *= scaling.x; matrix._21 *= scaling.x; matrix._31 *= scaling.x;
				matrix._12 *= scaling.y; matrix._22 *= scaling.y; matrix._32 *= scaling.y;
				matrix._13 *= scaling.z; matrix._23 *= scaling.z; matrix._33 *= scaling.z;
			}
			DUAL_BUILD_FUNCTION void scaleShape(float scaling)
			{
				matrix._11 *= scaling; matrix._21 *= scaling; matrix._31 *= scaling;
				matrix._12 *= scaling; matrix._22 *= scaling; matrix._32 *= scaling;
				matrix._13 *= scaling; matrix._23 *= scaling; matrix._33 *= scaling;
			}
			DUAL_BUILD_FUNCTION void rotateShape(const math::float3x3& rotation)
			{
				math::float3x3 rotpart(matrix._11, matrix._12, matrix._13,
				                       matrix._21, matrix._22, matrix._23,
				                       matrix._31, matrix._32, matrix._33);
				rotpart = rotpart * rotation;
				matrix._11 = rotpart._11; matrix._12 = rotpart._12; matrix._13 = rotpart._13;
				matrix._21 = rotpart._21; matrix._22 = rotpart._22; matrix._23 = rotpart._23;
				matrix._31 = rotpart._31; matrix._32 = rotpart._32; matrix._33 = rotpart._33;
			}
			DUAL_BUILD_FUNCTION void transformShape(const math::float4x4& transformation)
			{
				matrix = matrix * transformation;
			}

			DUAL_BUILD_FUNCTION void translateScope(const math::float3& translation)
			{
				matrix._14 += translation.x;
				matrix._24 += translation.y;
				matrix._34 += translation.z;
			}
			DUAL_BUILD_FUNCTION void scaleScope(const math::float3& scaling)
			{
				matrix._11 *= scaling.x; matrix._12 *= scaling.x; matrix._13 *= scaling.x;
				matrix._21 *= scaling.y; matrix._22 *= scaling.y; matrix._23 *= scaling.y;
				matrix._31 *= scaling.z; matrix._32 *= scaling.z; matrix._33 *= scaling.z;
			}
			DUAL_BUILD_FUNCTION void scaleScope(float scaling)
			{
				matrix._11 *= scaling; matrix._12 *= scaling; matrix._13 *= scaling;
				matrix._21 *= scaling; matrix._22 *= scaling; matrix._23 *= scaling;
				matrix._31 *= scaling; matrix._32 *= scaling; matrix._33 *= scaling;
			}
			DUAL_BUILD_FUNCTION void rotateScope(const math::float3x3& rotation)
			{
				math::float3x3 rotpart(matrix._11, matrix._12, matrix._13,
					matrix._21, matrix._22, matrix._23,
					matrix._31, matrix._32, matrix._33);
				rotpart = rotation * rotpart;
				matrix._11 = rotpart._11; matrix._12 = rotpart._12; matrix._13 = rotpart._13;
				matrix._21 = rotpart._21; matrix._22 = rotpart._22; matrix._23 = rotpart._23;
				matrix._31 = rotpart._31; matrix._32 = rotpart._32; matrix._33 = rotpart._33;
				math::float3 transpart = matrix.column4();
				transpart = rotation * transpart;
				matrix._14 = transpart.x; matrix._24 = transpart.y; matrix._34 = transpart.z;
			}
			DUAL_BUILD_FUNCTION void transformScope(const math::float4x4& transformation)
			{
				math::float4x4 expanded(matrix._11, matrix._12, matrix._13, matrix._14,
					matrix._21, matrix._22, matrix._23, matrix._24,
					matrix._31, matrix._32, matrix._33, matrix._34,
					0.0f, 0.0f, 0.0f, 1.0f);
				expanded = transformation * expanded;
				matrix = math::float3x4::from_rows(expanded.row1(), expanded.row2(), expanded.row3());
			}
			DUAL_BUILD_FUNCTION math::float3 applyToVertex(const math::float3& vertex) const
			{
				return matrix*math::float4(vertex, 1.0f);
			}
			DUAL_BUILD_FUNCTION math::float3 applyToNormal(const math::float3& normal) const
			{
				// TODO: we would want the inverse transpose here 
				return normalize(matrix*math::float4(normal, 0.0f));
			}
		};

		template<typename... Layers>
		class CoordinateframeScope : public CoordinateFrame
		{

		private:
			Augmentation<Layers...> augmentation;

		public:
			DUAL_BUILD_FUNCTION CoordinateframeScope() = default;
			DUAL_BUILD_FUNCTION CoordinateframeScope(const math::float3x4& initalframe)
			{
				matrix = initalframe;
			}
			DUAL_BUILD_FUNCTION CoordinateframeScope(const math::float3x4& initalframe, const Layers & ... layers) : augmentation(layers...)
			{
				matrix = initalframe;
			}

			template<int Sublayer>
			DUAL_BUILD_FUNCTION
			typename TypeQuery<Sublayer, Layers...>::Type& accessLayer()
			{
				return augmentation . template access <Sublayer>();
			}

			template<int Sublayer>
			DUAL_BUILD_FUNCTION
			const typename TypeQuery<Sublayer, Layers...>::Type& accessLayer() const
			{
				return augmentation . template access <Sublayer>();
			}
		};

		template<>
		class CoordinateframeScope<> : public CoordinateFrame
		{
		public:

			DUAL_BUILD_FUNCTION CoordinateframeScope() = default;
			DUAL_BUILD_FUNCTION CoordinateframeScope(const math::float3x4& initalframe)
			{
				matrix = initalframe;
			}
		};

		template<class TShape, class TScope>
		struct ScopedShape
		{
			typedef TScope Scope;
			typedef TShape Shape;

			Scope scope;
			Shape shape;

			DUAL_BUILD_FUNCTION ScopedShape() = default;
			DUAL_BUILD_FUNCTION ScopedShape(Shape shape, Scope scope) : scope(scope), shape(shape) { }
		};
	}
}

#endif
