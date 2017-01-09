


#include "error.h"
#include "resource.h"
#include "texture.h"


namespace CU
{
	void initResourceViewDescriptor(CUDA_RESOURCE_VIEW_DESC& desc, CUresourceViewFormat format, size_t width, size_t height, size_t depth, unsigned int first_level, unsigned int last_level, unsigned int first_layer, unsigned int last_layer)
	{
		desc.format = format;
		desc.width = width;
		desc.height = height;
		desc.depth = depth;
		desc.firstMipmapLevel = first_level;
		desc.lastMipmapLevel = last_level;
		desc.firstLayer = first_layer;
		desc.lastLayer = last_layer;
	}

	void initTextureDescriptor(CUDA_TEXTURE_DESC& desc, CUaddress_mode address_mode_u, CUaddress_mode address_mode_v, CUaddress_mode address_mode_w, CUfilter_mode filter_mode, unsigned int flags, unsigned int max_anisotropy, CUfilter_mode mipmap_filter_mode, float level_bias, float min_level_clamp, float max_level_clamp)
	{
		desc.addressMode[0] = address_mode_u;
		desc.addressMode[1] = address_mode_v;
		desc.addressMode[2] = address_mode_w;
		desc.filterMode = filter_mode;
		desc.flags = flags;
		desc.maxAnisotropy = max_anisotropy;
		desc.mipmapFilterMode = mipmap_filter_mode;
		desc.mipmapLevelBias = level_bias;
		desc.maxMipmapLevelClamp = max_level_clamp;
		desc.minMipmapLevelClamp = min_level_clamp;
	}

	unique_texture createTextureObject(const CUDA_RESOURCE_DESC& resource_desc, const CUDA_RESOURCE_VIEW_DESC& resource_view_desc, const CUDA_TEXTURE_DESC& texture_desc)
	{
		CUtexObject tex;
		checkError(cuTexObjectCreate(&tex, &resource_desc, &texture_desc, &resource_view_desc));
		return unique_texture(tex);
	}
}
