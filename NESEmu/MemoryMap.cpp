#include "MemoryMap.h"

MemoryModule* MemoryMap::GetModule(uint16_t address)
{
	if (IsBetween(address, 0x2008, 0x3FFF)) // PPU mirrors
	{
		// Redirect to PPU directly
		address -= 0x2000;
		address %= 8;
		address += 0x2000;
	}

	for (int i = 0; i < modules.size(); i++)
	{
		if (modules[i] == nullptr)
		{
			modules.erase(modules.begin() + i, modules.begin() + i + 1);
			i--;
			continue;
		}

		if (IsBetween(address, modules[i]->minAddr, modules[i]->maxAddr))
		{
			return modules[i];
		}
	}


	LOGP("Trying to access unmapped memory at 0x" << std::hex << address, "MemoryMap - " << mapName);
	throw exception("UnmappedMemoryAccess");

	return nullptr;
}

void MemoryMap::AddModule(MemoryModule* module)
{
	modules.push_back(module);
}

/*byte& NES::MemoryMap::operator[](int n)
{
	// TODO: tu wstawiæ instrukcjê return
	for (int i = 0; i < modules.size(); i++)
	{
		if (modules[i] == nullptr)
		{
			modules.erase(modules.begin() + i, modules.begin() + i + 1);
			i--;
			continue;
		}

		if (IsBetween(n, modules[i]->minAddr, modules[i]->maxAddr))
		{
			return modules[i]->Read(n - modules[i]->minAddr);
		}
	}

	LOGP("Trying to access unmapped memory at " << std::hex << n, "NES::MemoryMap");
	throw exception("UnmappedMemoryAccess");

	return nullValue;
}*/

byte MemoryMap::Read(uint16_t address)
{
	MemoryModule* module = GetModule(address);
	if (module == nullptr)
		return 0;

	return module->Read(address - module->minAddr);
}

bool MemoryMap::Write(uint16_t address, byte value)
{
	MemoryModule* module = GetModule(address);
	if (module == nullptr)
		return 0;

	return module->Write(address - module->minAddr, value);
}

MemoryMap::MemoryMap(string mapName)
{
	this->mapName = mapName;
}
