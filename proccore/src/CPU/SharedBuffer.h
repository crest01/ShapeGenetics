#ifndef CPU_SHARED_BUFFER
#define CPU_SHARED_BUFFER

#pragma once

#include <atomic>
#include <memory>
#include <limits>
#include <algorithm>

namespace PGG
{
	namespace CPU
	{
		template<typename T>
		class SharedBuffer
		{
		public:
			struct OffsetPointer
			{
				T* pointer;
				size_t offset;
				OffsetPointer() = default;
				OffsetPointer(T* p, size_t offset) : pointer(p), offset(offset) { }
				operator T*()
				{
					return pointer;
				}
			};
		private:
			struct Segment
			{
				std::unique_ptr<T[]> buff;
				size_t bsize;
				size_t overalloffset;
				std::atomic<size_t> count;
				std::atomic<size_t> usedup;
				std::atomic<Segment*> next;
				Segment(size_t segmentSize = 1024, size_t segmentOffset = 0) : 
					buff(new T[segmentSize]), 
					bsize(segmentSize), 
					overalloffset(segmentOffset), 
					count(0), 
					usedup(0), 
					next(nullptr)
				{
				}
				OffsetPointer reserve(size_t c, Segment*& used)
				{
					size_t prev = count.fetch_add(c);
					if (prev + c < bsize)
					{ 
						used = this;
						usedup.fetch_add(c);
						return OffsetPointer(&buff[prev], overalloffset + prev);
					}
					//else
					if (prev < bsize)
					{
						Segment* n = new Segment(2 * bsize, overalloffset + prev);
						next.store(n);
						return n->reserve(c, used);
					}
					//else
					Segment* n = next.load();
					while (n == nullptr)
						n = next.load();
					return n->reserve(c, used);
				}
				~Segment()
				{
					Segment* n = next.load();
					if (n)
						delete n;
				}
				size_t size() const
				{
					Segment* n = next.load();
					return usedup.load() + (n ? n->size() : 0);
				}
				size_t counts() const
				{
					Segment* n = next.load();
					return bsize + (n ? n->counts() : 0);
				}
				size_t copy(T* target, size_t offset, size_t maxelements) const
				{
					size_t N = usedup.load();
					Segment* n = next.load();
					size_t copied = 0;
					size_t newOffset = offset - N;
					if (offset < N)
					{
						copied = std::min(N - offset, maxelements);
						newOffset = 0;
						for (size_t i = 0; i < copied; ++i)
							target[i] = buff[offset + i];
					}

					if (n && maxelements + offset > N)
						return copied + n->copy(target + N, newOffset, maxelements - copied);
					return copied;
				}
			};

			std::unique_ptr<Segment> first;
			std::atomic<Segment*> current;
		public:
			SharedBuffer(size_t InitialSegmentSize = 1024) : first(new Segment(InitialSegmentSize))
			{ 
				current.store(first.get());
			}
			void resize(size_t size)
			{
				Segment* nSeg = new Segment(size);
				first->copy(&nSeg->buff[0], 0, size);
			}
			void clear(bool updateSize = true)
			{
				if (!updateSize)
				{
					Segment* n = first->next.load();
					if(n)
						delete n;
					first->count.store(0);
					first->usedup.store(0);
					first->next.store(nullptr);
				}
				else
				{
					size_t N = first->size();
					if (first->bsize >= N)
					{
						Segment* n = first->next.load();
						if (n)
							delete n;
						first->count.store(0);
						first->usedup.store(0);
						first->next.store(nullptr);
					}
					else
						first = std::unique_ptr<Segment>(new Segment(3*N/2));
				}
				current.store(first.get());
			}
			OffsetPointer reserve(size_t count)
			{
				Segment* used;
				Segment* c = current.load();
				OffsetPointer p = c->reserve(count, used);
				if (c != used)
					current.compare_exchange_weak(c, used);
				return p;
			}
			size_t size() const
			{
				return first->size();
			}
			size_t copy(T* target, size_t offset = 0, size_t maxelements = std::numeric_limits<size_t>::max() ) const
			{
				return first->copy(target, offset, maxelements);
			}
		};
	}
}

#endif //CPU_SHARED_BUFFER
