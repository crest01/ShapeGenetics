


#include "resource.h"


namespace CU
{
	void initResourceDescriptor(CUDA_RESOURCE_DESC& desc, CUarray array, unsigned int flags)
	{
		desc.resType = CU_RESOURCE_TYPE_ARRAY;
		desc.res.array.hArray = array;
		desc.flags = flags;
	}

	void initResourceDescriptor(CUDA_RESOURCE_DESC& desc, CUmipmappedArray mipmapped_array, unsigned int flags)
	{
		desc.resType = CU_RESOURCE_TYPE_MIPMAPPED_ARRAY;
		desc.res.mipmap.hMipmappedArray = mipmapped_array;
		desc.flags = flags;
	}

	void initResourceDescriptor(CUDA_RESOURCE_DESC& desc, CUdeviceptr memory, CUarray_format format, unsigned int num_channels, size_t size, unsigned int flags)
	{
		desc.resType = CU_RESOURCE_TYPE_LINEAR;
		desc.res.linear.devPtr = memory;
		desc.res.linear.format = format;
		desc.res.linear.numChannels = num_channels;
		desc.res.linear.sizeInBytes = size;
		desc.flags = flags;
	}

	void initResourceDescriptor(CUDA_RESOURCE_DESC& desc, CUdeviceptr memory, CUarray_format format, unsigned int num_channels, size_t width, size_t height, size_t pitch, unsigned int flags)
	{
		desc.resType = CU_RESOURCE_TYPE_ARRAY;
		desc.res.pitch2D.devPtr = memory;
		desc.res.pitch2D.format = format;
		desc.res.pitch2D.numChannels = num_channels;
		desc.res.pitch2D.width = width;
		desc.res.pitch2D.height = height;
		desc.res.pitch2D.pitchInBytes = pitch;
		desc.flags = flags;
	}
}
