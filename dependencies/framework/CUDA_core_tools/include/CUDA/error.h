


#ifndef INCLUDED_CUDA_ERROR
#define INCLUDED_CUDA_ERROR

#pragma once

#include <exception>

#include <cuda.h>


namespace CU
{
	class error : public std::exception
	{
	private:
		CUresult result;
	public:
		error(CUresult result);
		
		const char* what() const noexcept;
	};
	
	inline void checkError(CUresult result)
	{
		if (result != CUDA_SUCCESS)
			throw error(result);
	}
}

using CU::checkError;

#endif  // INCLUDED_CUDA_ERROR
