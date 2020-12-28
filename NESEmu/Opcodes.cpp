#include "CPU.h"

int CPU::SEI()
{
	SetFlag(Flags::Interrupt, true);

	skipCycles = 2; // 2 - 1
	return 1;
}

void CPU::FillOpcodes()
{
	opcodes[0x78] = Instruction("SEI", &CPU::SEI);
}
