#pragma once

#include "Memory.h"
#include <stdint.h>

class PPU
{
public:
	class PPUReg : public MemoryModule
	{
		bool _Write(uint16_t address, byte value);
		byte& _Read(uint16_t address);
	public:

		PPUReg(uint16_t minAddr = 0x2000, uint16_t maxAddr = 0x2007, PPUReg* mirrorParent = nullptr)
			:MemoryModule("PPU Reg", true, false, minAddr, maxAddr, mirrorParent)
		{

		}
	};

	PPUReg reg;

	PPU() {}
};