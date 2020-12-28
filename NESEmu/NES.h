#pragma once

#include <string>
#include <vector>
#include "Defines.h"
using namespace std;

#include "CPU.h"
#include "ROM.h"

class NES
{

public:
	class MemoryMap
	{
	protected:
		vector<MemoryModule*> modules;

	public:
		void AddModule(MemoryModule* module);

		byte& operator[](int);
	};

	//byte ReadMemory(uint16_t address);
	MemoryMap memory;

	vector<byte> RAM;
	ROM rom;
	CPU cpu;

	

	bool LoadROM(string path);

	void Run();

	NES();
};

