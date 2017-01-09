


#include "error.h"
#include "array.h"


namespace CU
{
	void initArray1DDescriptor(CUDA_ARRAY_DESCRIPTOR& desc, size_t width, CUarray_format format, unsigned int num_channels)
	{
		desc.Width = width;
		desc.Height = 0U;
		desc.Format = format;
		desc.NumChannels = num_channels;
	}

	void initArray1DDescriptor(CUDA_ARRAY3D_DESCRIPTOR& desc, size_t width, CUarray_format format, unsigned int num_channels, unsigned int flags)
	{
		desc.Width = width;
		desc.Height = 0U;
		desc.Depth = 0U;
		desc.Format = format;
		desc.NumChannels = num_channels;
		desc.Flags = flags;
	}

	void initArray1DLayeredDescriptor(CUDA_ARRAY3D_DESCRIPTOR& desc, size_t width, unsigned int layers, CUarray_format format, unsigned int num_channels, unsigned int flags)
	{
		desc.Width = width;
		desc.Height = 0U;
		desc.Depth = layers;
		desc.Format = format;
		desc.NumChannels = num_channels;
		desc.Flags = CUDA_ARRAY3D_LAYERED | flags;
	}

	void initArray2DDescriptor(CUDA_ARRAY_DESCRIPTOR& desc, size_t width, size_t height, CUarray_format format, unsigned int num_channels)
	{
		desc.Width = width;
		desc.Height = height;
		desc.Format = format;
		desc.NumChannels = num_channels;
	}

	void initArray2DDescriptor(CUDA_ARRAY3D_DESCRIPTOR& desc, size_t width, size_t height, CUarray_format format, unsigned int num_channels, unsigned int flags)
	{
		desc.Width = width;
		desc.Height = height;
		desc.Depth = 0U;
		desc.Format = format;
		desc.NumChannels = num_channels;
		desc.Flags = flags;
	}

	void initArray2DLayeredDescriptor(CUDA_ARRAY3D_DESCRIPTOR& desc, size_t width, size_t height, unsigned int layers, CUarray_format format, unsigned int num_channels, unsigned int flags)
	{
		desc.Width = width;
		desc.Height = height;
		desc.Depth = layers;
		desc.Format = format;
		desc.NumChannels = num_channels;
		desc.Flags = CUDA_ARRAY3D_LAYERED | flags;
	}

	void initArrayCubeDescriptor(CUDA_ARRAY3D_DESCRIPTOR& desc, size_t width, CUarray_format format, unsigned int num_channels, unsigned int flags)
	{
		desc.Width = width;
		desc.Height = width;
		desc.Depth = 6U;
		desc.Format = format;
		desc.NumChannels = num_channels;
		desc.Flags = CUDA_ARRAY3D_CUBEMAP | flags;
	}


	void initArrayCubeLayeredDescriptor(CUDA_ARRAY3D_DESCRIPTOR& desc, size_t width, unsigned int layers, CUarray_format format, unsigned int num_channels, unsigned int flags)
	{
		desc.Width = width;
		desc.Height = width;
		desc.Depth = 6U * layers;
		desc.Format = format;
		desc.NumChannels = num_channels;
		desc.Flags = CUDA_ARRAY3D_CUBEMAP | CUDA_ARRAY3D_LAYERED | flags;
	}

	void initArray3DDescriptor(CUDA_ARRAY3D_DESCRIPTOR& desc, size_t width, size_t height, size_t depth, CUarray_format format, unsigned int num_channels, unsigned int flags)
	{
		desc.Width = width;
		desc.Height = height;
		desc.Depth = depth;
		desc.Format = format;
		desc.NumChannels = num_channels;
		desc.Flags = flags;
	}


	unique_array createArray(const CUDA_ARRAY_DESCRIPTOR& desc)
	{
		CUarray array;
		checkError(cuArrayCreate(&array, &desc));
		return unique_array(array);
	}

	unique_array createArray(const CUDA_ARRAY3D_DESCRIPTOR& desc)
	{
		CUarray array;
		checkError(cuArray3DCreate(&array, &desc));
		return unique_array(array);
	}

	unique_mipmapped_array createArrayMipmapped(const CUDA_ARRAY3D_DESCRIPTOR& desc, unsigned int levels)
	{
		CUmipmappedArray array;
		checkError(cuMipmappedArrayCreate(&array, &desc, levels));
		return unique_mipmapped_array(array);
	}


	unique_array createArray1D(size_t width, CUarray_format format, unsigned int num_channels)
	{
		CUDA_ARRAY_DESCRIPTOR desc;
		initArray1DDescriptor(desc, width, format, num_channels);
		return createArray(desc);
	}

