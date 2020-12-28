#pragma once


#include <fstream>
#include <vector>
#include "Defines.h"

#include "Memory.h"

#include "Mapper.h"
#include <stdint.h>
using namespace std;

class ROM : public MemoryModule
{	
	bool _Load(string path);

	byte& _Read(uint16_t address);
	Mapper* mapper = nullptr;
public:
	const static uint16_t ROM_ADDR = 0x4020;

	class Header
	{
	public:
		enum Regions
		{
			NTSC,
			Dual,
			PAL,
			Dual2
		};

		byte prgSize = 0;
		byte chrSize = 0;
		byte flags[5];

		bool mirroring = false;
		bool prgRam = false;
		bool trainer = false;
		bool fourScreenVRAM = false;

		byte mapper = 0;
		bool VSUnisystem = false;
		bool pc10 = false; // Play Choice 10
		bool nes20 = false;

		byte prgRamSize;

		Regions region;

		bool Load(ifstream& stream);
	};
	//vector<byte> prg;

	Header header;

	bool Load(string path);

	ROM(uint16_t minAddr = ROM_ADDR, uint16_t maxAddr = 0xFFFF, ROM* mirrorParent = nullptr)
		:MemoryModule("ROM", true /* to change mappers banks */, true, minAddr, maxAddr, mirrorParent)
	{

	}

	~ROM();
};

#include "NES.h"