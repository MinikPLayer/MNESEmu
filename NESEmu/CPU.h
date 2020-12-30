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
	void DisplayStack();

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

	// Gets a byte using PC in immediate addressing mode, setting flags, increments PC automatically
	byte GetByte_I();
	// Gets a byte using PC in absolute addressing mode
	byte GetByte_A();
	// Gets a byte using PC in Zero Page addressing mode
	byte GetByte_D();
	// Gets a byte using PC in Zero Page indexed X addressing mode
	byte GetByte_D_X();
	// Gets a byte using PC in Zero Page indexed Y addressing mode
	byte GetByte_D_Y();
	// Gets a byte using PC in indexed indirect X addresing mode
	byte GetByte_ID_X();
	// Gets a byte using PC in indexed indirect Y addressing mode
	byte GetByte_ID_Y();

	// Gets an address using PC int immediate addresing mode
	uint16_t GetAddress_I();
	// Gets an address using PC in absolute addressing mode
	uint16_t GetAddress_A();
	// Gets an address using PC in zero mode addressing mode
	byte GetAddress_D();
	// Gets an address using PC in zero page indexed X addressing mode
	uint16_t GetAddress_D_X();
	// Gets an address using PC in zero page indexed Y addressing mode
	uint16_t GetAddress_D_Y();
	// Gets an address using PC in indexed indirect X addressing mode
	uint16_t GetAddress_ID_X();
	// Gets an address using PC in indexed indirect Y addressing mode
	uint16_t GetAddress_ID_Y();

	void FillOpcodes(); // Fill opcodes array

	// OpCodes declaration
	int NotImplementedFunction();

	// Helpers - generic functions ( main functons, where opcodes are just wrappers with different addresing modes )
	void LDX(byte value); // Load a byte into X register
	void LDY(byte value); // Load a byte into Y register
	void LDA(byte value); // Load a byte into acc register
	void STX(uint16_t address); // Store value of the X register under the address
	void STY(uint16_t address); // Store value of the Y register under the address
	void STA(uint16_t address); // Store value of the acc register under the address
	void BIT(byte value); // Test one or more bits in memory location 
	void JMP(uint16_t address); // Jump excecution to address
	void CMP(byte reg, byte val); // Compares content of Y register with an 8bit value and sets carry flag if M >= reg, zero flag if reg - M == 0 and negative flag if bit 7 of the result of reg - m is set
	void SBC(byte val); // Substracts an 8 bit value from the accumulator along with setting flags
	byte LSR(byte val); // Shifts val to the right, carry is val bit 0, zero if val = 0, negative if result bit 7 is 1 | returns shifted value
	byte ASL(byte val); // Shifts val to the left, carry is val bit 7, zero if val = 0, negative if result bit 7 is 1 | returns shifted value
	byte ROR(byte val); // Shifts val to the right, bit 7 is filled with carry flag, carry = val bit 0, zero if val = 0, 
	byte ROL(byte val); // Shifts val to the left, bit 0 is filled with carry flag, carry = val bit 7, zero if val = 0

	int SEI(); // 0x78 - set interrupt flag to 1
	int LDX_I(); // 0xA2 - load a byte into X register: Immediate addressing mode
	int LDX_A(); // 0xAE - load a byte into X register: Absolute addressing mode
	int LDY_I(); // 0xA0 - load a byte into Y register: Immediate addressing mdoe
	int LDY_A(); // 0xAC - load a byte into y register: Absolute addressing mode
	int LDA_I(); // 0xA9 - Loads a byte to the accumulator register, setting appropiate flags: Immediate addressing mode
	int LDA_A(); // 0xAD - Loads a byte to the accumulator register, setting appropiate flags: Absolute addressing mode
	int TXS(); // 0x9A - Copy content of X register to SP
	int TSX(); // 0xBA - Copies stack register to X and sets flags
	int INX(); // 0xE8 - Adds 1 to x register and sets flags
	int INY(); // 0xC8 - Adds 1 to y register and sets flags 
	int DEX(); // 0xCA - Substracts 1 from x register and sets flags
	int DEY(); // 0x88 - Substracts 1 from y register and sets flags
	int STX_A(); // 0x8E - Store value from x register into memory: Absolute addressing mode
	int STX_D(); // 0x8E - Store value from x register into memory: Zero page addressing mode
	int STY_D(); // 0x84 - Store value from y register into memory: Zero page addressing mode
	int STA_D(); // 0x85 - Store value from acc register into memory: Zero page addressing mode
	int BIT_A(); // 0x2C - Test one or more bits in memory location: Absolute addressing mode
	int BIT_D(); // 0x24 - Test one or more bits in memory location: Zero page addressing mode
	int JMP_A(); // 0x4C - Sets PC to specified address: Absolute adressing mode
	int JSR(); // 0x20 - Pushes address of the last byte of JSR instruction ( next instr - 1 ) to the stack: Absolute adressing mode
	int RTS(); // 0x60 - Returnes from subroutine by getting (PC-1) from stack
	int NOP(); // 0xEA - Do nothing
	int SEC(); // 0x38 - Set carry flag to 1
	int CLC(); // 0x18 - Set carry flag to 0
	int SED(); // 0xF8 - Set decimal flag to 1
	int CLD(); // 0xD8 - Set decimal flag to 0
	int CLV(); // 0xB8 - Set overflow flag to 0
	int BCS(); // 0xB0 - If carry flag is 1 add read byte to PC
	int BCC(); // 0x90 - If carry flag is 0 add read byte to PC
	int BEQ(); // 0xF0 - If zero flag is 1, add read byte to PC
	int BNE(); // 0xD0 - If zero flag is 0, add read byte to PC
	int BVS(); // 0x70 - If overflow flag is 1, add read byte to PC
	int BVC(); // 0x50 - If overflow flag is 0, add read byte to PC
	int BMI(); // 0x30 - If overflow flag is 1, add read byte to PC
	int BPL(); // 0x10 - If negative flag is 0, add read byte to PC
	int PHP(); // 0x08 - Pushes a copy of status register ( flags ) to the stack
	int PLP(); // 0x28 - Pulls an 8 bit value from the stack and puts it into the status register ( flags )
	int PHA(); // 0x48 - Pushes a copy of accumulator to the stack
	int PLA(); // 0x68 - Pulls an 8bit value from stack and puts it into the accumulator
	int AND_I(); // 0x29 - Performs a logical AND on the accumulator: Immediate addressing mode
	int ORA_I(); // 0x09 - Performs a logical IOR ( inclusive OR ) on the accumulator: Immediate addressing mode
	int EOR_I(); // 0x49 - Performs a logical EOR ( exclusive OR ) on the accumulator: Immediate addressing mode
	int CMP_I(); // 0xC9 - Compares content of accumulator with an 8bit value and sets carry flag if M >= acc, zero flag if acc - M == 0 and negative flag if bit 7 of the result of acc - m is set
	int CPY_I(); // 0xC0 - Compares content of Y register with an 8bit value and sets carry flag if M >= y, zero flag if y - M == 0 and negative flag if bit 7 of the result of y - m is set
	int CPX_I(); // 0xE0 - Compares content of Y register with an 8bit value and sets carry flag if M >= x, zero flag if x - M == 0 and negative flag if bit 7 of the result of x - m is set
	int ADC_I(); // 0x69 - Adds an 8 bit value to the accumulator along with setting flags
	int SBC_I(); // 0xE9 - Substracts an 8 bit value from the accumulator along with setting flags
	int TAX(); // 0xAA - Copies accumulator to X register and sets flags
	int TAY(); // 0xA8 - Copies accumulator to Y register and sets flags
	int TXA(); // 0x8A - Copies X to accumulator and sets flags
	int TYA(); // 0x98 - Copies Y to accumulator and sets flags
	int RTI(); // 0x40 - Pulls flags from stack, next PC from stack
	int LSR_ACC(); // 0x4A - Shifts acc to the right, carry is acc bit 0, zero if result = 0, negative if result bit 7 is 1
	int ASL_ACC(); // 0x0A - Shifts acc to the left, carry is acc bit 7, zero if result = 0, negative if result bit 7 is 1
	int ROL_ACC(); // 0x2A
	int ROR_ACC(); // 0x6A
};

#include "NES.h"
