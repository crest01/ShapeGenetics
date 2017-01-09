


#ifndef INCLUDED_CUDA_RESOURCE
#define INCLUDED_CUDA_RESOURCE

#pragma once

#include <cuda.h>


namespace CU
{
	void initResourceDescriptor(CUDA_RESOURCE_DESC& desc, CUarray array, unsigned int flags = 0U);
	void initResourceDescriptor(CUDA_RESOURCE_DESC& desc, CUmipmappedArray mipmapped_array, unsigned int flags = 0U);
	void initResourceDescriptor(CUDA_RESOURCE_DESC& desc, CUdeviceptr memory, CUarray_format format, unsigned int num_channels, size_t size, unsigned int flags = 0U);
	void initResourceDescriptor(CUDA_RESOURCE_DESC& desc, CUdeviceptr memory, CUarray_format format, unsigned int num_channels, size_t width, size_t height, size_t pitch, unsigned int flags = 0U);
}

#endif  // INCLUDED_CUDA_RESOURCE
