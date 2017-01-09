


#include "error.h"
#include "memory.h"


namespace CU
{
	unique_ptr allocMemory(size_t size)
	{
		CUdeviceptr ptr;
		checkError(cuMemAlloc(&ptr, size));
		return unique_ptr(ptr);
	}
	
	unique_ptr allocMemoryPitched(size_t& pitch, size_t row_size, size_t num_rows, unsigned int element_size)
	{
		CUdeviceptr ptr;
		checkError(cuMemAllocPitch(&ptr, &pitch, row_size, num_rows, element_size));
		return unique_ptr(ptr);
	}
	
	pitched_memory allocMemoryPitched(size_t row_size, size_t num_rows, unsigned int element_size)
	{
		CUdeviceptr ptr;
		size_t pitch;
		checkError(cuMemAllocPitch(&ptr, &pitch, row_size, num_rows, element_size));
		return pitched_memory(unique_ptr(ptr), pitch);
	}
}
