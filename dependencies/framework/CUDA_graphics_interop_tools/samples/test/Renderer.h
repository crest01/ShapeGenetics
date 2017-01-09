


#ifndef INCLUDED_RENDERER
#define INCLUDED_RENDERER

#pragma once

#include <cuda.h>
#include <CUDA/context.h>
#include <CUDA/module.h>
#include <CUDA/launch.h>


class Renderer
{
	CU::unique_context context;
	CU::unique_module module;

	CUsurfref color_buffer_ref;
	CU::Function<int, int> clear;

public:
	Renderer();

	void render(CUarray color_buffer, unsigned int buffer_width, unsigned int buffer_height);
};

#endif  // INCLUDED_RENDERER
