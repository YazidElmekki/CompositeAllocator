#ifndef __BLOCK_H_INCLUDED__
#define __BLOCK_H_INCLUDED__

namespace CustomAllocators
{
	struct Block
	{
		void* ptr = nullptr;
		size_t size = 0;
	};
}//namespace CustomAllocators

#endif // !__BLOCK_H_INCLUDED__
