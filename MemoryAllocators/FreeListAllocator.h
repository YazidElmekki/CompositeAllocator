#ifndef __FREELISTALLOCATOR_H_INCLUDED__
#define __FREELISTALLOCATOR_H_INCLUDED__

#include "Block.h"

namespace MemoryAllocators
{
	template<class A, size_t s>
	class FreeListAllocator
	{
		A parent;
		struct Node { Node* next; };

		Node* root = nullptr;

	public:

		Block allocate(size_t n)
		{
			if (n == s && root != nullptr)
			{
				Block b = { root, n };
				root = root->next;

				return b;
			}

			return parent.allocate(n);
		}

		bool owns(Block b)
		{
			return b.size == s || parent.owns(b);
		}

		void deallocate(Block b)
		{
			if (b.size != s)
			{
				return parent.deallocate(b);
			}

			Node* p = b.ptr;
			p.next = root;
			root = p;
		}

	};
}//namespace MemoryAllocators
#endif // !__FREELISTALLOCATOR_H_INCLUDED__
