


#ifndef INCLUDED_CAMERA
#define INCLUDED_CAMERA

#pragma once

#include "GL/platform/interface.h"

#include <math/matrix.h>

#ifndef WIN32
#define __declspec(x) x
#define align(x) alignas(x)
#endif


class INTERFACE Camera
{
protected:
	Camera() = default;
	Camera(const Camera&) = default;
	Camera& operator =(const Camera&) = default;
	~Camera() = default;
public:
	struct UniformBuffer
	{
		__declspec(align(64)) math::affine_float4x4 V;
		__declspec(align(64)) math::affine_float4x4 V_inv;
		__declspec(align(64)) math::float4x4 P;
		__declspec(align(64)) math::float4x4 P_inv;
		__declspec(align(64)) math::float4x4 PV;
		__declspec(align(64)) math::float4x4 PV_inv;
		__declspec(align(16)) math::float3 position;
	};

	virtual void writeUniformBuffer(UniformBuffer* params, float aspect) const = 0;
};

#endif  // INCLUDED_CAMERA
