


#include "NVRTC/error.h"


namespace NVRTC
{
	error::error(nvrtcResult result)
		: result(result)
	{
	}

	const char* error::what() const noexcept
	{
		return nvrtcGetErrorString(result);
	}
}
