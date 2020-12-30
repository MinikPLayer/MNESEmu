#include "CPU.h"

int CPU::SEI()
{
	SetFlag(Flags::Interrupt, true);

	return 2;
}

void CPU::LDX(byte value) { rx = value; }
void CPU::LDY(byte value) { ry = value; }
void CPU::LDA(byte value) { acc = value; }

void CPU::STX(uint16_t address) { SetByte(address, rx); }
void CPU::STY(uint16_t address) { SetByte(address, ry); }
void CPU::STA(uint16_t address) { SetByte(address, acc); }

void CPU::BIT(byte value)
{
	//uint16_t addr = Get2Bytes((++pc)++); // Getting 2 bytes so have to increment twice
	//byte v = GetByteRaw(addr);
	byte result = acc & value;

	SetFlag(Negative, GetBit(result, 7));
	SetFlag(Overflow, GetBit(result, 6));

	if (result == 0)
		SetFlag(Zero, true);
}

void CPU::JMP(uint16_t addr)
{
	pc = addr;
	skipPCIncrement = true; // Because of CPU::Exceute pc++ 
}

void CPU::CMP(byte reg, byte val)
{
	byte result = reg - val;
	SetReadFlags(result); // zero and negative
	if (reg >= val)
		SetFlag(Carry, true);
}

byte CPU::LSR(byte val)
{
	SetFlag(Carry, GetBit(val, 0));

	byte result = val >> 1;
	if (val == 0)
		SetFlag(Zero, true);
	SetFlag(Negative, false); // bit 7 is always 0

	return result;
}

byte CPU::ASL(byte val)
{
	SetFlag(Carry, GetBit(val, 7));

	byte result = val << 1;
	if (val == 0)
		SetFlag(Zero, true);
	if (GetBit(result, 7))
		SetFlag(Negative, true);

	return result;
}

byte CPU::ROR(byte val)
{
	byte result = val >> 1;
	SetBit(result, 7, GetFlag(Carry));
	SetFlag(Carry, GetBit(val, 0));

	if (GetBit(result, 7))
		SetFlag(Negative, true);

	if (val == 0)
		SetFlag(Zero, true);

	return result;
}

byte CPU::ROL(byte val)
{
	byte result = val << 1;
	SetBit(result, 0, GetFlag(Carry));
	SetFlag(Carry, GetBit(val, 7));

	if (GetBit(result, 7))
		SetFlag(Negative, true);

	if (val == 0)
		SetFlag(Zero, true);

	return result;
}

// PROBABLY WRONG
void CPU::SBC(byte v)
{
	uint16_t result = (uint16_t)acc - v + (byte)GetFlag(Carry); // It's easier to check if carry flag needs to be set when doing operation on 16 bit, rather than 8 bit

// Check if need to set carry bit
	if (result > 255)
	{
		SetFlag(Carry, false);
	}

	byte ret = (byte)result;
	if (result > 127) // 7th bit is 1
	{
		// Result is negative
		SetFlag(Negative, true);

		if (acc > 127 && v <= 127) // Both are positive and result is negative
			SetFlag(Overflow, true);
	}
	else if (acc <= 127 && v > 127) // Both are negative and result is positive
		SetFlag(Overflow, false);

	acc = ret;
}

int CPU::LDX_I()
{
	LDX(GetByte_I());//GetByte(++pc); // Immediate addressing mode
	return 2;
}

int CPU::LDX_A()
{
	LDX(GetByte_A());
	return 4;
}

int CPU::LDY_I()
{
	LDY(GetByte_I()); //GetByte(++pc);
	return 2;
}

int CPU::LDY_A()
{
	LDY(GetByte_A());
	return 4;
}

int CPU::LDA_I()
{
	LDA(GetByte_I());//GetByte(++pc);
	return 2;
}

int CPU::LDA_A()
{
	LDA(GetByte_A());
	return 4;
}

int CPU::TXS()
{
	//PushToStack(rx);
	sp = rx;
	SetReadFlags(sp);

	return 2;
}

