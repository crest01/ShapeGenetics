#include "CuBuffer.h"
#include "CuCheckError.h"

namespace PGA {
namespace GPU {


void CUBufferBase::_init(int size, size_t elements_size)
{
	_buffer = CU::allocMemory(size * elements_size);
	_size = size;
}

void CUBufferBase::_copyToDevice(void* source, int elements, size_t elements_size)
{
	_assureSize(elements, false, elements_size);
	cuCheckError(cuMemcpyHtoD(_buffer,source, elements * elements_size));
}

void CUBufferBase::_copyToHost(void* target, int elements, size_t elements_size)
{
	if (_size < elements) {
		elements = _size;
	}
	cuCheckError(cuMemcpyDtoH(target,_buffer, elements * elements_size));
}

void CUBufferBase::_setToZero(size_t elements_size)
{
	cuCheckError(cuMemsetD8(_buffer, 0, _size * elements_size));
}

void CUBufferBase::_assureSize(int new_size, bool keep, size_t elements_size)
{
	if (_size == 0) {
		_init(new_size, elements_size);
	}
	else if (_size < new_size)
	{
		int size2 = _size;
		while (size2 < new_size)
			size2 <<= 1;

		new_size = size2;

		if (keep)
		{
			CU::unique_ptr temp = CU::allocMemory(new_size * elements_size);
			cuCheckError(cuMemcpy(temp, _buffer, _size * elements_size));
			swap(_buffer,temp);
		}
		else
		{
			_buffer = CU::allocMemory(new_size * elements_size);
		}
		_size = new_size;
	}
}

} // namespace GPU
} // namespace PGA
