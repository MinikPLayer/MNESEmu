#pragma once

#include "Memory.h"
#include <stdint.h>

class APU
{
public:
	class APUReg : public MemoryModule
	{
		bool _Write(uint16_t address, byte value);
		byte& _Read(uint16_t address);

	public:
		APUReg(uint16_t minAddr = 0x4000, uint16_t maxAddr = 0x4017, APUReg* mirrorParent = nullptr)
			:MemoryModule("APU Reg", true, true, minAddr, maxAddr, mirrorParent)
		{

		}
	};

	APUReg reg;

	APU() {}
};