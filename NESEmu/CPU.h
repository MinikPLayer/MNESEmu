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
		Decimal = 3,
		B1 = 4,
		B2 = 5,
		Overflow = 6,
		Negative = 7,
	};
private:

	NES* nes;

	// registers
	byte acc = 0; // Accumulator
	byte rx = 0; // Index X
	byte ry = 0; // Index Y
	uint16_t pc = 0; // Program Counter
	byte sp = 0xFF; // Stack pointer
	byte sr = 0; // Status register

	void SetFlag(Flags flag, bool value);
	bool GetFlag(Flags flag);

	// If opcode takes more than 1 cycles (n) to execute - set this value to n
	int skipCycles = 1;
	// If opcode doesn't require pc++ after executing set this flag to 1
	bool skipPCIncrement = false;

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

	// Pushes 1 byte value to stack
	void PushToStack(byte value);
	// Pushes 2 byte value to stack
	void Push2ToStack(uint16_t value);
	// Pulls 1 byte value from stack
	byte PullFromStack();
	// Pulls 2 byte value from stack
	uint16_t Pull2FromStack();

	void Reset();
	void Reset(uint16_t overridePC);
	void Tick();

	int Execute(byte instr);

	CPU(NES* nes);

	void SetReadFlags(byte value);
	// Gets next opcode
	byte GetOpcode();
	// Gets a byte without settings flags
	byte GetByteRaw(uint16_t address);
	// Gets a byte and sets Zero and Negative flags
	byte GetByte(uint16_t address);
	// Gets 16-bit address from memory
	uint16_t Get2Bytes(uint16_t address);
	// Sets a byte
	void SetByte(uint16_t address, byte value);

	void FillOpcodes(); // Fill opcodes array

	// OpCodes declaration
	int NotImplementedFunction();

	int SEI(); // 0x78 - set interrupt flag to 1
	int LDX_I(); // 0xA2 - load a byte into X register: Immediate addressing mode
	int LDA_I(); // 0xA9 - Loads a byte to the accumulator register, setting appropiate flags: Immediate addressing mode
	int TXS(); // 0x9A - Copy content of X register to SP
	int INX(); // 0xE8 - Add 1 to x register
	int STX_A(); // 0x8E - Store value from x register into memory: Absolute addressing mode
	int STX_D(); // 0x8E - Store value from x register into memory: Zero page addressing mode
	int STA_D(); // 0x85 - Store value from acc register into memory: Zero page addressing mode
	int BIT_A(); // 0x2C - Test one or more bits in memory location: Absolute addressing mode
	int BIT_D(); // 0x24 - Test one or more bits in memory location: Zero page addressing mode
	int JMP_A(); // 0x4C - Sets PC to specified address: Absolute adressing mode
	int JSR(); // 0x20 - Pushes address of the last byte of JSR instruction ( next instr - 1 ) to the stack: Absolute adressing mode
	int RTS(); // 0x60 - Returnes from subroutine by getting (PC-1) from stack
	int NOP(); // 0xEA - Do nothing
	int SEC(); // 0x38 - Set carry flag to 1
	int CLC(); // 0x18 - Set carry flag to 0
	int BCS(); // 0xB0 - If carry flag is 1 add read byte to PC
	int BCC(); // 0x90 - If carry flag is 0 add read byte to PC
	int BEQ(); // 0xF0 - If zero flag is 1, add read byte to PC
	int BNE(); // 0xD0 - If zero flag is 0, add read byte to PC
	int BVS(); // 0x70 - If overflow flag is 1, add read byte to PC
	int BVC(); // 0x50 - If overflow flag is 0, add read byte to PC
	int BMI(); // 0x30 - If overflow flag is 1, add read byte to PC
	int BPL(); // 0x10 - If negative flag is 0, add read byte to PC
	
};

#include "NES.h"
