


#ifndef INCLUDED_NVRTC_ERROR
#define INCLUDED_NVRTC_ERROR

#pragma once

#include <exception>

#include <nvrtc.h>


namespace NVRTC
{
	class error : public std::exception
	{
		nvrtcResult result;
	public:
		error(nvrtcResult result);

		const char* what() const noexcept;
	};

	inline void checkError(nvrtcResult result)
	{
		if (result != NVRTC_SUCCESS)
			throw error(result);
	}
}

using NVRTC::checkError;

#endif  // INCLUDED_NVRTC_ERROR
