#include "CPU.h"

int CPU::SEI()
{
	SetFlag(Flags::Interrupt, true);

	return 2;
}

int CPU::LDX_I()
{
	rx = GetByte(++pc); // Immediate addressing mode

	return 2;
}

int CPU::TXS()
{
	PushToStack(rx);


	return 2;
}

int CPU::INX()
{
	SetReadFlags(rx);
	rx += 1;

	return 2;
}

int CPU::STX_A()
{
	uint16_t addr = Get2Bytes((++pc)++); // Getting 2 bytes so have to increment twice
	SetByte(addr, rx);

	return 4;
}

int CPU::STX_D()
{
	byte addr = GetByteRaw(++pc);
	SetByte(addr, rx);

	return 3;
}

int CPU::BIT_A()
{
	uint16_t addr = Get2Bytes((++pc)++); // Getting 2 bytes so have to increment twice
	byte v = GetByteRaw(addr);
	byte result = acc & v;

	SetFlag(Negative, GetBit(result, 7));
	SetFlag(Overflow, GetBit(result, 6));

	if (result == 0)
		SetFlag(Zero, true);

	return 4;
}

int CPU::JMP_A()
{
	uint16_t addr = Get2Bytes((++pc)); // Getting 2 bytes so have to increment twice but don't have to because of pc assignment
	pc = addr;

	skipPCIncrement = true; // Because of CPU::Exceute pc++ 
	return 3;
}

int CPU::JSR()
{
	uint16_t addr = Get2Bytes((++pc)++);
	PushToStack(pc);

	pc = addr;
	skipPCIncrement = true;

	return 6;
}

int CPU::NOP()
{
	return 2;
}

int CPU::SEC()
{
	SetFlag(Carry, true);

	return 2;
}

int CPU::CLC()
{
	SetFlag(Carry, false);

	return 2;
}

int CPU::BCS()
{
	pc++; // We have to do this anyway
	if(GetFlag(Carry))
	{
		byte offset = GetByte(pc);
		pc += offset;

		return 3;
	}
	return 2;
}

int CPU::BCC()
{
	pc++; // We have to do this anyway
	if (!GetFlag(Carry))
	{
		byte offset = GetByte(pc);
		pc += offset;

		return 3;
	}

	return 2;
}

void CPU::FillOpcodes()
{
	opcodes[0x18] = Instruction("CLC", &CPU::CLC);
	opcodes[0x20] = Instruction("JSR", &CPU::JSR);
	opcodes[0x2C] = Instruction("BIT.a", &CPU::BIT_A);
	opcodes[0x38] = Instruction("SEC", &CPU::SEC);
	opcodes[0x4C] = Instruction("JMP.a", &CPU::JMP_A);
	opcodes[0x78] = Instruction("SEI", &CPU::SEI);
	opcodes[0x86] = Instruction("STX.d", &CPU::STX_D);
	opcodes[0x90] = Instruction("BCC", &CPU::BCC);
	opcodes[0x8E] = Instruction("STX.a", &CPU::STX_A);
	opcodes[0x9A] = Instruction("TXS", &CPU::TXS);
	opcodes[0xA2] = Instruction("LDX.I", &CPU::LDX_I);
	opcodes[0xB0] = Instruction("BCS", &CPU::BCS);
	opcodes[0xE8] = Instruction("INX", &CPU::INX);
	opcodes[0xEA] = Instruction("NOP", &CPU::NOP);
}
