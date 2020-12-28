#pragma once

#include "Defines.h"
#include <stdint.h>

#include "Util.h"
#include <string>


class MemoryModule
{
public:
	string name = "";
protected:

	virtual bool _Write(uint16_t address, byte value) { LOGP("Memory write is not implemented!", name); return false; }
	virtual byte& _Read(uint16_t address) { LOGP("Memory read is not implemented!", name); return nullValue; }

public:
	uint16_t minAddr = 0;
	uint16_t maxAddr = 0;

	bool writeable = false;
	bool readable = false;

	static byte nullValue;

	byte& Read(uint16_t address)
	{
		if (readable)
		{
			return _Read(address);
		}

		LOGP(std::hex << address << " is not a readable address", name);
		return nullValue;
	}

	bool Write(uint16_t address, byte value)
	{
		if (writeable)
		{
			return _Write(address, value);
		}

		LOGP(std::hex << address << " is not a writeable address", name);
		return false;
	}

	MemoryModule(string name, bool writeable, bool readable, uint16_t minAddr, uint16_t maxAddr);
};