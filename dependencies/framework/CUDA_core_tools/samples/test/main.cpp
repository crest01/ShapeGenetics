


#include <string>
#include <sstream>
#include <iostream>
#include <exception>

#include <CUDA/device.h>
#include <CUDA/context.h>

#include "launch.h"


int main(int argc, char* argv[])
{
	try
	{
		expect<CUDA_ERROR_INVALID_VALUE, CUDA_ERROR_NOT_INITIALIZED>(cuDeviceGetCount(nullptr));

		succeed(cuInit(0U));

		int device_count;
		succeed(cuDeviceGetCount(&device_count));

		for (int i = 0; i < device_count; ++i)
		{
			CUdevice device = CU::getDevice(i);

			std::cout << "running on device " << i << ": " << CU::getDeviceName(device) << '\n';

			CU::unique_context context = CU::createContext(0U, device);
			CU::context_scope ctx(context);

			launch();
		}
	}
	catch (const CU::error& e)
	{
		std::cout << e.name() << ": " << e.what() << std::endl;
		return -1;
	}
	catch (const std::exception& e)
	{
		std::cout << "error: " << e.what() << std::endl;
		return -1;
	}
	catch (...)
	{
		std::cout << "unknown exception" << std::endl;
		return -128;
	}

	return 0;
}
