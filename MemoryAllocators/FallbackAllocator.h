#ifndef __FALLBACKALLOCATOR_H_INCLUDED__
#define __FALLBACKALLOCATOR_H_INCLUDED__

#include "Block.h"

namespace CustomAllocators
{
	template<class Primary, class Fallback>
	class FallbackAllocator : private Primary, private Fallback
	{
	public:
		FallbackAllocator() = default;
		FallbackAllocator(const FallbackAllocator&) = delete;
		FallbackAllocator(FallbackAllocator&&) = delete;

		Block allocate(size_t s)
		{
			Block b = Primary::allocate(s);

			if (b.ptr == nullptr)
			{
				b = Fallback::allocate(s);
			}

			return b;
		}
		
		void deallocate(Block& b)
		{
			if (Primary::owns(b))
			{
				Primary::deallocate(b);
			}
			else
			{
				Fallback::deallocate(b);
			}
		}

		void deallocateAll()
		{
			Primary::deallocateAll();
			Fallback::deallocateAll();
		}

		bool owns(Block& b)
		{
			return Primary::owns(b) || Fallback::owns(b);
		}
	};

}//namespace CustomAllocators
#endif // !__FALLBACKALLOCATOR_H_INCLUDED__