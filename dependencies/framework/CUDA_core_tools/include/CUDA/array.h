


#ifndef INCLUDED_CUDA_ARRAY
#define INCLUDED_CUDA_ARRAY

#pragma once

#include <cuda.h>

#include <CUDA/unique_handle.h>


namespace CU
{
	void initArray1DDescriptor(CUDA_ARRAY_DESCRIPTOR& desc, size_t width, CUarray_format format, unsigned int num_channels);
	void initArray1DDescriptor(CUDA_ARRAY3D_DESCRIPTOR& desc, size_t width, CUarray_format format, unsigned int num_channels, unsigned int flags = 0U);
	void initArray1DLayeredDescriptor(CUDA_ARRAY3D_DESCRIPTOR& desc, size_t width, unsigned int layers, CUarray_format format, unsigned int num_channels, unsigned int flags = 0U);
	
	void initArray2DDescriptor(CUDA_ARRAY_DESCRIPTOR& desc, size_t width, size_t height, CUarray_format format, unsigned int num_channels);
	void initArray2DDescriptor(CUDA_ARRAY3D_DESCRIPTOR& desc, size_t width, size_t height, CUarray_format format, unsigned int num_channels, unsigned int flags = 0U);
	void initArray2DLayeredDescriptor(CUDA_ARRAY3D_DESCRIPTOR& desc, size_t width, size_t height, unsigned int layers, CUarray_format format, unsigned int num_channels, unsigned int flags = 0U);
	
	void initArrayCubeDescriptor(CUDA_ARRAY3D_DESCRIPTOR& desc, size_t width, CUarray_format format, unsigned int num_channels, unsigned int flags = 0U);
	void initArrayCubeLayeredDescriptor(CUDA_ARRAY3D_DESCRIPTOR& desc, size_t width, unsigned int layers, CUarray_format format, unsigned int num_channels, unsigned int flags = 0U);
	
	void initArray3DDescriptor(CUDA_ARRAY3D_DESCRIPTOR& desc, size_t width, size_t height, size_t depth, CUarray_format format, unsigned int num_channels, unsigned int flags = 0U);
	
	
	struct ArrayDestroyDeleter
	{
		void operator ()(CUarray array) const
		{
			cuArrayDestroy(array);
		}
	};
	
	using unique_array = unique_handle<CUarray, nullptr, ArrayDestroyDeleter>;
	
	
	struct MipmappedArrayDestroyDeleter
	{
		void operator ()(CUmipmappedArray array) const
		{
			cuMipmappedArrayDestroy(array);
		}
	};
	
	using unique_mipmapped_array = unique_handle<CUmipmappedArray, nullptr, MipmappedArrayDestroyDeleter>;
	
	
	unique_array createArray(const CUDA_ARRAY_DESCRIPTOR& desc);
	unique_array createArray(const CUDA_ARRAY3D_DESCRIPTOR& desc);
	unique_mipmapped_array createArrayMipmapped(const CUDA_ARRAY3D_DESCRIPTOR& desc, unsigned int levels);
	
	
	unique_array createArray1D(size_t width, CUarray_format format, unsigned int num_channels);
	unique_array createArray1D(size_t width, CUarray_format format, unsigned int num_channels, unsigned int flags);
	unique_array createArray1DLayered(size_t width, unsigned int layers, CUarray_format format, unsigned int num_channels, unsigned int flags = 0U);
	unique_mipmapped_array createArray1DMipmapped(size_t width, unsigned int levels, CUarray_format format, unsigned int num_channels, unsigned int flags = 0U);
	unique_mipmapped_array createArray1DLayeredMipmapped(size_t width, unsigned int layers, unsigned int levels, CUarray_format format, unsigned int num_channels, unsigned int flags = 0U);
	
	unique_array createArray2D(size_t width, size_t height, CUarray_format format, unsigned int num_channels);
	unique_array createArray2D(size_t width, size_t height, CUarray_format format, unsigned int num_channels, unsigned int flags);
	unique_array createArray2DLayered(size_t width, size_t height, unsigned int layers, CUarray_format format, unsigned int num_channels, unsigned int flags = 0U);
	unique_mipmapped_array createArray2DMipmapped(size_t width, size_t height, unsigned int levels, CUarray_format format, unsigned int num_channels, unsigned int flags = 0U);
	unique_mipmapped_array createArray2DLayeredMipmapped(size_t width, size_t height, unsigned int layers, unsigned int levels, CUarray_format format, unsigned int num_channels, unsigned int flags = 0U);
	
	unique_array createArrayCube(size_t width, CUarray_format format, unsigned int num_channels, unsigned int flags = 0U);
	unique_array createArrayCubeLayered(size_t width, unsigned int layers, CUarray_format format, unsigned int num_channels, unsigned int flags = 0U);
	unique_mipmapped_array createArrayCubeMipmapped(size_t width, unsigned int levels, CUarray_format format, unsigned int num_channels, unsigned int flags = 0U);
	unique_mipmapped_array createArrayCubeLayeredMipmapped(size_t width, unsigned int layers, unsigned int levels, CUarray_format format, unsigned int num_channels, unsigned int flags = 0U);
	
	unique_array createArray3D(size_t width, size_t height, size_t depth, CUarray_format format, unsigned int num_channels, unsigned int flags = 0U);
}

#endif  // INCLUDED_CUDA_ARRAY
