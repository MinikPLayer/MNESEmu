#pragma once

#include "Memory.h"
#include "MemoryMap.h"
#include "PatternTable.h"
#include <stdint.h>


class PPU : public MemoryModule
{
	bool _Write(uint16_t address, byte value);
	byte _Read(uint16_t address);

	byte reg[8];

	MemoryMap memory;

	// Internal registers
	uint16_t VRAMaddress = 0;
	bool firstWrite = false;

	PatternTable patternTables[2] = { PatternTable(0x0000, 0x0FFF), PatternTable(0x1000, 0x1FFF) };
public:
	/*class PPUReg
	{
		byte value;
		bool writeable;
		bool readable;
	public:

		PPUReg(string name, bool writeable, bool readable, uint16_t minAddr = 0x2000, uint16_t maxAddr = 0x2007, PPUReg* mirrorParent = nullptr)
		{

		}
	};*/

	bool WriteReg(int n, byte value);
	byte ReadReg(int n);

	enum Flags_CTRL
	{
		Nametable1 = 0,
		Namteable2 = 1,
		VRAMIncrementMode = 2,
		SpriteTileSelect = 3,
		BGTileSelect = 4,
		SpriteHeight = 5,
		MasterSlave = 6,
		NMIEnabled = 7
	};
	enum Flags_Mask
	{
		Greyscale = 0,
		BGLCol = 1,
		SpriteLCol = 2,
		BGEnabled = 3,
		SpriteEnable = 4,
		ColorEmphasisR = 5,
		ColorEmphasisG = 6,
		ColorEmphasisB = 7
	};
	enum Flags_Status
	{
		SpriteOverflow = 5,
		Sprite0Hit = 6,
		VBlank = 7
	};

	/*PPUReg reg[8] = { 
		PPUReg("PPUCTRL", true, false), 
		PPUReg("PPUMASK", true, false),
		PPUReg("PPUSTATUS", false, true),
		PPUReg("OAMADDR", true, false),
		PPUReg("OAMDATA", true, true),
		PPUReg("PPUSCROLL", true, false),
		PPUReg("PPUADDR", true, false),
		PPUReg("PPUDATA", true, true)
	};*/
	
	// Sets a flag, reg - reg number, p - bit to change, value - new value
	void SetFlag(int reg, int p, bool value);
	void SetFlag(Flags_CTRL flag, bool value);
	void SetFlag(Flags_Mask flag, bool value);
	void SetFlag(Flags_Status flag, bool value);

	bool GetFlag(int reg, int p);
	bool GetFlag(Flags_CTRL flag);
	bool GetFlag(Flags_Mask flag);
	bool GetFlag(Flags_Status flag);

	//PPUReg reg;

	PPU(uint16_t minAddr = 0x2000, uint16_t maxAddr = 0x2007, PPU* mirrorParent = nullptr);

	void Tick();

	void Init();
	void Reset();
};