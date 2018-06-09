// MemoryAllocators.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <new>
#include <iostream>


#include "StackAllocator.h"
#include "NullAllocator.h"
#include "FallbackAllocator.h"

#define ARR_SIZE 102

struct DataStruct
{
	int data[ARR_SIZE];
};

int main()
{
	CustomAllocators::FallbackAllocator<CustomAllocators::StackAllocator<1024, 64>, CustomAllocators::NullAllocator> fallbackAlloctor;

	CustomAllocators::Block b0 = fallbackAlloctor.allocate(sizeof(DataStruct));
	CustomAllocators::Block b1 = fallbackAlloctor.allocate(sizeof(DataStruct));

	DataStruct* dataStruct0 = new (b0.ptr) DataStruct;
	DataStruct* dataStruct1 = new (b1.ptr) DataStruct;

	for (int i = 0; i < ARR_SIZE; ++i)
	{
		dataStruct0->data[i] = i;
		dataStruct1->data[i] = i + ARR_SIZE;
	}

	std::cout << "Data0 owned : " << std::boolalpha << fallbackAlloctor.owns(b0) << std::endl;
	std::cout << "Data1 owned : " << std::boolalpha << fallbackAlloctor.owns(b1) << std::endl;


	for (int i = 0; i < ARR_SIZE; ++i)
	{
		std::cout << "Data0[" << i << "] : " << dataStruct0->data[i] << std::endl;
		assert(dataStruct0->data[i] == i && "Data0 at index i not equal to i");

		std::cout << "Data1[" << i << "] : " << dataStruct1->data[i] << std::endl;
		assert(dataStruct1->data[i] == i + ARR_SIZE && "Data1 at index i not equal to i");
	}

	fallbackAlloctor.deallocate(b0);
	fallbackAlloctor.deallocate(b1);

	fallbackAlloctor.deallocateAll();

    return 0;
}