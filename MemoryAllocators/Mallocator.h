#ifndef __MALLOCATOR_H_INCLUDED__
#define __MALLOCATOR_H_INCLUDED__

#include "Block.h"

#include <memory>

namespace CustomAllocators
{
	class Mallocator
	{
	public:
		
		Block allocate(size_t size)
		{
			return { std::malloc(size), size };
		}

		Block allocateAligned(size_t size)
		{
			size_t alignedSize = roundToAlligned(size, ALIGNEMENT);
			return { std::malloc(alignedSize), alignedSize };
		}

		void deallocate(Block& b)
		{
			std::free(b.ptr);
		}
	};
}//namespace CustomAllocators
#endif // !__MALLOCATOR_H_INCLUDED__