int CPU::TSX()
{
	rx = sp;
	SetReadFlags(rx);

	return 2;
}

int CPU::INX()
{
	rx += 1;
	SetReadFlags(rx);

	return 2;
}

int CPU::INY()
{
	ry += 1;
	SetReadFlags(ry);

	return 2;
}

int CPU::DEX()
{
	rx -= 1;
	SetReadFlags(rx);
	return 2;
}

int CPU::DEY()
{
	ry -= 1;
	SetReadFlags(ry);
	return 2;
}

int CPU::STX_A()
{
	//uint16_t addr = Get2Bytes((++pc)++); // Getting 2 bytes so have to increment twice
	//SetByte(addr, rx);
	STX(GetAddress_A());
	return 4;
}

int CPU::STX_D()
{
	//byte addr = GetByteRaw(++pc);
	//SetByte(addr, rx);
	STX(GetAddress_D());
	return 3;
}

int CPU::STY_D()
{
	STY(GetAddress_D());
	return 3;
}

int CPU::STA_D()
{
	//byte addr = GetByteRaw(++pc);
	//SetByte(addr, acc);
	STA(GetAddress_D());
	return 3;
}

int CPU::BIT_A()
{
	/*uint16_t addr = Get2Bytes((++pc)++); // Getting 2 bytes so have to increment twice
	byte v = GetByteRaw(addr);
	byte result = acc & v;

	SetFlag(Negative, GetBit(result, 7));
	SetFlag(Overflow, GetBit(result, 6));

	if (result == 0)
		SetFlag(Zero, true);*/

	BIT(GetByte_A());
	return 4;
}

int CPU::BIT_D()
{
	/*byte addr = GetByteRaw(++pc);
	byte v = GetByteRaw(addr);
	byte result = acc & v;

	SetFlag(Negative, GetBit(result, 7));
	SetFlag(Overflow, GetBit(result, 6));

	if (result == 0)
		SetFlag(Zero, true);*/

	BIT(GetByte_D());
	return 4;
}

int CPU::JMP_A()
{
	//uint16_t addr = Get2Bytes((++pc)); // Getting 2 bytes so have to increment twice but don't have to because of pc assignment
	JMP(GetAddress_A());
	return 3;
}

int CPU::JSR()
{
	uint16_t addr = Get2Bytes((++pc)++);
	Push2ToStack(pc);

	pc = addr;
	skipPCIncrement = true;

	return 6;
}