	unique_array createArray1D(size_t width, CUarray_format format, unsigned int num_channels, unsigned int flags)
	{
		CUDA_ARRAY3D_DESCRIPTOR desc;
		initArray1DDescriptor(desc, width, format, num_channels, flags);
		return createArray(desc);
	}

	unique_array createArray1DLayered(size_t width, unsigned int layers, CUarray_format format, unsigned int num_channels, unsigned int flags)
	{
		CUDA_ARRAY3D_DESCRIPTOR desc;
		initArray1DLayeredDescriptor(desc, width, layers, format, num_channels, flags);
		return createArray(desc);
	}

	unique_mipmapped_array createArray1DMipmapped(size_t width, unsigned int levels, CUarray_format format, unsigned int num_channels, unsigned int flags)
	{
		CUDA_ARRAY3D_DESCRIPTOR desc;
		initArray1DDescriptor(desc, width, format, num_channels, flags);
		return createArrayMipmapped(desc, levels);
	}

	unique_mipmapped_array createArray1DLayeredMipmapped(size_t width, unsigned int layers, unsigned int levels, CUarray_format format, unsigned int num_channels, unsigned int flags)
	{
		CUDA_ARRAY3D_DESCRIPTOR desc;
		initArray1DLayeredDescriptor(desc, width, layers, format, num_channels, flags);
		return createArrayMipmapped(desc, levels);
	}


	unique_array createArray2D(size_t width, size_t height, CUarray_format format, unsigned int num_channels)
	{
		CUDA_ARRAY_DESCRIPTOR desc;
		initArray2DDescriptor(desc, width, height, format, num_channels);
		return createArray(desc);
	}

	unique_array createArray2D(size_t width, size_t height, CUarray_format format, unsigned int num_channels, unsigned int flags)
	{
		CUDA_ARRAY3D_DESCRIPTOR desc;
		initArray2DDescriptor(desc, width, height, format, num_channels, flags);
		return createArray(desc);
	}

	unique_array createArray2DLayered(size_t width, size_t height, unsigned int layers, CUarray_format format, unsigned int num_channels, unsigned int flags)
	{
		CUDA_ARRAY3D_DESCRIPTOR desc;
		initArray2DLayeredDescriptor(desc, width, height, layers, format, num_channels, flags);
		return createArray(desc);
	}

	unique_mipmapped_array createArray2DMipmapped(size_t width, size_t height, unsigned int levels, CUarray_format format, unsigned int num_channels, unsigned int flags)
	{
		CUDA_ARRAY3D_DESCRIPTOR desc;
		initArray2DDescriptor(desc, width, height, format, num_channels, flags);
		return createArrayMipmapped(desc, levels);
	}

	unique_mipmapped_array createArray2DLayeredMipmapped(size_t width, size_t height, unsigned int layers, unsigned int levels, CUarray_format format, unsigned int num_channels, unsigned int flags)
	{
		CUDA_ARRAY3D_DESCRIPTOR desc;
		initArray2DLayeredDescriptor(desc, width, height, layers, format, num_channels, flags);
		return createArrayMipmapped(desc, levels);
	}


	unique_array createArrayCube(size_t width, CUarray_format format, unsigned int num_channels, unsigned int flags)
	{
		CUDA_ARRAY3D_DESCRIPTOR desc;
		initArrayCubeDescriptor(desc, width, format, num_channels, flags);
		return createArray(desc);
	}

	unique_array createArrayCubeLayered(size_t width, unsigned int layers, CUarray_format format, unsigned int num_channels, unsigned int flags)
	{
		CUDA_ARRAY3D_DESCRIPTOR desc;
		initArrayCubeLayeredDescriptor(desc, width, layers, format, num_channels, flags);
		return createArray(desc);
	}

	unique_mipmapped_array createArrayCubeMipmapped(size_t width, unsigned int levels, CUarray_format format, unsigned int num_channels, unsigned int flags)
	{
		CUDA_ARRAY3D_DESCRIPTOR desc;
		initArrayCubeDescriptor(desc, width, format, num_channels, flags);
		return createArrayMipmapped(desc, levels);
	}

	unique_mipmapped_array createArrayCubeLayeredMipmapped(size_t width, unsigned int layers, unsigned int levels, CUarray_format format, unsigned int num_channels, unsigned int flags)
	{
		CUDA_ARRAY3D_DESCRIPTOR desc;
		initArrayCubeLayeredDescriptor(desc, width, layers, format, num_channels, flags);
		return createArrayMipmapped(desc, levels);
	}


	unique_array createArray3D(size_t width, size_t height, size_t depth, CUarray_format format, unsigned int num_channels, unsigned int flags)
	{
		CUDA_ARRAY3D_DESCRIPTOR desc;
		initArray3DDescriptor(desc, width, height, depth, format, num_channels, flags);
		return createArray(desc);
	}
}
