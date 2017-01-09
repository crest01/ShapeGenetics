


#include "error.h"
#include "resource.h"
#include "surface.h"


namespace CU
{
	unique_surface createSurfaceObject(const CUDA_RESOURCE_DESC& desc)
	{
		CUsurfObject surf;
		checkError(cuSurfObjectCreate(&surf, &desc));
		return unique_surface(surf);
	}

	unique_surface createSurfaceObject(CUarray array)
	{
		CUDA_RESOURCE_DESC desc;
		initResourceDescriptor(desc, array);
		return createSurfaceObject(desc);
	}

	unique_surface createSurfaceObject(CUmipmappedArray mipmapped_array)
	{
		CUDA_RESOURCE_DESC desc;
		initResourceDescriptor(desc, mipmapped_array);
		return createSurfaceObject(desc);
	}

	unique_surface createSurfaceObject(CUdeviceptr memory, CUarray_format format, unsigned int num_channels, size_t size)
	{
		CUDA_RESOURCE_DESC desc;
		initResourceDescriptor(desc, memory, format, num_channels, size);
		return createSurfaceObject(desc);
	}

	unique_surface createSurfaceObject(CUdeviceptr memory, CUarray_format format, unsigned int num_channels, size_t width, size_t height, size_t pitch)
	{
		CUDA_RESOURCE_DESC desc;
		initResourceDescriptor(desc, memory, format, num_channels, width, height, pitch);
		return createSurfaceObject(desc);
	}
}
