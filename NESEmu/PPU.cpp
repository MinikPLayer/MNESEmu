#include "PPU.h"

bool PPU::_Write(uint16_t address, byte value)
{
    //LOGP("PPU IS NOT IMPLEMENTED YET", "PPU::_Write");
    int n = address % 8;
    return WriteReg(n, value);
}

byte PPU::_Read(uint16_t address)
{
    int n = address % 8;
    return ReadReg(n);
}

bool PPU::WriteReg(int n, byte value)
{
    if (n == 2)
    {
        LOGP("PPU Reg 2 is not writeable!", "PPU::WriteReg");
        return false;
    }

    reg[n] = value;

    switch (n)
    {
    case 0:
    case 1: // Do nothing
        break;

    case 6:
        if (firstWrite)
            VRAMaddress += ((uint16_t)value) << 8;
        else
            VRAMaddress += value;
        firstWrite = !firstWrite;
        break;
    case 7:
        memory.Write(VRAMaddress, value);
        VRAMaddress += GetFlag(VRAMIncrementMode) ? 32 : 1;
        break;

    default:
        LOGP("Reg " << n << " write switch not implemented", "PPU::WriteReg");
        LOG("");
        break;
    }
}

byte PPU::ReadReg(int n)
{
    if (n == 0 || n == 1 || n == 5 || n == 6)
    {
        LOGP("PPU Reg " << n << " is not readable!", "PPU::ReadReg");
        return 0;
    }

    return reg[n];
}

void PPU::SetFlag(int regN, int p, bool value)
{
    SetBit(reg[regN], p, value);
}
void PPU::SetFlag(Flags_CTRL flag, bool value) { SetFlag(0, (int)flag, value); }
void PPU::SetFlag(Flags_Mask flag, bool value) { SetFlag(1, (int)flag, value); }
void PPU::SetFlag(Flags_Status flag, bool value) { SetFlag(2, (int)flag, value); }

bool PPU::GetFlag(int regN, int p)
{
    return GetBit(reg[regN], p);
}
bool PPU::GetFlag(Flags_CTRL flag) { return GetFlag(0, (int)flag); }
bool PPU::GetFlag(Flags_Mask flag) { return GetFlag(1, (int)flag); }
bool PPU::GetFlag(Flags_Status flag) { return GetFlag(2, (int)flag); }


PPU::PPU(uint16_t minAddr, uint16_t maxAddr, PPU* mirrorParent)
    :MemoryModule("PPU", true, true, minAddr, maxAddr, mirrorParent), memory("PPU")
{
    Init();

    memory.AddModule(&(patternTables[0]));
    memory.AddModule(&(patternTables[1]));
}

void PPU::Tick()
{

}

void PPU::Init()
{
    reg[0] = 0;
    reg[1] = 0;
    //SetBit(reg[2], 6, false);
    SetBit(reg[2], 7, true);
    SetBit(reg[2], 6, false);
    SetBit(reg[2], 5, true);
    reg[3] = 0;
    reg[5] = 0;
    reg[6] = 0;
    reg[7] = 0;
}

void PPU::Reset()
{
    reg[0] = 0;
    reg[1] = 0;
    reg[5] = 0;
    reg[7] = 0;
}
