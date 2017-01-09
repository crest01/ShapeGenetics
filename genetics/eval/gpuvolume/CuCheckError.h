/*
 * CudaCheckError.h
 *
 *  Created on: Jan 12, 2016
 *      Author: Karl Haubenwallner
 */

#ifndef GPUPROCGENETICS_SRC_EVALUATION_GPU_CUCHECKERROR_H_
#define GPUPROCGENETICS_SRC_EVALUATION_GPU_CUCHECKERROR_H_

#include <cuda.h>
#include <stdio.h>

namespace PGA {

namespace GPU {

#define cuCheckError(ans) { _cuCheckError((ans), __FILE__, __LINE__); }
inline void _cuCheckError(CUresult code, const char *file, int line)
{
	if (code != CUDA_SUCCESS)
	{
		char* err_str;
		cuGetErrorString(code, const_cast<const char**>(&err_str));
		char* err_name;
		cuGetErrorName(code, const_cast<const char**>(&err_name));
		if (err_str != nullptr && err_name != nullptr)
			fprintf(stderr,"CU Error %s: %s [%s:%d]\n", err_name, err_str, file, line);
		else
			fprintf(stderr,"unknown CU Error (%d) [%s %d]\n", code, file, line);
	}
}
} // namespace GPU
} // namespace PGA



#endif /* GPUPROCGENETICS_SRC_EVALUATION_GPU_CUCHECKERROR_CUH_ */
