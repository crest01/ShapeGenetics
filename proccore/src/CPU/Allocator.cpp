#include "Allocator.h"
#include <iostream>

#include <thread>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <list>
#include <iostream>

namespace PGG
{
	namespace CPU
	{
		namespace LoopyAllocator
		{
			static const int SuperblockSize = 1024 * 1024;
			static const bool KeepTrack = false;

			template<int SIZE>
			struct SuperBlock
			{
				static const int size = SIZE;
				char data[SIZE];
				volatile std::atomic<unsigned int> count_active;
				volatile std::atomic<int> offset;
				SuperBlock() : count_active(0), offset(0)
				{ }
				void* alloc(int bytes)
				{
					bytes += 16;
					int old = offset.fetch_add(bytes);
					if (old + bytes < size)
					{
						count_active.fetch_add(1);
						char *mydata = data + old;
						//remember offset
						*reinterpret_cast<int*>(mydata) = static_cast<int>(mydata - reinterpret_cast<char*>(this));
						*reinterpret_cast<int*>(mydata + 8) = bytes;
						return  mydata + 16;
					}
					return nullptr;
				}
				static SuperBlock* free(void* data, int & size)
				{
					char* mydata = reinterpret_cast<char*>(data);
					mydata -= 16;
					int thisoffset = *reinterpret_cast<int*>(mydata);
					SuperBlock* sblock = reinterpret_cast<SuperBlock*>(mydata - thisoffset);
					size = *reinterpret_cast<int*>(mydata + 8);
					if (sblock->count_active.fetch_sub(1) == 0x1U)
						return sblock;
					else
						return nullptr;
				}

				bool deactivate()
				{
					unsigned int old = count_active.fetch_and(0x7FFFFFFFU);
					if (old == 0x80000000U)
					{ 
						offset.store(0);
						return true;
					}
					return false;
				}
				void activate()
				{
					count_active.store(0x80000000U);
				}
			};

			template<int SUPERBLOCKSIZE>
			class Allocator
			{
				static const int Heaps = 16;
				std::atomic<SuperBlock<SUPERBLOCKSIZE>*> volatile heaps[Heaps];

				volatile std::atomic<int> alloced;
				volatile std::atomic<int> inUse;

				std::mutex maxInuseMutex;
				unsigned int maxInuse;

				std::mutex freeBlocksMutex;
				std::atomic<SuperBlock<SUPERBLOCKSIZE>*> volatile freeSuperblockList;
				std::atomic<SuperBlock<SUPERBLOCKSIZE>*> volatile freeSuperblockListBack;
				SuperBlock<SUPERBLOCKSIZE>* getFreeSuperblock()
				{
					SuperBlock<SUPERBLOCKSIZE>* res = nullptr;
					{
						std::lock_guard<std::mutex> g(freeBlocksMutex);
						res = freeSuperblockList.load();
						if (res != nullptr)
						{
							SuperBlock<SUPERBLOCKSIZE>* next = *reinterpret_cast<SuperBlock<SUPERBLOCKSIZE>**>(res->data);
							freeSuperblockList.store(next);
							if (next == nullptr)
								freeSuperblockListBack.store(nullptr);
						}
					}
					if (res == nullptr)
					{
						res = new SuperBlock<SUPERBLOCKSIZE>();
					}
					res->activate();
					return res;
				}
				void addFreeSuperblock(SuperBlock<SUPERBLOCKSIZE>* sb)
				{
					std::lock_guard<std::mutex> g(freeBlocksMutex);
					*reinterpret_cast<SuperBlock<SUPERBLOCKSIZE>**>(sb->data) = nullptr;

					SuperBlock<SUPERBLOCKSIZE>* back = freeSuperblockListBack.load();
					if (back != nullptr)
						*reinterpret_cast<SuperBlock<SUPERBLOCKSIZE>**>(back->data) = sb;
					else
						freeSuperblockList.store(sb);
					freeSuperblockListBack.store(sb);
				}
				void cleanFreeSuperblocks()
				{
					std::lock_guard<std::mutex> g(freeBlocksMutex);
					SuperBlock<SUPERBLOCKSIZE>* sb = freeSuperblockList.load();
					while (sb != nullptr)
					{
						SuperBlock<SUPERBLOCKSIZE>* next = *reinterpret_cast<SuperBlock<SUPERBLOCKSIZE>**>(sb->data);
						delete sb;
						sb = next;
					}
					freeSuperblockList.store(nullptr);
					freeSuperblockListBack.store(nullptr);
				}
			public:
				Allocator() : freeSuperblockList(nullptr), freeSuperblockListBack(nullptr)
				{
					for (int i = 0; i < Heaps; ++i)
						heaps[i] = nullptr;
				}
				~Allocator()
				{
					cleanFreeSuperblocks();
					for (int i = 0; i < Heaps; ++i)
						if (heaps[i].load()) delete heaps[i].load();
				}
				void* alloc(int bytes, int heap = 0)
				{
					if (KeepTrack)
					{
						alloced.fetch_add(bytes);
						int current = inUse.fetch_add(bytes) + bytes;
						if (current > 0)
						{
							std::lock_guard<std::mutex> g(maxInuseMutex);
							maxInuse = std::max<unsigned int>(maxInuse, current);
						}
					}
					void* res;
					SuperBlock<SUPERBLOCKSIZE>* old_sb = nullptr;
					SuperBlock<SUPERBLOCKSIZE>* sb = heaps[heap].load();
					while (true)
					{
						if (sb != nullptr)
						{
							res = sb->alloc(bytes);
							if (res != nullptr)
								return res;
							else
							{
								old_sb = sb;
								sb = nullptr;
							}
						}
						if (sb == nullptr)
						{
							sb = getFreeSuperblock();
							//SuperBlock<SUPERBLOCKSIZE>* old = atomic::cas(heaps[heap], old_sb, sb);
							if (!heaps[heap].compare_exchange_strong(old_sb, sb)) //(old_sb != old)
							{
								addFreeSuperblock(sb);
								sb = old_sb; // sb = old;
							}
							else if (old_sb != nullptr)
							{ 
								if (old_sb->deactivate())
									addFreeSuperblock(old_sb);
							}
							old_sb = nullptr;
						}
					}
				}
				void free(void* data)
				{
					int s;
					SuperBlock<SUPERBLOCKSIZE>* sb = SuperBlock<SUPERBLOCKSIZE>::free(data, s);
					if (sb)
					{
						sb->offset.exchange(0);
						addFreeSuperblock(sb);
					}
					if (KeepTrack)
					{
						if (inUse > s)
							inUse.fetch_sub(s);
					}
				}
				void clean()
				{
					cleanFreeSuperblocks();
					for (int i = 0; i < Heaps; ++i)
						if (heaps[i].load())
						{
							delete heaps[i].load();
							heaps[i].store(nullptr);
						}
				}
				void stats()
				{
					std::cout << "Loopy: " << inUse / 1024.0 << "KB in use, " << maxInuse / 1024.0 << "KB max " << alloced / 1024.0 << "KB alloced\n";
					maxInuse = 0;
					alloced = 0;
				}
			};

			Allocator<SuperblockSize>& getAllocator()
			{
				static Allocator<SuperblockSize> allocator;
				return allocator;
			}
			void* alloc(int bytes, int heap)
			{
				//return ::malloc(bytes);
				return getAllocator().alloc(bytes, heap);
			}
			void free(void* data)
			{
				//::free(data);
				getAllocator().free(data);
			}
			void stats()
			{
				getAllocator().stats();
			}
			void clean()
			{
				getAllocator().clean();
			}
		}
	}
}