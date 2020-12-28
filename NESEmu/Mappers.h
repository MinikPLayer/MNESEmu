#pragma once

#include "Mapper.h"
#include "Util.h"

#include "ROM.h"
#include "Mappers.h"


// NROM
class Mapper0 : public Mapper
{
public:
	Mapper0() : Mapper(0) {}

	byte& Read(uint16_t address)
	{
		uint16_t absolute = address + ROM::ROM_ADDR; // because of address mapping ( addr - minAddr ) 
		
		if (IsBetween(absolute, 0x8000, 0xFFFF))
		{
			uint16_t addr = absolute - 0x8000;
			//LOGP("Reading from memory at 0x" << std::hex << absolute << " - relative address is 0x" << std::hex << addr, "Mapper " << id);
			if (addr >= prg.size()) // if 16KB PRG is loaded and trying to load 32KB address space
			{
				addr -= prg.size();
				if (addr >= prg.size())
				{
					LOGP("Trying to read memory outside of the PRG memory at: " << (absolute - 0x8000) , "Mapper" << id << "::Read");
					throw exception("MapperBadRead");
					return MemoryModule::nullValue;
				}
			}
			//LOGP("Final relative read address: 0x" << std::hex << addr, "Mapper " << id);
#if LOG_MEM == true
			LOGP("Reading from 0x" << hex << absolute << ": 0x" << hex << (int)prg[addr], "Mapper" << id << "::Read");
#endif
			return prg[addr];
		}

		LOGP("Trying to read unmapped mapper memory at " << absolute - 0x8000, "Mapper" << id << "::Read");
		throw exception("MapperBadRead");
		return MemoryModule::nullValue;
	}
};

Mapper* GetMapper(int id)
{
	switch (id)
	{
	case 0:
		return new Mapper0();

	default:
		LOGP("Mapper " << id << " not implemented!!!", "GetMapper");
		throw exception("Mapper not implemented");
		return nullptr;
	}
}

/*Mapper* GetMapper(int id)
{
	switch (id)
	{
	case 0:
		return new Mapper0();

	default:
		LOGP("Mapper " << id << " not implemented!!!", "GetMapper");
		throw exception("Mapper not implemented");
		return nullptr;
	}
}*/