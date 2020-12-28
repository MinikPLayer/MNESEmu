#pragma once

#include <stdint.h>
#include <vector>

using namespace std;
#include "Util.h"

class Mapper
{
public:
	int id;
	vector<byte> prg;
	vector<byte> chr;

	virtual byte& Read(uint16_t address) { LOGP("Mapper read not implemented!", "Mapper " << id); return MemoryModule::nullValue; }
	virtual bool Write(uint16_t address) { LOGP("Mapper write not implemented!", "Mapper: " << id); return false; }


	Mapper(int id)
	{
		this->id = id;
	}
};

