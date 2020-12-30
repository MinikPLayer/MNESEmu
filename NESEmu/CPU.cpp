#include "CPU.h"

void CPU::SetFlag(Flags flag, bool value)
{
	SetBit(sr, (int)flag, value);
}

bool CPU::GetFlag(Flags flag)
{
	return GetBit(sr, (int)flag);
}

void CPU::DisplayStack()
{
	for (int i = 255; i > sp && i > 0; i--)
	{
		LOGP("\t0x" << hex << i << ": " << (uint16_t)GetByte(STACK_BOTTOM + i), "Stack::Display");
	}
}

void CPU::PushToStack(byte value)
{
	//LOGP("Adding 0x" << hex << (uint16_t)value << " to stack at 0x" << hex << (uint16_t)sp, "Stack");

	SetByte(STACK_BOTTOM + sp, value);
	sp--; // descending empty stack
	//DisplayStack();
}

void CPU::Push2ToStack(uint16_t value)
{
	PushToStack(GetBits(value, 8, 8)); // Higher bytes
	PushToStack(GetBits(value, 0, 8)); // Lower bytes
}

byte CPU::PullFromStack()
{
	sp++; // descending empty stack
	
	//return GetByte(STACK_BOTTOM + sp);
	byte v = GetByte(STACK_BOTTOM + sp);
	//LOGP("Taking 0x" << hex << (uint16_t)v << " from stack at 0x" << hex << (uint16_t)sp, "Stack");
	//DisplayStack();
	return v;
}

uint16_t CPU::Pull2FromStack()
{
	//return uint16_t();
	//return (((uint16_t)PullFromStack()) << 8) + PullFromStack();
	uint16_t lower = PullFromStack();
	uint16_t higher = ((uint16_t)PullFromStack()) << 8;
	return lower + higher;
	//return (uint16_t)PullFromStack() + (((uint16_t)PullFromStack()) << 8);
}

void CPU::Reset()
{
	sp = 0xFF;
	pc = ((uint16_t)GetByteRaw(0xFFFD) << 8) + GetByteRaw(0xFFFC);
	LOGP("PC: 0x" << std::hex << pc, "CPU::Reset");
}

void CPU::Reset(uint16_t overridePC)
{
	Reset();
	pc = overridePC;
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

	if (!skipPCIncrement)
		pc++;
	else
		skipPCIncrement = false;

	skipCycles = result;
}

int CPU::Execute(byte instr)
{
	Instruction opcode = opcodes[instr];
	LOGP("Executing " << opcode.opcode << " at 0x" << hex << pc, "CPU");
	return (this->*(opcode.func))();

	LOGP("Cannot execute opcode", "CPU::Exceute");
	return -1;
}

CPU::CPU(NES* nes)
{
	this->nes = nes;

	FillOpcodes();
}

void CPU::SetReadFlags(byte val)
{
	if (val > 127) // 7-th bit is set: set negative bit to 1
		SetFlag(Negative, true);
	else if (val == 0)
		SetFlag(Zero, true);
}

CPU::Instruction::Instruction(string opcode, int(CPU::*func)())
{
	this->opcode = opcode;
	this->func = func;
}

int CPU::NotImplementedFunction()
{
	byte opcode = GetOpcode();

	LOGP("\n\nOpcode     0x" << std::hex << (int)opcode << "     not implemented\n\n", "CPU");
	
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
	return GetByteRaw(pc); //nes->memory[pc];
}

byte CPU::GetByteRaw(uint16_t address)
{
	return nes->memory.Read(address); //nes->memory[address];
}

byte CPU::GetByte(uint16_t address)
{
	byte val = GetByteRaw(address);//nes->memory[address];
	SetReadFlags(val);

	return val;
}

uint16_t CPU::Get2Bytes(uint16_t address)
{
	return GetByteRaw(address) + (GetByteRaw(address + 1) << 8);
}

void CPU::SetByte(uint16_t address, byte value)
{
	//nes->memory[address] = value;
	nes->memory.Write(address, value);
}

byte CPU::GetByte_I() { return GetByte(GetAddress_I()); }
byte CPU::GetByte_A() { return GetByte(GetAddress_A()); }
byte CPU::GetByte_D() { return GetByte(GetAddress_D()); }
byte CPU::GetByte_D_X() { return GetByte(GetAddress_D_X()); }
byte CPU::GetByte_D_Y() { return GetByte(GetAddress_D_Y()); }
byte CPU::GetByte_ID_X() { return GetByte(GetAddress_ID_X()); }
byte CPU::GetByte_ID_Y() { return GetByte(GetAddress_ID_Y()); }

uint16_t CPU::GetAddress_I() { return ++pc; }
uint16_t CPU::GetAddress_A() { return Get2Bytes((++pc)++); }
byte CPU::GetAddress_D() { return GetByteRaw(++pc); }

uint16_t CPU::GetAddress_D_X()
{
	byte arg = GetByte(++pc);
	return (arg + rx) % 256;
}

uint16_t CPU::GetAddress_D_Y()
{
	byte arg = GetByte(++pc);
	return (arg + ry) % 256;
}

uint16_t CPU::GetAddress_ID_X()
{
	byte arg = GetByte(++pc);
	return GetByte((arg + rx) % 256) + (((uint16_t)GetByte((arg + rx + 1) % 256)) << 8);
}

uint16_t CPU::GetAddress_ID_Y()
{
	byte arg = GetByte(++pc);
	return GetByte(arg) + (((uint16_t)GetByte((arg + 1) % 256)) << 8) + ry;
}
