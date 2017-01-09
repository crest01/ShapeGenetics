


#include "error.h"
#include "stream.h"


namespace CU
{
	unique_stream createStream(unsigned int flags)
	{
		CUstream stream;
		checkError(cuStreamCreate(&stream, flags));
		return unique_stream(stream);
	}
}
