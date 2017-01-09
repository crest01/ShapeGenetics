#ifndef INCLUDED_MATRIX_TRANSFORMATIONS
#define INCLUDED_MATRIX_TRANSFORMATIONS

#pragma once

#include <math/matrix.h>
#include "DualBuild.h"

namespace math
{
	DUAL_BUILD_FUNCTION inline float3x3 rotateX(float angle)
	{

		float cval = math::cos(angle);
		float sval = math::sin(angle);

#ifdef LEFTHANDED
		return float3x3(1.0f, 0.0f, 0.0f,
			0.0f, cval, sval,
			0.0f, -sval, cval);
#else
		return float3x3(
			1.0f, 0.0f,  0.0f,
			0.0f, cval, -sval,
			0.0f, sval,  cval);
#endif
	}
	DUAL_BUILD_FUNCTION inline float3x3 rotateY(float angle)
	{
		float cval = math::cos(angle);
		float sval = math::sin(angle);
#ifdef LEFTHANDED
		return float3x3(
			cval, 0.0f, -sval,
			0.0f, 1.0f,  0.0f,
			sval, 0.0f,  cval);
#else
		return float3x3 (
			 cval, 0.0f, sval,
			 0.0f, 1.0f, 0.0f,
			-sval, 0.0f, cval);
#endif
	}
	DUAL_BUILD_FUNCTION inline float3x3 rotateZ(float angle)
	{
		float cval = math::cos(angle);
		float sval = math::sin(angle);
#ifdef LEFTHANDED
		return float3x3(
			 cval, sval, 0.0f,
			-sval, cval, 0.0f,
			 0.0f, 0.0f, 1.0f);
#else
		return float3x3(
			cval, -sval, 0.0f,
			sval,  cval, 0.0f,
			0.0f,  0.0f, 1.0f);
#endif
	}

	DUAL_BUILD_FUNCTION inline float4x4 rotationAndTranslation(const float3x3 & rot, const float3& translation = float3(0,0,0))
	{
		return float4x4(
			rot._11, rot._12, rot._13, translation.x,
			rot._21, rot._22, rot._23, translation.y,
			rot._31, rot._32, rot._33, translation.z,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	DUAL_BUILD_FUNCTION inline float4x4 translate(const float3 & translation)
	{
		return float4x4(
			1.0f, 0.0f, 0.0f, translation.x,
			0.0f, 1.0f, 0.0f, translation.y,
			0.0f, 0.0f, 1.0f, translation.z,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	DUAL_BUILD_FUNCTION inline float4x4 translate(float x, float y, float z)
	{
		return float4x4(
			1.0f, 0.0f, 0.0f, x,
			0.0f, 1.0f, 0.0f, y,
			0.0f, 0.0f, 1.0f, z,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
}

#endif