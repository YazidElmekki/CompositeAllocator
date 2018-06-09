// MemoryAllocators.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define _CRTDBG_MAP_ALLOC #include <stdlib.h> #include <crtdbg.h>  

#include <new>
#include <iostream>

#include "StackAllocator.h"
#include "NullAllocator.h"
#include "FallbackAllocator.h"
#include "FreeListAllocator.h"
#include "BitmapAllocator.h"
#include "Mallocator.h"
#include "Segregator.h"
#include "BucketizerAllocator.h"

template<size_t ARR_SIZE>
struct DataStruct
{
	DataStruct()
	{
		std::cout << "Create DataStruct" << std::endl;
	}

	~DataStruct()
	{
		std::cout << "Destroy DataStruct" << std::endl;
	}

	void InitializeData(int initStart = 0)
	{
		for (size_t i = 0; i < ARR_SIZE; ++i)
		{
			data_[i] = i + initStart;
		}
	}

	void PrintData()
	{
		for (size_t i = 0; i < ARR_SIZE; ++i)
		{
			std::cout << "Data[" << i << "] : " << data_[i] << std::endl;
		}
	}

	int data_[ARR_SIZE];
};

using namespace CustomAllocators;

int main()
{
	{
		using FList = FreeListAllocator<Mallocator, 0, 0>;

		using AdvancedAllocator = Segregator<
			8, FreeListAllocator<Mallocator, 0, 8>, Segregator<
				129, BucketizerAllocator<FList, 1, 128, 16>, Segregator<
					257, BucketizerAllocator<FList, 129, 256, 32>, Segregator<
						513, BucketizerAllocator<FList, 257, 512, 64>, Segregator<
							1025, BucketizerAllocator<FList, 513, 1024, 128>, Segregator<
								2049, BucketizerAllocator<FList, 1025, 2048, 256>, Segregator<
									3585, BucketizerAllocator<FList, 2049, 3584, 512>, Mallocator
									>
								>
							>
						>
					>
				>
			>;
			
		AdvancedAllocator advancedAllocator;

		CustomAllocators::Block b0 = advancedAllocator.allocate(sizeof(DataStruct<2>));
		CustomAllocators::Block b1 = advancedAllocator.allocate(sizeof(DataStruct<64>));
		CustomAllocators::Block b2 = advancedAllocator.allocate(sizeof(DataStruct<128>));

		DataStruct<2>* data0 = new(b0.ptr) DataStruct<2>;
		DataStruct<2>* data1 = new(b1.ptr) DataStruct<2>;
		DataStruct<2>* data2 = new(b2.ptr) DataStruct<2>;

		data0->InitializeData();
		std::cout << "Data0 : " << std::endl;
		data0->PrintData();
		
		data1->InitializeData(2);
		std::cout << "Data1 : " << std::endl;
		data1->PrintData();

		data2->InitializeData(4);
		std::cout << "Data2 : " << std::endl;
		data2->PrintData();

		data0->~DataStruct();
		data1->~DataStruct();
		data2->~DataStruct();

		advancedAllocator.deallocate(b0);
		advancedAllocator.deallocate(b1);
		advancedAllocator.deallocate(b2);
	}

	_CrtDumpMemoryLeaks();
    
	return 0;
}