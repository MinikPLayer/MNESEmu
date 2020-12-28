#pragma once

#include "Defines.h"
#include <stdint.h>
#include <string>

using namespace std;

class NES;
class CPU
{
public:
	enum Flags
	{
		Carry = 0,
		Zero = 1,
		Interrupt = 2,
		Decimal = 4,
		B1 = 8,
		B2 = 16,
		Overflow = 32,
		Negative = 64,
	};
private:

	NES* nes;

	// registers
	byte acc = 0; // Accumulator
	byte indX = 0; // Index X
	byte indY = 0; // Index Y
	uint16_t pc = 0; // Program Counter
	byte sp = 0xFF; // Stack pointer
	byte sr = 0; // Status register

	void SetFlag(Flags flag, bool value);

	// If opcode takes more than 1 cycles (n) to execute - set this value to n
	int skipCycles = 1;

public:
	class Instruction
	{
	public:
		string opcode = "";
		int (CPU::*func)() = nullptr;

		Instruction(string opcode, int(CPU::*func)());
		Instruction();
	};

	Instruction opcodes[256];

	void Reset();
	void Tick();

	int Execute(byte instr);

	CPU(NES* nes);

	byte GetOpcode();

	void FillOpcodes(); // Fill opcodes array

	// OpCodes declaration
	int NotImplementedFunction();

	int SEI(); // 0x78 - set interrupt flag to 1
};

#include "NES.h"
