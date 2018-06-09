#ifndef __STACKALLOCATOR_H_INCLUDED__
#define __STACKALLOCATOR_H_INCLUDED__

#include "Block.h"

#include <iostream>
#include <cassert>

namespace CustomAllocators
{
	template<size_t stackSize, size_t alignement = 64>
	class StackAllocator
	{
		char data[stackSize];
		char* ptr;
	public:
		StackAllocator()
			: ptr(data)
		{
		}
		
		StackAllocator(const StackAllocator&) = delete;
		StackAllocator(StackAllocator&&) = delete;

		size_t roundToAlligned(size_t s)
		{
			if (alignement == 0)
			{
				return s;
			}

			size_t rest = s % alignement;
			if (rest == 0)
			{
				return s;
			}

			return s + alignement - rest;
		}
		
		Block allocate(size_t s)
		{
			Block b;

			long long addrBefore = (long long)(ptr);

			size_t alignedSize = roundToAlligned(s);

			if (ptr + alignedSize <= &data[0] + stackSize)
			{
				b.ptr = ptr;
				b.size = alignedSize;

				ptr += alignedSize;

				assert((long long)ptr - addrBefore == alignedSize && "Invalid pointer move in stack allocator");
			}

			return b;
		}

		void deallocate(Block& b)
		{
			if ((char*)b.ptr + b.size == ptr)
			{
				ptr -= b.size;
			}
		}

		void deallocateAll()
		{
			ptr = &data[0];
		}

		bool owns(Block& b)
		{
			if (b.ptr >= data && b.ptr <= ptr)
			{
				return true;
			}

			return false;
		}
	};

}//namespace CustomAllocators

#endif // !__STACKALLOCATOR_H_INCLUDED__
