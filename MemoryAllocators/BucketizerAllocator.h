#ifndef __BUCKETIZERALLOCATOR_H_INCLUDED__
#define __BUCKETIZERALLOCATOR_H_INCLUDED__

#include "Block.h"

namespace CustomAllocators
{
	template<typename Allocator, size_t minSize, size_t maxSize, size_t step>
	class BucketizerAllocator
	{
	private:

		static constexpr size_t allocatorNumber = (maxSize - minSize + 1) / step;
		static constexpr size_t allocationStep = step;
		static constexpr size_t minAllocSize = minSize;
		static constexpr size_t maxAllocSize = maxSize;

		Allocator allocators[allocatorNumber];

	public:
		BucketizerAllocator()
		{
			for (size_t i = 0; i < allocatorNumber; i++)
			{
				allocators[i].setSizes(minAllocSize + (allocationStep*i), minAllocSize + (i+1) * allocationStep-1);
			}
		}

		Block allocate(size_t size)
		{
			Block b;

			if (size < minAllocSize || size > maxAllocSize)
			{
				b.ptr = nullptr;
				b.size = 0;
				return b;
			}

			for (size_t i = 0; i < allocatorNumber; ++i)
			{
				if (allocators[i].getMinSize() <= size && size <= allocators[i].getMaxSize())
				{
					b = allocators[i].allocate(size);
					return b;
				}
			}
		}

		void deallocate(Block& b)
		{
			if (b.size < minAllocSize || b.size > maxAllocSize)
				return;

			for (size_t i = 0; i < allocatorNumber; ++i)
			{
				if (allocators[i].getMinSize() <= b.size && b.size <= allocators[i].getMaxSize())
				{
					return allocators[i].deallocate(b);
				}
			}
		}

		void deallocateAll()
		{
			for (size_t i = 0; i < allocatorNumber; ++i)
			{
				allocators[i].deallocateAll();
			}
		}

		bool owns(Block& b)
		{
			return minAllocSize <= b.size && b.size < maxAllocSize;
		}
	};
}//namespace CustomAllocators
#endif // !__BUCKETIZERALLOCATOR_H_INCLUDED__