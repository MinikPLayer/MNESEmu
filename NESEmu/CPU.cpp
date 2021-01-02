#include "CPU.h"

void CPU::SetFlag(Flags flag, bool value)
{
	SetBit(sr, (int)flag, value);
}

bool CPU::GetFlag(Flags flag)
{
	return GetBit(sr, (int)flag);
}

void CPU::PrintUnimplemented()
{
	LOGP("\n\nUnimplemented opcodes: ", "CPU");
	for (int i = 0; i < unimplementedOpcodes.size(); i++)
	{
		LOGP("\t0x" << hex << (int)unimplementedOpcodes[i], "CPU");
	}
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

byte CPU::PullFromStack(bool setFlags)
{
	sp++; // descending empty stack
	
	//return GetByte(STACK_BOTTOM + sp);
	if (setFlags)
	{
		return GetByte(STACK_BOTTOM + sp);
	}
	else
	{
		return GetByteRaw(STACK_BOTTOM + sp);
	}
	//byte v = GetByte(STACK_BOTTOM + sp);
	//LOGP("Taking 0x" << hex << (uint16_t)v << " from stack at 0x" << hex << (uint16_t)sp, "Stack");
	//DisplayStack();
	//return v;
}

uint16_t CPU::Pull2FromStack(bool setFlags)
{
	//return uint16_t();
	//return (((uint16_t)PullFromStack()) << 8) + PullFromStack();
	uint16_t lower = PullFromStack(setFlags);
	uint16_t higher = ((uint16_t)PullFromStack(setFlags)) << 8;
	return lower + higher;
	//return (uint16_t)PullFromStack() + (((uint16_t)PullFromStack()) << 8);
}

void CPU::Reset()
{
	//sp = 0xFD;
	//sp -= 3; // stack decremented by 3 // Disable to debug
	pc = ((uint16_t)GetByteRaw(0xFFFD) << 8) + GetByteRaw(0xFFFC);
	//LOGP("PC: 0x" << std::hex << pc, "CPU::Reset");
}

void CPU::Reset(uint16_t overridePC)
{
	Reset();
	pc = overridePC;
}

void CPU::Tick()
{
	if (!running) return;

	// Skip cycles, we have to wait
	if (skipCycles > 1)
	{
		skipCycles--;
		return;
	}

	byte instr = GetOpcode();

	int result = Execute(instr);
	if (result == -1) return;
	if (result == 0)
	{
		LOGP("Function 0x" << hex << (int)instr << " doesn't return cycles count!", "!CPU::Tick!");
		//throw("Incomplete function");
	}

	if (!skipPCIncrement)
		pc++;
	else
		skipPCIncrement = false;

	skipCycles = result;

	//getchar();
}

int CPU::Execute(byte instr)
{
	Instruction opcode = opcodes[instr];
	//LOGP("Executing " << opcode.opcode << " at 0x" << hex << pc, "CPU");
	static int counter = 1;
	
	LOG(hex << pc << " " << hex << (int)instr << " " << opcode.opcode << " A:" << hex << (int)acc << " X:" << hex << (int)rx << " Y:" << hex << (int)ry << " P:" << hex << (int)sr << " SP:" << hex << (int)sp);
	int v = (this->*(opcode.func))();
	counter++;
	return v;
	
	LOGP("Cannot execute opcode", "CPU::Exceute");
	return -1;
}

CPU::CPU(NES* nes)
{
	this->nes = nes;

	FillOpcodes();

	acc = rx = ry = 0;
	sp = 0xFD;
	sr = 0x24;
}

void CPU::SetReadFlags(byte val)
{
	/*if (val > 127) // 7-th bit is set: set negative bit to 1
		SetFlag(Negative, true);
	else if (val == 0)
		SetFlag(Zero, true);*/
	SetFlag(Negative, val > 127);
	SetFlag(Zero, val == 0);
}

CPU::Instruction::Instruction(string opcode, int(CPU::*func)())
{
	this->opcode = opcode;
	this->func = func;
}

int CPU::NotImplementedFunction()
{
	byte opcode = GetOpcode();

	//LOGP("\n\nOpcode     0x" << std::hex << (int)opcode << "     not implemented\n\n", "!CPU!");
	cout << "[!CPU!]" << "\n\nOpcode     0x" << std::hex << (int)opcode << "     not implemented\n\n";
	bool found = false;
	for (int i = 0; i < unimplementedOpcodes.size(); i++)
	{
		if (unimplementedOpcodes[i] == opcode)
		{
			found = true;
			break;
		}
	}
	if (!found)
		unimplementedOpcodes.push_back(opcode);

	//throw exception("Opcode not implemented");
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
byte CPU::GetByteRaw_I() { return GetByteRaw(GetAddress_I()); }
byte CPU::GetByte_A() { return GetByte(GetAddress_A()); }
byte CPU::GetByteRaw_A() { return GetByteRaw(GetAddress_A()); }
byte CPU::GetByte_A_X() { return GetByte(GetAddress_A_X()); }
byte CPU::GetByteRaw_A_X() { return GetByteRaw(GetAddress_A_X()); }
byte CPU::GetByte_A_Y() { return GetByte(GetAddress_A_Y()); }
byte CPU::GetByteRaw_A_Y() { return GetByteRaw(GetAddress_A_Y()); }
byte CPU::GetByte_D() { return GetByte(GetAddress_D()); }
byte CPU::GetByteRaw_D() { return GetByteRaw(GetAddress_D()); }
byte CPU::GetByte_D_X() { return GetByte(GetAddress_D_X()); }
byte CPU::GetByteRaw_D_X() { return GetByteRaw(GetAddress_D_X()); }
byte CPU::GetByte_D_Y() { return GetByte(GetAddress_D_Y()); }
byte CPU::GetByteRaw_D_Y() { return GetByteRaw(GetAddress_D_Y()); }
byte CPU::GetByte_ID_X() { return GetByte(GetAddress_ID_X()); }
byte CPU::GetByteRaw_ID_X() { return GetByteRaw(GetAddress_ID_X()); }
byte CPU::GetByte_ID_Y() { return GetByte(GetAddress_ID_Y()); }
byte CPU::GetByteRaw_ID_Y() { return GetByteRaw(GetAddress_ID_Y()); }

/*byte CPU::GetByte_I() 
{
	byte b = GetByte(GetAddress_I()); 
	LOG(hex << (int)b << " ");
	return b;
}
byte CPU::GetByte_A() { return GetByte(GetAddress_A()); }
byte CPU::GetByte_A_X() { return GetByte(GetAddress_A_X()); }
byte CPU::GetByte_A_Y() { return GetByte(GetAddress_A_Y()); }
byte CPU::GetByte_D() { return GetByte(GetAddress_D()); }
byte CPU::GetByte_D_X() { return GetByte(GetAddress_D_X()); }
byte CPU::GetByte_D_Y() { return GetByte(GetAddress_D_Y()); }
byte CPU::GetByte_ID_X() { return GetByte(GetAddress_ID_X()); }
byte CPU::GetByte_ID_Y() { return GetByte(GetAddress_ID_Y()); }*/

uint16_t CPU::GetAddress_I() { return ++pc; }
uint16_t CPU::GetAddress_A() { return Get2Bytes((++pc)++); }
uint16_t CPU::GetAddress_A_X()
{
	uint16_t arg = Get2Bytes((++pc)++);
	return arg + rx; //GetByteRaw(arg + rx);
}
uint16_t CPU::GetAddress_A_Y()
{
	uint16_t arg = Get2Bytes((++pc)++);
	return arg + ry; //GetByteRaw(arg + ry);
}
byte CPU::GetAddress_D() { return GetByteRaw(++pc); }

uint16_t CPU::GetAddress_D_X()
{
	byte arg = GetByteRaw(++pc);
	return (arg + rx) % 256;
}

uint16_t CPU::GetAddress_D_Y()
{
	byte arg = GetByteRaw(++pc);
	return (arg + ry) % 256;
}

uint16_t CPU::GetAddress_ID_X()
{
	byte arg = GetByteRaw(++pc);
	return GetByteRaw((arg + rx) % 256) + (((uint16_t)GetByteRaw((arg + rx + 1) % 256)) << 8);
}

uint16_t CPU::GetAddress_ID_Y()
{
	byte arg = GetByteRaw(++pc);
	return GetByteRaw(arg) + (((uint16_t)GetByteRaw((arg + 1) % 256)) << 8) + ry;
}
