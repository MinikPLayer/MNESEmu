#include "NES.h"

#include <fstream>
#include <iostream>

#include "Util.h"

bool NES::LoadROM(string path)
{
	return rom.Load(path);
}

void NES::Run(uint16_t overridePC)
{
	if (overridePC != 0)
		cpu.Reset(overridePC);
	else
		cpu.Reset();

	/* LOGP("DEBUG!", "NES::Run");
	LOGP("Value at 0xFFFC: " << memory[0xFFFC], "NES::Run");
	LOGP("Value at 0xFFFD: " << memory[0xFFFD], "NES::Run");
	LOGP("Value at 0x0000: " << memory[0x0000], "NES::Run");*/

	while (true)
	{
		cpu.Tick();
	}
}

NES::NES()
	:cpu(this)
{
	memory.AddModule(&rom);
	memory.AddModule(&ram);
	memory.AddModule(&ppu.reg);
	memory.AddModule(&apu.reg);
}

static byte nullValue;

void NES::MemoryMap::AddModule(MemoryModule* module)
{
	modules.push_back(module);
}

byte& NES::MemoryMap::operator[](int n)
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
}
