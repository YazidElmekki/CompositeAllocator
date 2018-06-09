#ifndef __NULLALLOCATOR_H_INCLUDED__
#define __NULLALLOCATOR_H_INCLUDED__

#include "Block.h"

#include <cassert>

namespace CustomAllocators
{
	class NullAllocator
	{
	public:
		Block allocate(size_t s)
		{
			return { nullptr, 0 };
		}

		void deallocate(Block& b)
		{
			assert(b.ptr == nullptr && "Attempted to free non null block with NullAllocator");
		}

		void deallocateAll()
		{
		}

		bool owns(Block& b)
		{
			return false;
		}
	};
}

#endif // !__NULLALLOCATOR_H_INCLUDED__
