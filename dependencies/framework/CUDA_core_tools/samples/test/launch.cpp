


#include <iostream>

#include <CUDA/device.h>
#include <CUDA/context.h>
#include <CUDA/module.h>
#include <CUDA/launch.h>

#include "compile.h"
#include "launch.h"


const char source[] = R"""(
extern "C"
{
	__global__
	void test_1()
	{
		printf("hello from  {%d,%d,%d} {%d,%d,%d}\n", blockIdx.x, blockIdx.y, blockIdx.z, threadIdx.x, threadIdx.y, threadIdx.z);
	}


	__device__ unsigned int counter;

	__global__
	void test_2(unsigned int* counter, int increment)
	{
		atomicAdd(counter, increment);
	}
}
)""";

void launch()
{
	auto device = CU::getDevice(CU::getCurrentContext());

	int cc_major;
	succeed(cuDeviceGetAttribute(&cc_major, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR, device));

	int cc_minor;
	succeed(cuDeviceGetAttribute(&cc_minor, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR, device));

	auto ptx = compile(source, cc_major, cc_minor);

	//std::cout << ptx << std::endl;

	auto module = CU::loadModule(&ptx[0]);

	CU::Function<> test_1 { CU::getFunction(module, "test_1") };
	test_1({ 2, 2, 2 }, { 8, 8, 4 }, 0U, 0U);
	succeed(cuCtxSynchronize());


	auto counter = CU::getGlobal(module, "counter");

	succeed(cuMemsetD32(counter, 0U, 1U));

	CU::Function<unsigned int*, int> test_2 { CU::getFunction(module, "test_2") };
	test_2({ 2, 2, 2 }, { 8, 8, 4 }, 0U, 0U, counter, 1);
	
	CU::device_type_t<unsigned int> c;
	succeed(cuMemcpyDtoH(&c, counter, sizeof(c)));

	return;
}
