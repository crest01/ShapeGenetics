#ifndef INCLUDED_CUBUFFER_H
#define INCLUDED_CUBUFFER_H

#pragma once

#include "CUDA/memory.h"

namespace PGA {
namespace GPU {

class CUBufferBase {
protected:
	CU::unique_ptr 	_buffer;
	size_t			_size;

	CUBufferBase(): _buffer(0), _size(0) {}
	~CUBufferBase() {}


	void _init(int size, size_t elements_size);
	void _copyToDevice(void* source, int elements, size_t elements_size);
	void _copyToHost(void* target, int elements, size_t elements_size);
	void _setToZero(size_t elements_size);
	void _assureSize(int new_size, bool keep, size_t elements_size);
};


template <class T>
class CUBuffer : CUBufferBase
{
public:
	CUBuffer() {}
	~CUBuffer() {}

	int size() {
		return _size();
	}

	void init(int size)
	{
		_init(size, sizeof(T));
	}

	CUdeviceptr operator()()
	{
		return _buffer;
	}

	void copyToDevice(T* source, int elements)
	{
		_copyToDevice(static_cast<void*>(source), elements, sizeof(T));
	}

	void copyToHost(T* target, int elements)
	{
		_copyToHost(static_cast<void*>(target), elements, sizeof(T));
	}
	void setToZero()
	{
		_setToZero(sizeof(T));
	}

	void assureSize(int new_size, bool keep = false)
	{
		_assureSize(new_size, keep, sizeof(T));
	}
};

} // namespace GPU
} // namespace PGA

#endif  // INCLUDED_CUDABUFFER_H
