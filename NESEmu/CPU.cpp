#include "CPU.h"

void CPU::SetFlag(Flags flag, bool value)
{
	SetBit(sr, (int)flag, value);
}

void CPU::Reset()
{
	pc = ((uint16_t)(nes->memory[0xFFFD]) << 8) + nes->memory[0xFFFC];
	LOGP("PC: 0x" << std::hex << pc, "CPU::Reset");
}

void CPU::Tick()
{
	// Skip cycles, we have to wait
	if (skipCycles > 1)
	{
		skipCycles--;
		return;
	}

	byte instr = GetOpcode();

	int result = Execute(instr);
	if (result == -1) return;

	pc += result;
}

int CPU::Execute(byte instr)
{
	Instruction opcode = opcodes[instr];
	LOGP("Executing " << opcode.opcode, "CPU");
	return (this->*(opcode.func))();

	LOGP("Cannot execute opcode", "CPU::Exceute");
	return -1;
}

CPU::CPU(NES* nes)
{
	this->nes = nes;

	FillOpcodes();
}

CPU::Instruction::Instruction(string opcode, int(CPU::*func)())
{
	this->opcode = opcode;
	this->func = func;
}

int CPU::NotImplementedFunction()
{
	byte opcode = GetOpcode();

	LOGP("Opcode 0x" << std::hex << (int)opcode << " not implemented", "CPU");
	throw exception("Opcode not implemented");
	return 0;
}

CPU::Instruction::Instruction()
{
	this->opcode = "NOT IMPLEMENTED";
	this->func = &NotImplementedFunction;
}

byte CPU::GetOpcode()
{
	return nes->memory[pc];
}