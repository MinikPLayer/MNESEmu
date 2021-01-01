#include "CPU.h"

int CPU::SEI()
{
	SetFlag(Flags::Interrupt, true);

	return 2;
}

void CPU::LDX(byte value) 
{
	//SetReadFlags(rx);
	rx = value;
}
void CPU::LDY(byte value) 
{	
	//SetReadFlags(ry);
	ry = value; 
}
void CPU::LDA(byte value) 
{ 
	//SetReadFlags(acc);
	acc = value; 
}

void CPU::STX(uint16_t address) 
{
	SetByte(address, rx); 
}
void CPU::STY(uint16_t address) 
{ 
	SetByte(address, ry); 
}
void CPU::STA(uint16_t address) 
{ 
	SetByte(address, acc); 
}

void CPU::BIT(byte value)
{
	//uint16_t addr = Get2Bytes((++pc)++); // Getting 2 bytes so have to increment twice
	//byte v = GetByteRaw(addr);
	byte result = acc & value;

	SetFlag(Negative, GetBit(value, 7));
	SetFlag(Overflow, GetBit(value, 6));

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
	/*if (reg >= val)
		SetFlag(Carry, true);*/
	SetFlag(Carry, reg >= val);
}


void CPU::ADC(byte v)
{
	uint16_t result = (uint16_t)acc + v + (byte)GetFlag(Carry); // It's easier to check if carry flag needs to be set when doing operation on 16 bit, rather than 8 bit

	// Check if need to set carry bit
	if (result > 255)
		SetFlag(Carry, true);
	else
		SetFlag(Carry, false);

	byte ret = (byte)result;
	if (ret > 127) // 7th bit is 1
	{
		// Result is negative
		SetFlag(Negative, true);

		if (acc <= 127 && v <= 127) // Both are positive and result is negative
			SetFlag(Overflow, true);
		else
			SetFlag(Overflow, false);
	}
	else
	{
		SetFlag(Negative, false);
		if (acc > 127 && v > 127) // Both are negative and result is positive
			SetFlag(Overflow, true);
		else
			SetFlag(Overflow, false);
	}

	acc = ret;
	SetFlag(Zero, acc == 0);
}

byte CPU::LSR(byte val)
{
	SetFlag(Carry, GetBit(val, 0));

	byte result = val >> 1;
	/*if (result == 0)
		SetFlag(Zero, true);*/
	SetFlag(Zero, result == 0);
	SetFlag(Negative, false); // bit 7 is always 0

	return result;
}

byte CPU::ASL(byte val)
{
	SetFlag(Carry, GetBit(val, 7));

	byte result = val << 1;
	/*if (val == 0)
		SetFlag(Zero, true);
	if (GetBit(result, 7))
		SetFlag(Negative, true);*/
	SetFlag(Zero, result == 0);
	SetFlag(Negative, GetBit(result, 7));

	return result;
}

byte CPU::ROR(byte val)
{
	byte result = val >> 1;
	SetBit(result, 7, GetFlag(Carry));
	SetFlag(Carry, GetBit(val, 0));

	/*if (GetBit(result, 7))
		SetFlag(Negative, true);

	if (val == 0)
		SetFlag(Zero, true);*/
	SetFlag(Negative, GetBit(result, 7));
	SetFlag(Zero, val == 0);

	return result;
}

byte CPU::ROL(byte val)
{
	byte result = val << 1;
	SetBit(result, 0, GetFlag(Carry));
	SetFlag(Carry, GetBit(val, 7));

	/*if (GetBit(result, 7))
		SetFlag(Negative, true);

	if (val == 0)
		SetFlag(Zero, true);*/
	SetFlag(Negative, GetBit(result, 7));
	SetFlag(Zero, val == 0);

	return result;
}

void CPU::DEC(uint16_t addr)
{
	byte v = GetByteRaw(addr) - 1;
	SetReadFlags(v);
	SetByte(addr, v);
}

void CPU::INC(uint16_t addr)
{
	byte v = GetByteRaw(addr) + 1;
	SetReadFlags(v);
	SetByte(addr, v);
}

void CPU::AND(byte val)
{
	acc = acc & val;
	SetReadFlags(acc);
}

void CPU::ORA(byte val)
{
	acc = acc | val;
	SetReadFlags(acc);
}

void CPU::EOR(byte val)
{
	acc = acc ^ val;
	SetReadFlags(acc);
}

