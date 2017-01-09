#ifndef INCLUDED_DUAL_BUILD
#define INCLUDED_DUAL_BUILD

#pragma once
#ifdef __CUDACC__
#define DUAL_BUILD_FUNCTION __host__ __device__
#define DEVICE_FUNCTION __device__
#ifndef BUILD_CUDA
#define BUILD_CUDA 1
#endif
#ifdef __CUDA_ARCH__
#define IS_CUDE_CODE 1
#else
#define IS_CUDE_CODE 0
#endif
#else
#define DUAL_BUILD_FUNCTION
#ifndef BUILD_CUDA
#define BUILD_CUDA 0
#endif
#define IS_CUDE_CODE 0
#endif


#if IS_CUDE_CODE == 0
DUAL_BUILD_FUNCTION inline int __float_as_int(float x)
{
	return *reinterpret_cast<int*>(&x);
}
DUAL_BUILD_FUNCTION inline unsigned int __float_as_uint(float x)
{
	return *reinterpret_cast<unsigned int*>(&x);
}
DUAL_BUILD_FUNCTION inline float __int_as_float(int x)
{
	return *reinterpret_cast<float*>(&x);
}
DUAL_BUILD_FUNCTION inline float __uint_as_float(unsigned int x)
{
	return *reinterpret_cast<float*>(&x);
}
#else
DUAL_BUILD_FUNCTION inline unsigned int __float_as_uint(float x)
{
	return static_cast<unsigned int>(__float_as_int(x));
}
#endif


#endif //INCLUDED_DUAL_BUILD
