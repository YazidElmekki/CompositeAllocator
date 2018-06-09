#ifndef __BITMAPALLOCATOR_H_INCLUDED__
#define __BITMAPALLOCATOR_H_INCLUDED__

#include "Block.h"

#include <intrin.h>
#include <cassert>

namespace CustomAllocators
{
	template<class A, size_t minSize, size_t maxSize>
	class BitmapAllocator
	{
	private:
		typedef size_t	Word;
		enum
		{
			WordSize = sizeof(Word) * 8,
			WordMask = WordSize - 1,
		};

		A allocator_;
		Block memBlock_;
		size_t bitmap_ = 0;

		void SetBit(size_t index, bool set)
		{
			size_t arrayIndex = ceil(index / WordSize);
			size_t bitNumber = index - (arrayIndex * WordSize);

			if (set)
			{
				bitmap_ |= 1 << bitNumber;
			}
			else
			{
				bitmap_ &= ~((Word)(1 << bitNumber));
			}
		}

		size_t bSize_ = maxSize;
		size_t minAllocationSize_ = minSize;
		size_t maxAllocationSize_ = maxSize;

	public:
		BitmapAllocator()
		{
			InitializeMemory();
		}
		
		~BitmapAllocator()
		{
			allocator_.deallocate(memBlock_);
		}

		BitmapAllocator(const BitmapAllocator&) = delete;
		BitmapAllocator(BitmapAllocator&&) = delete;

		void setSizes(size_t minS, size_t maxS)
		{
			assert
			(
				maxAllocationSize_ == 0 &&
				maxS != 0				&&
				minS < maxS				&&
				"Trying to change size of Bitmap allocator after memory was initialised");
			
			minAllocationSize_ = minS;
			maxAllocationSize_ = maxS;
			bSize_ = maxS;

			InitializeMemory();
		}

		size_t getSize()	{ return bSize_; }
		size_t getMinSize() { return minAllocationSize_; }
		size_t getMaxSize() { return maxAllocationSize_; }

		void InitializeMemory()
		{
			if (bSize_ != 0)
			{
				memBlock_ = allocator_.allocate(bSize_ * WordSize);
				assert(memBlock_.ptr != nullptr && "BitmapAllocator can't allocate memory");
			}
		}

		Block allocate(size_t s)
		{
			Block b;

			if (bSize_ == 0 || s != bSize_ || bitmap_ == ~0)
			{
				b.ptr = nullptr;
				b.size = 0;

				return b;
			}

			size_t reversedBitmap = ~bitmap_;
			unsigned long index = 0;

			_BitScanForward64(&index, reversedBitmap);

			b.ptr = (char*)memBlock_.ptr + index * bSize_;
			b.size = s;

			SetBit(index, true);

			return b;
		}

		void deallocate(Block& b)
		{
			if (bSize_ == 0 || b.size != bSize_)
				return;

			unsigned long index = ((unsigned long)b.ptr - (unsigned long)memBlock_.ptr) / bSize_;

			SetBit(index, false);
		}

		void deallocateAll()
		{
			bitmap_ = 0u;
		}

		bool owns(Block& b)
		{
			return bSize_ != 0 && b.size == blockSize;
		}
	};
}//namespace CustomAllocators
#endif // !__BITMAPALLOCATOR_H_INCLUDED__