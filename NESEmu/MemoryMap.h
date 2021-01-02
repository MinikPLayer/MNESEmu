#pragma once

#include <vector>
#include "Memory.h"
using namespace std;

class MemoryMap
{
protected:
	vector<MemoryModule*> modules;

	MemoryModule* GetModule(uint16_t address);

	string mapName;
public:
	void AddModule(MemoryModule* module);

	//byte& operator[](int);
	byte Read(uint16_t address);
	bool Write(uint16_t address, byte value);

	MemoryMap(string mapName = "");
};