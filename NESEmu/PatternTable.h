#pragma once

#include "Memory.h"

class PatternTable : public MemoryModule
{
	bool _Write(uint16_t address, byte value);
	byte _Read(uint16_t address);
public:
	byte* memory = nullptr;

	PatternTable(uint16_t minAddr = 0x0000, uint16_t maxAddr = 0x0FFF, PatternTable* mirroringParent = nullptr)
		:MemoryModule("Pattern table", true, true, minAddr, maxAddr, mirroringParent)
	{
		if (mirroringParent == nullptr)
		{
			memory = new byte[0x1000];
		}
	}

	~PatternTable()
	{
		LOGP("~PatternTable", "PatternTable");
		if (memory != nullptr)
		{
			delete[] memory;
			memory = nullptr;
		}
	}

};