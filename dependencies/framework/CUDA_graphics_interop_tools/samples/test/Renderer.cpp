


#include <sstream>
#include <iostream>

#include <NVRTC/program.h>

#include <GL/gl.h>
#include <cuda.h>
#include <cudaGL.h>
#include <CUDA/error.h>
#include <CUDA/device.h>
#include <CUDA/context.h>

#include "Renderer.h"


namespace
{
	std::string compile(const char* source, int cc_major, int cc_minor)
	{
		std::ostringstream arch_builder;
		arch_builder << "-arch=compute_" << cc_major << cc_minor;
		auto arch = arch_builder.str();

		auto program = NVRTC::createProgram(source, "@", nullptr, 0, nullptr);

		NVRTC::compileProgram(program, { "-default-device", "-std=c++11", "-lineinfo", "-dc", arch.c_str() });

		std::cout << NVRTC::getProgramLog(program) << std::endl;

		auto ptx = NVRTC::getPTX(program);
		return { &ptx.data[0], ptx.size - 1 };
	}

	CU::unique_context createContext()
	{
		unsigned int device_count;
		CUdevice device;
		succeed(cuGLGetDevices(&device_count, &device, 1U, CU_GL_DEVICE_LIST_ALL));

		std::cout << "running on " << CU::getDeviceName(device) << '\n';

		CU::unique_context context = CU::createContext(0U, device);
		return context;
	}

	CU::unique_module createModule(const char* source)
	{
		auto device = CU::getDevice(CU::getCurrentContext());

		int cc_major;
		succeed(cuDeviceGetAttribute(&cc_major, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR, device));

		int cc_minor;
		succeed(cuDeviceGetAttribute(&cc_minor, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR, device));

		auto ptx = compile(source, cc_major, cc_minor);

		std::cout << ptx;

		return CU::loadModule(ptx.c_str());
	}
}


const char source[] = R"""(

surface<void, cudaSurfaceType2D> color_buffer;

__global__
void clear(int width, int height)
{
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;

	if (x < width && y < height)
	{
		surf2Dwrite<uchar4>(make_uchar4(x * 255.0f / width, y * 255.0f / height, 0.0f, 1.0f), color_buffer, x * 4U, y);
	}
}

)""";


Renderer::Renderer()
	: context(createContext()),
	  module(createModule(source)),
	  color_buffer_ref(CU::getSurfaceReference(module, "color_buffer")),
	  clear(CU::getFunction(module, "_Z5clearii"))
{
}


void Renderer::render(CUarray color_buffer, unsigned int buffer_width, unsigned int buffer_height)
{
	constexpr auto block_dim_x = 16U;
	constexpr auto block_dim_y = 16U;

	auto blocks_x = CU::divup(buffer_width, block_dim_x);
	auto blocks_y = CU::divup(buffer_width, block_dim_y);

	succeed(cuSurfRefSetArray(color_buffer_ref, color_buffer, 0U));

	clear({ blocks_x, blocks_y }, { block_dim_x, block_dim_y }, 0U, 0U, buffer_width, buffer_height);

	succeed(cuCtxSynchronize());
}
