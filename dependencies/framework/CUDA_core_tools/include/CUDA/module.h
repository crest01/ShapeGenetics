


#ifndef INCLUDED_CUDA_MODULE
#define INCLUDED_CUDA_MODULE

#pragma once

#include <cuda.h>

#include <CUDA/unique_handle.h>


namespace CU
{
	struct ModuleUnloadDeleter
	{
		void operator ()(CUmodule module) const
		{
			cuModuleUnload(module);
		}
	};
	
	using unique_module = unique_handle<CUmodule, nullptr, ModuleUnloadDeleter>;
	
	unique_module loadModuleFile(const char* filename);
	unique_module loadModule(const void* image);
	
	CUfunction getFunction(CUmodule module, const char* name);
	CUdeviceptr getGlobal(CUmodule module, const char* name);
	CUtexref getTextureReference(CUmodule module, const char* name);
	CUsurfref getSurfaceReference(CUmodule module, const char* name);
}

#endif  // INCLUDED_CUDA_MODULE
