#ifndef ALLOCATOR_INCLUDED
#define ALLOCATOR_INCLUDED

#pragma once

namespace PGG
{ 
	namespace CPU
	{
		namespace LoopyAllocator
		{
			void* alloc(int bytes, int heap = 0);
			void free(void* data);
			void stats();
			void clean();
		}
	}
}

#endif