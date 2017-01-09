


#include "error.h"
#include "module.h"


namespace CU
{
	unique_module loadModule(const void* image)
	{
		CUmodule module;
		checkError(cuModuleLoadData(&module, image));
		return unique_module(module);
	}

	unique_module loadModuleFile(const char* filename)
	{
		CUmodule module;
		checkError(cuModuleLoad(&module, filename));
		return unique_module(module);
	}

	CUfunction getFunction(CUmodule module, const char* name)
	{
		CUfunction function;
		checkError(cuModuleGetFunction(&function, module, name));
		return function;
	}

	CUdeviceptr getGlobal(CUmodule module, const char* name)
	{
		CUdeviceptr ptr;
		size_t size;
		checkError(cuModuleGetGlobal(&ptr, &size, module, name));
		return ptr;
	}

	CUtexref getTextureReference(CUmodule module, const char* name)
	{
		CUtexref ref;
		checkError(cuModuleGetTexRef(&ref, module, name));
		return ref;
	}

	CUsurfref getSurfaceReference(CUmodule module, const char* name)
	{
		CUsurfref ref;
		checkError(cuModuleGetSurfRef(&ref, module, name));
		return ref;
	}
}
