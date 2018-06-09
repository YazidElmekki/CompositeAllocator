#ifndef __STACKALLOCATOR_H_INCLUDED__
#define __STACKALLOCATOR_H_INCLUDED__

#include "Block.h"

#include <iostream>
#include <cassert>

namespace CustomAllocators
{
	template<size_t stackSize>
	class StackAllocator
	{
		char data_[stackSize];
		char* ptr_;
	public:
		StackAllocator()
			: ptr_(data_)
		{
		}
		
		StackAllocator(const StackAllocator&) = delete;
		StackAllocator(StackAllocator&&) = delete;
		
		Block allocate(size_t s)
		{
			Block b;

			long long addrBefore = (long long)(ptr_);

			if (ptr_ + s <= &data_[0] + stackSize)
			{
				b.ptr = ptr_;
				b.size = s;

				ptr_ += s;

				assert((long long)ptr_ - addrBefore == s && "Invalid pointer move in stack allocator");
			}

			return b;
		}

		Block allocateAligned(size_t s)
		{
			allocate(roundToAlligned(s, ALIGNEMENT));
		}


		void deallocate(Block& b)
		{
			if ((char*)b.ptr + b.size == ptr_)
			{
				ptr_ -= b.size;
			}
		}

		void deallocateAll()
		{
			ptr_ = &data_[0];
		}

		bool owns(Block& b)
		{
			return b.ptr != nullptr && b.ptr >= data_ && b.ptr <= ptr_;
		}
	};

}//namespace CustomAllocators
#endif // !__STACKALLOCATOR_H_INCLUDED__