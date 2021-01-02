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

	// Virtual function to write to memory - returns if succeeded 
	virtual bool _Write(uint16_t address, byte value) { LOGP("Memory write is not implemented!", name); return false; }
	// Virtual function to read from memory - returns read value
	virtual byte _Read(uint16_t address) { LOGP("Memory read is not implemented!", name); return nullValue; }

	MemoryModule* mirrorParent;
public:
	uint16_t minAddr = 0;
	uint16_t maxAddr = 0;

	bool writeable = false;
	bool readable = false;

	static byte nullValue;

	// General wrapper for _Read function, returns read value or "nullValue" ( undefined value ) when not readable
	byte Read(uint16_t address)
	{
		if (readable)
		{
			if (mirrorParent != nullptr)
				return mirrorParent->_Read(address);
			else
				return _Read(address);
		}

		LOGP(std::hex << address << " is not a readable address", name);
		return nullValue;
	}

	// General wrapper for _Write function, returns true if succeded or false if not writeable / failed to read
	bool Write(uint16_t address, byte value)
	{
		if (writeable)
		{
			if (mirrorParent != nullptr)
				return mirrorParent->_Write(address, value);
			else
				return _Write(address, value);
		}

		LOGP(std::hex << address << " is not a writeable address", name);
		return false;
	}

	/* Memory module constructor
	 * @param name Module name
	 * @param writeable Defines if module can be written
	 * @param readable Defines if module can be read from
	 * @param minAddr Lowest address occupied in memory map by this module ( inclusive )
	 * @param maxAddr Highest address occupied in memory map by this module ( inclusive )
	 * @param mirrorParent If this is a mirrored module, specify a parent to mirror
	*/
	MemoryModule(string name, bool writeable, bool readable, uint16_t minAddr, uint16_t maxAddr, MemoryModule* mirrorParent);
};