int CPU::RTS()
{
	pc = Pull2FromStack();

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

int CPU::SED()
{
	SetFlag(Decimal, true);

	return 2;
}

int CPU::CLD()
{
	SetFlag(Decimal, false);

	return 2;
}

int CPU::CLV()
{
	SetFlag(Overflow, false);

	return 2;
}

int CPU::BCS()
{
	pc++; // We have to do this anyway
	if(GetFlag(Carry))
	{
		byte offset = GetByteRaw(pc);
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
		byte offset = GetByteRaw(pc);
		pc += offset;

		return 3;
	}

	return 2;
}

int CPU::BEQ()
{
	pc++; // Have to do this anyway
	if (GetFlag(Zero))
	{
		byte offset = GetByteRaw(pc);
		pc += offset;

		return 3;
	}

	return 2;
}

int CPU::BNE()
{
	pc++; // Have to do this anyway
	if (!GetFlag(Zero))
	{
		byte offset = GetByteRaw(pc);
		pc += offset;

		return 3;
	}

	return 2;
}

int CPU::BVS()
{
	pc++; // Have to do this anyway
	if (GetFlag(Overflow))
	{
		byte offset = GetByteRaw(pc);
		pc += offset;

		return 3;
	}

	return 2;
}

int CPU::BVC()
{
	pc++; // Have to do this anyway
	if (!GetFlag(Overflow))
	{
		byte offset = GetByteRaw(pc);
		pc += offset;

		return 3;
	}

	return 2;
}

int CPU::BMI()
{
	pc++; // Have to do this anyway
	if (GetFlag(Negative))
	{
		byte offset = GetByteRaw(pc);
		pc += offset;

		return 3;
	}

	return 2;
}

int CPU::BPL()
{
	pc++; // Have to do this anyway
	if (!GetFlag(Negative))
	{
		byte offset = GetByteRaw(pc);
		pc += offset;

		return 3;
	}

	return 2;
}

int CPU::PHP()
{
	PushToStack(sr);

	return 3;
}

int CPU::PLP()
{
	sr = PullFromStack();

	return 4;
}

int CPU::PHA()
{
	PushToStack(acc);

	return 3;
}

int CPU::PLA()
{
	acc = PullFromStack();

	return 4;
}

int CPU::AND_I()
{
	byte v = GetByte(++pc);
	acc = acc & v;

	return 2;
}

int CPU::ORA_I()
{
	byte v = GetByte(++pc);
	acc = acc | v;

	return 2;
}

int CPU::EOR_I()
{
	byte v = GetByte(++pc);
	acc = acc ^ v;

	return 2;
}

int CPU::CMP_I()
{
	/*byte v = GetByteRaw(++pc);
	byte result = acc - v;
	SetReadFlags(result); // zero and negative
	if (acc >= v)
		SetFlag(Carry, true);*/

	CMP(acc, GetByte_I());
	return 2;
}

int CPU::CPY_I()
{
	CMP(ry, GetByte_I());
	return 2;
}

int CPU::CPX_I()
{
	CMP(rx, GetByte_I());
	return 2;
}

int CPU::ADC_I()
{
	byte v = GetByteRaw(++pc);
	uint16_t result = (uint16_t)acc + v + (byte)GetFlag(Carry); // It's easier to check if carry flag needs to be set when doing operation on 16 bit, rather than 8 bit

	// Check if need to set carry bit
	if (result > 255)
	{
		SetFlag(Carry, true);
	}
	
	byte ret = (byte)result;
	if (result > 127) // 7th bit is 1
	{
		// Result is negative
		SetFlag(Negative, true);

		if (acc <= 127 && v <= 127) // Both are positive and result is negative
			SetFlag(Overflow, true);
	}
	else if (acc > 127 && v > 127) // Both are negative and result is positive
		SetFlag(Overflow, false);

	acc = ret;
	return 2;
}

int CPU::SBC_I()
{
	SBC(GetByte_I());
	return 2;
}

int CPU::TAX()
{
	rx = acc;
	SetReadFlags(rx);
	return 2;
}

int CPU::TAY()
{
	ry = acc;
	SetReadFlags(ry);
	return 2;
}

int CPU::TXA()
{
	acc = rx;
	SetReadFlags(acc);
	return 2;
}

int CPU::TYA()
{
	acc = ry;
	SetReadFlags(acc);
	return 2;
}

int CPU::RTI()
{
	sr = PullFromStack();
	pc = Pull2FromStack();

	skipPCIncrement = true;

	return 6;
}

int CPU::LSR_ACC()
{
	acc = LSR(acc);

	return 2;
}

int CPU::ASL_ACC()
{
	acc = ASL(acc);

	return 2;
}

int CPU::ROL_ACC()
{
	acc = ROL(acc);
	return 2;
}

int CPU::ROR_ACC()
{
	acc = ROR(acc);
	return 2;
}


void CPU::FillOpcodes()
{
	opcodes[0x08] = Instruction("PHP", &CPU::PHP);
	opcodes[0x09] = Instruction("ORA", &CPU::ORA_I);
	opcodes[0x0A] = Instruction("ASL.acc", &CPU::ASL_ACC);
	opcodes[0x10] = Instruction("BPL", &CPU::BPL);
	opcodes[0x18] = Instruction("CLC", &CPU::CLC);
	opcodes[0x20] = Instruction("JSR", &CPU::JSR);
	opcodes[0x24] = Instruction("BIT.d", &CPU::BIT_D);
	opcodes[0x28] = Instruction("PLP", &CPU::PLP);
	opcodes[0x29] = Instruction("AND.i", &CPU::AND_I);
	opcodes[0x2A] = Instruction("ROL.acc", &CPU::ROL_ACC);
	opcodes[0x2C] = Instruction("BIT.a", &CPU::BIT_A);
	opcodes[0x30] = Instruction("BMI", &CPU::BMI);
	opcodes[0x38] = Instruction("SEC", &CPU::SEC);
	opcodes[0x40] = Instruction("RTI", &CPU::RTI);
	opcodes[0x48] = Instruction("PHA", &CPU::PHA);
	opcodes[0x4A] = Instruction("LSR.acc", &CPU::LSR_ACC);
	opcodes[0x4C] = Instruction("JMP.a", &CPU::JMP_A);
	opcodes[0x49] = Instruction("EOR.i", &CPU::EOR_I);
	opcodes[0x50] = Instruction("BVC", &CPU::BVC);
	opcodes[0x60] = Instruction("RTS", &CPU::RTS);
	opcodes[0x68] = Instruction("PLA", &CPU::PLA);
	opcodes[0x69] = Instruction("ADC.i", &CPU::ADC_I);
	opcodes[0x6A] = Instruction("ROR.acc", &CPU::ROR_ACC);
	opcodes[0x70] = Instruction("BVS", &CPU::BVS);
	opcodes[0x78] = Instruction("SEI", &CPU::SEI);
	opcodes[0x84] = Instruction("STY.d", &CPU::STY_D);
	opcodes[0x85] = Instruction("STA.d", &CPU::STA_D);
	opcodes[0x86] = Instruction("STX.d", &CPU::STX_D);
	opcodes[0x88] = Instruction("DEY", &CPU::DEY);
	opcodes[0x8A] = Instruction("TXA", &CPU::TXA);
	opcodes[0x8E] = Instruction("STX.a", &CPU::STX_A);
	opcodes[0x90] = Instruction("BCC", &CPU::BCC);
	opcodes[0x98] = Instruction("TYA", &CPU::TYA);
	opcodes[0x9A] = Instruction("TXS", &CPU::TXS);
	opcodes[0xA0] = Instruction("LDY.i", &CPU::LDY_I);
	opcodes[0xA2] = Instruction("LDX.i", &CPU::LDX_I);
	opcodes[0xA8] = Instruction("TAY", &CPU::TAY);
	opcodes[0xA9] = Instruction("LDA.i", &CPU::LDA_I);
	opcodes[0xAA] = Instruction("TAX", &CPU::TAX);
	opcodes[0xAC] = Instruction("LDY.a", &CPU::LDY_A);
	opcodes[0xAD] = Instruction("LDA.a", &CPU::LDA_A);
	opcodes[0xAE] = Instruction("LDX.a", &CPU::LDX_A);
	opcodes[0xB0] = Instruction("BCS", &CPU::BCS);
	opcodes[0xBA] = Instruction("TSX", &CPU::TSX);
	opcodes[0xB8] = Instruction("CLV", &CPU::CLV);
	opcodes[0xC0] = Instruction("CPY.i", &CPU::CPY_I);
	opcodes[0xC8] = Instruction("INY", &CPU::INY);
	opcodes[0xC9] = Instruction("CMP.i", &CPU::CMP_I);
	opcodes[0xCA] = Instruction("DEX", &CPU::DEX);
	opcodes[0xD0] = Instruction("BNE", &CPU::BNE);
	opcodes[0xD8] = Instruction("CLD", &CPU::CLD);
	opcodes[0xE0] = Instruction("CPX.i", &CPU::CPX_I);
	opcodes[0xE8] = Instruction("INX", &CPU::INX);
	opcodes[0xE9] = Instruction("SBC.i", &CPU::SBC_I);
	opcodes[0xEA] = Instruction("NOP", &CPU::NOP);
	opcodes[0xF0] = Instruction("BEQ", &CPU::BEQ);
	opcodes[0xF8] = Instruction("SED", &CPU::SED);
}
