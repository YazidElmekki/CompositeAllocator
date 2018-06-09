#ifndef __SEGREGATOR_H_INCLUDED__
#define __SEGREGATOR_H_INCLUDED__

#include "Block.h"

#include <type_traits>

namespace CustomAllocators
{
	template< size_t threshold,class smallAllocator, class largeAllocator>
	class Segregator : private smallAllocator, private largeAllocator
	{
	private:
	
	public:
		Block allocate(size_t size)
		{
			if (size < threshold)
			{
				return smallAllocator::allocate(size);
			}

			return largeAllocator::allocate(size);
		}

		void deallocate(Block& b)
		{
			if (b.size < threshold)
			{
				return smallAllocator::deallocate(b);
			}

			return largeAllocator::deallocate(b);
		}

		void deallocateAll()
		{
			smallAllocator::deallocateAll();
			largeAllocator::deallocateAll();
		}

		bool owns(Block& b)
		{
			return smallAllocator::owns(b) || largeAllocator::owns(b);
		}
	};
}//namespace CustomAllocators
#endif // !__SEGREGATOR_H_INCLUDED__