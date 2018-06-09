#ifndef __FREELISTALLOCATOR_H_INCLUDED__
#define __FREELISTALLOCATOR_H_INCLUDED__

#include "Block.h"

namespace CustomAllocators
{
	template<class A, size_t minSize, size_t maxSize>
	class FreeListAllocator
	{
	private:
		A allocator_;
		struct Node { Node* next = nullptr; };

		Node* root_ = nullptr;

		size_t bSize_ = maxSize;
		size_t minAllocationSize = minSize;
		size_t maxAllocationSize = maxSize;

	public:
		~FreeListAllocator()
		{
			Block b = { nullptr, maxAllocationSize };

			while (root_ != nullptr)
			{
				b.ptr = (void*)root_;
				root_ = root_->next;
				allocator_.deallocate(b);
			}
		}

		void setSizes(size_t minS, size_t maxS)
		{
			assert
			(
				maxAllocationSize == 0	&&
				maxS != 0				&&
				minS < maxS				&&
				"Trying to change Freelist allocator after initialization"
			);

			minAllocationSize = minS;
			maxAllocationSize = maxS;
			bSize_ = maxAllocationSize;
		}

		size_t getSize()	{ return bSize_; }
		size_t getMinSize() { return minAllocationSize; }
		size_t getMaxSize() { return maxAllocationSize; }

		Block allocate(size_t n)
		{
			Block b = { nullptr, 0 };

			if (minAllocationSize <= n && n <= maxAllocationSize)
			{
				size_t alignedSize = roundToAlligned(n, maxAllocationSize);

				if (bSize_ != 0 && alignedSize == bSize_ && root_ != nullptr)
				{
					b = { root_, alignedSize };
					root_ = root_->next;

					return b;
				}

				b = allocator_.allocate(alignedSize);
				return b;
			}

			return b;
		}

		void deallocate(Block& b)
		{
			if (bSize_ == 0 && b.size != bSize_)
			{
				return allocator_.deallocate(b);
			}

			Node* p = (Node*)b.ptr;
			p->next = root_;
			root_ = p;
		}

		bool owns(Block& b)
		{
			return  b.ptr != nullptr && (b.size == blockSize || allocator_.owns(b));
		}
	};
}//namespace MemoryAllocators
#endif // !__FREELISTALLOCATOR_H_INCLUDED__