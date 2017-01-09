


#include "error.h"
#include "event.h"


namespace CU
{
	unique_event createEvent(unsigned int flags)
	{
		CUevent event;
		checkError(cuEventCreate(&event, flags));
		return unique_event(event);
	}
}
