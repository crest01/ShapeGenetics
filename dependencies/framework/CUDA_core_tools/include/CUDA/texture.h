


#ifndef INCLUDED_CUDA_TEXTURE
#define INCLUDED_CUDA_TEXTURE

#pragma once

#include <cuda.h>

#include <CUDA/unique_handle.h>


namespace CU
{
	void initResourceViewDescriptor(CUDA_RESOURCE_VIEW_DESC& desc, CUresourceViewFormat format, size_t width, size_t height, size_t depth, unsigned int first_level, unsigned int last_level, unsigned int first_layer, unsigned int last_layer);
	
	void initTextureDescriptor(CUDA_TEXTURE_DESC& desc, CUaddress_mode address_mode_u, CUaddress_mode address_mode_v, CUaddress_mode address_mode_w, CUfilter_mode filter_mode, unsigned int flags, unsigned int max_anisotropy, CUfilter_mode mipmap_filter_mode, float level_bias, float min_level_clamp, float max_level_clamp);
	
	struct TexObjectDestroyDeleter
	{
		void operator ()(CUtexObject texture) const
		{
			cuTexObjectDestroy(texture);
		}
	};
	
	using unique_texture = unique_handle<CUtexObject, 0ULL, TexObjectDestroyDeleter>;
	
	unique_texture createTextureObject(const CUDA_RESOURCE_DESC& resource_desc, const CUDA_RESOURCE_VIEW_DESC& resource_view_desc, const CUDA_TEXTURE_DESC& texture_desc);
}

#endif  // INCLUDED_CUDA_TEXTURE
