#ifndef __BLOCK_H_INCLUDED__
#define __BLOCK_H_INCLUDED__

#define ALIGNEMENT 64

namespace CustomAllocators
{
	struct Block
	{
		void* ptr = nullptr;
		size_t size = 0;
	};

	static inline size_t roundToAlligned(size_t s, size_t roundTo)
	{
		if (roundTo == 0)
		{
			return s;
		}

		size_t rest = s % roundTo;
		if (rest == 0)
		{
			return s;
		}

		return s + roundTo - rest;
	}

	namespace traits
	{
		template <typename T> struct has_deallocateAll
		{
			template <typename U, void (U::*)()noexcept> struct Check;
			template <typename U> static constexpr bool test(Check<U, &U::deallocate_all> *) { return true; }
			template <typename U> static constexpr bool test(...) { return false; }

			static constexpr bool value = test<T>(nullptr);
		};
	}

}//namespace CustomAllocators
#endif // !__BLOCK_H_INCLUDED__