// PROBABLY WRONG
void CPU::SBC(byte v)
{
	uint16_t result = (uint16_t)acc - v - (byte)(!GetFlag(Carry)); // It's easier to check if carry flag needs to be set when doing operation on 16 bit, rather than 8 bit

	/*// Check if need to set carry bit
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

	acc = ret;*/
	/*if (acc == 0)
		SetFlag(Zero, true);
	else
		SetFlag(Zero, false);*/

	
	//SetFlag(Negative, result > 255); // If result is under 0, then it's FFFF - x, so > 255

	/*if (result > 255)
		SetFlag(Carry, false);
	else
		SetFlag(Carry, true);*/
	SetFlag(Carry, !(result > 255));

	byte ret = (byte)result;
	//SetFlag(Overflow, result > 255);
	if (ret > 127) // 7th bit is 1
	{
		// Result is negative
		SetFlag(Negative, true);

		if (acc <= 127 && v > 127) // + - (-) = -: overflow
			SetFlag(Overflow, true);
		else
			SetFlag(Overflow, false);
	}
	else
	{
		SetFlag(Negative, false);
		if (acc > 127 && v <= 127) // - - (+) = +: overflow
			SetFlag(Overflow, true);
		else
			SetFlag(Overflow, false);
	}
	//SetFlag(Negative, GetBit(ret, 7));
	SetFlag(Zero, ret == 0);
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

int CPU::LDX_A_Y()
{
	LDX(GetByte_A_Y());
	return 4;
}

int CPU::LDX_D()
{
	LDX(GetByte_D());
	return 3;
}

int CPU::LDX_D_Y()
{
	LDX(GetByte_D_Y());
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

int CPU::LDY_A_X()
{
	LDY(GetByte_A_X());
	return 4;
}

int CPU::LDY_D()
{
	LDY(GetByte_D());
	return 3;
}

int CPU::LDY_D_X()
{
	LDY(GetByte_D_X());
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

int CPU::LDA_A_Y()
{
	if (pc == 0xDF60)
	{
		uint16_t addr = GetAddress_A_Y();
		byte v = GetByte(addr);
		LDA(v);
		return 5;
	}
	LDA(GetByte_A_Y());
	return 5;
}

int CPU::LDA_A_X()
{
	LDA(GetByte_A_X());
	return 5;
}

int CPU::LDA_D()
{
	LDA(GetByte_D());
	return 3;
}

int CPU::LDA_D_X()
{
	LDA(GetByte_D_X());
	return 4;
}

int CPU::LDA_ID_X()
{
	LDA(GetByte_ID_X());
	return 6;
}

int CPU::LDA_ID_Y()
{

	LDA(GetByte_ID_Y());
	return 5;
}

int CPU::TXS()
{
	//PushToStack(rx);
	sp = rx;
	//SetReadFlags(sp);

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

int CPU::INC_D()
{
	/*byte addr = GetAddress_D();
	byte v = GetByteRaw(addr) + 1;
	SetReadFlags(v);
	SetByte(addr, v);*/
	INC(GetAddress_D());

	return 5;
}

int CPU::INC_D_X()
{
	INC(GetAddress_D_X());
	return 6;
}

int CPU::INC_A()
{
	INC(GetAddress_A());
	return 6;
}

int CPU::INC_A_X()
{
	INC(GetAddress_A_X());
	return 7;
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

int CPU::DEC_D()
{
	/*byte addr = GetAddress_D();
	byte v = GetByteRaw(addr) - 1;
	SetReadFlags(v);
	SetByte(addr, v);*/
	DEC(GetAddress_D());
	return 5;
}

int CPU::DEC_D_X()
{
	DEC(GetAddress_D_X());
	return 6;
}

int CPU::DEC_A()
{
	DEC(GetAddress_A());
	return 6;
}

int CPU::DEC_A_X()
{
	DEC(GetAddress_A_X());
	return 7;
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

int CPU::STX_D_Y()
{
	STX(GetAddress_D_Y());
	return 4;
}

int CPU::STY_A()
{
	STY(GetAddress_A());
	return 4;
}

int CPU::STY_D()
{
	STY(GetAddress_D());
	return 3;
}


int CPU::STY_D_X()
{
	STY(GetAddress_D_X());
	return 4;
}

int CPU::STA_A()
{
	STA(GetAddress_A());
	return 4;
}

int CPU::STA_A_Y()
{
	STA(GetAddress_A_Y());
	return 5;
}

int CPU::STA_A_X()
{
	STA(GetAddress_A_X());
	return 5;
}

int CPU::STA_D()
{
	//byte addr = GetByteRaw(++pc);
	//SetByte(addr, acc);
	STA(GetAddress_D());
	return 3;
}

int CPU::STA_D_X()
{
	STA(GetAddress_D_X());
	return 4;
}

int CPU::STA_ID_X()
{
	STA(GetAddress_ID_X()); //GetByteRaw_ID_X());
	return 6;
}

int CPU::STA_ID_Y()
{
	STA(GetAddress_ID_Y());//GetByteRaw_ID_Y());
	return 5;
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

int CPU::JMP_ID()
{
	uint16_t arg = Get2Bytes((++pc)++); // memory address to take jump address from

	//LOGP("Address: 0x" << hex << arg, "Opcodes::JMP_ID");
	byte lower = (byte)arg;
	// End of the page - there is a 6502 bug
	if (lower == 0xFF)
		JMP(GetByteRaw(arg) + (GetByteRaw(arg - 0xFF) << 8));
	else
		JMP(Get2Bytes(arg));

	return 5;
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
	pc = Pull2FromStack(false);

	return 6;
}

int CPU::NOP()
{
	return 2;
}

int CPU::NOP_I()
{
	GetByteRaw_I(); // Read a byte and do nothing
	return 2;
}

int CPU::NOP_A()
{
	GetByteRaw_A(); // Read a byte from the memory and do nothing
	return 4;
}

int CPU::NOP_A_X()
{
	GetByteRaw_A_X(); // Read a byte from the memory and do nothing
	return 5;
}

int CPU::NOP_D()
{
	GetByteRaw_D(); // Read a byte from the memory and do nothing
	return 3;
}

int CPU::NOP_D_X()
{
	GetByteRaw_D_X(); // Read a byte from the memory and do nothing
	return 4;
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
		pc += (char)offset; // signed byte

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
	byte val = sr;
	SetBit(val, 4, true);
	SetBit(val, 5, true);
	PushToStack(val);

	return 3;
}

int CPU::PLP()
{
	byte val = PullFromStack();

	SetBit(val, 4, GetBit(sr, 4));
	SetBit(val, 5, GetBit(sr, 5));
	sr = val;
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
	SetReadFlags(acc);

	return 4;
}

int CPU::AND_I()
{
	/*byte v = GetByte(++pc);
	acc = acc & v;*/
	AND(GetByte_I());

	return 2;
}

int CPU::AND_D()
{
	//byte v = GetByte_D();
	//acc = acc & v;
	AND(GetByte_D());
	return 3;
}

int CPU::AND_D_X()
{
	//byte v = GetByte_D_X();
	//acc = acc & v;
	AND(GetByte_D_X());
	return 4;
}

int CPU::AND_A()
{
	//byte v = GetByte_A();
	//acc = acc & v;
	AND(GetByte_A());
	return 4;
}

int CPU::AND_A_Y()
{
	//byte v = GetByte_A_Y();
	//acc = acc & v;
	AND(GetByte_A_Y());
	return 5;
}

int CPU::AND_A_X()
{
	//byte v = GetByte_A_X();
	//acc = acc & v;
	AND(GetByte_A_X());
	return 5;
}

int CPU::AND_ID_X()
{
	//byte v = GetByte_ID_X();
	//acc = acc & v;
	AND(GetByte_ID_X());
	return 6;
}

int CPU::AND_ID_Y()
{
	//byte v = GetByte_ID_Y();
	//acc = acc & v;
	AND(GetByte_ID_Y());
	return 5;
}

int CPU::ORA_I()
{
	//byte v = GetByte(++pc);
	//acc = acc | v;
	ORA(GetByte_I());
	return 2;
}

int CPU::ORA_D()
{
	//byte v = GetByte_D();
	//acc = acc | v;
	ORA(GetByte_D());
	return 3;
}

int CPU::ORA_D_X()
{
	//byte v = GetByte_D_X();
	//acc = acc | v;
	ORA(GetByte_D_X());
	return 4;
}

int CPU::ORA_A()
{
	//byte v = GetByte_A();
	//acc = acc | v;
	ORA(GetByte_A());
	return 4;
}

int CPU::ORA_A_Y()
{
	//byte v = GetByte_A_Y();
	//acc = acc | v;
	ORA(GetByte_A_Y());
	return 5;
}

int CPU::ORA_A_X()
{
	//byte v = GetByte_A_X();
	//acc = acc | v;
	if (pc == 0xE20B)
	{
		uint16_t addr = GetAddress_A_X();
		byte v = GetByte(addr);
		ORA(v);
		return 5;
	}
	ORA(GetByte_A_X());
	return 5;
}

int CPU::ORA_ID_X()
{
	//byte v = GetByte_ID_X();
	//acc = acc | v;
	ORA(GetByte_ID_X());
	return 6;
}

int CPU::ORA_ID_Y()
{
	//byte v = GetByte_ID_Y();
	//acc = acc | v;
	ORA(GetByte_ID_Y());
	return 5;
}

int CPU::EOR_I()
{
	//byte v = GetByte(++pc);
	//acc = acc ^ v;
	EOR(GetByte_I());
	return 2;
}

int CPU::EOR_D()
{
	//byte v = GetByte_D();
	//acc = acc ^ v;
	EOR(GetByte_D());
	return 3;
}

int CPU::EOR_D_X()
{
	//byte v = GetByte_D_X();
	//acc = acc ^ v;
	EOR(GetByte_D_X());
	return 4;
}

int CPU::EOR_A()
{
	//byte v = GetByte_A();
	//acc = acc ^ v;
	EOR(GetByte_A());
	return 4;
}

int CPU::EOR_A_Y()
{
	//byte v = GetByte_A_Y();
	//acc = acc ^ v;
	EOR(GetByte_A_Y());
	return 5;
}

int CPU::EOR_A_X()
{
	//byte v = GetByte_A_X();
	//acc = acc ^ v;
	EOR(GetByte_A_X());
	return 5;
}

int CPU::EOR_ID_X()
{
	//byte v = GetByte_ID_X();
	//acc = acc ^ v;
	EOR(GetByte_ID_X());
	return 6;
}

int CPU::EOR_ID_Y()
{
	//byte v = GetByte_ID_Y();
	//acc = acc ^ v;
	EOR(GetByte_ID_Y());
	return 5;
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

int CPU::CMP_D()
{
	CMP(acc, GetByte_D());
	return 3;
}

int CPU::CMP_D_X()
{
	CMP(acc, GetByte_D_X());
	return 4;
}

int CPU::CMP_A()
{
	CMP(acc, GetByte_A());
	return 4;
}

int CPU::CMP_A_Y()
{
	CMP(acc, GetByte_A_Y());
	return 5;
}

int CPU::CMP_A_X()
{
	CMP(acc, GetByte_A_X());
	return 5;
}

int CPU::CMP_ID_X()
{
	CMP(acc, GetByte_ID_X());
	return 6;
}

int CPU::CMP_ID_Y()
{
	CMP(acc, GetByte_ID_Y());
	return 5;
}

int CPU::CPY_I()
{
	CMP(ry, GetByte_I());
	return 2;
}

int CPU::CPY_D()
{
	CMP(ry, GetByte_D());
	return 3;
}

int CPU::CPY_A()
{
	CMP(ry, GetByte_A());
	return 4;
}

int CPU::CPX_I()
{
	CMP(rx, GetByte_I());
	return 2;
}

int CPU::CPX_D()
{
	CMP(rx, GetByte_D());
	return 3;
}

int CPU::CPX_A()
{
	CMP(rx, GetByte_A());
	return 4;
}

int CPU::ADC_I()
{
	/*byte v = GetByteRaw(++pc);
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

	acc = ret;*/
	ADC(GetByte_I());

	return 2;
}

int CPU::ADC_D()
{
	ADC(GetByte_D());
	return 3;
}

int CPU::ADC_D_X()
{
	ADC(GetByte_D_X());
	return 4;
}

int CPU::ADC_A()
{
	ADC(GetByte_A());
	return 4;
}

int CPU::ADC_A_Y()
{
	ADC(GetByte_A_Y());
	return 5;
}

int CPU::ADC_A_X()
{
	ADC(GetByte_A_X());
	return 5;
}

int CPU::ADC_ID_X()
{
	ADC(GetByte_ID_X());

	return 6;
}

int CPU::ADC_ID_Y()
{
	ADC(GetByte_ID_Y());
	return 5;
}

int CPU::SBC_I()
{
	SBC(GetByte_I());
	return 2;
}

int CPU::SBC_D()
{
	SBC(GetByte_D());
	return 3;
}

int CPU::SBC_D_X()
{
	SBC(GetByte_D_X());
	return 4;
}

int CPU::SBC_A()
{
	SBC(GetByte_A());
	return 4;
}

int CPU::SBC_A_Y()
{
	SBC(GetByte_A_Y());
	return 5;
}

int CPU::SBC_A_X()
{
	if (pc == 0xE30F)
	{
		uint16_t addr = GetAddress_A_X();
		byte v = GetByte(addr);
		SBC(v);
		return 5;
	}
	SBC(GetByte_A_X());
	return 5;
}

int CPU::SBC_ID_X()
{
	SBC(GetByte_ID_X());
	return 6;
}

int CPU::SBC_ID_Y()
{
	SBC(GetByte_ID_Y());
	return 5;
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
	byte v = PullFromStack(false); // Because we have to ignore 4 and 5 bits
	SetBit(v, 4, GetBit(sr, 4));
	SetBit(v, 5, GetBit(sr, 5));
	sr = v;
	pc = Pull2FromStack(false);

	skipPCIncrement = true;

	return 6;
}

int CPU::LSR_ACC()
{
	acc = LSR(acc);

	return 2;
}

int CPU::LSR_D()
{
	//acc = LSR(acc);
	uint16_t address = GetAddress_D();
	SetByte(address, LSR(GetByteRaw(address)));
	return 5;
}

int CPU::LSR_D_X()
{
	uint16_t address = GetAddress_D_X();
	SetByte(address, LSR(GetByteRaw(address)));
	return 6;
}

int CPU::LSR_A()
{
	uint16_t address = GetAddress_A();
	SetByte(address, LSR(GetByteRaw(address)));
	return 6;
}

int CPU::LSR_A_X()
{
	uint16_t address = GetAddress_A_X();
	SetByte(address, LSR(GetByteRaw(address)));
	return 7;
}

int CPU::ASL_ACC()
{
	acc = ASL(acc);

	return 2;
}

int CPU::ASL_D()
{
	uint16_t addr = GetAddress_D();
	SetByte(addr, ASL(GetByteRaw(addr)));
	return 3;
}

int CPU::ASL_D_X()
{
	uint16_t addr = GetAddress_D_X();
	SetByte(addr, ASL(GetByteRaw(addr)));
	return 6;
}

int CPU::ASL_A()
{
	uint16_t addr = GetAddress_A();
	SetByte(addr, ASL(GetByteRaw(addr)));
	return 6;
}

int CPU::ASL_A_X()
{
	uint16_t addr = GetAddress_A_X();
	SetByte(addr, ASL(GetByteRaw(addr)));
	return 7;
}

int CPU::ROL_ACC()
{
	acc = ROL(acc);
	return 2;
}

int CPU::ROL_D()
{
	uint16_t address = GetAddress_D();
	SetByte(address, ROL(GetByteRaw(address)));
	return 5;
}

int CPU::ROL_D_X()
{
	uint16_t address = GetAddress_D_X();
	SetByte(address, ROL(GetByteRaw(address)));
	return 6;
}

int CPU::ROL_A()
{
	uint16_t address = GetAddress_A();
	SetByte(address, ROL(GetByteRaw(address)));
	return 6;
}

int CPU::ROL_A_X()
{
	uint16_t address = GetAddress_A_X();
	SetByte(address, ROL(GetByteRaw(address)));
	return 7;
}

int CPU::ROR_ACC()
{
	acc = ROR(acc);
	return 2;
}

int CPU::ROR_D()
{
	uint16_t address = GetAddress_D();
	SetByte(address, ROR(GetByteRaw(address)));
	return 5;
}

int CPU::ROR_D_X()
{
	uint16_t address = GetAddress_D_X();
	SetByte(address, ROR(GetByteRaw(address)));
	return 6;
}

int CPU::ROR_A()
{
	uint16_t address = GetAddress_A();
	SetByte(address, ROR(GetByteRaw(address)));
	return 6;
}

int CPU::ROR_A_X()
{
	uint16_t address = GetAddress_A_X();
	SetByte(address, ROR(GetByteRaw(address)));
	return 7;
}


int CPU::STP()
{
	LOGP("Processor stopped!", "CPU::STP");
	running = false;

	if (unimplementedOpcodes.size() > 0)
	{
		PrintUnimplemented();
	}
	return 1;
}


void CPU::RRA(byte value)
{
	//acc = acc + ROR(value);
	ADC(ROR(value));
}

int CPU::RRA_D()
{
	RRA(GetByte_D());
	return 5;
}

int CPU::RRA_D_X()
{
	RRA(GetByte_D_X());
	return 6;
}

int CPU::RRA_A()
{
	RRA(GetByte_A());
	return 6;
}

int CPU::RRA_A_Y()
{
	RRA(GetByte_A_Y());
	return 7;
}

int CPU::RRA_A_X()
{
	RRA(GetByte_A_X());
	return 7;
}

int CPU::RRA_ID_Y()
{
	RRA(GetByte_ID_Y());
	return 8;
}

int CPU::RRA_ID_X()
{
	RRA(GetByte_ID_X());
	return 8;
}


void CPU::FillOpcodes()
{
	// STP 
	opcodes[0x12] = opcodes[0x32] = opcodes[0x52] = opcodes[0x72] = opcodes[0x92] = opcodes[0xB2] = opcodes[0xD2] = opcodes[0xF2] 
		= opcodes[0x02] = opcodes[0x22] = opcodes[0x42] = opcodes[0x62] = Instruction("STP", &CPU::STP);

	// Single NOP
	opcodes[0x1A] = opcodes[0x3A] = opcodes[0x5A] = opcodes[0x7A] = opcodes[0x9A] = opcodes[0xBA] = opcodes[0xDA] = opcodes[0xFA]
		= opcodes[0x1A] = opcodes[0x3A] = opcodes[0x5A] = opcodes[0x7A] = opcodes[0xDA] = opcodes[0xFA] = Instruction("NOP", &CPU::NOP);

	// Double NOP
	opcodes[0x80] = opcodes[0x82] = opcodes[0x89] = opcodes[0xC2] = opcodes[0xE2] = Instruction("NOP.i", &CPU::NOP_I);

	// NOP.a
	opcodes[0x0C] = Instruction("NOP.a", &CPU::NOP_A);

	// NOP.a,x
	opcodes[0x1C] = opcodes[0x3C] = opcodes[0x5C] = opcodes[0x7C] = opcodes[0xDC] = opcodes[0xFC] = Instruction("NOP.a,x", &CPU::NOP_A_X);

	// NOP.d
	opcodes[0x04] = opcodes[0x44] = opcodes[0x64] = Instruction("NOP.d", &CPU::NOP_D);

	// NOP.d,x
	opcodes[0x14] = opcodes[0x34] = opcodes[0x54] = opcodes[0x74] = opcodes[0xD4] = opcodes[0xF4] = Instruction("NOP.d,x", &CPU::NOP_D_X);

	opcodes[0x63] = Instruction("RRA.(d,x)", &CPU::RRA_ID_X);
	opcodes[0x67] = Instruction("RRA.d", &CPU::RRA_D);
	opcodes[0x6F] = Instruction("RRA.a", &CPU::RRA_A);
	opcodes[0x73] = Instruction("RRA.(d),y", &CPU::RRA_ID_Y);
	opcodes[0x77] = Instruction("RRA.d,x", &CPU::RRA_D_X);
	opcodes[0x7B] = Instruction("RRA.a,y", &CPU::RRA_A_Y);
	opcodes[0x7F] = Instruction("RRA.a,x", &CPU::RRA_A_X);

	// Official opcodes
	opcodes[0x01] = Instruction("ORA.(d,x)", &CPU::ORA_ID_X);
	opcodes[0x05] = Instruction("ORA.d", &CPU::ORA_D);
	opcodes[0x06] = Instruction("ASL.d", &CPU::ASL_D);
	opcodes[0x08] = Instruction("PHP", &CPU::PHP);
	opcodes[0x09] = Instruction("ORA", &CPU::ORA_I);
	opcodes[0x0A] = Instruction("ASL.acc", &CPU::ASL_ACC);
	opcodes[0x0D] = Instruction("ORA.a", &CPU::ORA_A);
	opcodes[0x0E] = Instruction("ASL.a", &CPU::ASL_A);
	opcodes[0x10] = Instruction("BPL", &CPU::BPL);
	opcodes[0x11] = Instruction("ORA.(d),y", &CPU::ORA_ID_Y);
	opcodes[0x15] = Instruction("ORA.d,x", &CPU::ORA_D_X);
	opcodes[0x16] = Instruction("ASL.d,x", &CPU::ASL_D_X);
	opcodes[0x18] = Instruction("CLC", &CPU::CLC);
	opcodes[0x19] = Instruction("ORA.a,y", &CPU::ORA_A_Y);
	opcodes[0x1D] = Instruction("ORA.a,x", &CPU::ORA_A_X);
	opcodes[0x1E] = Instruction("ASL.a,x", &CPU::ASL_A_X);
	opcodes[0x20] = Instruction("JSR", &CPU::JSR);
	opcodes[0x21] = Instruction("AND.(d,x)", &CPU::AND_ID_X);
	opcodes[0x24] = Instruction("BIT.d", &CPU::BIT_D);
	opcodes[0x25] = Instruction("AND.d", &CPU::AND_D);
	opcodes[0x26] = Instruction("ROL.d", &CPU::ROL_D);
	opcodes[0x28] = Instruction("PLP", &CPU::PLP);
	opcodes[0x29] = Instruction("AND.i", &CPU::AND_I);
	opcodes[0x2A] = Instruction("ROL.acc", &CPU::ROL_ACC);
	opcodes[0x2C] = Instruction("BIT.a", &CPU::BIT_A);
	opcodes[0x2D] = Instruction("AND.a", &CPU::AND_A);
	opcodes[0x2E] = Instruction("ROL.a", &CPU::ROL_A);
	opcodes[0x30] = Instruction("BMI", &CPU::BMI);
	opcodes[0x31] = Instruction("AND.(d),y", &CPU::AND_ID_Y);
	opcodes[0x35] = Instruction("AND.d,x", &CPU::AND_D_X);
	opcodes[0x36] = Instruction("ROL.d,x", &CPU::ROL_D_X);
	opcodes[0x38] = Instruction("SEC", &CPU::SEC);
	opcodes[0x39] = Instruction("AND.a,y", &CPU::AND_A_Y);
	opcodes[0x3D] = Instruction("AND.a,x", &CPU::AND_A_X);
	opcodes[0x3E] = Instruction("ROL.a,x", &CPU::ROL_A_X);
	opcodes[0x40] = Instruction("RTI", &CPU::RTI);
	opcodes[0x41] = Instruction("EOR.(d,x)", &CPU::EOR_ID_X);
	opcodes[0x45] = Instruction("EOR.d", &CPU::EOR_D);
	opcodes[0x46] = Instruction("LSR.d", &CPU::LSR_D);
	opcodes[0x48] = Instruction("PHA", &CPU::PHA);
	opcodes[0x4A] = Instruction("LSR.acc", &CPU::LSR_ACC);
	opcodes[0x4C] = Instruction("JMP.a", &CPU::JMP_A);
	opcodes[0x4D] = Instruction("EOR.a", &CPU::EOR_A);
	opcodes[0x4E] = Instruction("LSR.a", &CPU::LSR_A);
	opcodes[0x49] = Instruction("EOR.i", &CPU::EOR_I);
	opcodes[0x50] = Instruction("BVC", &CPU::BVC);
	opcodes[0x51] = Instruction("EOR.(d),y)", &CPU::EOR_ID_Y);
	opcodes[0x55] = Instruction("EOR.d,x", &CPU::EOR_D_X);
	opcodes[0x56] = Instruction("LSR.d,x", &CPU::LSR_D_X);
	opcodes[0x59] = Instruction("AOR.a,y", &CPU::EOR_A_Y);
	opcodes[0x5D] = Instruction("EOR.a,x", &CPU::EOR_A_X);
	opcodes[0x5E] = Instruction("LSR.a,x", &CPU::LSR_A_X);
	opcodes[0x60] = Instruction("RTS", &CPU::RTS);
	opcodes[0x61] = Instruction("ADC.(d,x)", &CPU::ADC_ID_X);
	opcodes[0x65] = Instruction("ADC.d", &CPU::ADC_D);
	opcodes[0x66] = Instruction("ROR.d", &CPU::ROR_D);
	opcodes[0x68] = Instruction("PLA", &CPU::PLA);
	opcodes[0x69] = Instruction("ADC.i", &CPU::ADC_I);
	opcodes[0x6A] = Instruction("ROR.acc", &CPU::ROR_ACC);
	opcodes[0x6C] = Instruction("JMP.(a)", &CPU::JMP_ID);
	opcodes[0x6D] = Instruction("ADC.a", &CPU::ADC_A);
	opcodes[0x6E] = Instruction("ROR.a", &CPU::ROR_A);
	opcodes[0x70] = Instruction("BVS", &CPU::BVS);
	opcodes[0x71] = Instruction("ADC.(d),y", &CPU::ADC_ID_Y);
	opcodes[0x75] = Instruction("ADC.d,x", &CPU::ADC_D_X);
	opcodes[0x76] = Instruction("ROR.d,x", &CPU::ROR_D_X);
	opcodes[0x78] = Instruction("SEI", &CPU::SEI);
	opcodes[0x79] = Instruction("ADC.a,y", &CPU::ADC_A_Y);
	opcodes[0x7D] = Instruction("ADC.a,x", &CPU::ADC_A_X);
	opcodes[0x7E] = Instruction("ROR.a,x)", &CPU::ROR_A_X);
	opcodes[0x81] = Instruction("STA.(d,x)", &CPU::STA_ID_X);
	opcodes[0x84] = Instruction("STY.d", &CPU::STY_D);
	opcodes[0x85] = Instruction("STA.d", &CPU::STA_D);
	opcodes[0x86] = Instruction("STX.d", &CPU::STX_D);
	opcodes[0x88] = Instruction("DEY", &CPU::DEY);
	opcodes[0x8A] = Instruction("TXA", &CPU::TXA);
	opcodes[0x8C] = Instruction("STY.a", &CPU::STY_A);
	opcodes[0x8D] = Instruction("STA.a", &CPU::STA_A);
	opcodes[0x8E] = Instruction("STX.a", &CPU::STX_A);
	opcodes[0x90] = Instruction("BCC", &CPU::BCC);
	opcodes[0x91] = Instruction("STA.(d),y", &CPU::STA_ID_Y);
	opcodes[0x94] = Instruction("STY.d,x", &CPU::STY_D_X);
	opcodes[0x95] = Instruction("STA.d,x", &CPU::STA_D_X);
	opcodes[0x96] = Instruction("STX.d,y", &CPU::STX_D_Y);
	opcodes[0x98] = Instruction("TYA", &CPU::TYA);
	opcodes[0x99] = Instruction("STA.a,y", &CPU::STA_A_Y);
	opcodes[0x9A] = Instruction("TXS", &CPU::TXS);
	opcodes[0x9D] = Instruction("STA.a,x", &CPU::STA_A_X);
	opcodes[0xA0] = Instruction("LDY.i", &CPU::LDY_I);
	opcodes[0xA1] = Instruction("LDA.(d,x)", &CPU::LDA_ID_X);
	opcodes[0xA2] = Instruction("LDX.i", &CPU::LDX_I);
	opcodes[0xA4] = Instruction("LDY.d", &CPU::LDY_D);
	opcodes[0xA5] = Instruction("LDA.d", &CPU::LDA_D);
	opcodes[0xA6] = Instruction("LDX.d", &CPU::LDX_D);
	opcodes[0xA8] = Instruction("TAY", &CPU::TAY);
	opcodes[0xA9] = Instruction("LDA.i", &CPU::LDA_I);
	opcodes[0xAA] = Instruction("TAX", &CPU::TAX);
	opcodes[0xAC] = Instruction("LDY.a", &CPU::LDY_A);
	opcodes[0xAD] = Instruction("LDA.a", &CPU::LDA_A);
	opcodes[0xAE] = Instruction("LDX.a", &CPU::LDX_A);
	opcodes[0xB0] = Instruction("BCS", &CPU::BCS);
	opcodes[0xB1] = Instruction("LDA.(d),y", &CPU::LDA_ID_Y);
	opcodes[0xB4] = Instruction("LDY.d,x", &CPU::LDY_D_X);
	opcodes[0xB5] = Instruction("LDA.d,x", &CPU::LDA_D_X);
	opcodes[0xB6] = Instruction("LDX.d,y", &CPU::LDX_D_Y);
	opcodes[0xB8] = Instruction("CLV", &CPU::CLV);
	opcodes[0xB9] = Instruction("LDA.a,y", &CPU::LDA_A_Y);
	opcodes[0xBA] = Instruction("TSX", &CPU::TSX);
	opcodes[0xBC] = Instruction("LDY.a,x", &CPU::LDY_A_X);
	opcodes[0xBD] = Instruction("LDA.a,x", &CPU::LDA_A_X);
	opcodes[0xBE] = Instruction("LDX.a,y", &CPU::LDX_A_Y);
	opcodes[0xC0] = Instruction("CPY.i", &CPU::CPY_I);
	opcodes[0xC1] = Instruction("CMP.(d,x)", &CPU::CMP_ID_X);
	opcodes[0xC4] = Instruction("CPY.d", &CPU::CPY_D);
	opcodes[0xC5] = Instruction("CMP.d", &CPU::CMP_D);
	opcodes[0xC6] = Instruction("DEC.d", &CPU::DEC_D);
	opcodes[0xC8] = Instruction("INY", &CPU::INY);
	opcodes[0xC9] = Instruction("CMP.i", &CPU::CMP_I);
	opcodes[0xCA] = Instruction("DEX", &CPU::DEX);
	opcodes[0xCC] = Instruction("CPY.a", &CPU::CPY_A);
	opcodes[0xCD] = Instruction("CMP.a", &CPU::CMP_A);
	opcodes[0xCE] = Instruction("DEC.a", &CPU::DEC_A);
	opcodes[0xD0] = Instruction("BNE", &CPU::BNE);
	opcodes[0xD1] = Instruction("CMP.(d),y", &CPU::CMP_ID_Y);
	opcodes[0xD5] = Instruction("CMP.d,x", &CPU::CMP_D_X);
	opcodes[0xD6] = Instruction("DEC.d,x", &CPU::DEC_D_X);
	opcodes[0xD8] = Instruction("CLD", &CPU::CLD);
	opcodes[0xD9] = Instruction("CMP.a,y", &CPU::CMP_A_Y);
	opcodes[0xDD] = Instruction("CMP.a,x", &CPU::CMP_A_X);
	opcodes[0xDE] = Instruction("DEC.a,x", &CPU::DEC_A_X);
	opcodes[0xE0] = Instruction("CPX.i", &CPU::CPX_I);
	opcodes[0xE1] = Instruction("SBC.(d,x)", &CPU::SBC_ID_X);
	opcodes[0xE4] = Instruction("CPX.d", &CPU::CPX_D);
	opcodes[0xE5] = Instruction("SBC.d", &CPU::SBC_D);
	opcodes[0xE6] = Instruction("INC.d", &CPU::INC_D);
	opcodes[0xE8] = Instruction("INX", &CPU::INX);
	opcodes[0xE9] = Instruction("SBC.i", &CPU::SBC_I);
	opcodes[0xEA] = Instruction("NOP", &CPU::NOP);
	opcodes[0xEC] = Instruction("CPX.a", &CPU::CPX_A);
	opcodes[0xED] = Instruction("SBC.a", &CPU::SBC_A);
	opcodes[0xEE] = Instruction("INC.a", &CPU::INC_A);
	opcodes[0xF0] = Instruction("BEQ", &CPU::BEQ);
	opcodes[0xF1] = Instruction("SBC.(d),y", &CPU::SBC_ID_Y);
	opcodes[0xF5] = Instruction("SBC.d,x", &CPU::SBC_D_X);
	opcodes[0xF6] = Instruction("INC.d,x", &CPU::INC_D_X);
	opcodes[0xF8] = Instruction("SED", &CPU::SED);
	opcodes[0xF9] = Instruction("SBC.a,y", &CPU::SBC_A_Y);
	opcodes[0xFD] = Instruction("SBC.a,x", &CPU::SBC_A_X);
	opcodes[0xFE] = Instruction("INC.a,x", &CPU::INC_A_X);
}
