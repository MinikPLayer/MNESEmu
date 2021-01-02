#include "NES.h"

#include <fstream>
#include <iostream>

#include "Util.h"
#include <chrono>

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

	while (cpu.running)
	{
		auto begin = std::chrono::high_resolution_clock::now();
		// This linearity will be fixed later
		cpu.Tick();
		for (int i = 0; i < 3; i++) // PPU ticks 3x faster than CPU
			ppu.Tick(); 
		
		while (true)
		{
			auto end = std::chrono::high_resolution_clock::now();
			if (chrono::duration_cast<chrono::nanoseconds>(end - begin).count() > 558)
				break;
		}
	}
}

NES::NES()
	:cpu(this), memory("CPU")
{
	memory.AddModule(&rom);
	memory.AddModule(&ram);
	//memory.AddModule(&ppu.reg);
	memory.AddModule(&ppu);
	memory.AddModule(&apu.reg);
}

static byte nullValue;


