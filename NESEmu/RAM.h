#pragma once
#include "Memory.h"
#include <vector>

using namespace std;

class RAM : public MemoryModule
{
	// Implementation of virtual functions

	bool _Write(uint16_t address, byte value);
	byte _Read(uint16_t address);


	// RAM memory
	int memSize = -1;
	byte* memory = nullptr;
public:

	RAM(uint16_t minAddr = 0x0000, uint16_t maxAddr = 0x07FF, RAM* mirrored = nullptr)
		:MemoryModule("RAM", true, true, minAddr, maxAddr, mirrored)
	{
		memSize = maxAddr - minAddr + 1;
		memory = new byte[memSize];
	}

	~RAM()
	{
		if (memory != nullptr)
		{
			delete[] memory;
			memory = nullptr;
		}
	}
};