#pragma once

#include <string>
#include <vector>
#include "Defines.h"
#include "MemoryMap.h"
using namespace std;

#pragma region Modules Includes
	#include "CPU.h"
	#include "ROM.h"
	#include "RAM.h"
	#include "PPU.h"
	#include "APU.h"
#pragma endregion


class NES
{

public:


	//byte ReadMemory(uint16_t address);
	MemoryMap memory;

#pragma region Modules
	RAM ram;
	ROM rom;
	CPU cpu;
	PPU ppu;
	APU apu;
#pragma endregion



	bool LoadROM(string path);

	void Run(uint16_t overridePC = 0);

	NES();
};

