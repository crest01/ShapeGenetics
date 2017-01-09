


#ifndef INCLUDED_CUPTI_ERROR
#define INCLUDED_CUPTI_ERROR

#pragma once

#include <exception>

#include <cupti.h>


namespace CUPTI
{
	class error : public std::exception
	{
	private:
		CUptiResult result;
	public:
		error(CUptiResult result)
		  : result(result)
		{
		}

#if defined(_MSC_VER) && _MSC_VER <= 1800
		const char* what() const
#else
		const char* what() const noexcept
#endif
		{
			return "CUPTI fail";
		}
	};

	inline void checkError(CUptiResult result)
	{
		if (result != CUPTI_SUCCESS)
			throw CUPTI::error(result);
	}
}

using CUPTI::checkError;

#endif  // INCLUDED_CUPTI_ERROR
